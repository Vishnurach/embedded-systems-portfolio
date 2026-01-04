#include "uart.h"

void UART0_Init(void)
{
    LPC_SC->PCONP |= (1 << 3);              // Power On UART0
    LPC_SC->PCLKSEL0 &= ~(0x3 << 6);        // PCLK = 25 MHz (CCLK/4)
    
    // Configure Pin Select (P0.2 = TXD0, P0.3 = RXD0)
    LPC_PINCON->PINSEL0 &= ~(0xF << 4);
    LPC_PINCON->PINSEL0 |=  (0x5 << 4);
    
    // Line Control: 8-bit, No Parity, 1 Stop Bit, DLAB Enable
    LPC_UART0->LCR = 0x83;                  
    
    // Baud Rate Calculation for 9600 Baud @ 25MHz PCLK
    LPC_UART0->DLM = 0;
    LPC_UART0->DLL = 0xA3;                  
    
    LPC_UART0->LCR = 0x03;                  // Disable DLAB (Lock Baud)
    
    // FIFO Setup: Enable FIFO, Reset RX/TX, Trigger Level 0 (1 char)
    LPC_UART0->FCR = 0x07;                  
    
    LPC_UART0->IER = (1 << 0);              // Enable Receive Data Interrupt
    NVIC_EnableIRQ(UART0_IRQn);             // Enable UART0 in NVIC
}

/* Transmit a single character (Polling) */
void UART0_TxChar(char ch)
{
    while ((LPC_UART0->LSR & (1 << 5)) == 0); // Wait for THR empty
    LPC_UART0->THR = ch;
}

/* Transmit a string */
void UART0_TxString(char *str)
{
    while (*str != '\0')
    {
        UART0_TxChar(*str);
        str++;
    }
}

/*
 * UART0 Interrupt Service Routine
 * Handles incoming commands from PC (LED Control)
 *
 * NOTE:
 * This ISR handles only Receive Data Available (RDA) and
 * Character Timeout Indicator (CTI) interrupts for simplicity.
 * Error handling (OERR, FE, PE) can be extended if required.
 */
void UART0_IRQHandler(void)
{
    uint8_t iir = LPC_UART0->IIR;

    // Check if an interrupt is pending
    if ((iir & 0x01) == 0)
    {
        uint8_t int_id = (iir >> 1) & 0x07;

        // RDA or CTI interrupt
        if (int_id == 0x02 || int_id == 0x06)
        {
            char received_char = LPC_UART0->RBR; // Reading clears interrupt

            switch (received_char)
            {
                case 'A': // Turn User LED ON
                    LPC_GPIO0->FIOSET = (1 << 1);
                    break;

                case 'B': // Turn User LED OFF
                    LPC_GPIO0->FIOCLR = (1 << 1);
                    break;
            }
        }
    }
}
