#ifndef LCD_DRIVER_H
#define LCD_DRIVER_H

#include<LPC214X.h>
#include <stdint.h>

#define LCD_CMD_CLEAR            0x01
#define LCD_CMD_HOME             0x02
#define LCD_CMD_4BIT             0x28
#define LCD_CMD_ENTRY_MODE       0x06
#define LCD_CMD_DISPLAY_ON       0x0C
#define LCD_CMD_ROW_2            0xC0

void LCD_SendCommand(char word);
void LCD_SendChar(char word);
void LCD_SendString(char *str);
void LCD_Init(void);

// Updated function names for clarity
void LCD_ShowWelcome(void);     // Shows "Access Granted"
void LCD_ShowMainMenu(void);    // Shows "1.Monitor 2.Set"

#endif
