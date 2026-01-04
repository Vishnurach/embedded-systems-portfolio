#include "timer.h"

/*
 * Timer0 Interrupt Service Routine
 * Implements the system heartbeat LED on P0.0
 */
void TIMER0_IRQHandler(void)
{
    if (LPC_TIM0->IR & (1 << 0)) // Check MR0 Match
    {
        LPC_GPIO0->FIOPIN ^= (1 << 0); // Toggle P0.0
        LPC_TIM0->IR = (1 << 0);       // Clear Interrupt Flag
    }
}

void Timer0_Init(void)
{
    LPC_SC->PCONP |= (1 << 1);           // Power On Timer0
    LPC_SC->PCLKSEL0 &= ~(0x3 << 2);     // PCLK = 25 MHz
    
    LPC_TIM0->CTCR = 0x0;                // Timer Mode
    LPC_TIM0->PR = 24;                   // Prescaler: 1µs resolution
    LPC_TIM0->MR0 = 500000;              // Match: 500ms (500,000µs)
    LPC_TIM0->MCR = (1 << 0) | (1 << 1); // Interrupt & Reset on Match
    
    NVIC_EnableIRQ(TIMER0_IRQn);         // Enable in NVIC
    LPC_TIM0->TCR = 0x01;                // Start Timer
}

/*
 * Blocking delay using Timer1
 *
 * NOTE:
 * This function intentionally uses a blocking delay and
 * reconfigures Timer1 on each call.
 * Suitable for low-rate tasks and demonstration purposes.
 */
void delay_ms(uint32_t ms)
{
    LPC_SC->PCONP |= (1 << 2);           // Power On Timer1
    LPC_SC->PCLKSEL0 &= ~(0x3 << 4);     // PCLK = 25 MHz
    
    LPC_TIM1->CTCR = 0x0;
    LPC_TIM1->PR   = 24;                 // 1µs resolution
    
    LPC_TIM1->TCR = 0x02;                // Reset Counter
    LPC_TIM1->TCR = 0x01;                // Enable Timer
    
    while (LPC_TIM1->TC < (ms * 1000));  // Wait
    
    LPC_TIM1->TCR = 0x00;                // Disable Timer
}
