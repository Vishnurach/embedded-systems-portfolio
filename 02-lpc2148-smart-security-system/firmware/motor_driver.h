#ifndef MOTOR_DRIVER_H
#define MOTOR_DRIVER_H

#include <LPC214X.h>
#include <stdint.h>

void Motor_Init(void);
void Motor_SetState(uint8_t state); // 1 = ON, 0 = OFF

#endif
