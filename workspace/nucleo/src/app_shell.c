/*
 * app_shell.c
 *
 *  Created on: Mar 7, 2020
 *      Author: chaiminghuang
 */


#include "app_shell.h"

SHELL_CMD_T ShellCmd[] =
{
		{"help", 0, 0, ShellHelp, "Display this help message", "help"},
		{"boardInfo", 0, 0, boardInfo, "Print board info", "boardInfo"},
		{"reset", 0, 0, reset, "Software reset Nucleo board", "reset"},
		{"ttlOut", 1, 1, ttlOut, "TTL Command", "TTL <option>"}

};

static const char HELP_FORMAT_SHELL[] = "\n\r%14s  %-28s  %-20s\n\r";
const uint8_t NumShellCmd = sizeof(ShellCmd)/sizeof(SHELL_CMD_T);

/* --------------------------------------------------------------------------------------------
 * @description		Printf for shell command
 * @param[in]		Pointer to interface uart and format string
 * @return			None
 */
void ShellPrintf(UART_HandleTypeDef *hUart, const char *fmt, ...)
{
	if(hUart->gState == HAL_UART_STATE_READY)
	{
		char buffer[SHELL_BUFF_LEN];

		va_list va;
		va_start(va, fmt);
		vsprintf(buffer, fmt, va);
		va_end(va);

		//HAL_UART_Transmit(hUart, (uint8_t*)gdUsrCmdIn.cmdUsrPrompt, strlen(gdUsrCmdIn.cmdUsrPrompt), 0xFFFF );
		HAL_UART_Transmit(hUart, (uint8_t*)buffer,strlen(buffer), 0xFFFF );
		//HAL_UART_Transmit(hUart, (uint8_t*)gdUsrCmdIn.cmdUsrPrompt, strlen(gdUsrCmdIn.cmdUsrPrompt), 0xFFFF );
	}
}

/* --------------------------------------------------------------------------------------------
 * @description		Print help shell command
 * @param[in]		Number of argument (argc) and argument vector (argv)
 * @return			None
 */
void ShellHelp (int argc, char **argv)
{
	int indx;
	(void)argc;
	(void)argv;

	ShellPrintf(&uartIf, gdUsrCmdIn.cmdPrompt);
	ShellPrintf(&uartIf, HELP_FORMAT_SHELL, "COMMAND", "DESCRIPTION", "SYNTAX");
	for(indx = 0; indx < NumShellCmd; indx++)
	{
		ShellPrintf(&uartIf, HELP_FORMAT_SHELL,
				ShellCmd[indx].cmd,
				ShellCmd[indx].description,
				ShellCmd[indx].syntax);
	}
	ShellPrintf(&uartIf, gdUsrCmdIn.cmdPrompt);
}

/* --------------------------------------------------------------------------------------------
 * @description		TTL
 * @param[in]		Number of argument (argc) and argument vector (argv)
 * @return			None
 */
void ttlOut (int argc, char **argv)
{
	int indx;
	(void)argc;
	(void)argv;

	ShellPrintf(&uartIf, gdUsrCmdIn.cmdPrompt);
	ShellPrintf(&uartIf, HELP_FORMAT_SHELL, "COMMAND", "DESCRIPTION", "SYNTAX");
	for(indx = 0; indx < NumShellCmd; indx++)
	{
		ShellPrintf(&uartIf, HELP_FORMAT_SHELL,
				ShellCmd[indx].cmd,
				ShellCmd[indx].description,
				ShellCmd[indx].syntax);
	}
	ShellPrintf(&uartIf, gdUsrCmdIn.cmdPrompt);
}

void reset()
{
	NVIC_SystemReset();
}

/* --------------------------------------------------------------------------------------------
 * @description		Print board info
 * @param[in]		Number of argument (argc) and argument vector (argv)
 * @return			None
 */
void boardInfo()
{


	ShellPrintf(&uartIf, "\n\r-------------------------------------------\n\r");
	ShellPrintf(&uartIf, "-                                         -\n\r");
	ShellPrintf(&uartIf, "-        STM32F103RB Nucleo Board         -\n\r");
	ShellPrintf(&uartIf, "-                                         -\n\r");
	ShellPrintf(&uartIf, "- www.st.com/stm32nucleo                  -\n\r");
	ShellPrintf(&uartIf, "-                                         -\n\r");
	ShellPrintf(&uartIf, "- Flash        :128 Kbytes                -\n\r");
	ShellPrintf(&uartIf, "- SRAM         :20  Kbytes                -\n\r");
	ShellPrintf(&uartIf, "- Timers                                  -\n\r");
	ShellPrintf(&uartIf, "-     General-purpose  :3                 -\n\r");
	ShellPrintf(&uartIf, "-     Advanced-control :1                 -\n\r");
	ShellPrintf(&uartIf, "- Communication                           -\n\r");
	ShellPrintf(&uartIf, "-     SPI       :2                        -\n\r");
	ShellPrintf(&uartIf, "-     I2C       :2                        -\n\r");
	ShellPrintf(&uartIf, "-     USART     :3                        -\n\r");
	ShellPrintf(&uartIf, "-     USB       :1                        -\n\r");
	ShellPrintf(&uartIf, "-     CAN       :1                        -\n\r");
	ShellPrintf(&uartIf, "- GPIOs         :51                       -\n\r");
	ShellPrintf(&uartIf, "- 12-bit Synch ADC  :2 (16 Channels)      -\n\r");
	ShellPrintf(&uartIf, "- CPU Frequency     :72 MHz               -\n\r");
	ShellPrintf(&uartIf, "- Operating Voltage :2.0 to 3.6V          -\n\r");
	ShellPrintf(&uartIf, "-                                         -\n\r");
	ShellPrintf(&uartIf, "-                                         -\n\r");
	ShellPrintf(&uartIf, "-                   by Team2130 WhisIQey  -\n\r");
	ShellPrintf(&uartIf, "-------------------------------------------\n\r");

	ShellPrintf(&uartIf, gdUsrCmdIn.cmdPrompt);
}

/* --------------------------------------------------------------------------------------------
 * @description		Process command line input from user
 * @param[in]		Pointer to array of char
 * @return			None
 */
uint32_t ProcessCommandline(char Buffer[])
{
	uint8_t latchError = 0;
	uint8_t argc;              //Number of argument from command line
	char *argv[MAX_ARGS + 1];  //Argument vector from command line
	uint8_t commandFound_Flag = 0;

	//uint8_t buffLen = strlen(Buffer);
	//Buffer[buffLen-1] = 0;     //Get ride of the carriage return char "/r"

	RemoveDeleteChar(Buffer);       //Remove delete and backspace char
	argc = ParseArgv(Buffer, argv); //Tokenize command line buffer into argument vector

	/* Search and match the command from user*/
	if(argc)
	{
		uint8_t i;
		for(i = 0; i < NumShellCmd; i++)
		{
			if(strcmp(ShellCmd[i].cmd, argv[0]) == 0)
			{
				if(((argc-1) >= ShellCmd[i].minArgs)  && ((argc-1) <= ShellCmd[i].maxArgs))
				{
					ShellCmd[i].func(argc, argv);
					commandFound_Flag = 1;
				}
				else
				{
					ShellPrintf(&uartIf, "\n\r\tInput Error: Too many arguments.\n\r");
					latchError++;
				}
				//End of if(minArg <= argc <= maxArg)
			}//End of if(cmd == argv[0])
		}//End of for()
	}

	if(commandFound_Flag == 0)
	{
		ShellPrintf(&uartIf, "\n\r\tInput Error: Unknown command.\n\r");
		latchError++;
	}

	return latchError;

}//End of ProcessCommandline().


/* --------------------------------------------------------------------------------------------
 * @description		Remove delete and backspace char from command line input buffer
 * @param[in]		Pointer to array of char
 * @return			None
 */
void RemoveDeleteChar(char Buffer[])
{
	uint8_t pos = 0, k;
	while(*(Buffer+pos) != 0)
	{
		switch(*(Buffer+pos))
		{
		case BACKSPACE_CHAR:  //Backspace
		case DELETE_CHAR:  //Delete
			if(pos == 0)
			{
				k = 0;
				while(*(Buffer+k) != 0)
				{
					*(Buffer+k-1) = *(Buffer+k+1);
					k++;
				}
				*(Buffer+k+1) = 0;
				pos = -1;
			}

			if(pos > 0)
			{
				k = pos;
				while(*(Buffer+k) != 0)
				{
					*(Buffer+k-1) = *(Buffer+k+1);
					k++;
				}
				*(Buffer+k+1) = 0;
				pos = pos - 2;
			}
			break;
		}//End of switch()
		pos++;
	}//End of while(Buff != null)

}//End of RemoveDeleteChar()

/* --------------------------------------------------------------------------------------------
 * @description		Tokenize command line input buffer into argument vector
 * @param[in]		Pointer to array of char
 * @return			Pointer to array char, and number of argument
 */
uint8_t ParseArgv(char *cmdline, char *argv[])
{
	uint8_t argc = 0;
	char *token;
	token = strtok(cmdline, " ");
	while(token != NULL)
	{
		argv[argc++] = token;
		token = strtok(NULL, " ");
	}
	return argc;
}

