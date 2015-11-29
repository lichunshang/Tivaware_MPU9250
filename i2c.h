/*
 * i2c0.h
 *
 *  Created on: Nov 28, 2015
 *      Author: cli
 */

#include <stdint.h>

#ifndef I2C_H_
#define I2C_H_


void i2c0_init();

void i2c0_read_byte(uint8_t addr, uint8_t reg_addr, uint8_t *ret);
void i2c0_read_bytes(uint8_t addr, uint8_t reg_addr, uint8_t *buffer, uint8_t length);
void i2c0_write_byte(uint8_t addr, uint8_t reg_addr, uint8_t data);

#endif /* I2C_H_ */
