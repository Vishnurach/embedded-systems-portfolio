#ifndef UART_H_
#define UART_H_

#include <LPC17xx.h>

void UART0_Init(void);
void UART0_TxChar(char ch);
void UART0_TxString(char *str);

#endif /* UART_H_ */
