#include <LPC214X.h>
#include "system_init.h"
#include "keypad_driver.h"

// --- KEYPAD CONFIGURATION (PORT 1) ---
// Rows (Outputs): P1.16 - P1.19
// Cols (Inputs):  P1.20 - P1.23

void KEYPAD_Init(void)
{
    // Set Rows (P1.16-P1.19) as Outputs
    // Set Cols (P1.20-P1.23) as Inputs
    // 0000 0000 0000 1111 0000 0000 0000 0000
    IODIR1 |= 0x000F0000; // Rows Output
    IODIR1 &= ~0x00F00000; // Cols Input
    
    // Set all Rows HIGH initially (Inactive)
    IO1SET = 0x000F0000; 
}

char KEYPAD_Read(void)
{   
    // Reset all Rows to HIGH (Inactive)
    IO1SET = 0x000F0000;
    
    // --- ROW 1 Check (P1.16) ---
    IO1CLR = (1 << 16); // Pull Row 1 LOW
    if(!(IO1PIN & (1 << 20))) { while(!(IO1PIN & (1<<20))); return '7'; }
    if(!(IO1PIN & (1 << 21))) { while(!(IO1PIN & (1<<21))); return '8'; }
    if(!(IO1PIN & (1 << 22))) { while(!(IO1PIN & (1<<22))); return '9'; }
    if(!(IO1PIN & (1 << 23))) { while(!(IO1PIN & (1<<23))); return '/'; }
    IO1SET = (1 << 16); // Reset Row 1 HIGH

    // --- ROW 2 Check (P1.17) ---
    IO1CLR = (1 << 17); // Pull Row 2 LOW
    if(!(IO1PIN & (1 << 20))) { while(!(IO1PIN & (1<<20))); return '4'; }
    if(!(IO1PIN & (1 << 21))) { while(!(IO1PIN & (1<<21))); return '5'; }
    if(!(IO1PIN & (1 << 22))) { while(!(IO1PIN & (1<<22))); return '6'; }
    if(!(IO1PIN & (1 << 23))) { while(!(IO1PIN & (1<<23))); return '*'; }
    IO1SET = (1 << 17); 

    // --- ROW 3 Check (P1.18) ---
    IO1CLR = (1 << 18); 
    if(!(IO1PIN & (1 << 20))) { while(!(IO1PIN & (1<<20))); return '1'; }
    if(!(IO1PIN & (1 << 21))) { while(!(IO1PIN & (1<<21))); return '2'; }
    if(!(IO1PIN & (1 << 22))) { while(!(IO1PIN & (1<<22))); return '3'; }
    if(!(IO1PIN & (1 << 23))) { while(!(IO1PIN & (1<<23))); return '-'; }
    IO1SET = (1 << 18); 

    // --- ROW 4 Check (P1.19) ---
    IO1CLR = (1 << 19); 
    if(!(IO1PIN & (1 << 20))) { while(!(IO1PIN & (1<<20))); return 'C'; }
    if(!(IO1PIN & (1 << 21))) { while(!(IO1PIN & (1<<21))); return '0'; }
    if(!(IO1PIN & (1 << 22))) { while(!(IO1PIN & (1<<22))); return '='; }
    if(!(IO1PIN & (1 << 23))) { while(!(IO1PIN & (1<<23))); return '+'; }
    IO1SET = (1 << 19); 

    return 'X'; // No key pressed
}
