/* * File: lcd_driver.h
 * Author: Vishnu
 * Description: HAL for HD44780 LCD (4-bit/8-bit modes)
 */

#ifndef LCD_DRIVER_H
#define LCD_DRIVER_H

#include <xc.h>
#include <stdint.h> // Use standard types (uint8_t)

// ==========================================
// HARDWARE CONFIGURATION (Change these only)
// ==========================================
#define LCD_DATA_PORT      PORTC
#define LCD_DATA_TRIS      TRISC
#define LCD_CTRL_PORT      PORTB
#define LCD_CTRL_TRIS      TRISB

#define LCD_RS_PIN         0  // RB0
#define LCD_RW_PIN         1  // RB1
#define LCD_EN_PIN         2  // RB2

// ==========================================
// COMMAND DEFINITIONS (No more Magic Numbers)
// ==========================================
#define LCD_CMD_CLEAR           0x01
#define LCD_CMD_RETURN_HOME     0x02
#define LCD_CMD_ENTRY_MODE      0x06
#define LCD_CMD_DISPLAY_ON      0x0C
#define LCD_CMD_FUNC_8BIT_2LINE 0x38

// ==========================================
// FUNCTION PROTOTYPES
// ==========================================
void LCD_Init(void);
void LCD_SendCmd(uint8_t cmd);
void LCD_SendData(uint8_t data);
void LCD_SendString(const char* str);
void LCD_SetCursor(uint8_t row, uint8_t col);

#endif
