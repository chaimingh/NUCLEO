/*
 * main.h
 *
 *  Created on: Mar 7, 2020
 *      Author: chaiminghuang
 */

#ifndef MAIN_H_
#define MAIN_H_

#include "stm32f1xx.h"
#include "stm32f1xx_nucleo.h"


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "string.h"

#include "app_sysclk.h"
#include "app_uart.h"
#include "app_shell.h"

/* --- Error Handler --- */
#define Error_Handler() Error_Handler_caller(__func__)
void Error_Handler_caller(char const * caller_name);

/* --- Interface UART --- */
UART_HandleTypeDef uartIf;

#define UART_TIMEOUT 0x0400        //Time out tick value in mili-second
#define MAX_CHAR_CMD 256
#define CMD_PROMPT "\n\r-$"
#define CMD_PREFIX "-$"

typedef struct
{
	uint8_t buff[MAX_CHAR_CMD];
	uint8_t buffIndex;
	uint8_t *cmdPrompt;

} userCmdIn_t;

uint8_t rxChar[2];          //one char + null termination
userCmdIn_t gdUsrCmdIn;

#define DEBUG_UTILS_ENABLE (1) /**< set to 1 to enable, anything else will
disable */
#define DEBUG_UTILS_BUFF_LEN (256) /**< max internal buffer size used to hold string, change as required */
void debug_printf(UART_HandleTypeDef *hUart, char *fmt, ...);

#endif /* MAIN_H_ */
