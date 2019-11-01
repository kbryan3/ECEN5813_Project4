 /********************************************************************
*
* @file statemachine.c
* @brief Can be used to run a state based and table based state machine
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
	my_bit_result bit = PASS;
	uint8_t timeout_count = 0;
	uint32_t data;
	while(1)
	{
		switch(state)
		{
			case TEMP_READING : //get temperature
				//clear Interrupt Flag
				g_alert = 0;
				PORTA->PCR[4] |= 0x1000000;
				data = (GPIOA->PDIR & 0x10);
				PRINTF("%d\n\r", data);
				//enable GPIO IRQ
				NVIC_EnableIRQ(PORTA_IRQn);
				//set LED Green
		    	toggleLED(1);
		    	//check for disconnect
		    	bit = runBIT();
		    	if(bit == BITFAIL)
		    	{
		    		state = DISCONNECTED;
		    		break;
		    	}
				getTemperature(temperature);
				//if tmp102 alert has occured go to TEMP_ALERT state
				if(g_alert == 1)
				{
					state = TEMP_ALERT;
				}
				else
				{
					state = AVERAGE_WAIT;
				}
				break;
			case AVERAGE_WAIT : //average temperature and wair
				//Disable Interrupt
				NVIC_DisableIRQ(PORTA_IRQn);
				(*numReadings)++;
				averageReading(numReadings, averageTemp, temperature);
				printAverageTemperature(averageTemp);
				//wait for 5 seconds
				for(uint8_t i = 0; i < 5; i++)
				{
					bit=runBIT();
					if(bit == BITFAIL)
					{
						state = DISCONNECTED;
						break;
					}
					else
					{
						delay1s();
						//set LED Green
				    	toggleLED(1);
					}
				}
				if(state == DISCONNECTED)
				{
					break;
				}
				else
				{
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
				}
			case TEMP_ALERT :
				//Disable Interrupt for Alert
				NVIC_DisableIRQ(PORTA_IRQn);
				//set LED Blue
		    	toggleLED(2);
		    	//set g_alert to 0
		    	g_alert = 0;
				getTemperature(temperature);
				state = AVERAGE_WAIT;
				break;
			case DISCONNECTED :
				toggleLED(0);
				return state;
		}

	}
}

machine_state stateTableMachine(int16_t * temperature, uint8_t * numReadings, int16_t * averageTemp,
		struct sStateTableEntry *currentState)
{
	struct sStateTableEntry *cstate = (struct sStateTableEntry*)malloc(sizeof(struct sStateTableEntry));
	*cstate = *currentState;
	uint8_t timeout_count = 0;
	my_bit_result bit = BITPASS;
	cstate->state = TEMP_READING;
	while(1)
	{
		if(cstate->state == TEMP_READING)
		{
	    	toggleLED(1);
	    	bit = runBIT();
	    	if(bit == BITFAIL)
	    	{
	    		HandleEventDisconnect(cstate);
	    	}
	    	else
	    	{
	    		getTemperature(temperature);
	    		HandleEventComplete(cstate);
	    	}
		}
		else if(cstate->state == AVERAGE_WAIT)
		{
			toggleLED(1);
			(*numReadings)++;
			averageReading(numReadings, averageTemp, temperature);
			printAverageTemperature(averageTemp);
			//wait for 15 seconds
			timeout_count++;
			if(timeout_count < 4)
			{
				HandleEventTimout3(cstate);
			}
			else
			{
				return cstate->state;
			}
		}
		else if(cstate->state == TEMP_ALERT)
		{
			//set LED Blue
			toggleLED(2);
			//disable interrupts for alert transitions
			getTemperature(temperature);
			HandleEventComplete(cstate);
		}
		else if(cstate->state == DISCONNECTED)
		{
			return cstate->state;
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

void HandleEventAlert(struct sStateTableEntry *currentState)
{
	currentState->state = currentState->alert;
}
void HandleEventComplete(struct sStateTableEntry *currentState)
{
	currentState->state = currentState->complete;
}
void HandleEventDisconnect(struct sStateTableEntry *currentState)
{
	currentState->state = currentState->disconnect;
}
void HandleEventTimout3(struct sStateTableEntry *currentState)
{
	currentState->state = currentState->timout1_3;
}
void HandleEventTimout4(struct sStateTableEntry *currentState)
{
	currentState->state = currentState->timeout4;
}

void Init_SysTick(void)
{
	SysTick->LOAD = (480000000L/16);
	SysTick->VAL = 0;
	SysTick->CTRL = SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;
}

void SysTick_Handler()
{
	__disable_irq();
	g_testrun = 1;
	g_count++;
	__enable_irq();
}

void PORTA_IRQHandler(void)
{
	NVIC_DisableIRQ(PORTA_IRQn);
	g_alert = 1;
	PORTA->ISFR |= 0x10;
}

void delay1s()
{
	uint32_t number = 4166667;
	while(number !=0)
	{
		__asm volatile("nop");
		number--;
	}
}
