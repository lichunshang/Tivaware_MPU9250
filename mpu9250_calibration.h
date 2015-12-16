/*
 * mpu9250_calibration.h
 *
 *  Created on: Dec 14, 2015
 *      Author: Chunshang Li
 */

#ifndef MPU9250_CALIBRATION_H_
#define MPU9250_CALIBRATION_H_


/*
 * Simple offset when sitting right side up
 */
void mpu9250_gyro_offset(int16_t *offset_x, int16_t *offset_y, int16_t *offset_z);
//void mpu9250_accel_offset();

/*
 * print out data for processing on a computer
 */
void mpu9250_accel_calibrate_data();
void mpu9250_mag_calibrate_data();

#endif /* MPU9250_CALIBRATION_H_ */
