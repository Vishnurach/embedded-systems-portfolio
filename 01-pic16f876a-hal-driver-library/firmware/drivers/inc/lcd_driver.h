/*
 * File        : lcd_driver.h
 * Description : HD44780 LCD HAL Driver (4-bit / 8-bit)
 * Toolchain   : XC8
 *
 * LCD Interface (PORTB based)
 *
 * 4-bit mode wiring:
 *   RS -> RB0
 *   RW -> RB1
 *   EN -> RB2
 *   D4 -> RB4
 *   D5 -> RB5
 *   D6 -> RB6
 *   D7 -> RB7
 *
 * 8-bit mode wiring:
 *   RS -> RB0
 *   RW -> RB1
 *   EN -> RB2
 *   D0 -> RB0
 *   D1 -> RB1
 *   D2 -> RB2
 *   D3 -> RB3
 *   D4 -> RB4
 *   D5 -> RB5
 *   D6 -> RB6
 *   D7 -> RB7
 *
 * NOTE:
 * - In 4-bit mode, upper nibble (RB4?RB7) is used for data.
 * - In 8-bit mode, full PORTB is written as data.
 * - RW can be tied to GND if read is not required.
 */

#ifndef LCD_DRIVER_H
#define LCD_DRIVER_H

#include <stdint.h>

/* ================= LCD MODE SELECTION ================= */
/* Select ONE */
#define LCD_MODE_4BIT
// #define LCD_MODE_8BIT

/* ================= LCD COMMANDS ================= */

#define LCD_CMD_CLEAR            0x01
#define LCD_CMD_HOME             0x02
#define LCD_CMD_ENTRY_MODE       0x06
#define LCD_CMD_DISPLAY_ON       0x0C

#define LCD_CMD_FUNC_8BIT_2LINE  0x38
#define LCD_CMD_FUNC_4BIT_2LINE  0x28

/* ================= PUBLIC API ================= */

void LCD_Init(void);
void LCD_SendCmd(uint8_t cmd);
void LCD_SendData(uint8_t data);
void LCD_SendString(const char *str);
void LCD_SetCursor(uint8_t row, uint8_t col);

#endif /* LCD_DRIVER_H */
