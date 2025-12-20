/* * File: lcd_driver.c
 */

#include "lcd_driver.h"

// Define CPU Frequency for Delay (if not defined elsewhere)
#ifndef _XTAL_FREQ
#define _XTAL_FREQ 4000000
#endif

// Helper function to toggle Enable Pin
// 'static' means this function is private to this file (Encapsulation)
static void LCD_PulseEnable(void) {
    LCD_CTRL_PORT |= (1 << LCD_EN_PIN);   // Enable High
    __delay_us(5);                        // Short pulse
    LCD_CTRL_PORT &= ~(1 << LCD_EN_PIN);  // Enable Low
    __delay_us(100);                      // Wait for controller to process
}

void LCD_SendCmd(uint8_t cmd) {
    // 1. Set Control Pins
    LCD_CTRL_PORT &= ~(1 << LCD_RS_PIN);  // RS = 0 (Command)
    LCD_CTRL_PORT &= ~(1 << LCD_RW_PIN);  // RW = 0 (Write)
    
    // 2. Load Data
    LCD_DATA_PORT = cmd;
    
    // 3. Pulse Enable
    LCD_PulseEnable();
    
    // 4. Handle long commands (Clear & Home take ~2ms)
    if(cmd < 4) {
        __delay_ms(2);
    }
}

void LCD_SendData(uint8_t data) {
    LCD_CTRL_PORT |= (1 << LCD_RS_PIN);   // RS = 1 (Data)
    LCD_CTRL_PORT &= ~(1 << LCD_RW_PIN);  // RW = 0 (Write)
    
    LCD_DATA_PORT = data;
    
    LCD_PulseEnable();
}

void LCD_Init(void) {
    // Set Direction Registers (Output)
    LCD_DATA_TRIS = 0x00; 
    
    // Set Control Pins as Output using bitmasking
    LCD_CTRL_TRIS &= ~((1<<LCD_RS_PIN) | (1<<LCD_RW_PIN) | (1<<LCD_EN_PIN));
    
    LCD_CTRL_PORT = 0x00; // Reset State

    __delay_ms(20); // Wait for power up
    
    // Initialization Sequence
    LCD_SendCmd(LCD_CMD_FUNC_8BIT_2LINE);
    LCD_SendCmd(LCD_CMD_DISPLAY_ON);
    LCD_SendCmd(LCD_CMD_RETURN_HOME);
    LCD_SendCmd(LCD_CMD_CLEAR);
    LCD_SendCmd(LCD_CMD_ENTRY_MODE);
}

void LCD_SendString(const char* str) {
    while(*str) {
        LCD_SendData(*str++);
    }
}
