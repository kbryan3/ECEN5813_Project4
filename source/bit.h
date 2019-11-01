 /********************************************************************
*
* @file bit.h
* @brief Contains a function to run BIT
*
* @author Kyle Bryan
* @date November 2019
* version 1.0
*
***********************************************************************/
#ifndef __BIT_H__
#define __BIT_H__

#include <stdint.h>
#include <stdlib.h>
#include "i2c.h"

typedef enum my_bit_result
{
	BITPASS, //passes BIT
	BITFAIL  //fails BIT
}my_bit_result;

/**
* @brief runs BIT when called
*
*@return bit_result whether BIT Passes or Fails
*/
my_bit_result runBIT();

/**
* @brief delay to be used between reads(prevents a hangup)
*
*/
void delay();

#endif /* __BIT_H__ */
