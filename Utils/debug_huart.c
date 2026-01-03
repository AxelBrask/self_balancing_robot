#include "debug_huart.h"

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

static USART_HandleTypeDef *s_husart = NULL;


#define DBG_PRINTF_BUF_SIZE 128

void DBG_Init(USART_HandleTypeDef *hu)
{
  s_husart = hu;
}

void DBG_Print(const char *s)
{
  if (s_husart == NULL || s == NULL) return;

  // strlen requires a null-terminated string
  uint16_t len = (uint16_t)strlen(s);


  (void)HAL_USART_Transmit(s_husart, (uint8_t*)s, len, 100);
}

void DBG_Printf(const char *fmt, ...)
{
  if (s_husart == NULL || fmt == NULL) return;

  char buf[DBG_PRINTF_BUF_SIZE];

  va_list ap;
  va_start(ap, fmt);
  int n = vsnprintf(buf, sizeof(buf), fmt, ap);
  va_end(ap);

  if (n < 0) return;

  // clamp length to buffer size
  uint16_t len = (n < (int)sizeof(buf)) ? (uint16_t)n : (uint16_t)(sizeof(buf) - 1);

  (void)HAL_USART_Transmit(s_husart, (uint8_t*)buf, len, 100);
}
