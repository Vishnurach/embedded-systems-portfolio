#include "adc.h"

void ADC_Init(void)
{
    LPC_SC->PCONP |= (1 << 12);          // Power On ADC
    LPC_SC->PCLKSEL0 &= ~(0x3 << 24);    // PCLK = 25 MHz
    
    // Configure P0.23 as AD0.0
    LPC_PINCON->PINSEL1 &= ~(0x3 << 14);
    LPC_PINCON->PINSEL1 |=  (0x1 << 14); 
    
    /*
     * ADC Clock Configuration:
     * ADC Clock = PCLK / (CLKDIV + 1)
     * = 25 MHz / (4 + 1) = 5 MHz (Safe, well below 13 MHz max)
     */
    LPC_ADC->ADCR = (1 << 0) | (4 << 8) | (1 << 21);
}

uint16_t ADC_Read(void)
{
    uint16_t result;
    
    LPC_ADC->ADCR |= (1 << 24);          // Start Conversion
    
    // Wait for DONE bit (Bit 31)
    while ((LPC_ADC->ADGDR & (1UL << 31)) == 0);
    
    result = LPC_ADC->ADGDR;             // Read Result
    result = (result >> 4) & 0xFFF;      // Extract 12-bit value
    
    LPC_ADC->ADCR &= ~(1 << 24);         // Stop Conversion
    
    return result;
}
