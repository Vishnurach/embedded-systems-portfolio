/*
 * File: examples/spi_loopback_demo.c
 * Description: Single-Chip SPI Driver Verification
 * Hardware: Connect wire between RC5 (SDO) and RC4 (SDI)
 */

#include <xc.h>
#include "../config.h"
#include "../drivers/inc/uart_driver.h"
#include "../drivers/inc/spi_driver.h"


void main(void)
{
    uint8_t tx_char = 'A';
    uint8_t rx_char;

    /* 1. Initialize Drivers */
    UART_Init();
    
    // Initialize as MASTER for the loopback to generate the clock
    SPI_Init_Master();

    UART_SendString("=== SPI Loopback Test ===\r\n");
    UART_SendString("Connect RC5 (SDO) to RC4 (SDI)\r\n");

    while (1)
    {
        /* 2. Send 'A' via SPI. 
         * Since SDO is connected to SDI, the hardware receives 'A' simultaneously.
         */
        rx_char = SPI_Exchange(tx_char);

        /* 3. Verify results via UART */
        UART_SendString("Sent: ");
        UART_SendChar(tx_char);
        UART_SendString(" | Rcvd: ");
        UART_SendChar(rx_char);
        UART_SendString("\r\n");

        /* 4. Prepare next char */
        tx_char++;
        if (tx_char > 'Z') tx_char = 'A';

        __delay_ms(500);
    }
}