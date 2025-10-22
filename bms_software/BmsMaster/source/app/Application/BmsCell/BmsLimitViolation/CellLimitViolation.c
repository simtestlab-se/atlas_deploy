// cell_limit_violation.c
#include "CellLimitViolation.h"
#include "global_db.h"
#include "../cell_configuration/cell_configuration.h"
#include <stdbool.h>
#include <math.h>

// External variable declaration (defined in bms.c)
extern float pack_current;

// Violation flags - actual definitions
bool violation_overvoltage = false;         // Overvoltage
bool violation_undervoltage = false;        // Undervoltage  
bool violation_overtemperature = false;     // Overtemperature
bool violation_undertemperature = false;    // Undertemperature
bool violation_overcurrent = false;         // Overcurrent
bool violation_cell_imbalance = false;      // Cell Imbalance

// Threshold constants based on LiFePO4 cell configuration
// Note: MAX_CELL_VOLTAGE and MIN_CELL_VOLTAGE are defined in cell_configuration.h
#define MAX_CELL_TEMP        60.0f   // Overtemperature (°C)
#define MIN_CELL_TEMP        0.0f    // Undertemperature during charge (°C)
#define MAX_CURRENT          100.0f  // Overcurrent threshold (Amperes)
#define MAX_CELL_IMBALANCE  0.050f   // Cell imbalance (50mV difference)

// External data from global_db
//extern float gpio_tempValues[10 * TOTAL_IC];
//extern float current;
//extern float ntc_temperatures_c[NUM_NTC_CHANNELS];
//extern float cellValues[12 * TOTAL_IC];



void Check_CellLimitViolations(void) {
    violation_overvoltage = false;
    violation_undervoltage = false;
    violation_overtemperature = false;
    violation_undertemperature = false;
    violation_overcurrent = false;
    violation_cell_imbalance = false;

    float max_voltage = 0.0f;
    float min_voltage = 5.0f;

    // Check voltage limits
    for (int i = 0; i < 12 * TOTAL_IC; i++) {
        float v = cellValues[i];
        if (v > MAX_CELL_VOLTAGE) violation_overvoltage = true;
        if (v < MIN_CELL_VOLTAGE) violation_undervoltage = true;
        if (v > max_voltage) max_voltage = v;
        if (v < min_voltage) min_voltage = v;
    }

    // Imbalance check
    if ((max_voltage - min_voltage) > MAX_CELL_IMBALANCE) {
        violation_cell_imbalance = true;
    }

    // Check NTC and GPIO temperature sensors
    for (int i = 0; i < NUM_NTC_CHANNELS; i++) {
        float temp = ntc_temperatures_c[i];
        if (temp > MAX_CELL_TEMP) violation_overtemperature = true;
        if (temp < MIN_CELL_TEMP) violation_undertemperature = true;
    }

    for (int i = 0; i < 10 * TOTAL_IC; i++) {
        float temp = gpio_tempValues[i];
        if (temp > MAX_CELL_TEMP) violation_overtemperature = true;
        if (temp < MIN_CELL_TEMP) violation_undertemperature = true;
    }

    // Overcurrent check
    if (fabsf(pack_current) > MAX_CURRENT) {
        violation_overcurrent = true;
    }
}

