#ifndef ADC_DRIVER_H
#define ADC_DRIVER_H

#include <LPC214X.h>
#include <stdint.h>

void ADC_Init(void);
unsigned int ADC_Read(void);

#endif
