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

//void setAlert;

void getTemperature(int16_t * temperature)
{
	uint32_t val;
	uint8_t * rawTemp = (uint8_t *)malloc(1);
	//read from pointer register 0x00(temperature)
	//byte 1 is MSB, byte 2 is LSB(T3-T0)
	val = i2cReadBytes(0x90,0x00,rawTemp,2);
	*temperature = 0;
	*temperature |= ((rawTemp[0]) << 4);
	*temperature |= ((rawTemp[1]) >> 4);
	*temperature = (*temperature & 0x800) ? (*temperature|0xF000) : *temperature;
	free(rawTemp);
	printTemperature(temperature);
}

void printTemperature(int16_t * temperature)
{
	int32_t printTemp;
	printTemp = ((int32_t)*temperature) * .0625;
	PRINTF("%d", printTemp);

}

