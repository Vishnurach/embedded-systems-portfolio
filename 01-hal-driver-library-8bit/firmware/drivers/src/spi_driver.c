/**
 * @file    spi_driver.c
 * @author  Vishnu
 * @brief   SPI Driver Implementation
 */

#include "spi_driver.h"

/* ================= Pin Mapping ================= */
/* Define pins here for easy porting to other PICs */
#define SPI_SCK_DIR     TRISC3
#define SPI_SDI_DIR     TRISC4
#define SPI_SDO_DIR     TRISC5

#define DIR_INPUT       1
#define DIR_OUTPUT      0

/* ================= Implementations ================= */

void SPI_Init_Master(void)
{
    /* 1. Configure Pin Directions */
    SPI_SCK_DIR = DIR_OUTPUT; 
    SPI_SDI_DIR = DIR_INPUT;
    SPI_SDO_DIR = DIR_OUTPUT;

    /* 2. Reset Status */
    SSPSTAT = 0x00;

    /* 3. Configure Control: Enable + Master Mode */
    SSPCON = SPI_ENABLE | SPI_MASTER_FOSC4; 
}

void SPI_Init_Slave(void)
{
    /* 1. Configure Pin Directions */
    SPI_SCK_DIR = DIR_INPUT;
    SPI_SDI_DIR = DIR_INPUT;
    SPI_SDO_DIR = DIR_OUTPUT;
    
    /* 2. Reset Status */
    SSPSTAT = 0x00;

    /* 3. Configure Control: Enable + Slave (SS Disabled) */
    SSPCON = SPI_ENABLE | SPI_SLAVE_SS_DIS; 
}

void SPI_Write(uint8_t data)
{
    SSPBUF = data;
}

void SPI_Wait(void)
{
    /* Wait for Interrupt Flag (Transaction Complete) */
    while(SSPIF == 0); 
    SSPIF = 0;         
}