#include "mpu6050.h"

#define REG_PWR_MGMT_1 0x6B
#define REG_WHO_AM_I 0x75
#define REG_ACCEL_XOUT_H 0x3B
#define G 9.80665f

static const float accel_LSB_sensitivity = 16384; // 16384 LSB/g
static const float gyro_LSB_sensitivity = 131;// LSB/°/s
static const float deg2rad = 0.017453293f;

static float acc_bias_x = 0.0f;
static float acc_bias_y = 0.0f;
static float acc_bias_z = 0.0f;

static float gyro_bias_x_rads = 0.0f;
static float gyro_bias_y_rads = 0.0f;
static float gyro_bias_z_rads = 0.0f;



static HAL_StatusTypeDef write_u8(MPU6050 *d, uint8_t reg, uint8_t v)
{
  return HAL_I2C_Mem_Write(d->i2c, d->addr, reg, I2C_MEMADD_SIZE_8BIT, &v, 1, 100);
}

static HAL_StatusTypeDef read_u8(MPU6050 *d, uint8_t reg, uint8_t *out)
{
  return HAL_I2C_Mem_Read(d->i2c, d->addr, reg, I2C_MEMADD_SIZE_8BIT, out, 1, 100);
}

void MPU_Init(MPU6050 *d, I2C_HandleTypeDef *i2c, uint8_t addr7){
	d -> i2c = i2c;
	d -> addr = (uint16_t)(addr7 << 1);
}


uint8_t Calibrate(MPU6050 *d, uint16_t number_of_samples, uint16_t delay){
	int64_t sum_acc_x, sum_acc_y, sum_acc_z;
	int64_t sum_gyro_x, sum_gyro_y, sum_gyro_z;

	// read number of samples many IMU readings and sum them
	for(uint16_t i =0; i < number_of_samples; i++){
		uint8_t b[14];

		// read the 14 bytes from the MPU6050 into the 14-bytes array
		if (HAL_I2C_Mem_Read(d->i2c, d->addr, REG_ACCEL_XOUT_H,
				I2C_MEMADD_SIZE_8BIT, b, 14, 100) != HAL_OK){
			return 0;
		}

		int16_t ax = (int16_t)((b[0] << 8) | b[1]);
		int16_t ay = (int16_t)((b[2] << 8) | b[3]);
		int16_t az = (int16_t)((b[4] << 8) | b[5]);
		sum_acc_x += ax;
		sum_acc_y += ay;
		sum_acc_z += az;

		int16_t gx = (int16_t)((b[8] << 8) | b[9]);
		int16_t gy = (int16_t)((b[10] << 8) | b[11]);
		int16_t gz = (int16_t)((b[12] << 8) | b[13]);
		sum_gyro_x += gx;
		sum_gyro_y += gy;
		sum_gyro_z += gz;

	}

	// average the accelerations and gyro readings
	float ax_avg = (float)sum_acc_x/(float)number_of_samples;
	float ay_avg = (float)sum_acc_y/(float)number_of_samples;
	float az_avg = (float)sum_acc_z/(float)number_of_samples;

	float gx_avg = (float)sum_gyro_x/(float)number_of_samples;
	float gy_avg = (float)sum_gyro_y/(float)number_of_samples;
	float gz_avg = (float)sum_gyro_z/(float)number_of_samples;

	// Convert them to SI units and these will be our starting biases
	acc_bias_x = (ax_avg / accel_LSB_sensitivity) * G;
	acc_bias_y = (ay_avg / accel_LSB_sensitivity) * G;
	acc_bias_z = (az_avg / accel_LSB_sensitivity) * G;

	gyro_bias_x_rads = (gx_avg / gyro_LSB_sensitivity) * deg2rad;
	gyro_bias_y_rads = (gy_avg / gyro_LSB_sensitivity) * deg2rad;
	gyro_bias_z_rads = (gz_avg / gyro_LSB_sensitivity) * deg2rad;

	return 1;
}

uint8_t MPU_Wake_Up(MPU6050 *d,uint8_t *who_out){

	  if (HAL_I2C_IsDeviceReady(d->i2c, d->addr, 3, 100) != HAL_OK) {
	    DBG_Printf("MPU not ready, I2C err=0x%lX\r\n", HAL_I2C_GetError(d->i2c));
	    if (who_out) *who_out = 0xFF;
	    return 0;
	  }


	  if (write_u8(d, REG_PWR_MGMT_1, 0x00) != HAL_OK) {
	    DBG_Printf("Write PWR_MGMT_1 failed, err=0x%lX\r\n", HAL_I2C_GetError(d->i2c));
	    if (who_out) *who_out = 0xFE;
	    return 0;
	  }

	  HAL_Delay(10);

	  uint8_t who = 0;
	  if (read_u8(d, REG_WHO_AM_I, &who) != HAL_OK) {
	    DBG_Printf("Read WHO_AM_I failed, err=0x%lX\r\n", HAL_I2C_GetError(d->i2c));
	    if (who_out) *who_out = 0xFD;
	    return 0;
	  }

	  if (who_out) *who_out = who;

	  return ((who & 0x7E) == 0x68);
}

uint8_t MPU_Read(MPU6050 *d, IMU_Sample * sample){
	// allocate 14 bytes for reading MPU6050
	uint8_t b[14];

	// read the 14 bytes from the MPU6050 into the 14-bytes array
	if (HAL_I2C_Mem_Read(d->i2c, d->addr, REG_ACCEL_XOUT_H,
			I2C_MEMADD_SIZE_8BIT, b, 14, 100) != HAL_OK){
		return 0;
	}
	// moves 8 bits of b[0] into the upper 8 bits of a 16 bit value and b[1] to the lower 8
	int16_t ax = (int16_t)((b[0] << 8) | b[1]);
	int16_t ay = (int16_t)((b[2] << 8) | b[3]);
	int16_t az = (int16_t)((b[4] << 8) | b[5]);
	int16_t temp = (int16_t)((b[6] << 8) | b[7]);
	int16_t gx = (int16_t)((b[8] << 8) | b[9]);
	int16_t gy = (int16_t)((b[10] << 8) | b[11]);
	int16_t gz = (int16_t)((b[12] << 8) | b[13]);

	// convert the acceleration angular velocities to SI units
	sample -> acceleration_x = (ax / accel_LSB_sensitivity) * G;
	sample -> acceleration_y = (ay / accel_LSB_sensitivity) * G;
	sample -> acceleration_z = (az / accel_LSB_sensitivity) * G;

	sample -> angular_rate_x = (gx/gyro_LSB_sensitivity) * deg2rad;
	sample -> angular_rate_y = (gy/gyro_LSB_sensitivity) * deg2rad;
	sample -> angular_rate_z = (gz/gyro_LSB_sensitivity) * deg2rad;

	return 1;

}


