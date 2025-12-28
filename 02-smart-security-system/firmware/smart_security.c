/*
 * File: smart_security.c
 * Description: Main application with PWM Motor Control & Stopwatch Timer.
 * Author: Vishnu Rach K R
 *
 * SIMULATION NOTE:
 * Due to Proteus 8.11 model limitations, the LM35 and DC Motor are 
 * visually present in the simulation window but disconnected from the MCU 
 * to prevent instability caused by mixed-signal conflicts (ADC/PWM vs LCD).
 * The firmware logic remains fully functional for hardware deployment.
 */

#include <LPC214X.h>
#include <stdio.h>
#include <string.h>
#include "system_init.h"
#include "lcd_driver.h"
#include "keypad_driver.h"
#include "uart_driver.h"
#include "adc_driver.h"
#include "motor_driver.h" 

// --- Global System Variables ---
char current_password[10] = "1234";
uint8_t current_state = 0;
uint8_t temp_threshold = 35; 

void Run_Settings_Mode(void)
{
    char key;
    char lcd_buf[20];
    char *input_str; 
    uint8_t update_needed = 1; 
    
    while(1)
    {
        LCD_SendCommand(LCD_CMD_CLEAR);
        LCD_SendString("1.Change Pass");
        LCD_SendCommand(LCD_CMD_ROW_2);
        LCD_SendString("2.Set Lmt C:Bk"); 
        
        do {
            key = KEYPAD_Read();
            if(key == 'C') return; 
        } while(key != '1' && key != '2');
        
        if(key == '1')
        {
            LCD_SendCommand(LCD_CMD_CLEAR);
            LCD_SendString("Check UART...");
            
            // Added \r\n for cleaner new lines
            UART_SendString("\r\n--- PASSWORD CHANGE MODE ---\r\n");
            
            UART_SendString("\r\nEnter Old Password: ");
            input_str = UART_ReceiveString();
            
            if(strcmp(input_str, current_password) == 0)
            {
                UART_SendString("\r\nEnter New Password: ");
                input_str = UART_ReceiveString();
                
                strcpy(current_password, input_str);
                
                UART_SendString("\r\nPassword Changed Successfully!\r\n");
                LCD_SendCommand(LCD_CMD_ROW_2);
                LCD_SendString("Success!");
            }
            else
            {
                UART_SendString("\r\nIncorrect Password.\r\n");
                LCD_SendCommand(LCD_CMD_ROW_2);
                LCD_SendString("Failed!");
            }
            delayms(1500); 
        }
        else if(key == '2')
        {
            LCD_SendCommand(LCD_CMD_CLEAR);
            LCD_SendString("Limit Setting:");
            update_needed = 1; 
            
            while(1)
            {
                if(update_needed)
                {
                    LCD_SendCommand(LCD_CMD_ROW_2);
                    sprintf(lcd_buf, "L:%d =:OK C:Bk", temp_threshold);
                    LCD_SendString(lcd_buf);
                    update_needed = 0; 
                }
                
                key = KEYPAD_Read();
                
                if(key == '+') { if(temp_threshold < 99) temp_threshold++; update_needed=1; delayms(200); }
                else if(key == '-') { if(temp_threshold > 0) temp_threshold--; update_needed=1; delayms(200); }
                else if(key == '=') {
                    LCD_SendCommand(LCD_CMD_CLEAR);
                    LCD_SendString("Limit Saved.");
                    delayms(1000);
                    break; 
                }
                else if(key == 'C') break; 
            }
        }
    }
}

/**
 * @brief Monitor Mode: Controls Fan (Motor) based on Temperature.
 */
void Run_Monitor_Mode(void)
{
    unsigned int temp_val = 25; 
    char buffer[32];   
    char key;
    uint8_t update_screen = 1;
    
    // Stopwatch Vars
    char timer_arr[20] = {'\0'};
    int i, s = 0, m = 0, h = 0;
    int tick_count = 0; 

    LCD_SendCommand(LCD_CMD_CLEAR);
    LCD_SendString("Monitor Active");
    delayms(1000);

    UART_SendString("\r\n--- MONITOR START ---\r\n");

    while(1)
    {
        // 1. Simulate Temp Input (Using Keypad)
        key = KEYPAD_Read();
        if(key == '+') { if(temp_val < 99) temp_val++; update_screen = 1; }
        else if(key == '-') { if(temp_val > 0) temp_val--; update_screen = 1; }
        else if(key == 'C') { 
            Motor_SetState(0); // SAFETY: Turn off motor before exiting
            return; 
        }

        // 2. Logic: Motor Control (PWM)
        if(temp_val > temp_threshold) {
            Motor_SetState(1); // TURN MOTOR ON (PWM Active)
        } else {
            Motor_SetState(0); // TURN MOTOR OFF
        }

        // 3. Screen Update
        if(update_screen)
        {
            LCD_SendCommand(LCD_CMD_CLEAR);
            LCD_SendString("Temp: ");
            sprintf(buffer, "%d C", temp_val); 
            LCD_SendString(buffer);
            
            LCD_SendCommand(LCD_CMD_ROW_2);
            if(temp_val > temp_threshold) { 
                LCD_SendString("FAN: ON       ");
            } else {
                LCD_SendString("FAN: OFF      ");
            }
            update_screen = 0; 
        }

        // 4. Stopwatch Logic (Only counts when Over-Temp)
        if(temp_val > temp_threshold)
        {
            tick_count++;
            if(tick_count >= 20) // 1 Second (approx)
            {
                s++;
                // Clear previous time on terminal
                for(i=0; timer_arr[i]!='\0'; i++) UART_SendChar('\b');
                
                if(s > 59) { m++; s=0; if(m > 59) { h++; s=0; m=0; } }
                
                sprintf(timer_arr, "Alert: %d:%d:%d", h, m, s);
                UART_SendString(timer_arr);
                
                tick_count = 0; 
            }
        }
        else
        {
            // Reset timer when temp is normal
            if(s!=0 || m!=0 || h!=0) {
                UART_SendString("\r\nNormal Temp. Timer Reset.\r\n");
                s=0; m=0; h=0; timer_arr[0]='\0';
            }
            tick_count = 0;
        }

        delayms(50); // Loop Pacing
    }
}

int main(void) 
{       
    uint8_t menu_stage = 0; 
    char key;

    pll();          
    UART_Init();    
    LCD_Init();     
    KEYPAD_Init();  
    ADC_Init();     
    Motor_Init(); 
    
    UART_CheckPassword(); 

    while(1)
    {
        if(menu_stage == 0) {
            LCD_ShowWelcome(); 
            delayms(2000); 
            menu_stage = 1;
        }
        else if(menu_stage == 1) {
            LCD_ShowMainMenu();
            menu_stage = 2; 
        }
        else if(menu_stage == 2) {
            key = KEYPAD_Read();
            if(key == '1') { Run_Monitor_Mode(); menu_stage = 1; }
            else if(key == '2') { Run_Settings_Mode(); menu_stage = 1; }
        }
    }
}
