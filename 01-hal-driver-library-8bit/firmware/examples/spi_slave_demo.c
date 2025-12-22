/**
 * @file    spi_slave_demo.c
 * @brief   Slave Application: LED Control with Responsive Logic
 */

#include <xc.h>
#include <stdint.h>
#include "config.h"
#include "spi_driver.h"
#include "uart_driver.h" // <--- Uses your uploaded driver

/* Shared state variables */
volatile uint8_t system_state = 0;
volatile uint8_t uart_echo_pending = 0;

/* ================= ISR ================= */
void __interrupt() isr(void)
{
    if (SSPIF)
    {
        /* Robustness: Clear SPI Overflow if occurred */
        if (SSPCONbits.SSPOV)
        {
            SSPCONbits.SSPOV = 0;
        }

        /* Read data (clears BF flag) and flag main loop */
        system_state = SSPBUF;   
        uart_echo_pending = 1;   

        SSPIF = 0;
    }
}

void main(void)
{
    /* 1. GPIO Init */
    TRISB = 0x00; // LEDs
    PORTB = 0x00;

    /* 2. Driver Init */
    SPI_Init_Slave();
    UART_Init();      // <--- Sets Baud 9600

    /* 3. Enable Interrupts */
    GIE  = 1;
    PEIE = 1;
    SSPIE = 1;

    while (1)
    {
        /* Handle UART Echo using your Driver (Non-blocking context) */
        if (uart_echo_pending)
        {
            UART_SendChar(system_state); 
            uart_echo_pending = 0;
        }

        /* ================= Application Logic ================= */

        if (system_state == 'T')
        {
            /* Sequence Pattern */
            for (int i = 0; i < 8; i++)
            {
                if (system_state != 'T') break; // Responsive Exit

                PORTB = (1 << i);
                __delay_ms(1000);
            }
        }
        else if (system_state == 'B')
        {
            /* Blink Pattern */
            PORTB = 0x00;
            __delay_ms(1000);

            if (system_state == 'B') // Responsive Check
            {
                PORTB = 0xFF;
                __delay_ms(1000);
            }
        }
        else
        {
            /* Default: LEDs Off */
            PORTB = 0x00;
        }
    }
}