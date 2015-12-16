
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
#include "tivaware_utils.h"
#include "mpu9250.h"

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

void print_values_test(){
	
	int8_t ret;
	
	i2c3_init();
	mpu9250_init();
	
	while(1){
		int16_t a[3], t, g[3], m[3];
		
		mpu9250_read_accel_temp_gyro(a, &t, g);
		ret = mpu9250_read_mag(m);
		
		
		UARTprintf("Accel %5d %5d %5d\n", a[0], a[1], a[2]);
		UARTprintf("Temp %5d\n", t);
		UARTprintf("Gyro %5d %5d %5d\n", g[0], g[1], g[2]);
		UARTprintf("Mag %5d %5d %5d ret:%d\n\n", m[0], m[1], m[2], ret);
		
		delay_ms(500);
	}
}

/*
 * main.c
 */
int main(void) {



    SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);

    ConfigureUART();

    UARTprintf("Program Start\n");
    print_values_test();

	return 0;
}
