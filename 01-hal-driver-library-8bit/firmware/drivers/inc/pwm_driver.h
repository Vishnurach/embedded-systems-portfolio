/*
 * File        : pwm_driver.h
 * Author      : Vishnu
 * Description : HAL driver for PIC16F876A PWM (CCP1) module
 * Toolchain   : XC8
 * Hardware    : Output on Pin RC2 (CCP1)
 */

#ifndef PWM_DRIVER_H
#define PWM_DRIVER_H

#include <xc.h>
#include <stdint.h>

/*
 * Function    : PWM_Init
 * Description : Initializes the CCP1 module for PWM mode.
 * - Timer2 configured with 1:4 prescaler
 * - Frequency set to EXACTLY 1 kHz (at F_osc = 4MHz)
 * - Duty cycle initialized to 0%
 */
void PWM_Init(void);

/*
 * Function    : PWM_SetDuty
 * Description : Updates the PWM duty cycle.
 * Parameters  : duty_val - 10-bit value (0 to 1023)
 * Notes       : 
 * - With PR2=249, the period width is 1000 counts.
 * - Input 0    = 0% Duty
 * - Input 500  = 50% Duty
 * - Input 1000 = 100% Duty
 * - Input >1000 = 100% Duty (Saturated)
 */
void PWM_SetDuty(uint16_t duty_val);

#endif /* PWM_DRIVER_H */