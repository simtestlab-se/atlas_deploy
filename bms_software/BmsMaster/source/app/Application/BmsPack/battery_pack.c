/**
 * @file battery_pack.c
 * @brief Battery Pack Management - Statistics, Validation, and Global State
 * 
 * This module handles pack-level operations including:
 * - Battery pack statistics calculation (min/max/avg SOC)
 * - Cell data validation and error recovery
 * - Global state management for external systems
 * - Pack-level health monitoring
 * 
 * @author BMS Team
 * @date September 2025
 */

#include "battery_pack.h"
#include "soc_estimation.h"
#include <global_db.h>
#include <math.h>
#include <stdint.h>
#include <stdbool.h>
#include "bms_config.h"
/*==============================================================================
 * CONFIGURATION CONSTANTS
 *============================================================================*/


#define CELL_VOLTAGE_MIN_V   2.0f            // Minimum safe cell voltage
#define CELL_VOLTAGE_MAX_V   4.0f            // Maximum safe cell voltage

/*==============================================================================
 * EXTERNAL VARIABLES
 *============================================================================*/

// External global variables (defined in bms.c)
extern float min_soc;                        // Minimum SOC across all cells
extern float max_soc;                        // Maximum SOC across all cells  
extern float average_soc;                    // Average SOC across all cells
extern float cellValues[];                   // Cell voltage readings
extern float soc[];                          // Global SOC array for external access

/*==============================================================================
 * IMPLEMENTATION
 *============================================================================*/

/**
 * @brief Calculate battery pack statistics from individual cell SOC values
 * 
 * Updates global variables: min_soc, max_soc, average_soc
 * These values are used for CAN communication and system monitoring.
 */
void BatteryPack_UpdateStatistics(void) {
    float soc_total = 0.0f;
    min_soc = 100.0f;
    max_soc = 0.0f;
    
    for (int cell_index = 0; cell_index < TOTAL_CELLS; cell_index++) {
        float cell_soc = SOC_GetCellSOC(cell_index);
        
        soc_total += cell_soc;
        
        if (cell_soc < min_soc) {
            min_soc = cell_soc;
        }
        if (cell_soc > max_soc) {
            max_soc = cell_soc;
        }
    }
    
    average_soc = soc_total / TOTAL_CELLS;
}

/**
 * @brief Validate cell voltage data and handle error recovery
 * 
 * @param cell_index Index of the cell to validate (0-based)
 * @return true if cell data is valid, false if invalid
 * 
 * Checks if cell voltage is within safe operating range (2.0V - 4.0V for LiFePO4).
 * Implements graceful error recovery using last known good values.
 */
bool BatteryPack_ValidateCellData(int cell_index) {
    float cell_voltage = cellValues[cell_index];
    
    // Check if voltage is within safe operating range
    if (cell_voltage < CELL_VOLTAGE_MIN_V || cell_voltage > CELL_VOLTAGE_MAX_V) {
        // Invalid cell data - use conservative fallback
        // Note: The SOC estimation module handles the actual SOC recovery
        return false; // Indicate invalid data - skip normal processing
    }
    
    // Valid data: Continue with normal processing
    return true;
}

/**
 * @brief Validate calculated SOC values and update global arrays
 * 
 * Performs final validation of SOC calculations and handles:
 * - Output range checking (0-100%)
 * - Global array updates for external systems (CAN, EtherCAT)
 */
void BatteryPack_ValidateAndUpdateGlobals(void) {
    // === OUTPUT VALIDATION ===
    bool values_invalid = (min_soc < 0.0f || min_soc > 100.0f ||
                          max_soc < 0.0f || max_soc > 100.0f ||
                          average_soc < 0.0f || average_soc > 100.0f);
    
    if (values_invalid) {
        // Clamp to valid percentage range
        min_soc = fmaxf(0.0f, fminf(100.0f, min_soc));
        max_soc = fmaxf(0.0f, fminf(100.0f, max_soc));
        average_soc = fmaxf(0.0f, fminf(100.0f, average_soc));
    }
    
    // === GLOBAL ARRAY UPDATE ===
    // Copy individual cell SOC values for external access (CAN, EtherCAT)
    SOC_GetAllCellSOC(soc);
}

/**
 * @brief Get pack imbalance (difference between max and min SOC)
 * 
 * @return Pack imbalance in percentage points
 */
float BatteryPack_GetImbalance(void) {
    return max_soc - min_soc;
}

/**
 * @brief Check if pack is healthy based on SOC spread
 * 
 * @return true if pack is healthy, false if imbalanced
 */
bool BatteryPack_IsHealthy(void) {
    const float MAX_IMBALANCE_PERCENT = 5.0f; // 5% maximum allowed imbalance
    return (BatteryPack_GetImbalance() <= MAX_IMBALANCE_PERCENT);
}
