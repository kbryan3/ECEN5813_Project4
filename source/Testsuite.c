/*****************************************************************************
 *                                                                           *
 *  uCUnit - A unit testing framework for microcontrollers                   *
 *                                                                           *
 *  (C) 2007 - 2008 Sven Stefan Krauss                                       *
 *                  https://www.ucunit.org                                   *
 *                                                                           *
 *  File        : Testsuite.h                                                *
 *  Description : Sample testsuite                                           *
 *  Author      : Sven Stefan Krauss                                         *
 *  Contact     : www.ucunit.org                                             *
 *                                                                           *
 *****************************************************************************/

/*
 * This file is part of ucUnit.
 *
 * You can redistribute and/or modify it under the terms of the
 * Common Public License as published by IBM Corporation; either
 * version 1.0 of the License, or (at your option) any later version.
 *
 * uCUnit is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * Common Public License for more details.
 *
 * You should have received a copy of the Common Public License
 * along with uCUnit.
 *
 * It may also be available at the following URL:
 *       http://www.opensource.org/licenses/cpl1.0.txt
 *
 * If you cannot obtain a copy of the License, please contact the
 * author.
 */

 /*
 * Modified by Kyle Bryan to run with temp sensor Project 4
 * November 2019
 */

#include <stdio.h>
#include <stdlib.h>
#include "System.h"
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

#include "System.h"
#include "uCUnit.h"
#include "Testsuite.h"

static void Test_BasicChecks()
{
	UCUNIT_TestcaseBegin("DEMO:Simple Checks");
    //test 1
	my_bit_result bit = runBIT();
	UCUNIT_CheckIsEqual( bit, BITPASS ); /* Pass*/
	//test 2
	uint32_t val;
    uint8_t * rawTemp = (uint8_t *)malloc(2);
    val = i2cReadBytes(0x90,0x00,rawTemp,2);
	free(rawTemp);
	UCUNIT_CheckIsEqual(val, 1); /* Pass*/
	//test 3
	UCUNIT_CheckIsEqual(log_level, TEST);
	//test 4
	uint8_t data = 0;
	data =  i2cReadByte(0x90, 0x01);
	UCUNIT_CheckIs8Bit(data);
	//test 5
	UCUNIT_CheckIsEqual(data,0x62)
    UCUNIT_TestcaseEnd();
}

static void Test_Pointers()
{

    log_string((uint8_t*)"Pointer Related Tests", log_level, TEST_POINTERS);
	int16_t * temperature = (int16_t *)malloc(2);
	uint8_t * alertLowTemp = (uint8_t *)malloc(2);
	uint8_t * numReadings = (uint8_t *)malloc(1);
	int16_t * averageTemp = (int16_t *)malloc(2);

//test 6
    UCUNIT_CheckIsNotNull(temperature); /* Pass */
//    log_string
//test 7
    UCUNIT_CheckIsNotNull(alertLowTemp); /* Pass */
//test 8
    UCUNIT_CheckIsNotNull(numReadings); /* Pass */
//test 9
    UCUNIT_CheckIsNotNull(averageTemp); /* Pass */
//test 10
    getTemperature(temperature);
    UCUNIT_CheckIsNotNull(temperature);

    free(temperature);
    free(alertLowTemp);
    free(numReadings);
    free(averageTemp);



    UCUNIT_TestcaseEnd(); /* Pass */
}

int testSuite(void)
{
    log_level = TEST;
    log_string((uint8_t*)"Starting ucTest, Results Below:", TEST, TESTSUITE);
	UCUNIT_Init();
    UCUNIT_WriteString("\n\r**************************************");
    UCUNIT_WriteString("\n\rName:     ");
    UCUNIT_WriteString("uCUnit application");
    UCUNIT_WriteString("\n\rCompiled: ");
    UCUNIT_WriteString(__DATE__);
    UCUNIT_WriteString("\n\rTime:     ");
    UCUNIT_WriteString(__TIME__);
    UCUNIT_WriteString("\n\rVersion:  ");
    UCUNIT_WriteString(UCUNIT_VERSION);
    UCUNIT_WriteString("\n\r**************************************");
    Test_Pointers();
    Test_BasicChecks();
    UCUNIT_WriteSummary();
    UCUNIT_Shutdown();

    return 0;
}
