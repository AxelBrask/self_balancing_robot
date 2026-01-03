#pragma once
#include <stdint.h>
#include "stm32f3xx_hal.h"

typedef struct {
	I2C_HandleTypeDef *i2c;
	uint16_t addr;
} MPU6050;

typedef struct {
	float acceleration_x, acceleration_y, acceleration_z;
	float angular_rate_x, angular_rate_y, angular_rate_z;
	uint32_t time_ms;
} IMU_Sample;

uint8_t Calibrate(MPU6050 *d, uint16_t number_of_samples, uint16_t delay);
void MPU_Init(MPU6050 *d, I2C_HandleTypeDef *i2c, uint8_t addr7);
uint8_t MPU_Wake_Up(MPU6050 *d,uint8_t *who_out);
uint8_t MPU_Read(MPU6050 *d, IMU_Sample * sample);
