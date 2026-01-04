/*
 * File        : main.c
 * Description : Preprocessor-based Test Workbench
 * Selects and compiles one specific demo from the 'examples' folder.
 */

#include <xc.h>
#include "config.h"

/* ================= SELECT TEST (Uncomment EXACTLY ONE) ================= */

 #define RUN_LCD_DEMO      // <--- Currently Active
// #define RUN_UART_DEMO
// #define RUN_ADC_DEMO
// #define RUN_PWM_DEMO
// #define RUN_I2C_DEMO
// #define RUN_SPI_LOOPBACK         //Connect wire between RC5 (SDO) and RC4 (SDI)

/* ================= SAFETY CHECKS (Compiler Logic) ================= */

// Ensure user selected at least one demo
#if !defined(RUN_LCD_DEMO) && !defined(RUN_UART_DEMO) && \
    !defined(RUN_ADC_DEMO) && !defined(RUN_PWM_DEMO) && \
    !defined(RUN_I2C_DEMO) && !defined(RUN_SPI_LOOPBACK)
    #error "Please uncomment ONE demo definition at the top of main.c"
#endif

// (Optional) Check to ensure multiple demos are not defined simultaneously
// This prevents linker errors if you accidentally leave two uncommented.

/* ================= DEMO BINDINGS ================= */
// The preprocessor renames the 'main' function inside the included file
// to prevent conflict with the real 'main' entry point below.

#ifdef RUN_LCD_DEMO
    #define main lcd_demo_main
    #include "examples/lcd_demo.c"
    #undef main
#endif

#ifdef RUN_UART_DEMO
    #define main uart_demo_main
    #include "examples/uart_demo.c"
    #undef main
#endif

#ifdef RUN_ADC_DEMO
    #define main adc_demo_main
    #include "examples/adc_demo.c"
    #undef main
#endif

#ifdef RUN_PWM_DEMO
    #define main pwm_demo_main
    #include "examples/pwm_demo.c"
    #undef main
#endif

#ifdef RUN_I2C_DEMO
    #define main i2c_demo_main
    #include "examples/i2c_demo.c"
    #undef main
#endif

#ifdef RUN_SPI_LOOPBACK
    #define main spi_loopback_main
    // Points to the new SINGLE-CHIP verification file
    #include "examples/spi_loopback_demo.c" 
    #undef main
#endif

/* ================= MAIN ENTRY ================= */

void main(void)
{
    // Call the renamed entry point for the selected demo.
    
    #ifdef RUN_LCD_DEMO
        lcd_demo_main();
    #endif

    #ifdef RUN_UART_DEMO
        uart_demo_main();
    #endif

    #ifdef RUN_ADC_DEMO
        adc_demo_main();
    #endif
    
    #ifdef RUN_PWM_DEMO
        pwm_demo_main();
    #endif

    #ifdef RUN_I2C_DEMO
        i2c_demo_main();
    #endif

    #ifdef RUN_SPI_LOOPBACK
        spi_loopback_main();
    #endif

    /* Should never be reached as demos contain while(1) loops */
    while (1);
}