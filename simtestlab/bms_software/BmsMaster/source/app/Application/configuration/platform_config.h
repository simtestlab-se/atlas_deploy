/*
 * platform_config.h
 * 
 * Software platform configuration for BMS algorithm deployment
 * Hardware and chemistry specs are in cell_configuration.h
 * Auto-generated on: 2025-09-14 15:13:22
 */

#ifndef PLATFORM_CONFIG_H
#define PLATFORM_CONFIG_H

// Software feature configuration only
#include "global_db.h"
#include "mcu_wrapper.h"

// Feature flags
#define ENHANCED_2RC_MODEL 1

// Debug configuration
#ifdef DEBUG
    #define BMS_DEBUG_PRINT(fmt, ...) printf("[BMS] " fmt "\n", ##__VA_ARGS__)
#else
    #define BMS_DEBUG_PRINT(fmt, ...) ((void)0)
#endif

#endif // PLATFORM_CONFIG_H
