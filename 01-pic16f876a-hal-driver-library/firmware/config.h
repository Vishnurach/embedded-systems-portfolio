/*
 * File: config.h
 * Description: Device configuration bits and system clock definition
 * Toolchain : XC8
 * MCU       : PIC (XT oscillator, 4 MHz)
 */

#ifndef CONFIG_H
#define CONFIG_H

/* ================= Configuration Bits ================= */
/* NOTE: XC8 compiler–specific pragmas */

#pragma config FOSC = XT        /* XT oscillator */
#pragma config WDTE = OFF       /* Watchdog Timer disabled */
#pragma config PWRTE = OFF      /* Power-up Timer disabled */
#pragma config BOREN = OFF      /* Brown-out Reset disabled */
#pragma config LVP  = OFF       /* Low-voltage programming disabled */
#pragma config CPD  = OFF       /* Data EEPROM code protection off */
#pragma config WRT  = OFF       /* Flash write protection off */
#pragma config CP   = OFF       /* Program memory code protection off */

/* ================= System Clock ================= */
/* Required for __delay_ms() and __delay_us() macros */

#ifndef _XTAL_FREQ
#define _XTAL_FREQ 4000000UL    /* 4 MHz crystal frequency */
#endif

#endif /* CONFIG_H */
