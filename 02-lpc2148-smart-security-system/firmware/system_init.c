/*
 * File        : system_init.c
 * Description : System clock and timer initialization.
 *
 * NOTE:
 * Timer0-based delays are used instead of software loops to provide
 * predictable timing at 60 MHz system clock.
 */


#include <LPC214X.h>
#include <stdint.h>
#include "system_init.h"

void pll(void)
{
    // Configure PLL for 60MHz System Clock (assuming 12MHz Crystal)
    PLL0CON = 0x01;  // Enable PLL
    PLL0CFG = 0x24;  // Multiplier=5, Divider=2
    
    // Feed Sequence
    PLL0FEED = 0xAA;
    PLL0FEED = 0x55;
    
    // Wait for Lock
    while((PLL0STAT & (1<<10)) == 0);
    
    PLL0CON = 0x03;  // Connect PLL
    PLL0FEED = 0xAA;
    PLL0FEED = 0x55;
    
    VPBDIV = 0x01;   // PCLK = CCLK = 60MHz
}

void delayms(uint16_t del)          
{
    // Timer0 configuration for Millisecond delay
    T0CTCR = 0x00;   // Timer Mode
    T0PR   = 59999;  // Prescaler: 60000 ticks = 1ms (@60MHz)
    T0TC   = 0;      // Reset Counter
    T0TCR  = 0x01;   // Enable Timer
    
    while(T0TC < del); // Block until count reached
    
    T0TCR  = 0x00;   // Disable Timer
}
