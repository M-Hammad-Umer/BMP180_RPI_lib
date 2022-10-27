#include <stdio.h>
#include <stdint.h>
#include <pigpio.h>

#include "BMP180.h"


uint8_t fd;

int main()
{
    if (gpioInitialise() < 0)
    {
        printf("initialization failed");
    }
    else
    {
        fd = i2cOpen(1, BMP180_DEV_ADDR , 0);        
    }
    
    while(1)
    {
        BMP180_EPROM_DATA(fd );           //get callibrations from EEPROM 
             
        printf("Temp = %d C\t\t", BMP180_get_temp(fd));
        printf("Pressure = %d pa\n", (BMP180_get_bmp(fd ,BMP180_OSS_STANDARD)));
    }      
}

//gcc -Wall -o main main.c BMP180.c -lpigpio -lrt