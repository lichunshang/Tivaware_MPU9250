/*
 * MPU9250.c
 * Based off kriswiner/MPU-9250
 *
 *  Created on: Nov 29, 2015
 *      Author: cli
 */

#include "mpu9250.h"
#include "tivaware_i2c.h"


void mpu9250_write_byte(uint8_t addr, uint8_t reg_addr, uint8_t data){
	i2c3_write_byte(addr, reg_addr, data);
}

void mpu9250_read_bytes(uint8_t addr, uint8_t reg_addr, uint8_t *buffer, uint8_t length){
	i2c3_read_bytes(addr, reg_addr, buffer, length);
}

void mpu9250_read_byte(uint8_t addr, uint8_t reg_addr, uint8_t *ret){
	i2c3_read_byte(addr, reg_addr, ret);
}

void mpu9250_delay(uint32_t time_ms){
	delay_ms(time_ms);
}

void mpu9250_init(){
	// wake up device
	mpu9250_write_byte(MPU9250_ADDRESS, PWR_MGMT_1, 0x00); // Clear sleep mode bit (6), enable all sensors
	mpu9250_delay(100); // Wait for all registers to reset

	// get stable time source
	mpu9250_write_byte(MPU9250_ADDRESS, PWR_MGMT_1, 0x01);  // Auto select clock source to be PLL gyroscope reference if ready else
	mpu9250_delay(200);

	// Configure Gyro and Thermometer
	// Disable FSYNC and set thermometer and gyro bandwidth to 41 and 42 Hz, respectively;
	// minimum delay time for this setting is 5.9 ms, which means sensor fusion update rates cannot
	// be higher than 1 / 0.0059 = 170 Hz
	// DLPF_CFG = bits 2:0 = 011; this limits the sample rate to 1000 Hz for both
	// With the MPU9250, it is possible to get gyro sample rates of 32 kHz (!), 8 kHz, or 1 kHz
	mpu9250_write_byte(MPU9250_ADDRESS, CONFIG, 0x03);

	// Set sample rate = gyroscope output rate/(1 + SMPLRT_DIV)
	mpu9250_write_byte(MPU9250_ADDRESS, SMPLRT_DIV, 0x04);  // Use a 200 Hz rate; a rate consistent with the filter update rate
									// determined inset in CONFIG above

	// Set gyroscope full scale range
	// Range selects FS_SEL and AFS_SEL are 0 - 3, so 2-bit values are left-shifted into positions 4:3
	uint8_t c;
	mpu9250_read_byte(MPU9250_ADDRESS, GYRO_CONFIG, &c);
	//  writeRegister(GYRO_CONFIG, c & ~0xE0); // Clear self-test bits [7:5]
	mpu9250_write_byte(MPU9250_ADDRESS, GYRO_CONFIG, c & ~0x02); // Clear Fchoice bits [1:0]
	mpu9250_write_byte(MPU9250_ADDRESS, GYRO_CONFIG, c & ~0x18); // Clear AFS bits [4:3]
	mpu9250_write_byte(MPU9250_ADDRESS, GYRO_CONFIG, c | GYRO_SCALE << 3); // Set full scale range for the gyro
	// writeRegister(GYRO_CONFIG, c | 0x00); // Set Fchoice for the gyro to 11 by writing its inverse to bits 1:0 of GYRO_CONFIG

	// Set accelerometer full-scale range configuration
	mpu9250_read_byte(MPU9250_ADDRESS, ACCEL_CONFIG, &c);
	//  writeRegister(ACCEL_CONFIG, c & ~0xE0); // Clear self-test bits [7:5]
	mpu9250_write_byte(MPU9250_ADDRESS, ACCEL_CONFIG, c & ~0x18); // Clear AFS bits [4:3]
	mpu9250_write_byte(MPU9250_ADDRESS, ACCEL_CONFIG, c | ACCEL_SCALE << 3); // Set full scale range for the accelerometer

	// Set accelerometer sample rate configuration
	// It is possible to get a 4 kHz sample rate from the accelerometer by choosing 1 for
	// accel_fchoice_b bit [3]; in this case the bandwidth is 1.13 kHz
	mpu9250_read_byte(MPU9250_ADDRESS, ACCEL_CONFIG2, &c);
	mpu9250_write_byte(MPU9250_ADDRESS, ACCEL_CONFIG2, c & ~0x0F); // Clear accel_fchoice_b (bit 3) and A_DLPFG (bits [2:0])
	mpu9250_write_byte(MPU9250_ADDRESS, ACCEL_CONFIG2, c | 0x03); // Set accelerometer rate to 1 kHz and bandwidth to 41 Hz

	// The accelerometer, gyro, and thermometer are set to 1 kHz sample rates,
	// but all these rates are further reduced by a factor of 5 to 200 Hz because of the SMPLRT_DIV setting

	// Configure Interrupts and Bypass Enable
	// Set interrupt pin active high, push-pull, hold interrupt pin level HIGH until interrupt cleared,
	// clear on read of INT_STATUS, and enable I2C_BYPASS_EN so additional chips
	// can join the I2C bus and all can be controlled by the Arduino as master
	mpu9250_write_byte(MPU9250_ADDRESS, INT_PIN_CFG, 0x22);
	mpu9250_write_byte(MPU9250_ADDRESS, INT_ENABLE, 0x01);  // Enable data ready (bit 0) interrupt
	mpu9250_delay(100);

	//*********** Magnetometer Init *******************
	// First extract the factory calibration for each magnetometer axis
//	uint8_t rawData[3];  // x/y/z gyro calibration data stored here
	mpu9250_write_byte(AK8963_ADDRESS, AK8963_CNTL, 0x00); // Power down magnetometer
	mpu9250_delay(10);
	mpu9250_write_byte(AK8963_ADDRESS, AK8963_CNTL, 0x0F); // Enter Fuse ROM access mode
	mpu9250_delay(10);
//	mpu9250_read_bytes(AK8963_ADDRESS, AK8963_ASAX, 3, &rawData[0]);  // Read the x-, y-, and z-axis calibration values
//	destination[0] =  (float)(rawData[0] - 128)/256. + 1.;   // Return x-axis sensitivity adjustment values, etc.
//	destination[1] =  (float)(rawData[1] - 128)/256. + 1.;
//	destination[2] =  (float)(rawData[2] - 128)/256. + 1.;
	mpu9250_write_byte(AK8963_ADDRESS, AK8963_CNTL, 0x00); // Power down magnetometer
	mpu9250_delay(10);
	// Configure the magnetometer for continuous read and highest resolution
	// set Mscale bit 4 to 1 (0) to enable 16 (14) bit resolution in CNTL register,
	// and enable continuous mode data acquisition Mmode (bits [3:0]), 0010 for 8 Hz and 0110 for 100 Hz sample rates
	mpu9250_write_byte(AK8963_ADDRESS, AK8963_CNTL, MAG_SCALE << 4 | MAG_MODE); // Set magnetometer data resolution and sample ODR
	mpu9250_delay(10);

}

void mpu9250_self_test(){
	//TODO
}

/*
 * Length = 7
 * 
 */
void mpu9250_read_accel_temp_gyro(int16_t *accel, int16_t *temp, int16_t *gyro){
	
	uint8_t buff[14];
	
	mpu9250_read_bytes(MPU9250_ADDRESS, ACCEL_XOUT_H, buff, 14);
	
	accel[0] = ((int16_t)buff[0] << 8) | (int16_t)buff[1];
	accel[1] = ((int16_t)buff[2] << 8) | (int16_t)buff[3];
	accel[2] = ((int16_t)buff[4] << 8) | (int16_t)buff[5];
	
	*temp = ((int16_t)buff[6] << 8) | (int16_t)buff[7];

	gyro[0] = ((int16_t)buff[8] << 8) | (int16_t)buff[9];
	gyro[1] = ((int16_t)buff[10] << 8) | (int16_t)buff[11];
	gyro[2] = ((int16_t)buff[12] << 8) | (int16_t)buff[13];
}

/*
 * Length = 3
 */
void mpu9250_read_accel(int16_t *data){
	uint8_t buff[6];
	
	mpu9250_read_bytes(MPU9250_ADDRESS, ACCEL_XOUT_H, buff, 6);
	
	data[0] = ((int16_t)buff[0] << 8) | (int16_t)buff[1];
	data[1] = ((int16_t)buff[2] << 8) | (int16_t)buff[3];
	data[2] = ((int16_t)buff[4] << 8) | (int16_t)buff[5];
}

/*
 * Length = 1
 */
void mpu9250_read_temp(int16_t *data){
	uint8_t buff[2];
	
	mpu9250_read_bytes(MPU9250_ADDRESS, TEMP_OUT_H, buff, 2);
	
	*data = ((int16_t)buff[0] << 8) | (int16_t)buff[1];
}

/*
 * Length = 3
 */
void mpu9250_read_gyro(int16_t *data){
	uint8_t buff[6];
	
	mpu9250_read_bytes(MPU9250_ADDRESS, GYRO_XOUT_H, buff, 6);
	
	data[0] = ((int16_t)buff[0] << 8) | (int16_t)buff[1];
	data[1] = ((int16_t)buff[2] << 8) | (int16_t)buff[3];
	data[2] = ((int16_t)buff[4] << 8) | (int16_t)buff[5];
}

/*
 * Length = 3
 */
int8_t mpu9250_read_mag(int16_t *data){
	uint8_t buff[7];
	int8_t ret=0;
	
	mpu9250_read_byte(AK8963_ADDRESS, AK8963_ST1, buff);
	
	//check if data is ready
	if (buff[0] & 0x01){
		
		//check if previous data is skipped
		if (buff[0] & 0x02){
			ret = MAG_DATA_SKIPPED;
		}
		
		mpu9250_read_bytes(AK8963_ADDRESS, AK8963_XOUT_L, buff, 7);
		
		//check sensor overflow, in this case, we want to return the results anyways, might wrong though
		if (buff[6]  & 0x08){
			ret = MAG_DATA_SKIPPED;
		}
		
		data[0] = ((int16_t) buff[1] << 8) | (int16_t)buff[0];
		data[1] = ((int16_t) buff[3] << 8) | (int16_t)buff[2];
		data[2] = ((int16_t) buff[5] << 8) | (int16_t)buff[4];
		
		return ret;
	}
	
	return MAG_ERR_NOT_READY;
}

float mpu9250_accel_scale(){
	switch (ACCEL_SCALE)
	{
	// Possible accelerometer scales (and their register bit settings) are:
	// 2 Gs (00), 4 Gs (01), 8 Gs (10), and 16 Gs  (11). 
	// Here's a bit of an algorith to calculate DPS/(ADC tick) based on that 2-bit value:
	case AFS_2G:
		return 2.0/32768.0;
	case AFS_4G:
		return 4.0/32768.0;
	case AFS_8G:
		return 8.0/32768.0;
	case AFS_16G:
		return 16.0/32768.0;
	}
	
	return 0;
}

float mpu9250_gyro_scale(){
	switch (GYRO_SCALE)
	{
	// Possible gyro scales (and their register bit settings) are:
	// 250 DPS (00), 500 DPS (01), 1000 DPS (10), and 2000 DPS  (11). 
	// Here's a bit of an algorith to calculate DPS/(ADC tick) based on that 2-bit value:
	case GFS_250DPS:
		return 250.0/32768.0;
	case GFS_500DPS:
		return 500.0/32768.0;
	case GFS_1000DPS:
		return 1000.0/32768.0;
	case GFS_2000DPS:
		return 2000.0/32768.0;
	}
	
	return 0;
}

float mpu9250_mag_scale(){
	switch (MAG_SCALE)
	{
	// Possible magnetometer scales (and their register bit settings) are:
	// 14 bit resolution (0) and 16 bit resolution (1)
	case MFS_14BITS:
		return 10.*4912./8190.; // Proper scale to return milliGauss
	case MFS_16BITS:
		return 10.*4912./32760.0; // Proper scale to return milliGauss
	}
	
	return 0;
}

void mpu9250_gyro_calibrate(int16_t *data){
	const float OFFSET_X = 28; /*27.556999*/
	const float OFFSET_Y = 47; /*46.885799*/
	const float OFFSET_Z = -6; /*-6.369200*/
	
	data[0] -= OFFSET_X;
	data[1] -= OFFSET_Y;
	data[2] -= OFFSET_Z;
}

void mpu9250_accel_calibrate(int16_t *data){
	
}

void mpu9250_mag_calibrate(int16_t *data){
	
}
