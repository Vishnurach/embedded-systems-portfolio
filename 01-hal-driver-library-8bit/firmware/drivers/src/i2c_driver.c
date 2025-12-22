/*
 * File        : i2c_driver.c
 * Author      : Vishnu
 * Description : HAL driver implementation for PIC16F876A I2C module
 * Clock       : 4 MHz (Calculated for 100kHz I2C Baud)
 */

#include "i2c_driver.h"
#include "config.h" 

void I2C_Init(void)
{
    // 1. Configure Pins
    TRISC3 = 1;      // RC3 = SCL (Input for I2C master)
    TRISC4 = 1;      // RC4 = SDA (Input for I2C master)
    
    // 2. Configure I2C Module
    SSPSTAT = 0x80;  // SMP=1: Slew rate disabled (Standard Speed 100kHz)
    SSPCON = 0x28;   // SSPEN=1: Enable MSSP, SSPM=1000: I2C Master mode
    
    // 3. Set Baud Rate
    // Formula: (FOSC / (4 * Baud)) - 1
    // FOSC = 4,000,000 Hz, Baud = 100,000 Hz
    // (4000000 / 400000) - 1 = 9
    SSPADD = 9;     
}

void I2C_Wait(void)
{
    // Wait for Start, Stop, Ack sequences to complete (SSPCON2 bits 0-4)
    // Wait for Transmit in progress (SSPSTAT bit 2 - R_W)
    while ((SSPCON2 & 0x1F) || (SSPSTAT & 0x04));
}

void I2C_Start(void)
{
    I2C_Wait();
    SEN = 1;         // Initiate Start condition
}

void I2C_Stop(void)
{
    I2C_Wait();
    PEN = 1;         // Initiate Stop condition
}

void I2C_Restart(void)
{
    I2C_Wait();
    RSEN = 1;        // Initiate Repeated Start condition
}

void I2C_Ack(void)
{
    I2C_Wait();
    ACKDT = 0;       // 0 = Acknowledge
    ACKEN = 1;       // Initiate Acknowledge sequence
}

void I2C_Nak(void)
{
    I2C_Wait();
    ACKDT = 1;       // 1 = Not Acknowledge
    ACKEN = 1;       // Initiate Acknowledge sequence
}

int I2C_Write(uint8_t data)
{
    I2C_Wait();
    SSPBUF = data;   // Load data into buffer
    while(!SSPIF);   // Wait for interrupt flag to signal completion
    SSPIF = 0;       // Clear flag
    return ACKSTAT;  // Return ACK Status (0=ACK, 1=NACK)
}

uint8_t I2C_Read(void)
{
    I2C_Wait();
    RCEN = 1;        // Enable Receive mode
    while(!SSPIF);   // Wait for byte to be received
    SSPIF = 0;       // Clear flag
    I2C_Wait();
    return SSPBUF;   // Return received byte
}