#ifndef CELL_LIMIT_VIOLATION_H
#define CELL_LIMIT_VIOLATION_H

#include <stdbool.h>
#include <stdint.h>

/**
 * @brief Run all cell limit violation checks.
 *        This function should be called periodically in the main control loop.
 */
void Check_CellLimitViolations(void);

/**
 * @brief Estimate SOC from cell voltage using lookup + interpolation
 *
 * @param voltage_mV Cell voltage in millivolts
 * @return float State of Charge (%)
 */
float GetSocFromVoltage(int16_t voltage_mV);

/* Violation flags (externs) */
extern bool violation_overvoltage;        // Overvoltage
extern bool violation_undervoltage;       // Undervoltage
extern bool violation_overtemperature;    // Overtemperature
extern bool violation_undertemperature;   // Undertemperature
extern bool violation_overcurrent;        // Overcurrent
extern bool violation_cell_imbalance;     // Cell Imbalance

#endif /* CELL_LIMIT_VIOLATION_H */
