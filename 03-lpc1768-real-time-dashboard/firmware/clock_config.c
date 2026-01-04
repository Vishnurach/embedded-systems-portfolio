#include "clock_config.h"

void SetupClock(void)
{
    // 1. Disconnect & Disable PLL0
    LPC_SC->PLL0CON &= ~(1 << 1); 
    LPC_SC->PLL0FEED = 0xAA; LPC_SC->PLL0FEED = 0x55;
    LPC_SC->PLL0CON &= ~(1 << 0);
    LPC_SC->PLL0FEED = 0xAA; LPC_SC->PLL0FEED = 0x55;

    // 2. Enable Main OSC (12 MHz)
    LPC_SC->SCS |= (1 << 5);
    while (!(LPC_SC->SCS & (1 << 6)));

    // 3. Select Main OSC as Source
    LPC_SC->CLKSRCSEL = 0x01; 

    // 4. Configure PLL0 (M=50, N=3 -> 400 MHz)
    LPC_SC->PLL0CFG  = (49 << 0) | (2 << 16); 
    LPC_SC->PLL0FEED = 0xAA; LPC_SC->PLL0FEED = 0x55;

    // 5. Enable PLL0 & Wait for Lock
    LPC_SC->PLL0CON |= (1 << 0);
    LPC_SC->PLL0FEED = 0xAA; LPC_SC->PLL0FEED = 0x55;
    while (!(LPC_SC->PLL0STAT & (1 << 26)));

    // 6. CPU Clock Divider (400/4 = 100 MHz)
    LPC_SC->CCLKCFG = 3; 

    // 7. Flash Accelerator Setup (Safe for 100 MHz)
    LPC_SC->FLASHCFG = (LPC_SC->FLASHCFG & ~0x0000F000) | (0x5 << 12);

    // 8. Connect PLL0
    LPC_SC->PLL0CON |= (1 << 1);
    LPC_SC->PLL0FEED = 0xAA; LPC_SC->PLL0FEED = 0x55;
    while (!(LPC_SC->PLL0STAT & (1 << 25)));

    // 9. Peripheral Clock Selection (PCLK = CCLK/4 = 25 MHz)
    LPC_SC->PCLKSEL0 = 0x00; 
    LPC_SC->PCLKSEL1 = 0x00;
}
