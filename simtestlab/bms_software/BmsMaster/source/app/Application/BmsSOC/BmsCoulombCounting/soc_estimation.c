#include "soc_estimation.h"
#include "ocv.h"
#include "coulomb_counting.h"
#include <math.h>

#define CURRENT_THRESHOLD 0.1f  // Current threshold to determine if load is present (100mA)

// Function to check if load is present based on current
static bool isLoadPresent(void) {
    extern float pack_current;  // From global_db.h
    return (fabs(pack_current) > CURRENT_THRESHOLD);
}

// Simple per-cell SOC estimation - no blending, clear method selection
float EstimateSocFromInputs(int16_t voltage_mV, float used_mAh, float capacity_mAh) {
    // Calculate SOC from Coulomb counting
    float cc_soc = 100.0f * (1.0f - (used_mAh / capacity_mAh));
    
    // If no load present, use OCV-based SOC
    if (!isLoadPresent()) {
        return GetSocFromVoltage(voltage_mV);
    }
    
    // Under load, return Coulomb counting result
    return cc_soc;
}
