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
#define SPI_ENABLE          0x20  /* SSPEN bit */
#define SPI_MASTER_FOSC4    0x00  /* FOSC/4 Speed */
#define SPI_SLAVE_SS_DIS    0x05  /* Slave mode, SS disabled */

/* ================= Function Prototypes ================= */
void SPI_Init_Master(void);
void SPI_Init_Slave(void);

/**
 * @brief Sends a byte and returns the received byte (Full Duplex)
 * @param data Byte to send
 * @return Byte received
 */
uint8_t SPI_Exchange(uint8_t data);

/* Legacy/Helper functions */
void SPI_Write(uint8_t data);
void SPI_Wait(void);
uint8_t SPI_Read(void);

#endif