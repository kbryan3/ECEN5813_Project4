 /********************************************************************
*
* @file statemachine.h
* @brief Can be used to run a state based and table based statemachine
*
* @author Kyle Bryan
* @date October 2019
* version 1.0
*
***********************************************************************/
#ifndef __STATEMACHINE_H__
#define __STATEMACHINE_H__

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
#include "tmp102.h"

typedef enum machine_state
{
	TEMP_READING,    //reading temperature state
	AVERAGE_WAIT,      //averaging temperature and waiting state
	TEMP_ALERT,        //temperature alert
	DISCONNECTED       // sensor is disconnected

}machine_state;


machine_state stateStateMachine(int16_t * temperature, uint8_t * numReadings, int16_t * averageTemp);

void averageReading(uint8_t * numReadings, int16_t * averageTemp, int16_t * temperature);

void printAverageTemperature(int16_t * averageTemp);


#endif /* __STATEMACHINE_H__ */
