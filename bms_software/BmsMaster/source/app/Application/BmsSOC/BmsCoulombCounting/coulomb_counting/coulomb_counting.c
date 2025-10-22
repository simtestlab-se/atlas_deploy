
#include "coulomb_counting.h"
#include <stdint.h>

float EstimateSocFromCoulombs(float capacity_mAh, float used_mAh) {
    if (capacity_mAh <= 0.0f) return 0.0f;

    if (used_mAh < 0.0f) used_mAh = 0.0f;  // Clamp negative usage

    float remaining_mAh = capacity_mAh - used_mAh;
    float soc = (remaining_mAh / capacity_mAh) * 100.0f;

    if (soc > 100.0f) soc = 100.0f;
    if (soc < 0.0f) soc = 0.0f;

    return soc;
}

// Integrate current over time to update used_mAh (Coulomb Counting)
// current_A: current in Amperes
// now_ms: current timestamp in milliseconds
// prev_ms: pointer to previous timestamp (will be updated)
// num_cells: number of cells to update
void CoulombCounting_Update(float *used_mAh, float current_A, uint32_t now_ms, uint32_t *prev_ms, uint16_t num_cells) {
    float delta_t_hours = (now_ms - *prev_ms) / 3600000.0f; // ms to hours
    *prev_ms = now_ms;
    float current_mA = current_A * 1000.0f;
    for (uint16_t i = 0; i < num_cells; i++) {
        used_mAh[i] += current_mA * delta_t_hours;
        if (used_mAh[i] < 0.0f) used_mAh[i] = 0.0f;
    }
}

