
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "driverlib/gpio.h"
#include "utils/uartstdio.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"

#include "inc/hw_i2c.h"
#include "driverlib/i2c.h"


#include "tivaware_i2c.h"

void ConfigureUART(void)
{
    //
    // Enable the GPIO Peripheral used by the UART.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    //
    // Enable UART0
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);

    //
    // Configure GPIO Pins for UART mode.
    //
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    //
    // Use the internal 16MHz oscillator as the UART clock source.
    //
    UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);

    //
    // Initialize the UART for console I/O.
    //
    UARTStdioConfig(0, 115200, 16000000);
}


void test1(){
	//    Mag Stuff
	    i2c0_write_byte(0x1E, 0x02, 0x00);
	    UARTprintf("Error: %5u\n", I2CMasterErr(I2C3_BASE));

	    while(1){
	    	uint8_t buff[6];
	    	uint16_t x, y, z;
	    	i2c0_read_bytes(0x1E, 0x03, buff, 6);
	    	UARTprintf("Error: %5u\n", I2CMasterErr(I2C3_BASE));

	//    	i2c0_read_byte(0x1E, 0x03, &buff[0]);
	//    	i2c0_read_byte(0x1E, 0x04, &buff[1]);
	//    	i2c0_read_byte(0x1E, 0x05, &buff[2]);
	//    	i2c0_read_byte(0x1E, 0x06, &buff[3]);
	//    	i2c0_read_byte(0x1E, 0x07, &buff[4]);
	//    	i2c0_read_byte(0x1E, 0x08, &buff[5]);

	    	x = (((uint16_t)buff[0]) << 8) | ((uint16_t)buff[1]);
	    	y = (((uint16_t)buff[2]) << 8) | ((uint16_t)buff[3]);
	    	z = (((uint16_t)buff[4]) << 8) | ((uint16_t)buff[5]);

	    	UARTprintf("%5d %5d %5d\n", (int16_t)x, (int16_t)y, (int16_t)z);
	    }
}

void test2(){
    UARTprintf("Error: %5u\n", I2CMasterErr(I2C3_BASE));
    i2c3_write_byte(0x68, 27, GYRO_FULL_SCALE_2000_DPS);
    UARTprintf("Error: %5u\n", I2CMasterErr(I2C3_BASE));
    i2c3_write_byte(0x68, 28, ACC_FULL_SCALE_4_G);
    UARTprintf("Error: %5u\n", I2CMasterErr(I2C3_BASE));
    i2c3_write_byte(0x68, 0x37, 0x02);
    UARTprintf("Error: %5u\n", I2CMasterErr(I2C3_BASE));
    i2c3_write_byte(MAG_ADDRESS, 0x0A, 0x01);
    UARTprintf("Error: %5u\n", I2CMasterErr(I2C3_BASE));

    while(1){
    	uint8_t buff[14];
    	int16_t ax, ay, az, gx, gy, gz;
    	i2c3_read_bytes(0x68, 0x3B, buff, 14);
    	UARTprintf("Error: %5u\n", I2CMasterErr(I2C3_BASE));

    	ax = (int16_t)((((uint16_t)buff[0]) << 8) | ((uint16_t)buff[1]));
    	ay = (int16_t)((((uint16_t)buff[2]) << 8) | ((uint16_t)buff[3]));
    	az = (int16_t)((((uint16_t)buff[4]) << 8) | ((uint16_t)buff[5]));

    	gx = (int16_t)((((uint16_t)buff[8]) << 8) | ((uint16_t)buff[9]));
    	gy = (int16_t)((((uint16_t)buff[10]) << 8) | ((uint16_t)buff[11]));
    	gz = (int16_t)((((uint16_t)buff[12]) << 8) | ((uint16_t)buff[13]));

    	UARTprintf("%5d %5d %5d\n", gx, gy, gz);
//    	SysCtlDelay(1000u);
    }
}

/*
 * main.c
 */
int main(void) {

#define    MPU9250_ADDRESS            0x68
#define    MAG_ADDRESS                0x0C

#define    GYRO_FULL_SCALE_250_DPS    0x00
#define    GYRO_FULL_SCALE_500_DPS    0x08
#define    GYRO_FULL_SCALE_1000_DPS   0x10
#define    GYRO_FULL_SCALE_2000_DPS   0x18

#define    ACC_FULL_SCALE_2_G        0x00
#define    ACC_FULL_SCALE_4_G        0x08
#define    ACC_FULL_SCALE_8_G        0x10
#define    ACC_FULL_SCALE_16_G       0x18


    SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);

    ConfigureUART();

    UARTprintf("MPU9250Test\n");


    i2c3_init();


	return 0;
}
