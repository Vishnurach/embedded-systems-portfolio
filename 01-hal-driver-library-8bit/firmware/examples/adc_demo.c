/*
 * Hardware Setup:
 * 1. Potentiometer: Middle pin to RA0 (AN0), ends to VDD and VSS.
 * 2. UART: TX pin (RC6) connected to USB-TTL Converter RX pin.
 * 3. Oscillator: 4 MHz Crystal on OSC1/OSC2.
 */

#include <xc.h>
#include <stdio.h>
#include "../config.h"
#include "../drivers/inc/uart_driver.h"
#include "../drivers/inc/adc_driver.h"

/* Update for adc_demo.c main loop */
void main(void)
{
    char buffer[40];       // Increased buffer size
    uint16_t raw_adc;
    uint32_t voltage_mv;   // Use 32-bit for calculation to prevent overflow

    UART_Init();
    ADC_Init();

    while (1)
    {
        raw_adc = ADC_Read(0);

        /* * Convert to Millivolts using Integer Math
         * Formula: Voltage = (ADC * Vref) / Resolution
         * Vref = 5000 mV, Resolution = 1023
         * Note: 1023 * 5000 = 5,115,000 (Fits in uint32_t)
         */
        voltage_mv = ((uint32_t)raw_adc * 5000) / 1023;
        sprintf(buffer, "Raw: %u | Voltage: %lu mV\r\n", raw_adc, voltage_mv);
        UART_SendString(buffer);

        __delay_ms(1000);
    }
}
