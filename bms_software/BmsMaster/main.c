/*
 * main.c
 *
 *  Created on: 2025 Sep 02 11:54:57
 *  Author: akil
 */


#include "DAVE.h"
#include "bms_os.h"
#include "mcu_wrapper.h"
#include "bms_cmdlist.h"
#include "bms_libWrapper.h"
#include "bms_mcuWrapper.h"
#include "can_service.h"
#include "bms_config.h"

/**
 * @brief main() - Application entry point
 *
 * <b>Details of function</b><br>
 * This routine is the application entry point. It is invoked by the device startup code.
 */
int main(void)
{
    DAVE_STATUS_t status;

    status = DAVE_Init();  /* Initialization of DAVE APPs */
    if (status != DAVE_STATUS_SUCCESS)
    {
        XMC_DEBUG("DAVE APPs initialization failed\n");
        while(1U) { /* DAVE init error */ }
    }

    /* Consolidated BMS initialization */
    if (!Bms_Init())
    {
        XMC_DEBUG("BMS initialization failed\n");
        while(1U) { /* BMS init error */ }
    }

    /* Initialize and start FreeRTOS */
    bms_OS_Init();
    vTaskStartScheduler();

    /* Should never reach here */
    while(1U){};
}
