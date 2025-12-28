/*
 * File        : uart_driver.c
 * Description : UART0 driver (9600 baud).
 */

#include <LPC214X.h>
#include <string.h>
#include <stdio.h>
#include "uart_driver.h"
#include "system_init.h" // Provides access to current_password

void UART_Init(void)
{
    // Configure P0.8 (Tx) and P0.9 (Rx)
    PINSEL0 &= ~(0x000F0000);
    PINSEL0 |=  0x00050000;

    // Line Control: 8-bit, No Parity, 1 Stop bit, DLAB=1
    U1LCR = 0x83;  
    U1DLL = 0x87;  // Baud Rate: 9600 @ 60MHz PCLK
    U1DLM = 0x01;  
    U1LCR = 0x03;  // Disable DLAB
    U1FCR = 0x07;  // Enable FIFO
}

void UART_SendChar(char a)
{
    U1THR = a; 
    while((U1LSR & (1<<5)) == 0); // Wait for THR empty
}

void UART_SendString(char *str)
{
    while(*str) UART_SendChar(*str++);
}

char* UART_ReceiveString(void)
{
    static char input[16]; // Increased safety size
    int i = 0;
    char received_char;

    while(1)
    { 
        // Wait for Data Ready
        while((U1LSR & 0x01) == 0);  
        received_char = U1RBR; 
        
        // Handle Backspace (\b)
        if(received_char == '\b') {
            if(i > 0) { 
                i--; 
                UART_SendChar('\b'); // Move cursor back
                UART_SendChar(' ');  // Erase char
                UART_SendChar('\b'); // Move cursor back again
                input[i] = '\0';     // Clear from buffer
            }
            continue;
        }
        
        // Check for Enter key (\r or \n)
        if(received_char == '\r' || received_char == '\n') { 
            input[i] = '\0'; 
            break; 
        }

        // Store char ONLY if buffer has space (Limit to 14 chars)
        if(i < 14) {
            UART_SendChar(received_char); // Echo
            input[i] = received_char;
            i++;
        }
    }
    UART_SendChar('\n'); // New line after input
    return input;
}

void UART_CheckPassword(void)
{
    char *input;
    int count = 0;
    int i;
    char arr[10];

    while(current_state == 0)
    {
        UART_SendString("\r\n[LOCKED] Enter Password: ");
        input = UART_ReceiveString();
        
        if(strcmp(input, current_password) == 0) {
            UART_SendString("\r\nAccess Granted\r\n");
            current_state = 1;
            return;
        } else {
            UART_SendString("\r\nIncorrect Password.\r\n");
            count++;
        }
        
        // Lockout Logic (3 attempts)
        if(count >= 3) {
            UART_SendString("System Locked. Wait...\r\n");
            for(i = 3; i > 0; i--) {
                sprintf(arr, "%d.. ", i);
                UART_SendString(arr);
                delayms(1000);
            }
            count = 0;
            UART_SendString("\r\nReady.\r\n");
        }
    }
}
