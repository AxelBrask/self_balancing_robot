#include "mpu6050.h"

#define REG_PWR_MGMT_1 0x6B
#define REG_WHO_AM_I 0x75
#define REG_ACCEL_XOUT_H 0x3B

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

void MPU_Read(void){

}


