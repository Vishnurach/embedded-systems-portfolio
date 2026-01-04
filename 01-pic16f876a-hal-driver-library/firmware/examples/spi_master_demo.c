/**
 * @file    spi_master_demo.c
 * @brief   Master Application: UART Bridge -> SPI
 */

#include <xc.h>
#include <stdint.h>
#include "../config.h"
#include "../drivers/inc/spi_driver.h"
#include "../drivers/inc/uart_driver.h"

void main(void)
{
    /* 1. Initialize Drivers */
    SPI_Init_Master();
    UART_Init();          // <--- Sets Baud 9600, 8N1

    while (1)
    {
        /* 2. Receive from PC (Blocking) */
        /* Note: UART_ReceiveChar handles OERR internally */
        uint8_t rx_data = UART_ReceiveChar();

        /* 3. Echo back to PC (Optional confirmation) */
        UART_SendChar(rx_data);

        /* 4. Send to Slave via SPI */
        SPI_Write(rx_data);
        SPI_Wait();
    }
}