/*
 * File        : adc_driver.h
 * Author      : Vishnu
 * Description : Driver targeted for PIC16F876A ADC module
 * Toolchain   : XC8
 * Notes       : 
 */

#ifndef ADC_DRIVER_H
#define ADC_DRIVER_H

#include <xc.h>
#include <stdint.h>

void ADC_Init(void);
/* channel: 0?4 (AN0?AN4) */
uint16_t ADC_Read(uint8_t channel);

#endif