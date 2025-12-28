#ifndef KEYPAD_DRIVER_H
#define KEYPAD_DRIVER_H

#include <LPC214X.h>
#include <stdint.h>
#include "system_init.h"

// --- Function Prototypes ---
void KEYPAD_Init(void);  
char KEYPAD_Read(void);

#endif
