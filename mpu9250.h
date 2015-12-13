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
#include "tivaware_utils.h"

#define MPU9250_ADDRESS MPU9250_ADDRESS0 //ADO=0
#define GYRO_SCALE 	GFS_250DPS
#define ACCEL_SCALE AFS_2G
#define MAG_SCALE	MFS_16BITS
#define MAG_MODE	0x06 //100Hz
#define MAG_ERR_NOT_READY 	-1
#define MAG_ERR_OVERFLOW	-2
#define MAG_DATA_SKIPPED 	1


/* Functions for abstraction */
void mpu9250_write_byte(uint8_t addr, uint8_t reg_addr, uint8_t data);
void mpu9250_read_byte(uint8_t addr, uint8_t reg_addr, uint8_t *ret);
void mpu9250_read_bytes(uint8_t addr, uint8_t reg_addr, uint8_t *buffer, uint8_t length);
void delay(uint32_t time_ms);


/* Device functions*/
void mpu9250_init();
void mpu9250_calibrate();
void mpu9250_self_test();

void mpu9250_read_accel_temp_gyro(int16_t *accel, int16_t *temp, int16_t *gyro); //length = 7
void mpu9250_read_accel(int16_t *data); //length = 3
void mpu9250_read_temp(int16_t *data); //length = 1
void mpu9250_read_gyro(int16_t *data); //length = 3

int8_t mpu9250_read_mag(int16_t *data); //length = 3

#endif /* MPU9250_H_ */
