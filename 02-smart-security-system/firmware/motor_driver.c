/*
 * File: motor_driver.c
 * Description: PWM Motor Control Driver (P0.21 / PWM5).
 *
 * SIMULATION NOTE (Proteus 8.11):
 * -------------------------------
 * While this driver generates a valid PWM signal on P0.21, connecting an
 * inductive load (DC Motor) or Oscilloscope probe to this pin during
 * full-system simulation (with Keypad scanning and LCD refresh active)
 * causes simulation instability/crashing in the LPC2148 model.
 *
 * WORKAROUND: In the Proteus project file, the DC Motor is placed for
 * visual BOM reference but LEFT DISCONNECTED from P0.21 to ensure the
 * core logic and UI remain responsive. On physical hardware, P0.21
 * should be connected to the Motor Driver input.
 */

#include "motor_driver.h"

void Motor_Init(void)
{
    // 1. Configure P0.21 as PWM5
    // PINSEL1 [11:10] = 01 -> 0x00000400
    PINSEL1 &= ~(0x00000C00); // Clear bits 10,11
    PINSEL1 |=  0x00000400;   // Set P0.21 to PWM5
    
    // 2. PWM Configuration
    PWMTCR = (1 << 1);     // Reset Counter
    
    // Prescaler for 1ms Timer resolution
    // PCLK = 60MHz. 60,000,000 / 60,000 = 1000Hz (1ms)
    PWMPR  = 60000 - 1;    
    
    // Set Total Period = 10ms (100Hz)
    PWMMR0 = 10;           
    
    PWMMCR = 0x02;         // Reset PWMTC when it matches MR0
    
    // Enable PWM5 Output (Bit 13) and Set Single Edge Mode
    PWMPCR = (1 << 13);    
    
    // Initial State: OFF (Duty Cycle 0)
    PWMMR5 = 0;            
    
    // Latch Enable (Load MR0 and MR5)
    PWMLER = (1 << 0) | (1 << 5); 
    
    // Enable PWM and Counter
    PWMTCR = 0x09;         
}

void Motor_SetState(uint8_t state)
{
    if(state == 1)
    {
        // Turn ON: Set Duty Cycle to 10ms (100% Speed)
        PWMMR5 = 10; 
    }
    else
    {
        // Turn OFF: Set Duty Cycle to 0ms
        PWMMR5 = 0; 
    }
    
    // Latch the new value to update PWM hardware
    PWMLER = (1 << 5); 
}