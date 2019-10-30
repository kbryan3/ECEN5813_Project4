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
#include "led_control.h"
#include "bit.h"

typedef enum machine_state
{
	TEMP_READING,    //reading temperature state
	AVERAGE_WAIT,      //averaging temperature and waiting state
	TEMP_ALERT,        //temperature alert
	DISCONNECTED            // sensor is disconnected

}machine_state;

//table-driven state machine setup

struct sStateTableEntry
{
	machine_state state;
	machine_state complete;
	machine_state alert;
	machine_state disconnect;
	machine_state timout1_3;
	machine_state timeout4;
};

extern uint8_t g_count;
extern uint8_t g_testrun;
extern uint8_t g_alert;


/**
* @brief Handles state transitions for Alerts
*
* @param sStateTableEntry *currentState pointer to current state
*/
void HandleEventAlert(struct sStateTableEntry *currentState);

/**
* @brief Handles state transitions for Completed Actions
*
* @param sStateTableEntry *currentState pointer to current state
*/
void HandleEventComplete(struct sStateTableEntry *currentState);

/**
* @brief Handles state transitions for Disconnects
*
* @param sStateTableEntry *currentState pointer to current state
*/
void HandleEventDisconnect(struct sStateTableEntry *currentState);

/**
* @brief Handles state transitions for Timeouts1..3
*
* @param sStateTableEntry *currentState pointer to current state
*/
void HandleEventTimout3(struct sStateTableEntry *currentState);

/**
* @brief Handles state transitions for 4th Timeout
*
* @param sStateTableEntry *currentState pointer to current state
*/
void HandleEventTimout4(struct sStateTableEntry *currentState);

/**
* @brief A State based state machine for gathering and averaging temps
*
* @param int16_t *temperature pointer to where temperature is stored
* @param uint8_t* numReadings pointer to number of readings for averaging
* @param int16_t* averageTemp pointer to where average temp is stored
*
* @return machine_state the last state the state machine was in
*/
machine_state stateStateMachine(int16_t * temperature, uint8_t * numReadings, int16_t * averageTemp);

/**
* @brief A table based state machine for gathering and averaging temps
*
* @param int16_t *temperature pointer to where temperature is stored
* @param uint8_t* numReadings pointer to number of readings for averaging
* @param int16_t* averageTemp pointer to where average temp is stored
*
* @return machine_state the last state the state machine was in
*/
machine_state stateTableMachine(int16_t * temperature, uint8_t * numReadings, int16_t * averageTemp,
		struct sStateTableEntry *currentState);

/**
* @brief Averages last temp reading with current average
*
* @param int16_t *temperature pointer to where temperature is stored
* @param uint8_t* numReadings pointer to number of readings for averaging
* @param int16_t* averageTemp pointer to where average temp is stored
*
*/
void averageReading(uint8_t * numReadings, int16_t * averageTemp, int16_t * temperature);

/**
* @brief prints Average Temperature
*
* @param int16_t* averageTemp pointer to where average temp is stored
*
*/
void printAverageTemperature(int16_t * averageTemp);
void Init_SysTick(void);

#endif /* __STATEMACHINE_H__ */
