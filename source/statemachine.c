 /********************************************************************
*
* @file statemachine.c
* @brief Can be used to run a state based and table based statemachine
*
* @author Kyle Bryan
* @date October 2019
* version 1.0
*
* running average calculation found at:
*  //https://stackoverflow.com/questions/12636613/how-to-calculate-moving-average-without-keeping-the-count-and-data-total
***********************************************************************/

#include "statemachine.h"

machine_state stateStateMachine(int16_t * temperature, uint8_t * numReadings, int16_t * averageTemp)
{
	machine_state state = TEMP_READING;
	uint8_t timeout_count = 0;
	while(1)
	{
		switch(state)
		{
			case TEMP_READING : //get temperature
				//set LED Green
		    	toggleLED(1);
				getTemperature(temperature);
				state = AVERAGE_WAIT;
				break;
			case AVERAGE_WAIT : //average temperature and wair
				//set LED Green
		    	toggleLED(1);
				(*numReadings)++;
				averageReading(numReadings, averageTemp, temperature);
				printAverageTemperature(averageTemp);
				//wait for 15 seconds
				timeout_count++;
				if(timeout_count < 4)
				{
					state = TEMP_READING;
					break;
				}
				else
				{
					return state;
				}
			case TEMP_ALERT :
				//set LED Blue
		    	toggleLED(2);
				//disable interrupts for alert transitions
				getTemperature(temperature);
				state = AVERAGE_WAIT;
				break;
			case DISCONNECTED :
				toggleLED(0);
				return state;
		}

	}
}
void averageReading(uint8_t * numReadings, int16_t * averageTemp, int16_t * temperature)
{
	if(*numReadings == 1)
	{
		*averageTemp = *temperature;
	}
	else
	{
//https://stackoverflow.com/questions/12636613/how-to-calculate-moving-average-without-keeping-the-count-and-data-total
		*averageTemp = (((*averageTemp)*((*numReadings)-1)/(*numReadings))+
				((*temperature)/(*numReadings)))+1;
	}
	return;
}

void printAverageTemperature(int16_t * averageTemp)
{
	int32_t printTemp;
	printTemp = ((int32_t)*averageTemp) * .0625;
	log_string((uint8_t*)"Average Temperature: ",log_level, PRINTAVERAGETEMPERATURE);
	PRINTF("%d\n\r", printTemp);

}

