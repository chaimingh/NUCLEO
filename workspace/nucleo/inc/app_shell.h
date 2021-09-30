/*
 * app_shell.h
 *
 *  Created on: Mar 14, 2020
 *      Author: chaiminghuang
 */

#ifndef APP_SHELL_H_
#define APP_SHELL_H_

#include <main.h>
#include "stm32f1xx_hal.h"
#include <stdarg.h>

#define SHELL_BUFF_LEN 256
#define MAX_ARGS       8


typedef const struct
{
	char *cmd;
	int   minArgs;
	int   maxArgs;
	void  (*func)(int, char**);
	char *description;
	char *syntax;
} SHELL_CMD_T;


uint32_t ProcessCommandline(char Buffer[]);
uint8_t ParseArgv(char *cmdline, char *argv[]);
void RemoveDeleteChar(char Buffer[]);


void ShellHelp (int, char**);
void boardInfo();
void reset();
void ttlOut (int, char**);
void uptime(int, char**);
void ttlOutEnable(int, char**);

void setPrescale(int, char**);
void setPeriod(int, char**);



#endif /* APP_SHELL_H_ */
