/*
 * File        : lcd_driver.c
 * Description : Unified LCD driver supporting 4-bit and 8-bit modes
 */

#include <xc.h>
#include "config.h"      /* Only for _XTAL_FREQ */
#include "lcd_driver.h"

/* ================= PIN MAPPING =================
 *
 * PORTB usage:
 *   RB0 : RS
 *   RB1 : RW
 *   RB2 : EN
 *
 * 4-bit mode:
 *   RB4?RB7 : LCD D4?D7
 *
 * 8-bit mode:
 *   RB0?RB7 : LCD D0?D7
 */

#define LCD_PORT      PORTB
#define LCD_TRIS      TRISB

#define LCD_RS_PIN    0
#define LCD_RW_PIN    1
#define LCD_EN_PIN    2

/* ================= PRIVATE HELPERS ================= */

static void LCD_PulseEnable(void)
{
    LCD_PORT |= (1 << LCD_EN_PIN);
    __delay_us(1);
    LCD_PORT &= ~(1 << LCD_EN_PIN);
    __delay_us(50);
}

#ifdef LCD_MODE_4BIT
static void LCD_WriteNibble(uint8_t nibble)
{
    uint8_t temp = LCD_PORT;

    temp &= 0x0F;             /* Preserve RB0?RB3 */
    temp |= (nibble << 4);    /* Output on RB4?RB7 */

    LCD_PORT = temp;
    LCD_PulseEnable();
}
#endif

static void LCD_SendByte(uint8_t val, uint8_t is_data)
{
    if (is_data)
        LCD_PORT |= (1 << LCD_RS_PIN);
    else
        LCD_PORT &= ~(1 << LCD_RS_PIN);

    LCD_PORT &= ~(1 << LCD_RW_PIN); /* Write mode */

#ifdef LCD_MODE_4BIT
    LCD_WriteNibble((val >> 4) & 0x0F);
    LCD_WriteNibble(val & 0x0F);
#else
    LCD_PORT = val;
    LCD_PulseEnable();
#endif
}

/* ================= PUBLIC FUNCTIONS ================= */

void LCD_SendCmd(uint8_t cmd)
{
    LCD_SendByte(cmd, 0);
}

void LCD_SendData(uint8_t data)
{
    LCD_SendByte(data, 1);
}

void LCD_Init(void)
{
    /* Configure PORTB pins as output */
#ifdef LCD_MODE_4BIT
    LCD_TRIS &= 0x08;    /* RB0?RB2, RB4?RB7 outputs, RB3 untouched */
#else
    LCD_TRIS = 0x00;    /* RB0?RB7 outputs */
#endif

    LCD_PORT = 0x00;
    __delay_ms(20);     /* Power-up delay */

#ifdef LCD_MODE_4BIT
    /* ---- 4-bit reset sequence ---- */
    LCD_PORT &= ~(1 << LCD_RS_PIN);

    LCD_WriteNibble(0x03); __delay_ms(5);
    LCD_WriteNibble(0x03); __delay_us(150);
    LCD_WriteNibble(0x03);
    LCD_WriteNibble(0x02);    /* Switch to 4-bit */

    LCD_SendCmd(LCD_CMD_FUNC_4BIT_2LINE);
#else
    /* ---- 8-bit initialization ---- */
    LCD_SendCmd(LCD_CMD_FUNC_8BIT_2LINE);
#endif

    LCD_SendCmd(LCD_CMD_DISPLAY_ON);
    LCD_SendCmd(LCD_CMD_CLEAR);
    __delay_ms(2);
    LCD_SendCmd(LCD_CMD_ENTRY_MODE);
}

void LCD_SendString(const char *str)
{
    while (*str)
        LCD_SendData(*str++);
}

void LCD_SetCursor(uint8_t row, uint8_t col)
{
    uint8_t base = (row == 0) ? 0x80 : 0xC0;
    LCD_SendCmd(base + col);
}
