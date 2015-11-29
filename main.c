
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

	uint8_t ret;
	uint32_t err;


    SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);

    ConfigureUART();

    UARTprintf("MPU9250Test\n");

    while(1){

    i2c0_init();

    i2c0_write_byte(0x68, 0x0C, 123);
    err = I2CMasterErr(I2C0_BASE);
    if (err != 0){
    	UARTprintf("Error!\n");
    }


    i2c0_read_byte(0x68, 0x0C, &ret);
    err = I2CMasterErr(I2C0_BASE);
    if (err!= 0){
    	UARTprintf("Error!\n");
    }

    }
	return 0;
}
