/*
 * File        : lcd_driver.c
 * Description : 4-bit mode LCD driver.
 *
 * NOTES:
 * GPIO sequencing and conservative delays are used to ensure stable
 * operation in simulation environments where instruction timing
 * may not perfectly reflect real hardware behavior.
 */

#include <LPC214X.h>
#include "system_init.h"
#include "lcd_driver.h"

void LCD_PulseEnable(void)
{
    IO0SET = (1 << 2);      // EN High
    delayms(2);
    IO0CLR = (1 << 2);      // EN Low
}

void LCD_Write4Bits(uint8_t val)
{
    // Clear Data Pins (P0.4 - P0.7)
    IO0CLR = 0x000000F0; 
    
    // Shift Data to P0.4-P0.7 and Write
    IO0SET = ((val & 0x0F) << 4);
    
    LCD_PulseEnable();
}

void LCD_SendCommand(char cmd)
{
    IO0CLR = (1 << 0);        // RS = 0 (Command)
    LCD_Write4Bits(cmd >> 4); // Send Upper Nibble
    LCD_Write4Bits(cmd);      // Send Lower Nibble
    delayms(2);
}

void LCD_SendChar(char data)
{
    IO0SET = (1 << 0);         // RS = 1 (Data)
    LCD_Write4Bits(data >> 4); // Send Upper Nibble
    LCD_Write4Bits(data);      // Send Lower Nibble
    delayms(2);
}

void LCD_SendString(char *str)
{
    while (*str) LCD_SendChar(*str++);
}

void LCD_Init(void)
{
    // Configure Control & Data Pins as Output
    IODIR0 |= 0x000000F5;  
    IO0CLR = 0x000000F5; 
    
    delayms(50); // Power-up stabilization
    
    // 4-Bit Initialization Sequence
    IO0CLR = (1 << 0); 
    LCD_Write4Bits(0x02); 
    delayms(5);
    
    LCD_SendCommand(0x28); // 4-bit mode, 2 lines
    LCD_SendCommand(0x0C); // Display ON, Cursor OFF
    LCD_SendCommand(0x06); // Auto Increment Cursor
    LCD_SendCommand(LCD_CMD_CLEAR); 
}

void LCD_ShowWelcome(void)
{
    LCD_SendCommand(LCD_CMD_CLEAR);
    delayms(15); 
    LCD_SendCommand(0x80); 
    LCD_SendString("ACCESS GRANTED");
    LCD_SendCommand(LCD_CMD_ROW_2);
    LCD_SendString("System Active");
}

void LCD_ShowMainMenu(void)
{
    LCD_SendCommand(LCD_CMD_CLEAR);
    delayms(15); 
    LCD_SendCommand(0x80); 
    LCD_SendString("1.Monitor Mode");
    LCD_SendCommand(LCD_CMD_ROW_2);
    LCD_SendString("2.Settings");
}
