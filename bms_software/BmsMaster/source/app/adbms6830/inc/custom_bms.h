#ifndef INC_CUSTOM_BMS_H_
#define INC_CUSTOM_BMS_H_

#include "mcu_wrapper.h"
#include "DAVE.h"

extern uint8_t pwm_data[6];

/**
 * @brief Configures the BMS ICs with required settings.
 *
 * <b>Details of function</b><br>
 * Writes configuration registers to all BMS ICs in the chain.
 *
 * @return 0 on success, negative on error.
 */
int configBMS(void);

/**
 * @brief Requests a cell voltage measurement from the BMS ICs.
 *
 * <b>Details of function</b><br>
 * Initiates a cell voltage conversion on all BMS ICs.
 *
 * @return 0 on success, negative on error.
 */
int requestCellVotlage(void);

/**
 * @brief Polls cell voltage data from the BMS ICs.
 *
 * <b>Details of function</b><br>
 * Reads cell voltage registers from all BMS ICs and stores the result in the provided buffer.
 *
 * @param[out] buf Buffer to store cell voltage data.
 * @return 0 on success, negative on error.
 */
int pollCellVoltage(u8* buf);

/**
 * @brief Requests auxiliary voltage measurement from the BMS ICs.
 *
 * <b>Details of function</b><br>
 * Initiates an auxiliary voltage conversion on all BMS ICs.
 *
 * @return 0 on success, negative on error.
 */
int requestAuxVoltage(void);

/**
 * @brief Polls auxiliary voltage data from the BMS ICs.
 *
 * <b>Details of function</b><br>
 * Reads auxiliary voltage registers from all BMS ICs and stores the result in the provided buffer.
 *
 * @param[out] buf Buffer to store auxiliary voltage data.
 * @return 0 on success, negative on error.
 */
int pollAuxVoltage(u8* buf);

/**
 * @brief Reads configuration registers from the BMS ICs.
 *
 * <b>Details of function</b><br>
 * Reads configuration data from all BMS ICs for verification or diagnostics.
 *
 * @return 0 on success, negative on error.
 */
int readCFG(void);

/**
 * @brief Reads the SID (Status/ID) register from the BMS ICs.
 *
 * <b>Details of function</b><br>
 * Reads the SID register from all BMS ICs for identification or diagnostics.
 *
 * @return 0 on success, negative on error.
 */
int readSID(void);

/**
 * @brief Reads status and error registers from the BMS ICs.
 *
 * <b>Details of function</b><br>
 * Reads status and error information from all BMS ICs for fault detection.
 *
 * @return 0 on success, negative on error.
 */
int readStatErr(void);

/**
 * @brief Sets PWM values for cell balancing.
 *
 * <b>Details of function</b><br>
 * Writes PWM data to the BMS ICs to enable or disable cell balancing.
 *
 * @return 0 on success, negative on error.
 */
int PWMforBalancing(void);



#endif /* INC_CUSTOM_BMS_H_ */
