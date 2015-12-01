/*
 * MPU9250.c
 * Based off
 *
 *  Created on: Nov 29, 2015
 *      Author: cli
 */

#include "mpu9250.h"
#include "tivaware_i2c.h"
#include "tivaware_utils.h"

void mpu9250_write_byte(uint8_t addr, uint8_t reg_addr, uint8_t data){
	i2c3_write_byte(addr, reg_addr, data);
}

void mpu9250_read_bytes(uint8_t addr, uint8_t reg_addr, uint8_t *buffer, uint8_t length){
	i2c3_read_bytes(addr, reg_addr, buffer, length);
}

void mpu9250_read_byte(uint8_t addr, uint8_t reg_addr, uint8_t *ret){
	i2c3_read_byte(addr, reg_addr, ret);
}

void mpu9250_init(){

}

void mpu9250_calibrate(){

}

void mpu9250_self_test(){

}

void mpu9250_read_accel_temp_gyro(){

}

void mpu9250_read_accel(){

}

void mpu9250_read_temp(){

}

void mpu9250_read_gyro(){

}

void mpu9250_read_mag(){

}
