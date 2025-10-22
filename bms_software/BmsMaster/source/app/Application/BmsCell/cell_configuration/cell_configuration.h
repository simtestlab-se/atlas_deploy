#ifndef CELL_CONFIGURATION_H
#define CELL_CONFIGURATION_H

#include <stdint.h>

// Pack Configuration - Hardware Specifications
#define TARGET_HARDWARE "XMC4800"        // MCU platform
#define AFE_CHIP "BMS6830"               // Analog Front-End
#define CELL_COUNT 12                    // Series cell configuration  

// Battery Chemistry and Capacity Configuration - LiFePO4 Only
#define CELL_CHEMISTRY "LiFePO4"         // Battery chemistry
#define NOMINAL_CAPACITY_AH 75           // Pack capacity in Ah
#define NOMINAL_CAPACITY_MAH 75000       // Pack capacity in mAh
#define NOMINAL_CELL_VOLTAGE 3.2f        // LiFePO4 nominal voltage
#define MAX_CELL_VOLTAGE 3.65f           // LiFePO4 max safe voltage
#define MIN_CELL_VOLTAGE 2.50f           // LiFePO4 min safe voltage

// OCV-SOC Lookup Table Structure
typedef struct {
    int16_t voltage_mV;
    float value;  // SOC %
} LOOKUP_TABLE_s;

extern const LOOKUP_TABLE_s bc_stateOfChargeLookupTable[];
extern const uint16_t bc_stateOfChargeLookupTableLength;

#endif
