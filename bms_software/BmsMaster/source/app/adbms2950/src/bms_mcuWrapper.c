/*
 * bms_mcuWrapper.cpp
 *
 *  Created on: Nov 24, 2024
 *      Author: amrlxyz
 */
#include "bms_mcuWrapper.h"
#include "bms_config.h"
#include "mcu_wrapper.h"
#include "dave.h"
#include "FreeRTOS.h"
#include "task.h"

#define WAKEUP_DELAY 4       /// 1ms for 2950   /* BMS ic wakeup delay  */


void bms_csLow(void)
{
	DIGITAL_IO_SetOutputLow(&SS_PIN_001);
}


void bms_csHigh(void)
{
	Delay_us(50);
	DIGITAL_IO_SetOutputHigh(&SS_PIN_001);
}



// Wake up all the IC in the daisy chain
void bms_wakeupChain(void)
{
    for (uint8_t ic = 0; ic < TOTAL_AD_IC; ic++)
    {
        bms_csLow();
        Delay_ms(4);
        bms_csHigh();
        Delay_ms(4);
    }
}

void bms_delayMsActive(uint32_t ms)
{
    for (uint32_t i = 0; i < ms; i++)
    {
        bms_csLow();
        Delay_ms(3);
        bms_csHigh();
        Delay_ms(3);
    }
}




