#include "app.h"
#include "mpu6050.h"
#include "debug_huart.h"
#include "board.h"

static MPU6050 mpu;

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

}


void APP_Loop(void){

}
