/* * File: main.c
 * Description: Portfolio Project 1 - HAL Demo
 */

#include <xc.h>
#include "lcd_driver.h"

// Configuration Bits (Ideally in a separate config_bits.h)
#pragma config FOSC = HS
#pragma config WDTE = OFF
#pragma config PWRTE = OFF
#pragma config BOREN = OFF
#pragma config LVP = OFF

void main(void) {
    // 1. Hardware Initialization
    LCD_Init();

    // 2. Application Logic
    LCD_SendString("Vishnu's HAL");
    
    LCD_SendCmd(0xC0); // Move to 2nd line (Address 0x40 + 0x80)
    LCD_SendString("Portfolio Demo");

    while(1) {
        // Main loop is now empty or free for other tasks
        // In a real system, we might blink a heartbeat LED here
    }
}