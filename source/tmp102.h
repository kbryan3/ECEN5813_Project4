 /********************************************************************
*
* @file tmp102.h
* @brief Has functions to initialize and read temperature values from tmp102
*
* @author Kyle Bryan
* @date November 2019
* version 1.0
*
***********************************************************************/
#ifndef __TMP102_H__
#define __TMP102_H__

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
#include "i2c.h"

/**
 *  @brief enables TMP102 interrupt mode
 */
void enableInterruptMode();
/**
* @brief Sets the low temperature for the alert
*
* @param uint8_t* tempLow pointer to low temp value(note to feed in as though it is signed)
*/
void setAlertLow(uint8_t * tempLow);

/**
* @brief Gets temperature from tmp102
*
* @param int16_t* temperature pointer to the most recent temp reading
*/
void getTemperature(int16_t * temperature);

/**
* @brief Prints the temperature
*
* @param int16_t* temperature pointer to the most recent temp reading
*
*/
void printTemperature(int16_t * temperature);

/**
* @brief Initializes the alert pin(PTD0) on the FRDM board
*
*
*/
void initAlertPinInterrupt();



#endif /* __TMP102_H__ */
