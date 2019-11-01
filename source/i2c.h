 /********************************************************************
*
* @file i2c.h
* @brief Has functions to initialize, read and write via I2C
*
* @author Kyle Bryan
* @date October 2019
* version 1.0
*
***********************************************************************/
#ifndef __I2C_H__
#define __I2C_H__

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"
#include "fsl_debug_console.h"
#include "logger.h"

#define I2C_M_START I2C0->C1 |= I2C_C1_MST_MASK
#define I2C_M_STOP  I2C0->C1 &= ~I2C_C1_MST_MASK
#define I2C_M_RSTART I2C0->C1 |= I2C_C1_RSTA_MASK

#define I2C_TRAN I2C0->C1 |= I2C_C1_TX_MASK
#define I2C_REC  I2C0->C1 &= ~I2C_C1_TX_MASK

#define I2C_WAIT while((I2C0->S & I2C_S_IICIF_MASK)==0){} \
					I2C0->S |= I2C_S_IICIF_MASK;

#define NACK I2C0->C1 |= I2C_C1_TXAK_MASK
#define ACK I2C0->C1 &= ~I2C_C1_TXAK_MASK

typedef enum i2c_test
{
	PASS,
	FAIL// sensor is disconnected

}i2c_test;


/**
* @brief Initializes I2C controller
*
*@return void
*/
void i2cInit();

/**
* @brief Writes a single byte over the I2C bus
*
*  Given a byte of data this function writes it to a chosen device via I2C
*
*@param uint8_t dev device address to write to
*@param uint8_t reg register to write to
*@param uint8_t data data to write to register
*
*@return void
*/
void i2cWriteByte(uint8_t dev, uint8_t reg, uint8_t data);

/**
* @brief Writes multiple bytes over the I2C bus
*
*  Given bytes of data this function writes them to a chosen device via I2C
*
*@param uint8_t dev device address to write to
*@param uint8_t reg register to write to
*@param uint8_t data data to write to register
*@param uint8_t bytes number of bytes to write
*
*@return void
*/
void i2cWriteBytes(uint8_t dev, uint8_t reg, uint8_t * data, uint8_t bytes);

/**
* @brief Reads a single byte over the I2C bus
*
*  Given an address this function reads a byte from that peripherals register
*
*@param uint8_t dev device address to read from
*@param uint8_t reg register to read from
*
*@return uint8_t byte read from chosen device/register
*/
uint8_t i2cReadByte(uint8_t dev, uint8_t reg);

/**
* @brief Reads multiple bytes over the I2C bus
*
*  Given an address this function reads bytes from that peripherals register
*
*@param uint8_t dev device address to read from
*@param uint8_t reg register to read from
*@param uint8_t *data pointer to data that you want to read
*@param int8_t data_count number of bytes to read
*
*@return uint8_t byte to say complete
*/
uint8_t i2cReadBytes(uint8_t dev_adx, uint8_t reg_adx, uint8_t * data,
		int8_t data_count);

#endif /* __I2C_H__ */
