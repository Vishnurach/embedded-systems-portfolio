



#ifndef SYS_INIT_H
#define SYS_INIT_H

#include<LPC214X.h>
#include <stdint.h>

extern char current_password[10];
extern uint8_t current_state;
extern uint8_t temp_threshold;

void pll(void);
void delayms(uint16_t del);


#endif
