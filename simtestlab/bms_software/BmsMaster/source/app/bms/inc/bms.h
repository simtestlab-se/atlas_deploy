#ifndef BMS_H
#define BMS_H

#include <stdint.h>
#include <stdbool.h>
#include "DAVE.h"
#include "bms_config.h"

/* === Extern globals === */
extern float cellValues[TOTAL_CELLS];
extern float gpio_tempValues[TOTAL_GPIO_CHANNELS];
extern int16_t cell_values[TOTAL_CELLS];
extern float pack_voltage;
extern float temp ;

extern bool contactor_open_req ;
extern bool contactor_close_req ;

extern uint8_t cellVReg[CELL_VREG_SIZE];
extern uint8_t auxVReg[AUX_VREG_SIZE];

extern float pack_current;
extern float ntc_temperatures_c[NUM_NTC_CHANNELS];
extern uint16_t adc_temp_raw[NUM_NTC_CHANNELS];
extern float raw_temp[NUM_NTC_CHANNELS];
extern float last_temp[NUM_NTC_CHANNELS];
extern ADC_MEASUREMENT_CHANNEL_t* ntc_adc_channels[NUM_NTC_CHANNELS];
extern float link_voltage;
extern uint16_t link_adc_raw;
extern bool cell_balance;
extern bool Positive_contactor;
extern bool Negative_contactor;
extern bool Precharge_contactor;
extern uint8_t pwm_data[6];
extern float soc[TOTAL_CELLS];
extern float min_soc;
extern float max_soc;
extern float average_soc;
extern float pack_sum_voltage;
extern uint16_t gpio_values[GPIO_VALUES_SIZE];
extern uint16_t soc_scaled;

/* === Initialization Function === */
bool Bms_Init(void);

/* === Task prototypes === */
void TwinCATTask(void *pvParameters);
void TaskBms100ms(void *pvParameters);
void ContactorTask(void *pvParameters);
void TaskBmsCAN(void *pvParameters);
void TaskCellBal(void *pvParameters);
void TaskBms1s(void *pvParameters);
void Task_VehicleLoad(void *pvParameters);

/* === Core functions === */
void BmsVoltage(void);
void PackVoltage(void);
void LinkVoltage(void);
void NTCTemperature(void);
void Contactorcntl(void);
void BmsCAN(void);
void cellbalance(void);
void app_log_status(uint32_t node_status, uint8_t rx_err, uint8_t tx_err);
int voltage_loop(void);
void parse_print_cell_measurement(uint8_t* buff);
void parse_print_gpio_measurement(uint8_t* buff);

/* CAN send helpers */
void CAN_SendU16(uint32_t id, uint16_t value);
void CAN_SendBool(uint32_t id, bool value);

/* Data group senders */
void CAN_SendCellVoltages(void);
void CAN_SendGpioValues(void);
void CAN_SendSocValues(void);
void CAN_SendContactors(void);

float Read_Current_ch1(void);
float Read_Current_ch2(void);

float calculate_temperature_from_adc(uint32_t adc_raw);
float apply_ema_filter(uint8_t i, float new_temp);

#endif // BMS_H
