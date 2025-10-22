/**
 * @file BmsContactorControl.c
 * @brief Battery Management System Contactor Control Implementation
 * This module handles contactor control logic including:
 * - Contactor closing sequence management (with/without precharge)
 * - Safety monitoring and fault handling
 * - EtherCAT integration for remote control
 * - Current-based safe opening sequence
 * 
 * @author RajavelRajendiran
 * @date 21/09/2025
 */

#include "BmsContactorControl.h"
#include "global_db.h"
#include <math.h>
#include <stdio.h>
#include <stdint.h>

//=============================================================================
// EXTERNAL HARDWARE INTERFACES
//=============================================================================
extern bool Positive_contactor;        // Main positive contactor output
extern bool Negative_contactor;        // Main negative contactor output  
extern bool Precharge_contactor;       // Precharge contactor output

//=============================================================================
// EXTERNAL SAFETY MONITORING
//=============================================================================
extern bool violation_overvoltage;        // Overvoltage protection
extern bool violation_undervoltage;       // Undervoltage protection
extern bool violation_overtemperature;    // Overtemperature protection
extern bool violation_undertemperature;   // Undertemperature protection
extern bool violation_overcurrent;        // Overcurrent protection
extern bool violation_cell_imbalance;     // Cell imbalance protection

//=============================================================================
// EXTERNAL MEASUREMENTS
//=============================================================================
extern float pack_current;             // Pack current measurement [A]
extern float pack_voltage ;
extern float link_voltage ;

//=============================================================================
// CONFIGURATION CONSTANTS
//=============================================================================
#define PRECHARGE_VOLTAGE_THRESHOLD_V    5.0f   // Voltage difference threshold for precharge [V]
#define PRECHARGE_TIMEOUT_CYCLES         2      // Timeout cycles (200 for 20s at 100ms)
#define OPENING_CURRENT_THRESHOLD_A      5.0f   // Current threshold for safe opening [A]
#define MINOR_ERROR_DELAY_CYCLES 100 // 10s delay at 100ms cycle (calibration)
#define OPENING_TIMEOUT_CYCLES 300 // Timeout for forced opening if current does not decrease (30s @ 100ms cycle = 300 cycles)

//=============================================================================
// STATE MACHINE DEFINITIONS
//=============================================================================

/**
 * @brief Contactor control state machine states  
 * @note Handles both precharge and direct closing scenarios
 */
typedef enum {
    CONTACTOR_CTRL_INIT,            // Initial state - ready to start
    CONTACTOR_CTRL_PRECHARGE,      // Negative contactor closed, ready for precharge
    CONTACTOR_CTRL_PRECHARGE_ACTIVE,      // Precharge in progress (if needed)
    CONTACTOR_CTRL_OPEN_PRECHARGE, // Explicitly open precharge after main contactors closed
    CONTACTOR_CTRL_COMPLETE,        // Control successful, main contactors closed and precharge open
    CONTACTOR_CTRL_TIMEOUT,         // Control failed due to timeout
    CONTACTOR_CTRL_ERROR            // Control failed due to safety violation
} CONTACTOR_CTRL_STATE_e;

/**
 * @brief Error codes for contactor control state machine failures
 * @note Handles both precharge and direct closing error scenarios
 */
typedef enum {
    CONTACTOR_CTRL_ERR_NONE = 0,        // No error
    CONTACTOR_CTRL_ERR_TIMEOUT,         // Control timeout
    CONTACTOR_CTRL_ERR_SAFETY,          // Safety violation detected
    CONTACTOR_CTRL_ERR_CONTACTOR_FAULT, // Contactor fault (not responding)
    CONTACTOR_CTRL_ERR_OVERCURRENT,     // not used
    CONTACTOR_CTRL_ERR_UNEXPECTED       // not used
} CONTACTOR_CTRL_ERROR_e;

//=============================================================================
// PRIVATE STATE VARIABLES
//=============================================================================
static CONTACTOR_STATE_e contactorRequestedStates[3];    // Internal contactor state tracking
static CONTACTOR_CTRL_STATE_e ctrl_state = CONTACTOR_CTRL_INIT;             // Current contactor control state
static unsigned int precharge_cycles = 0;                                   // Precharge timeout counter
static CONTACTOR_CTRL_ERROR_e ctrl_error = CONTACTOR_CTRL_ERR_NONE;         // Last error code
static bool contactor_err_reset_req_prev = false;                           // Edge detection for EtherCAT reset

static unsigned int minor_error_delay_counter = 0;                          // Counter for minor error delay
static unsigned int contactor_opening_timeout_counter = 0;



typedef enum {
    ERROR_NONE,
    ERROR_MAJOR,
    ERROR_MINOR
} ERROR_SEVERITY_e;

//=============================================================================
// PRIVATE FUNCTION DECLARATIONS
//=============================================================================
static inline ERROR_SEVERITY_e GetErrorSeverity(void);
static inline bool IsAnyContactorEngaged(void);
static inline bool IsCurrentSafeForOpening(void);
static inline float GetVoltageDiscrepancy(void);
static void ForceAllContactorsOpen(void);
static void ExecuteContactorClosingSequence(bool any_contactor_engaged);
static void UpdateContactorHardware(CONTACTOR_TYPE_e type, CONTACTOR_STATE_e state);

/**
 * @brief Returns true when the system is considered fully closed
 * @note Closed means main positive and negative contactors are closed
 */
static inline bool IsSystemClosed(void) {
    return (!Precharge_contactor && Positive_contactor && Negative_contactor);
}

/**
 * @brief Returns true when the system is considered fully open (safe state)
 */
static inline bool IsSystemOpen(void) {
    return (!Positive_contactor && !Negative_contactor && !Precharge_contactor);
}

//=============================================================================
// PUBLIC API IMPLEMENTATION
//=============================================================================

/**
 * @brief Initialize the contactor control system
 * @note Calls ContactorControl_ResetAll() for comprehensive initialization
 */
void ContactorControl_Init(void) {
    ContactorControl_ResetAll();
}

/**
 * @brief Request a specific contactor state change
 * @param type Contactor type to control
 * @param requestedState Desired state (OPEN/CLOSE)
 * @note Updates both internal state and hardware output
 */
void ContactorControl_RequestState(CONTACTOR_TYPE_e type, CONTACTOR_STATE_e requestedState) {
    // Note: No bounds check - assumes valid enum values only
    // This is safe since function is only called internally with known enum values
    
    contactorRequestedStates[type] = requestedState;
    UpdateContactorHardware(type, requestedState);
}

/**
 * @brief Get the current requested state of a contactor
 * @param type Contactor type to query
 * @return Current requested state (OPEN/CLOSE)
 */
CONTACTOR_STATE_e ContactorControl_GetState(CONTACTOR_TYPE_e type) {
    // Note: No bounds check - assumes valid enum values only
    return contactorRequestedStates[type];
}

/**
 * @brief Reset all contactors to safe state and clear all errors
 * @note This is the master reset function used by Init() and ClearError()
 */
void ContactorControl_ResetAll(void) {
    // Directly open all contactors using the API
    ContactorControl_RequestState(CONTACTOR_POSITIVE, CONTACTOR_OPEN);
    ContactorControl_RequestState(CONTACTOR_NEGATIVE, CONTACTOR_OPEN);
    ContactorControl_RequestState(CONTACTOR_PRECHARGE, CONTACTOR_OPEN);
    // Force hardware to safe state
    Positive_contactor = false;
    Negative_contactor = false;
    Precharge_contactor = false;
    // Reset state machine
    ctrl_state = CONTACTOR_CTRL_INIT;
    precharge_cycles = 0;
    ctrl_error = CONTACTOR_CTRL_ERR_NONE;
}


/**
 * @brief Get the last contactor control error code
 * @return Error code (CONTACTOR_CTRL_ERR_*)
 */
int ContactorControl_GetLastError(void) {
    return (int)ctrl_error;
}



/**
 * @brief Main contactor control logic - call this periodically in RTOS loop
 * @note Optimized for maximum performance with inline safety checks
 */
void ContactorControl_UpdateLogic(void) {
    bool any_contactor_engaged = IsAnyContactorEngaged();
    ERROR_SEVERITY_e error_severity = GetErrorSeverity();

    // Error handling for limit violations
    //once opened for minor delay tbd
    if (error_severity == ERROR_MAJOR) {
        
        ctrl_error = CONTACTOR_CTRL_ERR_SAFETY;
        ctrl_state = CONTACTOR_CTRL_ERROR;
        ForceAllContactorsOpen();
        
        minor_error_delay_counter = 0; 
        return;
    } else if (error_severity == ERROR_MINOR) {
        minor_error_delay_counter++;
        if (minor_error_delay_counter >= MINOR_ERROR_DELAY_CYCLES) {
            ctrl_error = CONTACTOR_CTRL_ERR_SAFETY;
            ctrl_state = CONTACTOR_CTRL_ERROR;
            ForceAllContactorsOpen();
            minor_error_delay_counter = 0;
        }
        return;
    } else {
        minor_error_delay_counter = 0;
    }


    // Handle open request (current-based safety + timeout)
    if (!contactor_req) {
        if (IsSystemOpen()) {
            contactor_opening_timeout_counter = 0;
            if (ctrl_state != CONTACTOR_CTRL_ERROR) {
                ctrl_state = CONTACTOR_CTRL_INIT;
            }
            return;
        }
        if (IsCurrentSafeForOpening()) {
            ForceAllContactorsOpen();
            contactor_opening_timeout_counter = 0;
            if (ctrl_state != CONTACTOR_CTRL_ERROR) {
                ctrl_state = CONTACTOR_CTRL_INIT;
            }
            return;
        }
        // If current is not safe, increment wait counter and force open after timeout
        contactor_opening_timeout_counter++;
        if (contactor_opening_timeout_counter >= OPENING_TIMEOUT_CYCLES) {
            ForceAllContactorsOpen();
            contactor_opening_timeout_counter = 0;
            ctrl_error = CONTACTOR_CTRL_ERR_TIMEOUT;
            ctrl_state = CONTACTOR_CTRL_ERROR;
        }
        return;
    } else {
        // Reset counter if a close is requested
    contactor_opening_timeout_counter = 0;
    }

    // Handle close request - state machine execution
    if (contactor_req) {
        if (IsSystemClosed()) {
            if (ctrl_state != CONTACTOR_CTRL_ERROR) {
                ctrl_state = CONTACTOR_CTRL_COMPLETE;
            }
            return;
        }
        if (ctrl_state == CONTACTOR_CTRL_ERROR) {
            return; // Manual reset required
        }
        ExecuteContactorClosingSequence(any_contactor_engaged);
    }

       // Handle EtherCAT reset request (edge detection)
    bool reset_edge = (contactor_err_reset_req && !contactor_err_reset_req_prev);
    contactor_err_reset_req_prev = contactor_err_reset_req;

    if (reset_edge) {
        ContactorControl_ResetAll();
        minor_error_delay_counter = 0;
        return;
    }
}

//=============================================================================
// PRIVATE HELPER FUNCTIONS
//=============================================================================


/**
 * @brief Get error severity (major/minor/none) based on limit violations
 * @return ERROR_SEVERITY_e
 */
static inline ERROR_SEVERITY_e GetErrorSeverity(void) {
    if (violation_overvoltage || violation_undervoltage || violation_overtemperature || violation_undertemperature)
        return ERROR_MAJOR;
    if (violation_overcurrent || violation_cell_imbalance)
        return ERROR_MINOR;
    return ERROR_NONE;
}

/**
 * @brief Check if any contactor is currently engaged
 * @return true if any contactor is engaged, false if all open
 */
static inline bool IsAnyContactorEngaged(void) {
    return (Positive_contactor || Negative_contactor || Precharge_contactor);
}

/**
 * @brief Check if current is low enough for safe contactor opening
 * @return true if current is below threshold, false otherwise
 */
static inline bool IsCurrentSafeForOpening(void) {
    return (fabsf(pack_current) < OPENING_CURRENT_THRESHOLD_A);
}

/**
 * @brief Get voltage discrepancy between pack and link
 * @return Absolute voltage difference in volts
 */
static inline float GetVoltageDiscrepancy(void) {
    return fabsf(pack_voltage - link_voltage);
}

/**
 * @brief Force all contactors to open state (emergency function)
 * @note Updates both hardware outputs and internal state arrays
 */
static void ForceAllContactorsOpen(void) {
    // Force all contactors to safe state using centralized API
    ContactorControl_RequestState(CONTACTOR_POSITIVE, CONTACTOR_OPEN);
    ContactorControl_RequestState(CONTACTOR_NEGATIVE, CONTACTOR_OPEN);
    ContactorControl_RequestState(CONTACTOR_PRECHARGE, CONTACTOR_OPEN);
}

/**
 * @brief Execute the contactor control state machine
 * @param any_contactor_engaged Current contactor engagement status
 * @note Handles both precharge and direct closing scenarios
 */
static void ExecuteContactorClosingSequence(bool any_contactor_engaged) {
    switch (ctrl_state) {
        case CONTACTOR_CTRL_INIT:
            // Ensure clean starting state
            if (any_contactor_engaged) {
                ForceAllContactorsOpen();
                return; // Wait for next cycle
            }
            
            // Determine if precharge is needed
            if (GetVoltageDiscrepancy() >= PRECHARGE_VOLTAGE_THRESHOLD_V) {
                // Start precharge sequence: Close negative first
                ContactorControl_RequestState(CONTACTOR_NEGATIVE, CONTACTOR_CLOSE);
                ctrl_state = CONTACTOR_CTRL_PRECHARGE;
            } else {
                // Voltages already aligned - skip precharge
                ContactorControl_RequestState(CONTACTOR_POSITIVE, CONTACTOR_CLOSE);
                ContactorControl_RequestState(CONTACTOR_NEGATIVE, CONTACTOR_CLOSE);
                ContactorControl_RequestState(CONTACTOR_PRECHARGE, CONTACTOR_OPEN);
            }
            break;
        case CONTACTOR_CTRL_PRECHARGE:
            // Continue precharge sequence: Close precharge contactor
            ContactorControl_RequestState(CONTACTOR_PRECHARGE, CONTACTOR_CLOSE);
            ctrl_state = CONTACTOR_CTRL_PRECHARGE_ACTIVE;
            precharge_cycles = 0;
            break;
            
        case CONTACTOR_CTRL_PRECHARGE_ACTIVE:
            precharge_cycles++;
            // Verify precharge contactor is responding
            if (!Precharge_contactor) {
                ctrl_error = CONTACTOR_CTRL_ERR_CONTACTOR_FAULT;
                ctrl_state = CONTACTOR_CTRL_ERROR;
                ForceAllContactorsOpen();
                break;
            }

            // Check if precharge is complete
            if (GetVoltageDiscrepancy() < PRECHARGE_VOLTAGE_THRESHOLD_V) {
                // Success: Close main contactors, then explicitly open precharge
                ContactorControl_RequestState(CONTACTOR_POSITIVE, CONTACTOR_CLOSE);
                ContactorControl_RequestState(CONTACTOR_NEGATIVE, CONTACTOR_CLOSE);
                ctrl_state = CONTACTOR_CTRL_OPEN_PRECHARGE;
            } else if (precharge_cycles > PRECHARGE_TIMEOUT_CYCLES) {
                // Timeout: Precharge failed
                ctrl_state = CONTACTOR_CTRL_ERROR;
                ctrl_error = CONTACTOR_CTRL_ERR_TIMEOUT;
                ForceAllContactorsOpen();
            }
            break;

        case CONTACTOR_CTRL_OPEN_PRECHARGE:
            // Open precharge contactor, wait until confirmed open
            ContactorControl_RequestState(CONTACTOR_PRECHARGE, CONTACTOR_OPEN);
            ctrl_state = CONTACTOR_CTRL_COMPLETE;
    
            break;
            
        case CONTACTOR_CTRL_COMPLETE:
            if( IsSystemClosed()) {
                ctrl_state = CONTACTOR_CTRL_COMPLETE;
            }
            else {
                ctrl_state = CONTACTOR_CTRL_ERROR;
                ctrl_error = CONTACTOR_CTRL_ERR_CONTACTOR_FAULT;
                ForceAllContactorsOpen();
                break;
            }                   
            // Normal operation - no action needed
            // Safety monitoring handled at top level
            break;
            
        default:
            // Invalid state - reset to safe condition
            ctrl_state = CONTACTOR_CTRL_INIT;
            break;
    }
}

/**
 * @brief Update hardware contactor output based on requested state
 * @param type Contactor type to update
 * @param state Requested state (OPEN/CLOSE)
 */
static void UpdateContactorHardware(CONTACTOR_TYPE_e type, CONTACTOR_STATE_e state) {
    bool enable = (state == CONTACTOR_CLOSE);
    
    switch (type) {
        case CONTACTOR_POSITIVE:
            Positive_contactor = enable;
            break;
        case CONTACTOR_NEGATIVE:
            Negative_contactor = enable;
            break;
        case CONTACTOR_PRECHARGE:
            Precharge_contactor = enable;
            break;
        default:
            // Invalid contactor type - ignore
            break;
    }
}


