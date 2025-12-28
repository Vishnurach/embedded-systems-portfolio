/*
 * File: adc_driver.c
 * Description: Hardware ADC Driver for LPC2148.
 * Channel: AD0.1 (P0.28)
 *
 * IMPORTANT NOTE FOR SIMULATION (PROTEUS 8.11):
 * ---------------------------------------------
 * This file contains the ACTUAL Hardware ADC code using AD0CR/AD0DR registers.
 *
 * REASON FOR BYPASS IN SIMULATION:
 * In Proteus 8.11, the LPC2148 model has a known issue where concurrent 
 * high-speed Digital I/O (LCD/Keypad scanning) conflicts with the Analog 
 * Read process, causing the simulation to crash or return invalid data.
 * * Therefore, for simulation purposes, the main application (smart_security.c) 
 * manually simulates temperature changes using Keypad buttons (+/-).
 * * This file is intended for the PHYSICAL HARDWARE implementation.
 */

#include <LPC214X.h>
#include "adc_driver.h"
#include "system_init.h"

void ADC_Init(void)
{
    // 1. Configure P0.28 as AD0.1
    // PINSEL1 Bits[25:24] = 01
    PINSEL1 &= ~(0x03000000); // Clear bits
    PINSEL1 |=  (0x01000000); // Set P0.28 to AD0.1
    
    // 2. Configure ADC0 Control Register (AD0CR)
    // SEL = 0x02 (Channel 1)
    // CLKDIV = 0x04 (VPB Clock / 5) -> 12MHz ADC Clock (Max is 4.5MHz, needs adjustment)
    // BURST = 0 (Software Control)
    // CLKS = 0 (11 clocks/10 bits)
    // PDN = 1 (Operational)
    
    // Assuming PCLK = 60MHz. We need ADC_CLK <= 4.5MHz.
    // Divisor = (60 / 4.5) = 13.3 -> Let's use 14 (value 13 in bits).
    // AD0CR = (1<<1) | ((13)<<8) | (1<<21);
    
    AD0CR = (1 << 1)      |  // Select AD0.1
            (13 << 8)     |  // CLKDIV = 13 (60MHz / 14 = ~4.28MHz)
            (1 << 21);       // PDN = 1 (Enable ADC)
}

unsigned int ADC_Read(void)
{
    unsigned int adc_data;
    
    // 1. Start Conversion (Set START bit [26:24] to 001)
    AD0CR |= (1 << 24); 
    
    // 2. Wait for DONE bit (Bit 31 of AD0DR1)
    // We check the specific Data Register for Channel 1 (AD0DR1)
    do {
        adc_data = AD0DR1;
    } while ((adc_data & 0x80000000) == 0); // Wait until DONE=1
    
    // 3. Stop Conversion (Optional, clears START bits)
    AD0CR &= ~(0x07000000);
    
    // 4. Extract Result
    // Result is in bits [15:6]
    adc_data = (adc_data >> 6) & 0x3FF; // 10-bit result (0-1023)
    
    // 5. Convert to Temperature (Simple Scaling for LM35)
    // Vref = 3.3V. Step = 3.3/1023 = 3.22mV
    // LM35 = 10mV/C.
    // Temp = (ADC * 322) / 100 / 10 = approx ADC / 3.1
    // For simplicity in demo: Temp = ADC_Val / 3 (Adjust as needed for sensor)
    
    return (adc_data / 3); 
}
