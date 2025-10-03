/*
 * global_db.h
 *
 *  Created on: Jun 21, 2025
 *      Author: akil
 */

#ifndef GLOBAL_DB_H_
#define GLOBAL_DB_H_

#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "bms_config.h"

extern float cellValues[TOTAL_CELLS];
extern float gpio_tempValues[TOTAL_GPIO_CHANNELS];
extern float pack_current ;
extern float ntc_temperatures_c[NUM_NTC_CHANNELS] ;
extern float link_voltage ;
extern float pack_voltage ;
extern bool cell_balance ;
extern bool Positive_contactor ;
extern bool Negative_contactor ;
extern bool Precharge_contactor ;
extern bool contactor_req;
extern bool contactor_err_reset_req;

extern float min_soc;
extern float max_soc;
extern float average_soc;
extern float soc[TOTAL_CELLS];
extern uint32_t current_timestamp;

extern bool violation_overvoltage;        // Overvoltage
extern bool violation_undervoltage;       // Undervoltage
extern bool violation_overtemperature;    // Overtemperature
extern bool violation_undertemperature;   // Undertemperature
extern bool violation_overcurrent;        // Overcurrent
extern bool violation_cell_imbalance;     // Cell Imbalance

extern bool key_status;

extern float load_current_ch1;
extern float load_current_ch2;


#endif /* GLOBAL_DB_H_ */
