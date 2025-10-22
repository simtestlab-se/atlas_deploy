/*
 * bms_config.h
 *
 *  Created on: Sep 30, 2025
 *      Author: akil
 */

#ifndef SOURCE_APP_BMS_INC_BMS_CONFIG_H_
#define SOURCE_APP_BMS_INC_BMS_CONFIG_H_

/* === BMS Hardware Configuration === */

/* Number of BMS ICs in the system */
#define TOTAL_IC                     1

/* Cell configuration */
#define CELLS_PER_IC                 12
#define TOTAL_CELLS                  (TOTAL_IC * CELLS_PER_IC)

/* GPIO/Temperature configuration */
#define GPIO_CHANNELS_PER_IC         10
#define TOTAL_GPIO_CHANNELS          (TOTAL_IC * GPIO_CHANNELS_PER_IC)

/* NTC Temperature sensor configuration */
#define NUM_NTC_CHANNELS             1

/* ADC Configuration */
#define ADC_MAX_VALUE                4095.0f
#define V_REF                        3.3f

/* NTC Thermistor Parameters */
#define R_FIXED                      100000.0f    // 100kΩ pull-down resistor
#define R0                           100000.0f    // 100kΩ at 25°C
#define T0                           298.15f      // 25°C in Kelvin (273.15 + 25)
#define BETA                         3950.0f      // NTC B-value
#define CALIBRATION_OFFSET           0.0f         // Temperature calibration offset
#define ALPHA                        0.1f         // EMA smoothing factor

/* Current Sensor Configuration */
#define SENSOR_SENS_CH1              0.040f       // mV/A for channel 1
#define SENSOR_SENS_CH2              0.010f       // mV/A for channel 2
#define CUR_SEN_VOLTAGE              5.0f         // Current sensor supply voltage
#define OFFSET_VOLTAGE               2.5f         // Current sensor offset voltage
#define OFFSET_CURRENT_CH1           2.0f         // Current offset for channel 1
#define OFFSET_CURRENT_CH2           6.0f         // Current offset for channel 2

/* Voltage Divider Configuration */
#define R1_OHMS                      190000.0f    // High-side resistor
#define R2_OHMS                      10000.0f     // Low-side resistor
#define VOLTAGE_DIVIDER_SCALE        ((R1_OHMS + R2_OHMS) / R2_OHMS)  // = 20.0

/* Cell Voltage Conversion */
#define C_VOLT_OFFSET                10000        // 1.5v offset in ADC counts
#define C_VOLT_SCALE                 0.000150f    // Conversion scale factor
#define C_VOLT_CONV(v)               (((v) + C_VOLT_OFFSET) * C_VOLT_SCALE)

/* === FreeRTOS Task Configuration === */

/* Stack sizes (in words) */
#define TASK_STACK_SIZE_MINIMAL      (configMINIMAL_STACK_SIZE * 2)
#define TASK_STACK_SIZE_MEDIUM       (configMINIMAL_STACK_SIZE * 4)
#define TASK_STACK_SIZE_LARGE        (configMINIMAL_STACK_SIZE * 8)

/* Task priorities */
#define TASK_PRIORITY_HIGHEST        (tskIDLE_PRIORITY + 6)
#define TASK_PRIORITY_HIGH           (tskIDLE_PRIORITY + 5)
#define TASK_PRIORITY_MEDIUM         (tskIDLE_PRIORITY + 4)
#define TASK_PRIORITY_LOW            (tskIDLE_PRIORITY + 3)
#define TASK_PRIORITY_LOWEST         (tskIDLE_PRIORITY + 2)

/* Task periods in milliseconds */
#define TASK_PERIOD_5MS              5
#define TASK_PERIOD_10MS             10
#define TASK_PERIOD_100MS            100
#define TASK_PERIOD_200MS            200
#define TASK_PERIOD_500MS            500
#define TASK_PERIOD_1000MS           1000

/* === CAN Configuration === */

/* CAN baud rate */
#define CAN_DEFAULT_BAUD             500000U
#define CAN_DEFAULT_SP_PERCENT       75U
#define CAN_DEFAULT_SJW_TQ           2U

/* CAN message IDs */
#define CAN_ID_CELL_VOLTAGES_BASE    100
#define CAN_ID_GPIO_VALUES_BASE      103
#define CAN_ID_SOC_VALUES_BASE       106
#define CAN_ID_PACK_VOLTAGE          118
#define CAN_ID_PACK_CURRENT          119
#define CAN_ID_LINK_VOLTAGE          120
#define CAN_ID_TEMPERATURE           121
#define CAN_ID_MIN_SOC               122
#define CAN_ID_MAX_SOC               123
#define CAN_ID_AVG_SOC               124
#define CAN_ID_CONTACTOR_POSITIVE    125
#define CAN_ID_CONTACTOR_NEGATIVE    126
#define CAN_ID_CONTACTOR_PRECHARGE   127

/* CAN data lengths */
#define CAN_DLC_1BYTE                1U
#define CAN_DLC_2BYTES               2U
#define CAN_DLC_8BYTES               8U

/* === Buffer Sizes === */

/* ADBMS2950 register buffer sizes */
#define CELL_VREG_SIZE               (32 * TOTAL_IC)    // RDCVALL Size + padding
#define AUX_VREG_SIZE                (24 * TOTAL_IC)    // Total Aux2 reg size + padding

/* GPIO value buffer */
#define GPIO_VALUES_SIZE             (20 * TOTAL_IC)

/* === ADBMS6830 Configuration === */
#define TOTAL_AD_IC                  TOTAL_IC
#define TOTAL_AD68                   (TOTAL_AD_IC - 1)

#endif /* SOURCE_APP_BMS_INC_BMS_CONFIG_H_ */
