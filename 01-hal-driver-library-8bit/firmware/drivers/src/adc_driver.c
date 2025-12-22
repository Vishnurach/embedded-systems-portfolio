/*
 * File        : adc_driver.c
 * Author      : Vishnu
 * Description : HAL driver for PIC16F876A ADC module
 * Toolchain   : XC8
 * Notes       :
 *  - Provides blocking ADC read with full 10-bit resolution
 *  - Right-justified result
 *  - Supports AN0?AN4
 */

#include "adc_driver.h"
#include "config.h"

/*
 * Function    : ADC_init
 * Description : Initializes ADC module configuration
 *               - Right justified result
 *               - All AN pins configured as analog
 *               - ADC clock set to safe default
 */
void ADC_Init(void)
{
    /*
     * ADCON1 Register
     * bit 7  ADFM  = 1  ? Right justified result
     * bit 6  ADCS2 = 1  ? ADC clock select (with ADCON0)
     * bits 3:0 PCFG = 0000 ? AN0?AN4 as analog inputs
     */
    ADCON1 = 0b11000000;

    /*
     * ADCON0 Register
     * bit 0 ADON = 1 ? Enable ADC
     * bits 7:6 ADCS1:0 = 01 ? Fosc/8 (safe for most clocks)
     * bits 5:3 CHS = 000 ? Default channel AN0
     */
    ADCON0 = 0x41;

    /* Do not force full TRISA here to keep driver reusable */
}

/*
 * Function    : ADC_read
 * Parameters  : channel ? ADC channel number (0?4 for AN0?AN4)
 * Returns     : 10-bit ADC result (0?1023)
 * Description : Performs a blocking ADC conversion on selected channel
 */
uint16_t ADC_Read(uint8_t channel)
{
    /* Validate channel range (PIC16F876A supports AN0?AN4 only) */
    if (channel > 4)
    {
        return 0;
    }

    /* Configure corresponding PORTA pin as input */
    TRISA |= (1 << channel);

    /*
     * Select ADC channel
     * Clear CHS bits (bits 5?3)
     */
    ADCON0 &= 0b11000111;

    /* Set new channel */
    ADCON0 |= (channel << 3);

    /*
     * Acquisition delay
     * Required for sampling capacitor to charge (~20 µs)
     */
    __delay_us(20);

    /* Start ADC conversion */
    GO = 1;

    /* Wait until conversion completes */
    while (GO);

    /*
     * Read result
     * Right justified ? combine ADRESH and ADRESL
     */
    return ((uint16_t)ADRESH << 8) | ADRESL;
}
