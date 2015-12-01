/*
 * MPU9250.h
 *
 *  Created on: Nov 29, 2015
 *      Author: cli
 */

#ifndef MPU9250_H_
#define MPU9250_H_

#include <stdint.h>
#include "mpu9250_const.h"

#define MPU9250_ADDRESS MPU9250_ADDRESS0 //ADO=0

void mpu9250_write_byte(uint8_t addr, uint8_t reg_addr, uint8_t data);
void mpu9250_read_byte(uint8_t addr, uint8_t reg_addr, uint8_t *ret);
void mpu9250_read_bytes(uint8_t addr, uint8_t reg_addr, uint8_t *buffer, uint8_t length);

void mpu9250_init();
void mpu9250_calibrate();
void mpu9250_self_test();

void mpu9250_read_accel_temp_gyro();
void mpu9250_read_accel();
void mpu9250_read_temp();
void mpu9250_read_gyro();

void mpu9250_read_mag();

#endif /* MPU9250_H_ */
