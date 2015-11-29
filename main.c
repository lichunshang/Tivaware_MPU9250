
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




/*
 * main.c
 */
int main(void) {




    SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);

    ConfigureUART();

    UARTprintf("MPU9250Test\n");


    i2c0_init();
    i2c0_write_byte(0x1E, 0x02, 0x00);

    while(1){
    	uint8_t buff[6];
    	uint16_t x, y, z;
    	i2c0_read_bytes(0x1E, 0x03, buff, 6);

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


	return 0;
}
