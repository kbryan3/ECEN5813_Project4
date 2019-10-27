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

#ifndef PC
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"
#include "fsl_debug_console.h"
#include "led_control.h"
#include "i2c.h"
#endif
#include "logger.h"
#include "tmp102.h"
/* TODO: insert other include files here. */

/* TODO: insert other definitions and declarations here. */
const uint8_t RED = 0;
const uint8_t GREEN = 1;
const uint8_t BLUE = 2;
const uint8_t OFF = 3;
const uint8_t LENGTH = 16;
const uint8_t SEED = 29;

_Bool log_a;
//uint8_t * num_readings;

logger_level log_level;
//machine_state states;



uint8_t passCount = 0;

/*
 * @brief   Application entry point.
 */
int main(void) {

#ifndef PC
  	/* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
  	/* Init FSL debug console. */
    BOARD_InitDebugConsole();
    initializeLEDs();
    toggleLED(OFF);
    i2cInit();
    int16_t * temperature = (int16_t *)malloc(2);
    uint8_t * alertLowTemp = (uint8_t *)malloc(2);
    alertLowTemp[0] = 0; //set the top byte of alert register(signed)
    alertLowTemp[1] = 0; //bottom byte of alert register(signed)
    enableInterruptMode(); //enables TMP102 interrupt mode
    setAlertLow(alertLowTemp);
#endif
    log_a = 1;
    log_level = DBUG;
 //   states = TEMP_READING;




    /* Force the counter to be placed into memory. */
//    volatile static int i = 0 ;
    /* Enter an infinite loop*/
    while(1)
    {
    	log_string((uint8_t*)"LED Blue ON", log_level, TOGGLELED);
    	toggleLED(2);
    	log_string((uint8_t*)"LED Green ON", log_level, TOGGLELED);
    	toggleLED(1);


    	//read from pointer register 0x00(temperature)
    	//byte 1 is MSB, byte 2 is LSB(T3-T0)
    	getTemperature(temperature);







    	__asm volatile ("nop");
    }
    return 0 ;
}
