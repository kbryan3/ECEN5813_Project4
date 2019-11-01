 /********************************************************************
*
* @file bit.c
* @brief Contains a function to run BIT
*
* @author Kyle Bryan
* @date October 2019
* version 1.0
*
***********************************************************************/

#include "bit.h"

my_bit_result runBIT()
{

	uint8_t temp;
	temp = i2cReadByte(0x90, 0x00);
	uint8_t temp_result;
	delay();
	temp_result = i2cReadByte(0x90, 0x01);
	//if register read does not return expected value
	if((temp_result == 98) && (temp!=temp_result))
	{
		return BITPASS;
	}
	else if(temp_result == 96)
	{
		return BITPASS;
	}
	else
	{
		return BITFAIL;
	}
}

void delay()
{
	uint32_t number = 667;
	while(number !=0)
	{
		__asm volatile("nop");
		number--;
	}
}
