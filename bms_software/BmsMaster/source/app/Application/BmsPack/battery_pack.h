#ifndef BATTERY_PACK_H
#define BATTERY_PACK_H

#include <stdint.h>
#include <stdbool.h>

/**
 * @file battery_pack.h
 * @brief Battery Pack Management - Statistics, Validation, and Global State
 * 
 * This module provides pack-level management functions for:
 * - Pack statistics calculation
 * - Cell data validation
 * - Global state management
 * - Pack health monitoring
 */

// Pack statistics functions
void BatteryPack_UpdateStatistics(void);
float BatteryPack_GetImbalance(void);
bool BatteryPack_IsHealthy(void);

// Cell data validation
bool BatteryPack_ValidateCellData(int cell_index);

// Global state management
void BatteryPack_ValidateAndUpdateGlobals(void);

#endif // BATTERY_PACK_H