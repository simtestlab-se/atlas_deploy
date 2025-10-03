#include "mcu_wrapper.h"
#include "DAVE.h"
#include "stdlib.h"
#include "stdio.h"
#include "FreeRTOS.h"
#include "task.h"

#define CS_PORT			GPIOB
#define CS_PIN 			GPIO_PIN_1
uint8_t spi_rx[8];
uint32_t tx_status1 = 0;
uint32_t tx_status2 = 0;
uint32_t rx_status1 = 0;
uint32_t rx_status2 = 0;
// Constants
#define WAKE_DELAY 4U // Delay in milliseconds for wakeup signal
#define SPI_TIMEOUT 	1000 // Unlikely to fail but might still


void DWT_Init(void)
{
    // Enable TRC (Trace) so that DWT is accessible
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;

    // Reset the cycle counter
    DWT->CYCCNT = 0;

    // Enable the cycle counter
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
}


void Delay_us(uint32_t us)
{
    uint32_t cycles_per_us = SystemCoreClock / 1000000U;
    uint32_t start = DWT->CYCCNT;
    uint32_t delay_cycles = us * cycles_per_us;

    while ((DWT->CYCCNT - start) < delay_cycles);
}

void Delay_ms(uint32_t ms)
{
    uint32_t cycles_per_ms = SystemCoreClock / 1000U;
    uint32_t start = DWT->CYCCNT;
    uint32_t delay_cycles = ms * cycles_per_ms;

    while ((DWT->CYCCNT - start) < delay_cycles);
}



/**
 * @brief Writes data to SPI.
 *
 * <b>Details of function</b><br>
 * Clears SPI status flags, transmits data over SPI, and waits for transmission to complete.
 * Handles SPI status flags for data lost and transmit shift indication.
 *
 * @param tx_data Pointer to data buffer to transmit.
 * @param size Number of bytes to transmit.
 */
void spi_write(uint8_t *tx_data, uint16_t size) {

    for (uint16_t i = 0; i < size; i++) {
        if (i == size - 1) {
            // Mark EOF only on the LAST byte
            XMC_SPI_CH_EnableEOF(SPI_MASTER_0.channel);
        } else {
            XMC_SPI_CH_DisableEOF(SPI_MASTER_0.channel);
        }

        // Send single byte
        if (SPI_MASTER_Transmit(&SPI_MASTER_0, &tx_data[i], 1) == SPI_MASTER_STATUS_SUCCESS) {
            while (SPI_MASTER_0.runtime->tx_busy);
        }
    }


    // Disable EOF to restore default state
    XMC_SPI_CH_DisableEOF(SPI_MASTER_0.channel);
}


/**
 * @brief Reads data from SPI.
 *
 * <b>Details of function</b><br>
 * Clears SPI receive flags, sends dummy data to generate clock, and reads received data into buffer.
 * Waits for SPI transfer to complete and handles receive status flags.
 *
 * @param rx_data Pointer to buffer to store received data.
 * @param size Number of bytes to read.
 */
void spi_read(uint8_t *rx_data, uint16_t size) {

    uint8_t dummy_tx[size];
    memset(dummy_tx, 0x00, size);


    for (uint16_t i = 0; i < size; i++) {
        // Step 3: Enable EOF only on last byte
        if (i == size - 1) {
            XMC_SPI_CH_EnableEOF(SPI_MASTER_0.channel);
        } else {
            XMC_SPI_CH_DisableEOF(SPI_MASTER_0.channel);
        }

        uint8_t dummy = 0x00;

        // Full-duplex transfer: send dummy, receive real
        if (SPI_MASTER_Transfer(&SPI_MASTER_0, &dummy, &rx_data[i], 1U) == SPI_MASTER_STATUS_SUCCESS) {
            while (SPI_MASTER_0.runtime->tx_busy);
            while (SPI_MASTER_0.runtime->rx_busy);
        }
    }


    // Disable EOF after transfer is complete
    XMC_SPI_CH_DisableEOF(SPI_MASTER_0.channel);

}


/**
 * @brief Simultaneously writes and reads data over SPI.
 *
 * <b>Details of function</b><br>
 * Sends command bytes, then performs a full-duplex SPI transfer with dummy data to receive response.
 * Waits for SPI transfer to complete and handles receive status flags.
 *
 * @param tx_data Pointer to data buffer to transmit (first 4 bytes are command).
 * @param rx_data Pointer to buffer to store received data.
 * @param len Number of bytes to transfer after the command phase.
 */
void spi_read_write(uint8_t *tx_data, uint8_t *rx_data, uint16_t len) {
    // Step 1: Send 4 bytes (command phase)
    if (SPI_MASTER_Transmit(&SPI_MASTER_0, tx_data, 4) == SPI_MASTER_STATUS_SUCCESS) {
  	  while(SPI_MASTER_0.runtime->tx_busy)
  	          {
  	          }
    }

    for (uint16_t i = 0; i < len; i++) {
        uint8_t dummy = 0x00;

        // Enable EOF only on the last byte
        if (i == len - 1) {
            XMC_SPI_CH_EnableEOF(SPI_MASTER_0.channel);
        } else {
            XMC_SPI_CH_DisableEOF(SPI_MASTER_0.channel);
        }

        // Transmit dummy, receive into rx_data[i]
        if (SPI_MASTER_Transfer(&SPI_MASTER_0, &dummy, &rx_data[i], 1) == SPI_MASTER_STATUS_SUCCESS) {
            while (SPI_MASTER_0.runtime->tx_busy);
            while (SPI_MASTER_0.runtime->rx_busy);
        }
    }


    // Disable EOF after transfer is complete
    XMC_SPI_CH_DisableEOF(SPI_MASTER_0.channel);

}


/**
 * @brief Wakes up all BMS ICs in the daisy chain.
 *
 * <b>Details of function</b><br>
 * Toggles the chip select (CS) line low and high with a 4ms delay to wake up each IC in the chain.
 *
 * @param num_ic Number of ICs to wake up.
 */
void wakeup_chain(uint8_t num_ic) {
	FORIN(_x, num_ic){
        DIGITAL_IO_SetOutputLow(&SS_PIN_002); // CS low
        Delay_ms(4);
        DIGITAL_IO_SetOutputHigh(&SS_PIN_002); // CS high
        Delay_ms(4);
    }
}

/**
 * @brief Sets the SPI chip select (CS) line high.
 *
 * <b>Details of function</b><br>
 * Optionally delays before setting the CS line high.
 */
void spiCSHigh(void){
	Delay_us(10); // optional delay
	DIGITAL_IO_SetOutputHigh(&SS_PIN_002);
}

/**
 * @brief Sets the SPI chip select (CS) line low.
 *
 * <b>Details of function</b><br>
 * Pulls the CS line low to begin SPI communication.
 */
void spiCSLow(void){
	DIGITAL_IO_SetOutputLow(&SS_PIN_002);
	//Delay_us(50);
}

