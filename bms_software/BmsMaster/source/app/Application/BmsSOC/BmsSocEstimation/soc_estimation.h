#ifndef SOC_ESTIMATION_H
#define SOC_ESTIMATION_H

#include <stdint.h>
#include <stdbool.h>

// Estimate SOC for a single cell, using hardcoded mode (OCV or Coulomb Counting)
float EstimateSocFromInputs(int16_t voltage_mV, float used_mAh, float capacity_mAh);

// SOC estimation module functions
void SOC_InitializeEstimation(void);
void SOC_UpdateCalculations(void);
void SOC_UpdateLoadState(float current_mA);

// SOC data access functions
float SOC_GetCellSOC(int cell_index);
void SOC_GetAllCellSOC(float *soc_array);

#endif // SOC_ESTIMATION_H