/*
 * main_misc.c
 *
 *  Created on: Mar 8, 2020
 *      Author: chaiminghuang
 */

#include <main.h>




/* --------------------------------------------------------------------------------------------
 * @description		Catch function that encountered error.
 * @param[in]		pointer function of caller function
 * @return			None
 */
void Error_Handler_caller(char const * caller_name)
{
	while(1);
}

void debug_printf(UART_HandleTypeDef *hUart, char *fmt, ...)
{
  #if DEBUG_UTILS_ENABLE == 1
  if(hUart->gState == HAL_UART_STATE_READY)
  {
  char buffer[DEBUG_UTILS_BUFF_LEN];
  //use varaible argument helper functions to process the format string
  va_list va;
  va_start(va, fmt);
  vsprintf(buffer, fmt, va);
  va_end(va);
  HAL_UART_Transmit(hUart, (uint8_t*)buffer,strlen(buffer), 0xFFFF );
  }
  #endif
}
