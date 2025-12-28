
#ifndef UART_DRIVER_H
#define UART_DRIVER_H

#include<LPC214X.h>
#include <stdint.h>
#include "system_init.h"

void UART_Init(void);
void UART_SendChar(char a);
void UART_SendString(char *str);
char* UART_ReceiveString(void);
void UART_CheckPassword(void);

#endif

