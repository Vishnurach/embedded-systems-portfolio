/**
 * @file    spi_driver.h
 * @author  Vishnu
 * @brief   SPI Driver Header with Configuration Macros
 * @mcu     PIC16F877A / PIC16 Series
 */

#ifndef SPI_DRIVER_H
#define SPI_DRIVER_H

#include <xc.h>
#include <stdint.h>

/* ================= Configuration Macros ================= */

/* SSPCON Register Bits */
#define SPI_ENABLE          0x20  // SSPEN: Synchronous Serial Port Enable
#define SPI_MASTER_FOSC4    0x00  // Master mode, clock = Fosc/4
#define SPI_SLAVE_SS_DIS    0x05  // Slave mode, SS pin disabled (0101)

/* ================= Initialization APIs ================= */

/**
 * @brief Initialize SPI as Master
 * @note  Configures SCK/SDO as output, SDI as input. Fosc/4.
 */
void SPI_Init_Master(void);

/**
 * @brief Initialize SPI as Slave
 * @note  Configures SCK/SDI as input. SS pin is DISABLED (3-wire mode).
 */
void SPI_Init_Slave(void);

/* ================= Operation APIs ================= */

/**
 * @brief Load data into SSPBUF (Non-blocking write)
 * @param data Byte to transmit
 */
void SPI_Write(uint8_t data);

/**
 * @brief Wait for transaction to complete (Blocking)
 * @note  Polls SSPIF and clears it upon completion.
 */
void SPI_Wait(void);

#endif /* SPI_DRIVER_H */