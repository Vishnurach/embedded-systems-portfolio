/*
 * File        : pwm_driver.c
 * Author      : Vishnu
 * Description : Implementation of PWM driver for PIC16F876A (1 kHz)
 * Toolchain   : XC8
 */

#include "pwm_driver.h"

void PWM_Init(void)
{
    /* 1. Configure PWM Output Pin */
    TRISC2 = 0; 
    
    /*
     * 2. Configure Timer2
     * T2CON Register configuration:
     * - Bit 2 (TMR2ON) = 1 (Enable Timer2)
     * - Bits 1:0 (T2CKPS) = 01 (Prescaler 1:4)
     */
    T2CON = 0x05; 
    
    /*
     * 3. Set PWM Period (Frequency)
     * Target: 1000 Hz
     * Formula: PR2 = [F_osc / (F_pwm * 4 * Prescaler)] - 1
     * Calculation:
     * [4,000,000 / (1000 * 4 * 4)] - 1
     * [4,000,000 / 16,000] - 1
     * 250 - 1 = 249
     */
    PR2 = 249; 
    
    /* Initialize Duty Cycle to 0% */
    CCPR1L = 0;
    CCP1CON = 0x0C; // Set CCP1 mode to PWM
}

void PWM_SetDuty(uint16_t duty_val)
{
    if (duty_val > 1000)
    {
        duty_val = 1000;   // Saturate to 100%
    }

    CCPR1L = (uint8_t)(duty_val >> 2);

    /* Preserve PWM mode bits */
    CCP1CON = (CCP1CON & 0xCF) | ((duty_val & 0x03) << 4);
}