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
        BMP180_EPROM_DATA(fd );
        uint16_t ut = BMP180_READ_UT(fd) ;
        long up =BMP180_READ_UP(fd , 1);
        printf("Temp = %d C\t\t", BMP180_CALC_TEMP(ut));
        printf("Pressure = %d pa\n", (BMP180_CALC_PRESS(up ,1)));
    }      
}

//gcc -Wall -o main main.c BMP180.c -lpigpio -lrt