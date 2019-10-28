 /********************************************************************
*
* @file tmp102.c
* @brief Has functions to initialize and read temperature values from tmp102
*
* @author Kyle Bryan
* @date October 2019
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
}
void setAlertLow(uint8_t * tempLow)
{

	i2cWriteBytes(0x90, 0x02, tempLow, 2);
	log_string((uint8_t*)"Temp Low Set",log_level, SETALERTLOW);

}


void getTemperature(int16_t * temperature)
{
	uint32_t val;
	uint8_t * rawTemp = (uint8_t *)malloc(2);
	//read from pointer register 0x00(temperature)
	//byte 1 is MSB, byte 2 is LSB(T3-T0)
	val = i2cReadBytes(0x90,0x00,rawTemp,2);
	*temperature = 0;
	*temperature |= ((rawTemp[0]) << 4);
	*temperature |= ((rawTemp[1]) >> 4);
	*temperature = (*temperature & 0x800) ? (*temperature|0xF000) : *temperature;
	free(rawTemp);
	log_temp(temperature, log_level, GETTEMPERATURE);
}

void printTemperature(int16_t * temperature)
{
	int32_t printTemp;
	printTemp = ((int32_t)*temperature) * .0625;
	log_string((uint8_t*)"Temperature: ",log_level, PRINTTEMPERATURE);
	PRINTF("%d\n\r", printTemp);

}

