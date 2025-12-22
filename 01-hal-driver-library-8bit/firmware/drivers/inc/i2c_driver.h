/*
 * File        : i2c_driver.h
 * Author      : Vishnu
 * Description : I2C Master Driver for PIC16F876A
 * Toolchain   : XC8
 * Hardware    : PIC16F876A (XT Osc @ 4MHz)
 * Protocol    : I2C Master Mode @ 100kHz
 */

#ifndef I2C_DRIVER_H
#define I2C_DRIVER_H

#include <xc.h>
#include <stdint.h>

/**
 * @brief Initialize the I2C Module as Master.
 * @details Configures RC3 (SCL) and RC4 (SDA), sets the SSP module
 * to Master Mode, and sets baud rate to 100kHz (based on 4MHz Fosc).
 */
void I2C_Init(void);

/**
 * @brief Idle the I2C bus.
 * @details Blocks until the bus is idle (no Start/Stop/ACK sequence active)
 * and no transmission is in progress.
 */
void I2C_Wait(void);

/**
 * @brief Generate I2C Start Condition.
 */
void I2C_Start(void);

/**
 * @brief Generate I2C Stop Condition.
 */
void I2C_Stop(void);

/**
 * @brief Generate I2C Restart Condition.
 * @details Used for switching direction (Write -> Read) without releasing the bus.
 */
void I2C_Restart(void);

/**
 * @brief Send Acknowledge (ACK) bit.
 * @details Initiated by Master after receiving a byte.
 */
void I2C_Ack(void);

/**
 * @brief Send Not Acknowledge (NACK) bit.
 * @details Initiated by Master to signal end of read sequence.
 */
void I2C_Nak(void);

/**
 * @brief Write a single byte to the I2C bus.
 * @param data The 8-bit data to transmit.
 * @return 0 if ACK received (Device found/Ready), 1 if NACK received.
 */
int I2C_Write(uint8_t data);

/**
 * @brief Read a single byte from the I2C bus.
 * @return The 8-bit data received from the slave.
 */
uint8_t I2C_Read(void);

#endif