/*
 * tivaware_utils.c
 *
 *  Created on: Nov 30, 2015
 *      Author: Chunshang Li
 */

#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "tivaware_utils.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"

/*
 * Delay
 */
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


/*
 * Time
 */
void time_init(){
	SysCtlPeripheralEnable(SYSCTL_PERIPH_WTIMER0);
	
	TimerConfigure(WTIMER0_BASE, TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_PERIODIC | TIMER_CFG_A_ACT_NONE);
	TimerDisable(WTIMER0_BASE, TIMER_A);
	
	TimerLoadSet(WTIMER0_BASE, TIMER_A, 0xFFFFFFFF);
	
	//set scaling of clock speed / 1 million, so increments one every microsecond
	TimerPrescaleSet(WTIMER0_BASE, TIMER_A, SysCtlClockGet() / 1000000);
	
	TimerEnable(WTIMER0_BASE, TIMER_A);
}

/*
 * The timer counts down
 */
uint32_t time_micros(){
	return 0xFFFFFFFF - TimerValueGet(WTIMER0_BASE, TIMER_A);
}

