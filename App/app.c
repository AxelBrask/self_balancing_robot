#include "app.h"
#include "mpu6050.h"
#include "KalmanFilter.h"
#include "debug_huart.h"
#include "board.h"
#include <stdlib.h>
#include "math_utils.h"

static MPU6050 mpu;
static PitchState kalman_filter;

#include <math.h>


void APP_Init(void){
	DBG_Init(&husart1);
	HAL_Delay(500);

	MPU_Init(&mpu, &hi2c1, 0x68);

	uint8_t who = 0;
	if(!MPU_Wake_Up(&mpu,&who )){
	    DBG_Print("MPU init failed\r\n");
	  } else {
	    DBG_Print("MPU ok\r\n");
	  }

	if(!Calibrate(&mpu, 10, 10)){
		DBG_Print("MPU Calibration Failed");
	} else {
		DBG_Print("MPU Calibration Done");
	}

	if(Init_KalmanFilter(&kalman_filter)){
		DBG_Print("Kalman filter init failed");
	}

}


void APP_Loop(){
    static uint32_t last_time = 0;
    uint32_t current_time = HAL_GetTick();

    // Run control loop every 10ms
    if(current_time - last_time >= 10) {
        float dt = (current_time - last_time) / 1000.0f;
        last_time = current_time;

		IMU_Sample imu_sample;
	   if(!MPU_Read(&mpu, &imu_sample)) {
		   DBG_Print("MPU Read Failed\r\n");
		   return;
	   }

	   float gyro_rad_s = imu_sample.angular_rate_x;
	   float acc_angle = atan2f(imu_sample.acceleration_y,imu_sample.acceleration_z);
	   acc_angle = wrap_to_near(acc_angle, kalman_filter.Theta);

	   Kalman_Predict(&kalman_filter, gyro_rad_s, dt);
	   Kalman_Update(&kalman_filter, acc_angle, dt);

	   static uint32_t last_print = 0;
	   uint32_t now = HAL_GetTick();
	   if (now - last_print >= 500) {
		   last_print = now;
		   int32_t theta_cdeg = (int32_t)(kalman_filter.Theta * RAD2DEG_X100);
	   	   int32_t acc_cdeg   = (int32_t)(acc_angle * RAD2DEG_X100);

	   	  DBG_Printf("acc=%ld.%02ld deg  kf=%ld.%02ld deg\r\n",
	   	             (long)(acc_cdeg/100), (long)labs(acc_cdeg%100),
	   	             (long)(theta_cdeg/100), (long)labs(theta_cdeg%100));



	   }

    }
}


