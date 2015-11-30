/*
 * MPU9250.h
 *
 *  Created on: Nov 29, 2015
 *      Author: cli
 */

#ifndef MPU9250_H_
#define MPU9250_H_

#include "mpu9250_const.h"
#include "tivaware_i2c.h"

#define MPU9250_ADDRESS MPU9250_ADDRESS0 //ADO=0


void mpu9250_write_byte();
void mpu9250_read_byte();
void mpu9250_read_bytes();

void mpu9250_init();
void mpu9250_calibrate();
void mpu9250_self_test();

void mpu9250_read_accel_temp_gyro();
void mpu9250_read_accel();
void mpu9250_read_temp();
void mpu9250_read_gyro();

void mpu9250_read_mag();

#endif /* MPU9250_H_ */
