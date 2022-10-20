#include <stdio.h>
#include <stdint.h>
#include <pigpio.h>


//device address
#define BMP180_DEV_ADDR 0x77

//EEPROM base address
#define EEPROM_BASE_ADDRESS 0xAA

//oversampling settings
#define BMP180_OSS_LOWPOWER            0
#define BMP180_OSS_STANDARD            1
#define BMP180_OSS_HIGHRES             2
#define BMP180_OSS_ULTRAHIGHRES        3

//commands
#define BMP180_COM_CTRL_MEAS_TEMP      0x2E
#define BMP180_COM_CTRL_MEAS_PRESS     0x34
#define BMP180_COM_SOFT_RESET          0xB6

//addresses of control registers
#define BMP180_ADDR_OUT_XLSB           0xF8
#define BMP180_ADDR_OUT_LSB            0xF7
#define BMP180_ADDR_OUT_MSB            0xF6
#define BMP180_ADDR_CTRL_MEAS          0xF4
#define BMP180_ADDR_SOFT_RESET         0xE0
#define BMP180_ADDR_ID                 0xD0



#define BMP180_BIT_CTRL_MEAS_SCO       (1<<5)
#define BMP180_BIT_CTRL_MEAS_SCO_SHIFT 5
#define BMP180_BIT_CTRL_MEAS_OSS       (0x3<<6)
#define BMP180_BIT_CTRL_MEAS_OSS_SHIFT 6



void BMP180_EPROM_DATA(uint8_t fd );
uint16_t BMP180_READ_UT(uint8_t fd );
short BMP180_CALC_TEMP(unsigned long ut);
uint32_t BMP180_READ_UP(uint8_t fd , unsigned char oss);
uint16_t BMP180_READ_UT(uint8_t fd );
uint32_t BMP180_CALC_PRESS(unsigned long up , unsigned char oss);


