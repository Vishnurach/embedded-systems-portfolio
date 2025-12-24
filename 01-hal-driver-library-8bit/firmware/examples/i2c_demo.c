/*
 * File        : i2c_demo.c
 * Author      : Vishnu
 * Description : I2C Driver Verification using DS1307 RTC
 * Hardware    : PIC16F876A, DS1307 Real Time Clock
 * Flow        : 1. Init UART & I2C
 * 2. Reset RTC time to 00:00:00
 * 3. Loop: Read time and print via UART
 */

#include <xc.h>
#include <stdio.h>
#include <string.h>
#include "../config.h"
#include "../drivers/inc/i2c_driver.h"
#include "../drivers/inc/uart_driver.h"

void main() 
{
    char arr[20];
    int len;
    int h=0, m=0, s=0;

    // Initialize Drivers
    UART_Init();
    I2C_Init();       // Initializes I2C @ 100kHz

    // Initial Setup for DS1307 (Write Time 00:00:00)
    I2C_Start();
    
    // Poll device until it ACKs (Device Ready Check)
    // 0xD0 is the Write Address for DS1307
    while(I2C_Write(0xD0) == 1) 
    {
        I2C_Restart();
    }
    
    I2C_Write(0x00); // Register Pointer 0 (Seconds)
    I2C_Write(0x00); // Set Seconds
    I2C_Write(0x00); // Set Minutes
    I2C_Write(0x00); // Set Hours
    I2C_Stop();

    while(1)
    {
        // 1. Set Register Pointer to 0 to begin read
        I2C_Start();
        while(I2C_Write(0xD0) == 1)
        {
             I2C_Restart();
        }
        I2C_Write(0x00);
        
        // 2. Read Time (Seconds, Minutes, Hours)
        I2C_Restart();
        I2C_Write(0xD1);        // Read Address (0xD0 | 1)
        
        s = I2C_Read();
        I2C_Ack();              // ACK to request next byte
        
        m = I2C_Read();
        I2C_Ack();              // ACK to request next byte
        
        h = I2C_Read();
        I2C_Nak();              // NACK to signal end of read
        
        I2C_Stop();
        
        // 3. Display via UART (Formatted as HH:MM:SS)
        UART_SendHex(h);
        UART_SendChar(':');
        UART_SendHex(m);
        UART_SendChar(':');
        UART_SendHex(s);
        UART_SendChar('\r');
        
        __delay_ms(1000);
    }  
}