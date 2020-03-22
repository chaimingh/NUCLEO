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
#define USER_PROMPT "\n\r->"

typedef struct
{
	uint8_t cmd[MAX_CHAR_CMD];
	uint8_t cmdIndx;
	uint8_t *cmdUsrPrompt;

} uartIf_cmdIn_t;

uint8_t RxChar_uartIf[2];          //one char + null termination
uartIf_cmdIn_t gdUsrCmdIn;



#endif /* MAIN_H_ */
