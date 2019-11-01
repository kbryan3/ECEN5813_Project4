 /********************************************************************
*
* @file tmp102.c
* @brief Has functions to initialize and read temperature values from tmp102
*
* @author Kyle Bryan
* @date November 2019
* version 1.0
*
***********************************************************************/
#include "tmp102.h"

void enableInterruptMode()
{
	uint8_t * regValue = (uint8_t *)malloc(2);
	regValue[0] = 0x62;
	regValue[1] = 0xA0;
	i2cWriteBytes(0x90, 0x01, regValue, 2);
	initAlertPinInterrupt();
}

void setAlertLow(uint8_t * tempLow)
{
	if(!tempLow)
	{
		log_string((uint8_t*)"NULLPOINTER! ",DBUG, SETALERTLOW);
	}
	i2cWriteBytes(0x90, 0x02, tempLow, 2);
	log_string((uint8_t*)"Temp Low Set",DBUG, SETALERTLOW);

}


void getTemperature(int16_t * temperature)
{
	if(!temperature)
	{
		log_string((uint8_t*)"NULLPOINTER! ",DBUG, GETTEMPERATURE);
	}
	uint8_t * rawTemp = (uint8_t *)malloc(2);
	//read from pointer register 0x00(temperature)
	//byte 1 is MSB, byte 2 is LSB(T3-T0)
	i2cReadBytes(0x90,0x00,rawTemp,2);
	//shift rawTemp to be in memory as MSB|LSB and convert to 16 bits
	*temperature = 0;
	*temperature |= ((rawTemp[0]) << 4);
	*temperature |= ((rawTemp[1]) >> 4);
	*temperature = (*temperature & 0x800) ? (*temperature|0xF000) : *temperature;
	free(rawTemp);
	log_temp(temperature, TEST, GETTEMPERATURE);
	log_temp(temperature, STATUS, GETTEMPERATURE);
}

void printTemperature(int16_t * temperature)
{
	if(!temperature)
	{
		log_string((uint8_t*)"NULLPOINTER! ",DBUG, PRINTTEMPERATURE);
	}
	int32_t printTemp;
	printTemp = ((int32_t)*temperature) * .0625;
	log_string((uint8_t*)"Temperature: ",DBUG, PRINTTEMPERATURE);
	log_string((uint8_t*)"Temperature: ",STATUS, PRINTTEMPERATURE);
	PRINTF("%d\n\r", printTemp);

}

void initAlertPinInterrupt()
{
	//set PTA4 as input
	GPIOA->PDDR &= ~(0x20);

	//set PTA4 w/ pull-up
	PORTA->PCR[4] |= (PORT_PCR_PE_MASK|PORT_PCR_PS_MASK);

	//configure interrupt for PTA4 on low
	PORTA->PCR[4] &= 0xFFF0FFFFU;
	PORTA->PCR[4] |= 0x80000U;
}
