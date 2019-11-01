 /********************************************************************
*
* @file i2c.h
* @brief Has functions to initialize, read and write via I2C
*
* @author Kyle Bryan
* @date November 2019
* version 1.0
*
*Code for this module was taken from professor's announcement:
*Embedded Systems Fundamentals with Arm Cortex-M based Microcontrollers,
*  Dean, 2017, ARM Education Media
***********************************************************************/

#include "i2c.h"


void i2cInit()
{
	//clock i2c peripheral and port B
	SIM->SCGC4 |= SIM_SCGC4_I2C0_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;

	//set to 400k baud
	I2C0->F |= I2C_F_ICR(0x11) | I2C_F_MULT(0);

	//enable i2c and set to master mode
	I2C0->C1 |= (I2C_C1_IICEN_MASK);

	//select high drive mode
	I2C0->C2 |= (I2C_C2_HDRS_MASK);

	//set timeout
	I2C0->SLTH = I2C_SLTH_SSLT(0x01);

}


void i2cWriteByte(uint8_t dev, uint8_t reg, uint8_t data)
{
	I2C_TRAN; //set to transmit mode
	I2C_M_START; //send start
	I2C0->D = dev; //send dev address(write)
	I2C_WAIT //wait for ack

	I2C0->D = reg; //send register address
	I2C_WAIT

	I2C0->D = data; //send data
	I2C_WAIT

	I2C_M_STOP;
}

void i2cWriteBytes(uint8_t dev, uint8_t reg, uint8_t * data, uint8_t bytes)
{
	I2C_TRAN; //set to transmit mode
	I2C_M_START; //send start
	I2C0->D = dev; //send dev address(write)
	I2C_WAIT //wait for ack

	I2C0->D = reg; //send register address
	I2C_WAIT

	//send given number of bytes of data
	for(uint8_t i =0; i < bytes; i++)
	{
		I2C0->D = data[i]; //send data
		I2C_WAIT
	}

	I2C_M_STOP;
}
uint8_t i2cReadByte(uint8_t dev, uint8_t reg)
{
	uint8_t data;

	I2C_TRAN; //set to transmit mode
	I2C_M_START; //send start
	I2C0->D = dev; //send dev address(write)
	I2C_WAIT //wait for ack

	I2C0->D = reg; //send register address
	I2C_WAIT

	I2C_M_RSTART;	//repeated start
	I2C0->D = (dev|0x1);	//send dev address and read
	I2C_WAIT

	I2C_REC;	//set to receive mode
	NACK;

	data = I2C0->D; //dummy read
	I2C_WAIT

	I2C_M_STOP;  //send stop
	data = I2C0->D; //read data

	return data;
}

uint8_t i2cReadBytes(uint8_t dev_adx, uint8_t reg_adx, uint8_t *data,
		int8_t data_count)
{
//	uint8_t dummy;
	int8_t num_bytes_read=0;
	I2C_TRAN; //set to transmit mode
	I2C_M_START; //send start
	I2C0->D = dev_adx; //send dev address(write)
	I2C_WAIT //wait for ack

	I2C0->D = reg_adx; //send register address
	I2C_WAIT

	I2C_M_RSTART;
	I2C0->D = (dev_adx|0x1);
	I2C_WAIT

	I2C_REC;
	ACK;
	I2C0->D;
	I2C_WAIT
	do
	{
		ACK;
		data[num_bytes_read++] = I2C0->D;
		I2C_WAIT
	}while(num_bytes_read < data_count-2);

	NACK;
	data[num_bytes_read++] = I2C0->D;
	I2C_WAIT
	I2C_M_STOP;
	return 1;

}
