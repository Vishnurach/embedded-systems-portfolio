/*
 * File        : uart_driver.c
 * Description : UART driver implementation (Polling-based)
 * Toolchain   : XC8
 */

#include "uart_driver.h"
#include <stddef.h>

/* ================= Initialization ================= */

void UART_Init(void)
{
    /* Configure UART I/O pins */
    TRISCbits.TRISC7 = 1U;    /* RX as input */
    TRISCbits.TRISC6 = 0U;    /* TX as output */

    /* Configure transmitter */
    TXSTAbits.SYNC = 0U;      /* Asynchronous mode */
    TXSTAbits.BRGH = 1U;      /* High-speed baud rate */
    TXSTAbits.TXEN = 1U;      /* Enable transmitter */

    /* Configure receiver */
    RCSTAbits.SPEN = 1U;      /* Enable serial port */
    RCSTAbits.CREN = 1U;      /* Enable continuous reception */

    /* Baud rate configuration
     * FOSC = 4 MHz, Baud = 9600, BRGH = 1
     * SPBRG = 25
     */
    SPBRG = 25U;
}

/* ================= Transmit APIs ================= */

void UART_SendChar(uint8_t data)
{
    TXREG = data;

    /* Wait until transmit shift register is empty */
    while (TXSTAbits.TRMT == 0U)
    {
        /* Blocking wait */
    }
}

void UART_SendString(const char *str)
{
    while (*str != '\0')
    {
        UART_SendChar((uint8_t)(*str));
        str++;
    }
}

/* ================= Receive APIs ================= */

uint8_t UART_ReceiveChar(void)
{
    /* Wait for data to be received */
    while (PIR1bits.RCIF == 0U)
    {
        /* Blocking wait */
    }

    /* Handle overrun error */
    if (RCSTAbits.OERR == 1U)
    {
        RCSTAbits.CREN = 0U;
        RCSTAbits.CREN = 1U;
    }

    return (uint8_t)RCREG;
}

void UART_ReceiveString(char *buffer, uint16_t max_len)
{
    uint16_t index = 0U;
    uint8_t ch;

    if ((buffer == NULL) || (max_len == 0U))
    {
        return;
    }

    while (1)
    {
        ch = UART_ReceiveChar();

        /* Stop reception on Enter key */
        if ((ch == '\r') || (ch == '\n'))
        {
            break;
        }

        /* Store character if space is available */
        if (index < (max_len - 1U))
        {
            buffer[index] = (char)ch;
            index++;
        }
        /* Else: ignore extra characters until Enter */
    }

    buffer[index] = '\0';   /* Null-terminate string */
}

void UART_SendHex(unsigned char val) 
{
    // Convert Upper Nibble
    unsigned char nibble = (val >> 4) & 0x0F;
    UART_SendChar((nibble <= 9) ? (nibble + '0') : (nibble - 10 + 'A'));
    
    // Convert Lower Nibble
    nibble = val & 0x0F;
    UART_SendChar((nibble <= 9) ? (nibble + '0') : (nibble - 10 + 'A'));
}
