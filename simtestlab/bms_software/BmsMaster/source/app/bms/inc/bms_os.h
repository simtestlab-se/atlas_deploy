/*
 * os.h
 *
 *  Created on: Sep 3, 2025
 *      Author: akil
 */

#ifndef SOURCE_APP_BLDC_MOTOR_DRIVER_INC_BMS_OS_H_
#define SOURCE_APP_BLDC_MOTOR_DRIVER_INC_BMS_OS_H_

#include "FreeRTOS.h"
#include "task.h"

/* Task handles (optional, if you need to monitor them) */
extern TaskHandle_t hTaskBms100ms;
extern TaskHandle_t hTaskContactor;
extern TaskHandle_t hTaskBmsCAN;
extern TaskHandle_t hTaskAlgorithm;
extern TaskHandle_t hTaskCellBal;
extern TaskHandle_t hTaskTwinCAT;
extern TaskHandle_t hTaskBms1s;
extern TaskHandle_t hTask_VehicleLoad;

/* Initialize all tasks */
void bms_OS_Init(void);

#endif /* SOURCE_APP_BLDC_MOTOR_DRIVER_INC_BMS_OS_H_ */
