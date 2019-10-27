 /********************************************************************
*
* @file logger.c
* @brief Contains functions to log and print data
*
*
*
*
* @author Kyle Bryan
* @date October 2019
* version 1.0
*
***********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#ifndef PC
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"
#include "fsl_debug_console.h"
#endif
#include "logger.h"

void log_enable()
{
	log_a = 1;
}

void log_disable()
{
	log_a = 0;
}


_Bool log_status()
{
	if(log_a)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}


void log_data(uint32_t * loc, size_t length, logger_level level, function_called func)
{
	if(log_a)
	{
		uint8_t * bytes_ptr = (uint8_t *)loc;

		for(uint8_t i = 0; i<length; i++)
		{
#ifndef PC
			printLevel(level);
			printFunction(func);
			PRINTF("Address: 0x%X    Data: 0x%02X\n\r", bytes_ptr+i, bytes_ptr[i]);
		}

		PRINTF("\n\r");
#else
			printf("Address: %p    Data: 0x%02X\n\r", (void*)(bytes_ptr+i), bytes_ptr[i]);
		}

		printf("\n\r");
#endif
	}

}

void log_string(uint8_t * str, logger_level level, function_called func)
{
	if(log_a)
	{
#ifndef PC
		printLevel(level);
		printFunction(func);
		PRINTF("%s", str);
		PRINTF("\n\r");
#else
		printf("%s", str);
		printf("\n");
#endif
	}
}

void log_int(uint32_t * integer, logger_level level, function_called func)
{
	if(log_a)
	{
#ifndef PC
		printLevel(level);
		printFunction(func);
		PRINTF("%d", integer);
		PRINTF("\n\r");
#else
	printf("%ls", integer);
	printf("\n");
#endif
	}
}

void printLevel(logger_level level)
{
	if(level == TEST)
	{
		PRINTF("TEST: ");
	}
	else if(level == DBUG)
	{
		PRINTF("DBUG: ");
	}
	else
	{
		PRINTF("STATUS: ");
	}
}

void printFunction(function_called func)
{
	if(func == TOGGLELED)
	{
		PRINTF("toggleLED(): ");
	}
	else if(func == SETALERTLOW)
	{
		PRINTF("setAlertLow: ");
	}
	else if(func == PRINTTEMPERATURE)
	{
		PRINTF("printTemperature: ");
	}
	else if(func == PRINTAVERAGETEMPERATURE)
	{
		PRINTF("printAverageTemperature(): ");
	}
}

