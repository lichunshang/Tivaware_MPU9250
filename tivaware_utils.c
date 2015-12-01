/*
 * tivaware_utils.c
 *
 *  Created on: Nov 30, 2015
 *      Author: Chunshang Li
 */

#include <stdbool.h>
#include "tivaware_utils.h"
#include "driverlib/sysctl.h"

void delay_ms(uint32_t time_ms){
	// 1 clock cycle = 1 / SysCtlClockGet() second
	// 1 SysCtlDelay = 3 clock cycle = 3 / SysCtlClockGet() second
	// 1 second = SysCtlClockGet() / 3
	// 0.001 second = 1 ms = SysCtlClockGet() / 3 / 1000

	SysCtlDelay(time_ms * (SysCtlClockGet() / 3 / 1000));
}

void delay_us(uint32_t time_us){
	SysCtlDelay(time_us * (SysCtlClockGet() / 3 / 1000000));
}
