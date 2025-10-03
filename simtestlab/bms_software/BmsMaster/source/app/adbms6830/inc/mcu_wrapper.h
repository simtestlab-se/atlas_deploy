#ifndef SRC_MCU_WRAPPER_H_
#define SRC_MCU_WRAPPER_H_
#include <stdint.h>

#define u16 				uint16_t
#define u8 					uint8_t
#define f32 				float
#define f64 				double


#define FORIN(x,y)         for(int x = 0; x< y; x++) //This is a lazy define that I like


void DWT_Init(void);
void Delay_us(uint32_t us);
void Delay_ms(uint32_t ms);
/**
 * @brief Writes data to SPI.
 *
 * @param tx_data Pointer to data to transmit.
 * @param size Number of bytes to transmit.
 */
void spi_write(u8 * tx_data, u16 size);

/**
 * @brief Reads data from SPI.
 *
 * @param rx_data Pointer to buffer to store received data.
 * @param size Number of bytes to read.
 */
void spi_read(u8 * rx_data, u16 size);

/**
 * @brief Simultaneously writes and reads data over SPI.
 *
 * @param tx_data Pointer to data to transmit.
 * @param rx_data Pointer to buffer to store received data.
 * @param len Number of bytes to transfer.
 */
void spi_read_write(u8 * tx_data, u8 * rx_data, u16 len);

/**
 * @brief Wakes up all BMS ICs in the daisy chain.
 *
 * @param count Number of ICs to wake up.
 */
void wakeup_chain(u8 count);

/**
 * @brief Sets the SPI chip select line high.
 */
void spiCSHigh(void);

/**
 * @brief Sets the SPI chip select line low.
 */
void spiCSLow(void);



#endif /* SRC_MCU_WRAPPER_H_ */
