/*
 * Eth_cfg.h
 *
 *  Created on: Jun 3, 2025
 *      Author: akil
 */

#ifndef ETH_CFG_H_
#define ETH_CFG_H_

#include "DAVE.h"
#include "bms_config.h"

extern int16_t cell_values[TOTAL_CELLS];
extern uint16_t link_adc_raw ;
extern float pack_voltage ;
extern float pack_current;
extern uint16_t adc_temp_raw[NUM_NTC_CHANNELS];
extern bool cell_balance ;
extern bool Positive_contactor ;
extern bool Negative_contactor ;
extern bool Precharge_contactor ;
extern float soc[TOTAL_CELLS];
extern float min_soc;
extern float max_soc;
extern float average_soc;
extern bool contactor_req;
extern bool key_status;
extern float load_current_ch1;
extern float load_current_ch2;

#endif /* ETH_CFG_H_ */
