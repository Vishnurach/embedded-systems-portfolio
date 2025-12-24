/*
 * File        : pwm_demo.c
 * Author      : Vishnu
 * Description : Test harness for 1kHz PWM Driver
 */

#include <xc.h>
#include <stdint.h>
#include <stdio.h>
#include "../config.h"
#include "../drivers/inc/uart_driver.h"
#include "../drivers/inc/adc_driver.h"
#include "../drivers/inc/pwm_driver.h"

void main(void) 
{
    uint16_t adc_raw_val;
    uint8_t display_duty;

    UART_Init();
    ADC_Init();
    PWM_Init();

    UART_SendString("\r\n=== 1kHz PWM Driver Test ===\r\n");

    while(1)
    {
        /* 1. Read Potentiometer (0 - 1023) */
        adc_raw_val = ADC_Read(0);
        
        /* * 2. Update Hardware
         * ADC Max: 1023
         * PWM Max Period (PR2=249): 1000 counts
         * * We pass the ADC value directly. 
         * Values 1001-1023 will just stay at 100% duty (saturation).
         * This gives us a nearly perfect 1:1 control mapping.
         */
        PWM_SetDuty(adc_raw_val); 
        
        /* 3. Debug Output */
        // Calculate percentage for display
        display_duty = ((uint32_t)adc_raw_val * 100) / 1023;
        
        UART_SendString("ADC: ");
        UART_SendDec(adc_raw_val);
        UART_SendString(" | Duty: ");
        UART_SendDec(display_duty);
        UART_SendString("%\r");

        __delay_ms(100); 
    }
}