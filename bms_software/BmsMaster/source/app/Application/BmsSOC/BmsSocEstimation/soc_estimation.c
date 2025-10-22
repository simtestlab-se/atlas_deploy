#include "soc_estimation.h"
#include "ocv.h"
#include "coulomb_counting.h"
#include "cell_configuration.h"
#include <global_db.h>
#include <math.h>
#include <stdint.h>
#include <stdbool.h>
#include "bms_config.h"
// Include FreeRTOS for timing functions
#include "FreeRTOS.h"
#include "task.h"

/*==============================================================================
 * CONFIGURATION CONSTANTS
 *============================================================================*/

#define CURRENT_THRESHOLD 0.1f        // Current threshold to determine if load is present (100mA)
#define CURRENT_THRESHOLD_MA 50.0f    // No-load current threshold (50mA)

/*==============================================================================
 * EXTERNAL VARIABLES
 *============================================================================*/

// External global variables from global_db and algorithm
extern float pack_current;           // Pack current in Amperes
extern uint32_t current_timestamp;   // Current timestamp
extern float cellValues[];           // Cell voltage readings

/*==============================================================================
 * SOC ESTIMATION IMPLEMENTATION
 *============================================================================*/

/**
 * @brief Battery State Structure
 * 
 * Contains all state information for the battery pack including
 * per-cell SOC values, capacity tracking, and load state detection.
 */
typedef struct {
    // Pack Configuration
    int num_cells;                           // Number of cells in the pack
    float capacity_mAh;                      // Nominal capacity per cell (mAh)
    
    // Per-Cell State Data
    float soc[TOTAL_CELLS];                  // Current SOC per cell (%)
    int voltage_mV[TOTAL_CELLS];             // Cell voltages in millivolts
    float used_mAh[TOTAL_CELLS];             // Used capacity per cell (mAh)
    
    // Load State Detection
    bool is_load_connected;                  // True if pack current > threshold
    unsigned int no_load_timestamp;          // Time when load was disconnected
    
    // SOC Method Tracking (for diagnostics)
    float last_ocv_soc[TOTAL_CELLS];         // Last known OCV-based SOC
    float coulomb_soc[TOTAL_CELLS];          // Pure coulomb counting SOC
    bool is_ocv_valid[TOTAL_CELLS];          // OCV data validity flag
} BatteryState;

// Battery state instance - main data structure for SOC estimation
static BatteryState battery_state = {
    .num_cells = TOTAL_CELLS,
    .capacity_mAh = NOMINAL_CAPACITY_MAH,
    .is_load_connected = false,
    .no_load_timestamp = 0
};

// Function to check if load is present based on current
static bool isLoadPresent(void) {
    extern float pack_current;  // From global_db.h
    return (fabs(pack_current) > CURRENT_THRESHOLD);
}

// Simple per-cell SOC estimation - no blending, clear method selection
float EstimateSocFromInputs(int16_t voltage_mV, float used_mAh, float capacity_mAh) {
    // Calculate SOC from Coulomb counting
    float cc_soc = 100.0f * (1.0f - (used_mAh / capacity_mAh));
    
    // If no load present, use OCV-based SOC
    if (!isLoadPresent()) {
        return GetSocFromVoltage(voltage_mV);
    }
    
    // Under load, return Coulomb counting result
    return cc_soc;
}

/**
 * @brief Update load state detection for hybrid SOC method switching
 * 
 * @param current_mA Pack current in milliamperes
 * 
 * Monitors current levels to determine when load is connected/disconnected.
 * Manages the OCV settling timer for accurate voltage-based SOC estimation.
 */
void SOC_UpdateLoadState(float current_mA) {
    bool previous_load_state = battery_state.is_load_connected;
    bool current_load_state = (fabs(current_mA) > CURRENT_THRESHOLD_MA);
    
    battery_state.is_load_connected = current_load_state;
    
    // === LOAD DISCONNECTION EVENT ===
    if (previous_load_state && !current_load_state) {
        battery_state.no_load_timestamp = xTaskGetTickCount();
    }
    // === LOAD CONNECTION EVENT ===
    else if (!previous_load_state && current_load_state) {
        // Load connected - SOC estimation will switch to Coulomb counting
    }
}

/**
 * @brief Initialize SOC estimation for all cells using OCV method
 * 
 * Called once at startup to establish initial SOC values.
 * Uses Open Circuit Voltage lookup for LiFePO4 chemistry.
 */
void SOC_InitializeEstimation(void) {
    for (int cell_index = 0; cell_index < TOTAL_CELLS; cell_index++) {
        // Convert cell voltage to millivolts for OCV lookup
        int voltage_mV = (int)(cellValues[cell_index] * 1000.0f);
        float initial_soc = GetSocFromVoltage(voltage_mV);
        
        // Calculate initial used capacity based on SOC
        battery_state.used_mAh[cell_index] = (100.0f - initial_soc) / 100.0f * NOMINAL_CAPACITY_MAH;
        
        // Initialize all SOC tracking values
        battery_state.soc[cell_index] = initial_soc;
        battery_state.last_ocv_soc[cell_index] = initial_soc;
        battery_state.coulomb_soc[cell_index] = initial_soc;
        battery_state.is_ocv_valid[cell_index] = true;
    }
}

/**
 * @brief Update coulomb counting integration and calculate SOC for all cells
 * 
 * This is the core SOC estimation function that:
 * 1. Integrates current over time to update used capacity (Coulomb counting)
 * 2. Applies hybrid SOC estimation (OCV vs CC based on load)
 * 3. Maintains diagnostic SOC values for comparison
 * 
 * Method Selection:
 * - No Load (< 100mA): Uses OCV lookup table
 * - Under Load (≥ 100mA): Uses Coulomb counting
 */
void SOC_UpdateCalculations(void) {
    static TickType_t last_update_time = 0;
    
    // === COULOMB COUNTING INTEGRATION ===
    if (last_update_time != 0) {
        // Calculate time delta in hours
        float delta_time_hours = (current_timestamp - last_update_time) / 3600000.0f;
        // Calculate charge change in mAh (positive = charging, negative = discharging)
        float delta_mAh = pack_current * 1000.0f * delta_time_hours; // Convert A to mA⋅h
        
        // Update used capacity for all cells
        for (int cell_index = 0; cell_index < battery_state.num_cells; cell_index++) {
            // Subtract because charging reduces used capacity
            battery_state.used_mAh[cell_index] -= delta_mAh;
            
            // Apply capacity bounds (0% to 100% SOC equivalent)
            if (battery_state.used_mAh[cell_index] < 0.0f) {
                battery_state.used_mAh[cell_index] = 0.0f;
            }
            if (battery_state.used_mAh[cell_index] > battery_state.capacity_mAh) {
                battery_state.used_mAh[cell_index] = battery_state.capacity_mAh;
            }
        }
    }
    last_update_time = current_timestamp;
    
    // === SOC ESTIMATION FOR EACH CELL ===
    for (int cell_index = 0; cell_index < battery_state.num_cells; cell_index++) {
        // Only process cells with valid voltage data
        if (cellValues[cell_index] >= 2.0f && cellValues[cell_index] <= 4.0f) {
            
            // Convert voltage to millivolts for calculation functions
            int voltage_mV = (int)(cellValues[cell_index] * 1000.0f);
            
            // === PRIMARY SOC CALCULATION (Hybrid Method) ===
            battery_state.soc[cell_index] = EstimateSocFromInputs(
                voltage_mV, 
                battery_state.used_mAh[cell_index], 
                battery_state.capacity_mAh
            );
            
            // === DIAGNOSTIC SOC CALCULATION (Pure Coulomb Counting) ===
            battery_state.coulomb_soc[cell_index] = EstimateSocFromCoulombs(
                battery_state.capacity_mAh,
                battery_state.used_mAh[cell_index]
            );
        }
    }
}

/**
 * @brief Get SOC value for a specific cell
 * 
 * @param cell_index Cell index (0-based)
 * @return SOC percentage (0.0-100.0)
 */
float SOC_GetCellSOC(int cell_index) {
    if (cell_index < 0 || cell_index >= TOTAL_CELLS) {
        return 0.0f;
    }
    return battery_state.soc[cell_index];
}

/**
 * @brief Get all cell SOC values
 * 
 * @param soc_array Output array to store SOC values (must be at least TOTAL_CELLS size)
 */
void SOC_GetAllCellSOC(float *soc_array) {
    for (int i = 0; i < TOTAL_CELLS; i++) {
        soc_array[i] = battery_state.soc[i];
    }

}
