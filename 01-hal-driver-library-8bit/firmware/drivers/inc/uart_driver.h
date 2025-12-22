/*
 * File        : uart_driver.h
 * Author      : Vishnu
 * Description : Polling-based UART Hardware Abstraction Layer (HAL)
 * Toolchain   : XC8
 * Notes       : Blocking TX/RX, no interrupts
 */

#ifndef UART_DRIVER_H
#define UART_DRIVER_H

#include <xc.h>
#include <stdint.h>

/* ================= Configuration ================= */

/* RX buffer size (used by application, not internal static storage) */
#define UART_RX_BUF_SIZE   20U

/* ================= Public API ================= */

void UART_Init(void);
void UART_SendChar(uint8_t data);
void UART_SendString(const char *str);
uint8_t UART_ReceiveChar(void);
void UART_ReceiveString(char *buffer, uint16_t max_len);
void UART_SendHex(unsigned char val);

#endif /* UART_DRIVER_H */
