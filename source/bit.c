 /********************************************************************
*
* @file bit.c
* @brief Contains a function to run BIT
*
* @author Kyle Bryan
* @date November 2019
* version 1.0
*
***********************************************************************/

#include "bit.h"

my_bit_result runBIT()
{
	log_string((uint8_t*)"Starting BIT: ",DBUG, RUNBIT);
	uint8_t temp;
	temp = i2cReadByte(0x90, 0x00);
	uint8_t temp_result;
	//delay between reads
	delay();
	temp_result = i2cReadByte(0x90, 0x01);
	//if register read does not return expected value, fails BIT
	if((temp_result == 98) && (temp!=temp_result))
	{
		log_string((uint8_t*)"BIT PASSED: ",DBUG, RUNBIT);
		return BITPASS;
	}
	else if(temp_result == 96)
	{
		log_string((uint8_t*)"BIT PAssed: ",DBUG, RUNBIT);
		return BITPASS;
	}
	else
	{
		log_string((uint8_t*)"BIT FAILED: ",DBUG, RUNBIT);
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
