#include "ocv.h"
#include "cell_configuration.h"
#include <math.h>
#include <stdint.h>
#include <global_db.h>

static float LinearInterpolation(float x1, float y1, float x2, float y2, float x) {
    return y1 + ((x - x1) * (y2 - y1)) / (x2 - x1);
}

float GetSocFromVoltage(int16_t voltage_mV) {
    // Simple OCV lookup without any compensation
    uint16_t i;  // Declare loop variable at function start for C90 compatibility
    
    // Upper bound: if voltage is >= highest entry, return highest SOC
    if (voltage_mV >= bc_stateOfChargeLookupTable[0].voltage_mV) {
        return bc_stateOfChargeLookupTable[0].value;
    }

    // Lower bound: if voltage is <= lowest entry, return lowest SOC
    if (voltage_mV <= bc_stateOfChargeLookupTable[bc_stateOfChargeLookupTableLength - 1].voltage_mV) {
        return bc_stateOfChargeLookupTable[bc_stateOfChargeLookupTableLength - 1].value;
    }

    // Find the correct interval to interpolate within
    for (i = 0; i < bc_stateOfChargeLookupTableLength - 1; i++) {
        int16_t v_high = bc_stateOfChargeLookupTable[i].voltage_mV;
        int16_t v_low = bc_stateOfChargeLookupTable[i + 1].voltage_mV;

        if (voltage_mV <= v_high && voltage_mV >= v_low) {
            float soc_high = bc_stateOfChargeLookupTable[i].value;
            float soc_low  = bc_stateOfChargeLookupTable[i + 1].value;
            
            return LinearInterpolation(
                (float)v_low, soc_low, 
                (float)v_high, soc_high, 
                (float)voltage_mV
            );
        }
    }

    // Should never reach here if table is correct
    return 50.0f;
}
