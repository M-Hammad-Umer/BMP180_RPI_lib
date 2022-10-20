#include <stdio.h>
#include <stdint.h>
#include <pigpio.h>
#include "BMP180.h"

// Callibration stored in EEPROM
short  AC1;
short  AC2;
short  AC3;
unsigned short  AC4;
unsigned short  AC5;
unsigned short  AC6;
short  B1;
short  B2;
short  MB;
short  MC;
short  MD;
// calculated parameters
long B5;




void BMP180_EPROM_DATA(uint8_t fd)
{
	
	char data[22];

	//reading out the calibration registers
	for(uint8_t i = 0 ; i < 22 ; i++)
	{
		i2cWriteByte(fd , EEPROM_BASE_ADDRESS+i );
		data[i] = i2cReadByte(fd);
	}

	// /*parameters AC1-AC6*/
	AC1 =  ((data[0] << 8) | data[1]);
	AC2 =  ((data[2] << 8) | data[3]);
	AC3 =  ((data[4] << 8) | data[5]);
	AC4 =  ((data[6] << 8) | data[7]);
	AC5 =  ((data[8] << 8) | data[9]);
	AC6 =  ((data[10] << 8) | data[11]);

	// /*parameters B1,B2*/
	 B1 =  ((data[12] << 8) | data[13]);
	 B2 =  ((data[14] << 8) | data[15]);

	// /*parameters MB,MC,MD*/
	 MB =  ((data[16] << 8) | data[17]);
	 MC =  ((data[18] << 8) | data[19]);
	 MD =  ((data[20] << 8) | data[21]);
	
}



uint16_t BMP180_READ_UT(uint8_t fd)
{
	uint8_t outputMSB;
	uint8_t outputLSB;
	uint16_t ut;

	i2cWriteByteData(fd , BMP180_ADDR_CTRL_MEAS , BMP180_COM_CTRL_MEAS_TEMP);
	gpioDelay(45000);

	outputMSB = i2cReadWordData(fd ,BMP180_ADDR_OUT_MSB);
	outputLSB = i2cReadWordData(fd ,BMP180_ADDR_OUT_LSB);
	ut = (outputMSB << 8) | outputLSB;
	//printf("MSB = %x   LSB = %x    UT = %x  \n", outputMSB ,outputLSB ,ut);
	return ut;
}



uint32_t BMP180_READ_UP(uint8_t fd , unsigned char oss)
{
	uint8_t outputMSB;
	uint8_t outputLSB;
	uint8_t outputXLSB;
	uint32_t up;

	uint8_t command =BMP180_COM_CTRL_MEAS_PRESS | (oss << BMP180_BIT_CTRL_MEAS_OSS_SHIFT);
	i2cWriteByteData(fd , BMP180_ADDR_CTRL_MEAS , command);
	if(oss == BMP180_OSS_LOWPOWER)
	{
		gpioDelay(4500);
	}
	else if(oss == BMP180_OSS_STANDARD)
	{
		gpioDelay(7500);
	}
	else if(oss == BMP180_OSS_HIGHRES)
	{
		gpioDelay(13500);
	}
	else if(oss == BMP180_OSS_HIGHRES)
	{
		gpioDelay(25500);
	}
	outputMSB = i2cReadWordData(fd ,BMP180_ADDR_OUT_MSB);
	outputLSB = i2cReadWordData(fd ,BMP180_ADDR_OUT_LSB);
	outputXLSB = i2cReadWordData(fd ,BMP180_ADDR_OUT_XLSB);
	up=(outputMSB<<16 | outputLSB<<8 | outputXLSB )>>(8-oss);
	return up;
}






uint32_t BMP180_CALC_PRESS(unsigned long up , unsigned char oss)
{
	long pressure, x1, x2, x3, b3, b6;
	unsigned long b4, b7;

	b6 = B5 - 4000;

	/*****calculate B3************/
	x1 = (b6*b6) >> 12;
	x1 *= B2;
	x1 >>= 11;
	x2 = (AC2*b6);
	x2 >>= 11;
	x3 = x1 + x2;
	b3 = ((((((long)AC1)<<2) + x3) << oss)+2) >> 2;

	/*****calculate B4************/
	x1 = (AC3 * b6) >> 13;
	x2 = (B1 * ((b6*b6) >> 12)) >> 16;
	x3 = ((x1 + x2) + 2) >> 2;
	b4 = (AC4 * (unsigned long) (x3 + 32768)) >> 15;
	b7 = ((unsigned long)(up - b3) * (50000>>oss));
	if (b7 < 0x80000000)
		pressure = (b7 << 1) / b4;
	else
		pressure = (b7 / b4) << 1;
	x1 = pressure >> 8;
	x1 *= x1;
	x1 = (x1 * 3038) >> 16;
	x2 = (pressure * -7357) >> 16;
	pressure += (x1 + x2 + 3791) >> 4;/* pressure in Pa*/
	return pressure;
}







short BMP180_CALC_TEMP(unsigned long ut )
{
	short temperature;
	long x1, x2;
	x1=(((long)ut - (long)AC6) * (long)AC5) >> 15;
	x2=((long)MC << 11) / (x1 + MD);
	B5 = x1+x2;
	temperature = (B5+8) >> 4;
	return (temperature*0.1);
}