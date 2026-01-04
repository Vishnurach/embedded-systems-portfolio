/*
 * Project: LPC1768 Sensor Dashboard
 * Author: Vishnu
 * Description: Main application loop. Reads LM35 temperature sensor 
 * and communicates with PC via UART0.
 */

#include <stdio.h>
#include "clock_config.h"
#include "timer.h"
#include "uart.h"
#include "adc.h"

int main(void)
{
    char buffer[50];
    uint16_t adc_raw_val;
    float voltage_mv;
    float temperature_c;

    /* ----------------------------------------------------------------
       1. Hardware Initialization
       ---------------------------------------------------------------- */
    SetupClock();    // Configure System Clock to 100 MHz
    __enable_irq();  // Enable Global Interrupts

    /* Configure GPIO
       P0.0: Heartbeat LED (Timer0 Controlled)
       P0.1: User LED (UART Command Controlled) */
    LPC_GPIO0->FIODIR |= (1 << 0) | (1 << 1); 

    Timer0_Init();   // Start Heartbeat Timer (500ms)
    UART0_Init();    // Start UART (9600 Baud, Interrupt Enabled)
    ADC_Init();      // Start ADC (Channel 0)

    UART0_TxString("System Online. Mode: LM35 Temperature Monitor\r\n");

    /* ----------------------------------------------------------------
       2. Main Application Loop
       ---------------------------------------------------------------- */
    while (1)
    {
        // A. Read Raw ADC Value (0 - 4095)
        adc_raw_val = ADC_Read();
        
        // B. Convert to Voltage (Assuming VREF = 3.3V = 3300mV)
        voltage_mv = (adc_raw_val * 3300.0f) / 4095.0f;
        
        // C. Convert to Temperature (LM35: 10mV = 1 Degree Celsius)
        temperature_c = voltage_mv / 10.0f;
        
        // D. Format Data: "Temp: 25.4 C"
        // NOTE: sprintf used for clarity; may be replaced with a lighter formatter in production
        sprintf(buffer, "Temp: %.1f C\r\n", temperature_c);
        
        // E. Transmit to PC Dashboard
        UART0_TxString(buffer);
        
        // F. Update Rate (500ms)
        delay_ms(500); 
    }
}
