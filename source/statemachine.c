 /********************************************************************
*
* @file statemachine.c
* @brief Can be used to run a state based and table based state machine
*
* @author Kyle Bryan
* @date November 2019
* version 1.0
*
* running average calculation found at:
*  //https://stackoverflow.com/questions/12636613/how-to-calculate-moving-average-without-keeping-the-count-and-data-total
***********************************************************************/

#include "statemachine.h"

machine_state stateStateMachine(int16_t * temperature, uint8_t * numReadings, int16_t * averageTemp)
{
	if(!temperature || !numReadings || !averageTemp)
	{
		return DISCONNECTED;
	}
	machine_state state = TEMP_READING;
	my_bit_result bit = PASS;
	uint8_t timeout_count = 0;
	while(1)
	{
		switch(state)
		{
			case TEMP_READING : //get temperature
			log_string((uint8_t*)"In TEMP_READING State: ",DBUG, STATESTATEMACHINE);
				//clear Interrupt Flag
				g_alert = 0;
				PORTA->PCR[4] |= 0x1000000;
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
				log_string((uint8_t*)"In AVERAGE_WAIT State: ",DBUG, STATESTATEMACHINE);
				(*numReadings)++;
				averageReading(numReadings, averageTemp, temperature);
				printAverageTemperature(averageTemp);
				//wait for 5 seconds
				for(uint8_t i = 0; i < 15; i++)
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
				log_string((uint8_t*)"In TEMP_ALERT State: ",DBUG, STATESTATEMACHINE);
				//set LED Blue
		    	toggleLED(2);
		    	//set g_alert to 0
		    	g_alert = 0;
				getTemperature(temperature);
				state = AVERAGE_WAIT;
				break;
			case DISCONNECTED :
				NVIC_DisableIRQ(PORTA_IRQn);
				log_string((uint8_t*)"In DISCONNECTED State: ",DBUG, STATESTATEMACHINE);
				toggleLED(0);
				return state;
		}
	}
}

machine_state stateTableMachine(int16_t * temperature, uint8_t * numReadings, int16_t * averageTemp,
		struct sStateTableEntry *currentState)
{
	if(!temperature || !numReadings || !averageTemp || !currentState)
	{
		return DISCONNECTED;
	}
	struct sStateTableEntry *cstate = (struct sStateTableEntry*)malloc(sizeof(struct sStateTableEntry));
	*cstate = *currentState;
	uint8_t timeout_count = 0;
	my_bit_result bit = BITPASS;
	cstate->state = TEMP_READING;
	while(1)
	{
		if(cstate->state == TEMP_READING)
		{
			log_string((uint8_t*)"In TEMP_READING State: ",DBUG, STATETABLEMACHINE);
			//clear Interrupt Flag
			g_alert = 0;
			PORTA->PCR[4] |= 0x1000000;
			//enable GPIO IRQ
			NVIC_EnableIRQ(PORTA_IRQn);
			//set LED Green
			toggleLED(1);
			//check for disconnect
			bit = runBIT();
			if(bit == BITFAIL)
			{
				HandleEventDisconnect(cstate);
			}
			else
			{
				getTemperature(temperature);
				//if tmp102 alert has occured go to TEMP_ALERT state
				if(g_alert == 1)
				{
					HandleEventAlert(cstate);
				}
				else
				{
					HandleEventComplete(cstate);
				}
			}
		}
		else if(cstate->state == AVERAGE_WAIT)
		{
			//Disable Interrupt
			NVIC_DisableIRQ(PORTA_IRQn);
			log_string((uint8_t*)"In AVERAGE_WAIT State: ",DBUG, STATETABLEMACHINE);
			(*numReadings)++;
			averageReading(numReadings, averageTemp, temperature);
			printAverageTemperature(averageTemp);
			//wait for 15 seconds
			for(uint8_t i = 0; i < 15; i++)
			{
				bit=runBIT();
				if(bit == BITFAIL)
				{
					HandleEventDisconnect(cstate);
					break;
				}
				else
				{
					delay1s();
					//set LED Green
			    	toggleLED(1);
				}
			}
			if(cstate->state == DISCONNECTED)
			{
				HandleEventDisconnect(cstate);
			}
			else
			{
				timeout_count++;
				if(timeout_count < 4)
				{
					HandleEventComplete(cstate);
				}
				else
				{
					return cstate->state;
				}
			}
		}
		else if(cstate->state == TEMP_ALERT)
		{
			//Disable Interrupt for Alert
			NVIC_DisableIRQ(PORTA_IRQn);
			log_string((uint8_t*)"In TEMP_ALERT State: ",DBUG, STATETABLEMACHINE);
			//set LED Blue
	    	toggleLED(2);
	    	//set g_alert to 0
	    	g_alert = 0;
			getTemperature(temperature);
			HandleEventComplete(cstate);
		}
		else if(cstate->state == DISCONNECTED)
		{
			NVIC_DisableIRQ(PORTA_IRQn);
			log_string((uint8_t*)"In DISCONNECTED State: ",DBUG, STATETABLEMACHINE);
			toggleLED(0);
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
	log_string((uint8_t*)"Average Temperature: ",DBUG, PRINTAVERAGETEMPERATURE);
	log_string((uint8_t*)"Average Temperature: ",STATUS, PRINTAVERAGETEMPERATURE);
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
