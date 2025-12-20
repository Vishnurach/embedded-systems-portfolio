/**
 * @file lcd_driver.h
 * @brief Device driver for HD44780-compatible 16x2 LCD (PIC16F876A).
 *
 * This module implements a device-specific LCD driver for interfacing
 * HD44780-compatible 16x2 character LCDs with the PIC16F876A microcontroller
 * using an 8-bit parallel data interface.
 *
 * The driver encapsulates all LCD control logic, timing requirements,
 * and data bus handling, providing a clean API for the application layer.
 *
 * ---------------------------------------------------------------------------
 * Supported Hardware
 * ---------------------------------------------------------------------------
 * - Microcontroller : PIC16F876A
 * - Display         : 16x2 LCD (HD44780 compatible)
 * - Interface Mode  : 8-bit parallel
 *
 * ---------------------------------------------------------------------------
 * Software Architecture
 * ---------------------------------------------------------------------------
 * Application Layer:
 *   - main.c
 *     Calls LCD_* APIs to display data on the LCD.
 *
 * Driver Layer:
 *   - lcd_driver.h  : Public driver interface (this file)
 *   - lcd_driver.c  : Driver implementation
 *
 * Configuration:
 *   - config.h
 *     Defines LCD data and control pin mappings.
 *
 * ---------------------------------------------------------------------------
 * Features
 * ---------------------------------------------------------------------------
 * - LCD initialization sequence
 * - Command write interface
 * - Character and string display functions
 * - Clear separation between application and driver logic
 *
 * ---------------------------------------------------------------------------
 * Usage Example
 * ---------------------------------------------------------------------------
 * @code
 * #include "lcd_driver.h"
 *
 * int main(void)
 * {
 *     LCD_Init();
 *     LCD_Clear();
 *     LCD_SetCursor(0, 0);
 *     LCD_Print("Hello World");
 *
 *     while (1);
 * }
 * @endcode
 *
 * ---------------------------------------------------------------------------
 * Notes
 * ---------------------------------------------------------------------------
 * - This is a device driver, not a chip-agnostic HAL.
 * - Timing constraints are handled internally in the driver implementation.
 * - Designed for reuse in PIC16F876A-based applications.
 *
 * ---------------------------------------------------------------------------
  * Author
 * ---------------------------------------------------------------------------
  * Vishnu Rach
 * ---------------------------------------------------------------------------
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
