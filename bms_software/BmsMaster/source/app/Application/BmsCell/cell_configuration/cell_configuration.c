#include "cell_configuration.h"

/*
 * Battery Pack Specification:
 * - Hardware: XMC4800 MCU + BMS6830 AFE
 * - Configuration: 12S (12 cells in series)
 * - Chemistry: LiFePO4 (Lithium Iron Phosphate)
 * - Capacity: 75Ah nominal
 * - Voltage Range: 2.5V - 3.65V per cell
 * - Characteristics: Very flat discharge curve, long cycle life, high safety
 */

/*
 * LiFePO4 OCV-SOC Lookup Table (10 Points)
 * Optimized lookup table for accurate SOC estimation
 * Voltage values are in millivolts (mV)
 * SOC values are in percentage (%)
 * 
 * Table covers key inflection points in LiFePO4 discharge curve:
 * - Full charge plateau
 * - Normal operating range (flat curve)
 * - Low voltage cutoff region
 */
const LOOKUP_TABLE_s bc_stateOfChargeLookupTable[] = {
    {3650, 100.0f},  // 100% SOC - Full charge (3.65V max safe voltage)
    {3400, 90.0f},   // 90% SOC - End of charge plateau
    {3320, 80.0f},   // 80% SOC - Upper operating range
    {3300, 70.0f},   // 70% SOC - Good operating voltage
    {3280, 60.0f},   // 60% SOC - Mid-range
    {3260, 50.0f},   // 50% SOC - Half capacity
    {3240, 40.0f},   // 40% SOC - Lower mid-range
    {3220, 30.0f},   // 30% SOC - Caution range begins
    {3180, 20.0f},   // 20% SOC - Low battery warning
    {2500, 0.0f},    // 0% SOC - Cutoff voltage (2.5V min safe voltage)
};

const uint16_t bc_stateOfChargeLookupTableLength =
    sizeof(bc_stateOfChargeLookupTable) / sizeof(bc_stateOfChargeLookupTable[0]);
