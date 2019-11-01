 /********************************************************************
*
* @file led_control.h
* @brief Has functions to initialize LEDs as well as turn them on and off
*      RED LED is 0
*      Green LED is 1
*      Blue LED is 2
*
* @author Kyle Bryan
* @date November 2019
* version 1.0
*
***********************************************************************/
#ifndef __LED_CONTROL_H__
#define __LED_CONTROL_H__

#include <stdint.h>

/**
* @brief Initializes Red, Green and Blue LEDs so they are outputs
*
* This function calls the board.c macros for initializing the LED GPIO pins to
*  be outputs
*
*@return void
*/
void initializeLEDs();

/**
* @brief Toggles LEDs on/off
*
* This function toggles the given LED ON and the other colors off
*
* @param uint8_t ledColor which LED to toggle on/off
*          (0 is red, 1 is green, 2 is blue)
*
*@return void
*/
void toggleLED(uint8_t ledColor);



#endif /* __LED_CONTROL_H__ */
