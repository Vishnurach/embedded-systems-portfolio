/*
 * File        : main.c
 * Description : Portfolio Project ? LCD HAL Demo
 */

#include <xc.h>
#include "config.h"
#include "lcd_driver.h"

void main(void)
{
    /* Initialize LCD (mode handled inside driver) */
    LCD_Init();

    /* Display messages */
    LCD_SendString("Vishnu's HAL");
    LCD_SetCursor(1, 0);
    LCD_SendString("LCD 4-bit Demo");

    while (1)
    {
        /* Super loop */
    }
}
