#pragma once
#include "stm32f3xx_hal.h"

void DBG_Init(USART_HandleTypeDef *hu);
void DBG_Print(const char *s);
void DBG_Printf(const char *fmt, ...);
