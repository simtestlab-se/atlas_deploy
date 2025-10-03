#ifndef SOC_ESTIMATION_H
#define SOC_ESTIMATION_H

#include <stdint.h>
#include <stdbool.h>

// Estimate SOC for a single cell, using hardcoded mode (OCV or Coulomb Counting)
float EstimateSocFromInputs(int16_t voltage_mV, float used_mAh, float capacity_mAh);

#endif // SOC_ESTIMATION_H
