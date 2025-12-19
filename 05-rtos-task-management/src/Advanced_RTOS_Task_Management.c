/*******************************************************************************
 * @file    Advanced_RTOS_Task_Management.c
 * @brief   RTOS-based ADC acquisition and UART reporting example
 *
 * @details
 * This module demonstrates a producer–consumer RTOS design using µC/OS-II
 * on an AVR microcontroller. Two ADC tasks periodically sample different
 * ADC channels and send raw values to a UART task using an RTOS queue.
 *
 * Key features:
 *  - Mutex-protected ADC hardware access
 *  - Message pool for safe inter-task communication
 *  - Centralized UART task for formatted output
 *  - Diagnostic drop counter for queue overflow detection
 *  - Background LED blink task
 *
 * MISRA-C:
 *  - MISRA-C:2012 guidelines are followed where practical.
 *  - Required deviations are documented inline with justification.
 *
 * Target: AVR (ATmega series)
 * RTOS  : µC/OS-II
 ******************************************************************************/

#include "includes.h"   /* RTOS headers - external library (MISRA deviation) */
#include <string.h>    /* sprintf used for UART formatting */
#include <stdio.h>
#include <stdint.h>

/*==============================================================================
 *                              CONFIGURATION
 *============================================================================*/
/**
 * @brief System oscillator frequency in Hz
 */
#define FOSC_HZ        (16000000UL)

/**
 * @brief UART baud rate
 */
#define UART_BAUD      (9600U)

/**
 * @brief UART baud rate register value
 */
#define UART_UBRR      (FOSC_HZ / 16U / UART_BAUD - 1U)

/**
 * @brief RTOS queue depth
 */
#define QUEUE_SIZE     (8U)

/**
 * @brief Number of preallocated ADC message structures
 */
#define MSG_POOL_SIZE  (8U)

/**
 * @brief Stack size for each RTOS task
 */
#define STACK_SIZE     (256U)

/*==============================================================================
 *                              RTOS STACKS
 *============================================================================*/
/**
 * @brief Stack memory for ADC channel 1 task
 */
OS_STK TaskADC1Stack[STACK_SIZE];

/**
 * @brief Stack memory for ADC channel 2 task
 */
OS_STK TaskADC2Stack[STACK_SIZE];

/**
 * @brief Stack memory for UART task
 */
OS_STK TaskUARTStack[STACK_SIZE];

/**
 * @brief Stack memory for LED blink task
 */
OS_STK TaskBlinkStack[STACK_SIZE];

/*==============================================================================
 *                              RTOS OBJECTS
 *============================================================================*/
/**
 * @brief Mutex protecting ADC hardware access
 */
OS_EVENT *adcMutex;

/**
 * @brief Mutex protecting message pool and drop counter
 */
OS_EVENT *msgMutex;

/**
 * @brief Queue used to send ADC messages to UART task
 */
OS_EVENT *adcQueue;

/**
 * @brief Queue storage area (RTOS requirement)
 */
void *queueStorage[QUEUE_SIZE];

/**
 * @brief RTOS error variable
 */
INT8U osErr;

/*==============================================================================
 *                              DATA TYPES
 *============================================================================*/
/**
 * @brief ADC message structure passed through RTOS queue
 */
typedef struct
{
    uint8_t  id;       /**< ADC channel identifier (1 or 2) */
    uint16_t value;    /**< Raw ADC conversion result */
} ADC_MSG;

/*==============================================================================
 *                              GLOBAL DATA
 *============================================================================*/
/**
 * @brief Preallocated message pool for ADC messages
 */
ADC_MSG adcMsgPool[MSG_POOL_SIZE];

/**
 * @brief Index used for round-robin allocation from message pool
 */
static uint8_t msgIndex = 0U;

/**
 * @brief Counter for dropped ADC samples when queue is full
 *
 * @note Access is protected by msgMutex
 */
volatile uint32_t adc_drop_count = 0UL;

/*==============================================================================
 *                              ADC TASK
 *============================================================================*/
/**
 * @brief RTOS task that reads an ADC channel periodically
 *
 * @param p_data Task argument specifying ADC channel ID
 *               - (void *)1 ? ADC channel 0
 *               - (void *)2 ? ADC channel 1
 *
 * @details
 * This task:
 *  - Acquires the ADC mutex
 *  - Performs a blocking ADC conversion
 *  - Packages the result into a message from the pool
 *  - Sends the message to the UART task via queue
 *  - Delays for 1 second
 *
 * MISRA deviations:
 *  - Infinite loop required for RTOS task
 *  - Direct hardware register access required
 *  - void* cast required by RTOS API
 */
void ADCTask(void *p_data)
{
    uint8_t   adc_id = (uint8_t)p_data;   /* MISRA deviation: RTOS API */
    uint16_t  result;
    ADC_MSG  *msg;

    while (1)
    {
        /* ---- ADC hardware access ---- */
        OSMutexPend(adcMutex, 0U, &osErr);

        ADMUX  = (adc_id == 1U) ? 0x40U : 0x41U;
        ADCSRA |= (1U << ADSC);

        while ((ADCSRA & (1U << ADIF)) == 0U)
        {
            /* Busy wait for ADC completion */
        }

        result = (uint16_t)ADCL | ((uint16_t)ADCH << 8U);
        ADCSRA |= (1U << ADIF);

        OSMutexPost(adcMutex);

        /* ---- Message pool allocation ---- */
        OSMutexPend(msgMutex, 0U, &osErr);

        msg = &adcMsgPool[msgIndex];
        msgIndex = (uint8_t)((msgIndex + 1U) % MSG_POOL_SIZE);

        OSMutexPost(msgMutex);

        msg->id    = adc_id;
        msg->value = result;

        /* ---- Queue post ---- */
        if (OSQPost(adcQueue, msg) != OS_NO_ERR)
        {
            adc_drop_count++;
        }

        OSTimeDlyHMSM(0U, 0U, 1U, 0U);
    }
}

/*==============================================================================
 *                              UART FUNCTIONS
 *============================================================================*/
/**
 * @brief Initialize UART peripheral
 *
 * @param ubrr UART baud rate register value
 */
void UART_Init(unsigned int ubrr)
{
    UBRR0H = (uint8_t)(ubrr >> 8U);
    UBRR0L = (uint8_t)(ubrr);

    UCSR0B = (1U << RXEN0) | (1U << TXEN0);
    UCSR0C = (1U << USBS0) | (3U << UCSZ0);
}

/**
 * @brief Transmit a single character over UART
 *
 * @param c Character to transmit
 */
void UART_TxChar(char c)
{
    while ((UCSR0A & (1U << UDRE0)) == 0U)
    {
        /* Wait until transmit buffer is empty */
    }

    UDR0 = c;
}

/**
 * @brief Transmit a null-terminated string over UART
 *
 * @param str Pointer to string buffer
 */
void UART_TxString(const char *str)
{
    while (*str != '\0')
    {
        UART_TxChar(*str++);
    }
}

/*==============================================================================
 *                              UART TASK
 *============================================================================*/
/**
 * @brief RTOS task responsible for UART output
 *
 * @param p_data Unused
 *
 * @details
 * This task:
 *  - Waits for ADC messages from the queue
 *  - Prints diagnostic warnings if samples were dropped
 *  - Formats and prints ADC values via UART
 */
void UART_Task(void *p_data)
{
    ADC_MSG *msg;
    char     buffer[32];
    uint32_t drops;

    (void)p_data; /* Unused parameter */

    while (1)
    {
        msg = (ADC_MSG *)OSQPend(adcQueue, 0U, &osErr);

        OSMutexPend(msgMutex, 0U, &osErr);
        drops = adc_drop_count;
        adc_drop_count = 0UL;
        OSMutexPost(msgMutex);

        if (drops > 0UL)
        {
            sprintf(buffer, "WARN: dropped=%lu\r\n", drops);
            UART_TxString(buffer);
        }

        if (osErr == OS_NO_ERR)
        {
            if (msg->id == 1U)
            {
                sprintf(buffer, "ADC1=%u\r\n", msg->value);
            }
            else
            {
                sprintf(buffer, "ADC2=%u\r\n", msg->value);
            }

            UART_TxString(buffer);
        }
    }
}

/*==============================================================================
 *                              BLINK TASK
 *============================================================================*/
/**
 * @brief Background LED blink task
 *
 * @param p_data Unused
 */
void BlinkTask(void *p_data)
{
    (void)p_data;

    while (1)
    {
        PORTA ^= (1U << PA0);
        OSTimeDlyHMSM(0U, 0U, 0U, 500U);
    }
}

/*==============================================================================
 *                              MAIN
 *============================================================================*/
/**
 * @brief System entry point
 *
 * @return Program exit status (never returns)
 */
int main(void)
{
    DDRA |= (1U << DDA0);

    TCCR0 = 0x07U;
    TIMSK = 0x01U;

    UART_Init(UART_UBRR);
    ADCSRA = 0x87U;

    OSInit();

    msgMutex = OSMutexCreate(3U, &osErr);
    adcMutex = OSMutexCreate(4U, &osErr);
    adcQueue = OSQCreate(queueStorage, QUEUE_SIZE);

    OSTaskCreate(ADCTask, (void *)1U, &TaskADC1Stack[STACK_SIZE - 1U], 5U);
    OSTaskCreate(ADCTask, (void *)2U, &TaskADC2Stack[STACK_SIZE - 1U], 7U);
    OSTaskCreate(UART_Task, NULL, &TaskUARTStack[STACK_SIZE - 1U], 6U);
    OSTaskCreate(BlinkTask, NULL, &TaskBlinkStack[STACK_SIZE - 1U], 8U);

    OSStart();

    return 0;
}
