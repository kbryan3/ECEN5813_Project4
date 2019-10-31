/*
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of NXP Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @file    main.c
 * @brief   Application entry point.
 */
#include <stdio.h>
#include <stdlib.h>

#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"
#include "fsl_debug_console.h"
#include "led_control.h"
#include "i2c.h"
#include "logger.h"
#include "tmp102.h"
#include "statemachine.h"
#include "bit.h"
#include "Testsuite.h"

#define TEST;

const uint8_t RED = 0;
const uint8_t GREEN = 1;
const uint8_t BLUE = 2;
const uint8_t OFF = 3;
struct sStateTableEntry stateTablex[] ={
		{TEMP_READING, AVERAGE_WAIT, TEMP_ALERT, DISCONNECTED, TEMP_READING, TEMP_READING},
		{AVERAGE_WAIT, TEMP_READING, AVERAGE_WAIT, DISCONNECTED, TEMP_READING, AVERAGE_WAIT},
		{TEMP_ALERT, AVERAGE_WAIT, TEMP_ALERT, DISCONNECTED, TEMP_ALERT, TEMP_ALERT},
		{DISCONNECTED, DISCONNECTED, DISCONNECTED, DISCONNECTED, DISCONNECTED, DISCONNECTED}

};

_Bool log_a;
uint8_t  result;
uint8_t g_count;
uint8_t g_testrun;
uint8_t g_alert;

logger_level log_level;
//machine_state states;
machine_state state;
my_bit_result bit;



/*
 * @brief   Application entry point.
 */
int main(void) {
#ifndef TEST
  	/* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
  	/* Init FSL debug console. */
    BOARD_InitDebugConsole();
    initializeLEDs();
    toggleLED(OFF);
    i2cInit();

    //POST Test
	bit = runBIT();
	if(bit == BITFAIL)
	{
		toggleLED(0);
		while(1){bit = BITFAIL;}
	}

    enableInterruptMode(); //enables TMP102 interrupt mode
    int16_t * temperature = (int16_t *)malloc(2);
    uint8_t * alertLowTemp = (uint8_t *)malloc(2);
    uint8_t * numReadings = (uint8_t *)malloc(1);
    int16_t * averageTemp = (int16_t *)malloc(2);
    getTemperature(temperature);
    *numReadings = 0;
    alertLowTemp[0] = 0; //set the top byte of alert register(signed)
    alertLowTemp[1] = 0; //bottom byte of alert register(signed)
    log_a = 1;
    log_level = DBUG;
    g_count = 0;
    g_testrun = 0;
    g_alert = 0;

    /* Enter an infinite loop*/
    while(1)
    {
    	//run state based state machine
    	state = stateStateMachine(temperature,numReadings,averageTemp);
    	//returns average_wait after 4th timeout and moves to next machine
    	if(state == AVERAGE_WAIT)
    	{
    		//run Table based state machine
    		state = stateTableMachine(temperature,numReadings,averageTemp, stateTablex);
    		if(state == DISCONNECTED)
    		{
        		toggleLED(0);
        		while(1){bit = BITFAIL;}
    		}
    	}
    	//if disconnected then LED is red and wait for reset
    	else
    	{
    		toggleLED(0);
    		while(1){bit = BITFAIL;}
    	}
    }
    return 0;
#else
    log_a = 1;
    uint32_t test = testSuite();
#endif
}
