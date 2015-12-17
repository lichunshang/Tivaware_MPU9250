/*
 * tivaware_utils.h
 *
 *  Created on: Nov 30, 2015
 *      Author: Chunshang Li
 */

#ifndef TIVAWARE_UTILS_H_
#define TIVAWARE_UTILS_H_

#include <stdint.h>

/*
 * inaccurate delays
 */
void delay_ms(uint32_t time_ms);
void delay_us(uint32_t time_us);

/*
 * System time since startup in microseconds
 * Uses 64 bit timer 0 (wide timer)
 */
void time_init();
uint32_t time_micros();

#endif /* TIVAWARE_UTILS_H_ */
