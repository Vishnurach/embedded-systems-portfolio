/*
 * File        : main.c
 * Description : Portfolio Project 1 – UART HAL Demo (Echo Application)
 * Toolchain   : XC8
 */

#include "config.h"        /* Device configuration & clock settings */
#include <xc.h>
#include "uart_driver.h"

void main(void)
{
    /* Local RX buffer owned by application */
    char rxBuffer[UART_RX_BUF_SIZE];

    /* Initialize UART HAL */
    UART_Init();

    /* Startup message */
    UART_SendString("System Initialized. Type something:\r\n");

    while (1)
    {
        /* Blocking receive until Enter key */
        UART_ReceiveString(rxBuffer, UART_RX_BUF_SIZE);

        /* Echo received data */
        UART_SendString("Received: ");
        UART_SendString(rxBuffer);
        UART_SendString("\r\n");
    }
}
