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
	uint8_t temp_result;
	temp_result = i2cReadByte(0x90, 0x01);
	//if register read does not return expected value
	if(temp_result == 98)
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
