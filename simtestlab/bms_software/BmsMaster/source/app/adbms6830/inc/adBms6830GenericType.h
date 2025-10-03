/** @addtogroup BMS_DRIVER
*  @{
*
*/

/** @addtogroup GENERIC_TYPE GENERIC TYPE
*  @{
*
*/
#ifndef __adBmsGenericType_H
#define __adBmsGenericType_H

#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#define TX_DATA 		6
#define RX_DATA 		8

/**
 * @brief Calculates the CRC15 Packet Error Code (PEC) for a data buffer.
 *
 * @param len Number of bytes to use for PEC calculation.
 * @param data Pointer to the data buffer.
 * @return Calculated 16-bit CRC15 PEC value.
 */
uint16_t Pec15_Calc(uint8_t len, uint8_t *data);

/**
 * @brief Calculates the CRC10 Packet Error Code (PEC) for a data buffer.
 *
 * @param rx_cmd Set true if called for a received command, false otherwise.
 * @param len Number of bytes to use for PEC calculation.
 * @param data Pointer to the data buffer.
 * @return Calculated 10-bit CRC PEC value.
 */
uint16_t pec10_calc(bool rx_cmd, int len, uint8_t *data);

/**
 * @brief Sends a command over SPI to the BMS ICs.
 *
 * @param tx_cmd Pointer to the 2-byte command array to send.
 */
void spiSendCmd(uint8_t tx_cmd[2]);

/**
 * @brief Reads data from BMS ICs over SPI after sending a command.
 *
 * @param tIC Number of ICs in the daisy chain.
 * @param tx_cmd Pointer to the 2-byte command array to send.
 * @param rx_data Pointer to buffer to store received data.
 * @param pec_error Pointer to variable to store PEC error flags.
 * @param cmd_cntr Pointer to buffer to store command counters.
 * @param regData_size Number of bytes per register to read.
 */
void spiReadData(
    uint8_t tIC,
    uint8_t tx_cmd[2],
    uint8_t *rx_data,
    uint32_t *pec_error,
    uint8_t *cmd_cntr,
    uint8_t regData_size
);

/**
 * @brief Writes data to BMS ICs over SPI.
 *
 * @param tIC Number of ICs in the daisy chain.
 * @param tx_cmd Pointer to the 2-byte command array to send.
 * @param data Pointer to the data buffer to write.
 */
void spiWriteData(
    uint8_t tIC,
    uint8_t tx_cmd[2],
    uint8_t *data
);

/*
void adBmsReadData(uint8_t tIC, cell_asic *ic, uint8_t cmd_arg[2], TYPE type, GRP group);
void adBmsWriteData(uint8_t tIC, cell_asic *ic, uint8_t cmd_arg[2], TYPE type, GRP group);
uint32_t adBmsPollAdc(uint8_t tx_cmd[2]);
void adBms6830_Adcv
(
  RD rd,
  CONT cont,
  DCP dcp,
  RSTF rstf,
  OW_C_S owcs
);

void adBms6830_Adsv
(
  CONT cont,
  DCP dcp,
  OW_C_S owcs
);

void adBms6830_Adax
(
OW_AUX owaux, 							
PUP pup,
CH ch
);

void adBms6830_Adax2
(
  CH ch
);
*/
#endif
/** @}*/
/** @}*/
