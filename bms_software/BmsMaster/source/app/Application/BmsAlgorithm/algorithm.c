/**
 * @file algorithm.c
 * @brief BMS Algorithm Task Coordinator - Simplified Task Orchestration
 * 
 * This module implements the main BMS algorithm task responsible for:
 * - FreeRTOS task coordination and timing
 * - System health monitoring and communication checking
 * - Safe mode operation and error handling
 * - Orchestration of SOC estimation and pack management modules
 * 
 * Architecture: Clean task coordinator that delegates work to specialized modules:
 * - SOC estimation handled by soc_estimation.c
 * - Pack management handled by battery_pack.c
 * - Algorithm.c focuses on task coordination and system health
 * 
 * @author BMS Team
 * @date September 2025
 */

#include <global_db.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "../BmsSOC/BmsSocEstimation/soc_estimation.h"
#include "../BmsPack/battery_pack.h"
#include "../BmsCell/cell_configuration/cell_configuration.h"
#include "algorithm.h"
#include "Dave/Generated/FREERTOS/FreeRTOS.h"
#include "Dave/Generated/FREERTOS/task.h"
#include "Dave/Generated/FREERTOS/portable/GCC/portmacro.h"

/*==============================================================================
 * CONFIGURATION CONSTANTS
 *============================================================================*/

// Task Timing Configuration  
#define UPDATE_INTERVAL_MS   200             // Main algorithm update rate (5Hz)
#define DATA_TIMEOUT_MS      1000            // Max age for valid data (1s)
#define SAFE_MODE_SOC_CAP    30.0f           // SOC cap in communication failure

/*==============================================================================
 * GLOBAL VARIABLES
 *============================================================================*/

// External global variables (defined in bms.c)
extern float pack_current;                   // Pack current in Amperes

// Communication health tracking
static TickType_t last_good_data_time = 0;   // Last valid data timestamp
static bool communication_healthy = true;    // Communication status flag

/*==============================================================================
 * FUNCTION PROTOTYPES
 *============================================================================*/

static bool check_system_health(TickType_t current_time, TickType_t *last_data_timestamp);

/*==============================================================================
 * IMPLEMENTATION
 *============================================================================*/

/**
 * @brief Monitor system health and handle communication issues
 * 
 * @param current_time Current FreeRTOS tick count
 * @param last_data_timestamp Pointer to last data timestamp (updated by function)
 * @return true if system is healthy, false if in safe mode
 * 
 * Monitors:
 * - Data freshness (ensures measurements are recent)
 * - Current sensor validity (checks for NaN values)
 * - Communication health with external systems
 * 
 * Implements safe mode operation when communication fails.
 */
static bool check_system_health(TickType_t current_time, TickType_t *last_data_timestamp) {
    // === Data Freshness Check ===
    TickType_t data_age = current_time - *last_data_timestamp;
    if (data_age > pdMS_TO_TICKS(DATA_TIMEOUT_MS)) {
        communication_healthy = false;
    } else {
        communication_healthy = true;
        last_good_data_time = current_time;
    }
    *last_data_timestamp = current_timestamp; // Update from global timestamp
    
    // === Current Sensor Validation ===
    if (isnan(pack_current)) {
        pack_current = 0.0f; // Default to no current if invalid
    }
    
    // === Load State Detection ===
    SOC_UpdateLoadState(pack_current * 1000.0f);  // Convert A to mA for threshold check
    
    // === Safe Mode Handling ===
    if (!communication_healthy) {
        // Apply conservative SOC capping for safety
        // Note: This would need to be implemented in SOC module if needed
        
        return false; // System not healthy - skip normal operation
    }
    
    return true; // System healthy - continue normal operation
}

/*==============================================================================
 * MAIN ALGORITHM TASK
 *============================================================================*/

/**
 * @brief Main BMS Algorithm FreeRTOS Task - Clean Task Coordinator
 * 
 * @param pvParameters FreeRTOS task parameters (unused)
 * 
 * This is the main algorithm task that orchestrates SOC estimation and pack management.
 * Runs at 5Hz (200ms intervals) and coordinates all algorithm modules.
 * 
 * Task Flow:
 * 1. Initialize SOC estimation using SOC module
 * 2. Main Loop:
 *    - Check system health
 *    - Validate cell data (via battery pack module)  
 *    - Update SOC calculations (via SOC module)
 *    - Calculate pack statistics (via battery pack module)
 *    - Update global arrays (via battery pack module)
 *    - Wait for next cycle
 */
void bmsalgo_Task(void *pvParameters) {
    TickType_t wake_time;
    static TickType_t last_data_timestamp = 0;
    
    // === TASK INITIALIZATION ===
    wake_time = xTaskGetTickCount(); // Initialize wake time for periodic execution
    
    // Initialize SOC estimation using the SOC module
    SOC_InitializeEstimation();
    
    // === MAIN ALGORITHM LOOP ===
    for (;;) {
        TickType_t current_time = xTaskGetTickCount();
        
        // === SYSTEM HEALTH CHECK ===
        if (!check_system_health(current_time, &last_data_timestamp)) {
            // System not healthy - skip to next cycle with reduced processing
            vTaskDelayUntil(&wake_time, pdMS_TO_TICKS(UPDATE_INTERVAL_MS));
            continue;
        }
        
        // === CELL DATA VALIDATION ===
        bool all_cells_valid = true;
        for (int cell_index = 0; cell_index < 12; cell_index++) {
            if (!BatteryPack_ValidateCellData(cell_index)) {
                // Invalid cell data - continue processing but note the issue
                all_cells_valid = false;
            }
        }
        
        // === SOC CALCULATION UPDATE ===
        SOC_UpdateCalculations();
        
        // === PACK STATISTICS CALCULATION ===
        BatteryPack_UpdateStatistics(); // Updates min_soc, max_soc, average_soc globals
        
        // === GLOBAL ARRAY UPDATE ===
        BatteryPack_ValidateAndUpdateGlobals();
        
        // === WAIT FOR NEXT CYCLE ===
        vTaskDelayUntil(&wake_time, pdMS_TO_TICKS(UPDATE_INTERVAL_MS));
    }
}

