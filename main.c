
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
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
	int16_t a[3], t, g[3], m[3];
	float a_scale, g_scale, m_scale, mx, my, mz;
	char buff[128];
	
	a_scale = mpu9250_accel_scale();
	g_scale = mpu9250_gyro_scale();
	m_scale = mpu9250_mag_scale();
	
	while(1){
		
		mpu9250_read_accel_temp_gyro(a, &t, g);
		ret = mpu9250_read_mag(m);
		
		mpu9250_gyro_calibrate(g);
		
		sprintf(buff, "%10.3f %10.3f %10.3f\n",  a[0]*a_scale, a[1]*a_scale, a[2]*a_scale);
		UARTprintf(buff);
		sprintf(buff, "%10.3f %10.3f %10.3f\n",  g[0]*g_scale, g[1]*g_scale, g[2]*g_scale);
		UARTprintf(buff);
		
		mx = m[0]*m_scale;
		my = m[1]*m_scale;
		mz = m[2]*m_scale;
		sprintf(buff, "%10.3f %10.3f %10.3f    %10.3f ret=%d\n\n", mx, my, mz, 
				sqrtf(mx*mx + my*my + mz*mz),
				ret);
		UARTprintf(buff);

		delay_ms(500);
	}
}

void test_time(){
	
	uint32_t past, now;
	
	time_init();
	
	while(1){
		past = time_micros();
		delay_ms(1000);
		now = time_micros();
		
		UARTprintf("Now: %u us    Delta: %u us\n", now, now - past);
	}
	
}

void find_gyro_offset(){	
	int sumX=0, sumY=0, sumZ=0, samples=5000, i, skip=100; 
	int16_t data[3];
	float offsetX, offsetY, offsetZ;
	char buff[128];
	
	i2c3_init();
	mpu9250_init();
	
	for (i = 0; i < samples + skip; i++){
		
		mpu9250_read_gyro(data);
		UARTprintf("%5d %5d %5d\n", data[0], data[1], data[2]);
		
		if (i < skip){
			continue;
		}
		
		sumX += data[0];
		sumY += data[1];
		sumZ += data[2];
		
		delay_ms(10);
	}
	
	offsetX = (float)sumX / samples;
	offsetY = (float)sumY / samples;
	offsetZ = (float)sumZ / samples;
	
	sprintf(buff, "%f %f %f\n", offsetX, offsetY, offsetZ);
	UARTprintf(buff);
}

void collect_mag_data(){
	
	int16_t m[3];
	int8_t ret;
	uint32_t t;
	
	i2c3_init();
	time_init();
	mpu9250_init();
	
	
	while(1){
		t = time_micros();
		ret = mpu9250_read_mag(m);
		
		UARTprintf("%10u: [%3d %3d %3d] ret=%d\n", t, m[0], m[1], m[2], ret);
		delay_ms(4);
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
//    find_gyro_offset();
//    test_time();
//    collect_mag_data();

	return 0;
}
