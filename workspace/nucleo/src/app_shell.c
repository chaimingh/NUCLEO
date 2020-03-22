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

};

static const char HELP_FORMAT_SHELL[] = "%14s  %-28s  %-20s\n\r";
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

	ShellPrintf(&uartIf, gdUsrCmdIn.cmdUsrPrompt);
	ShellPrintf(&uartIf, HELP_FORMAT_SHELL, "COMMAND", "DESCRIPTION", "SYNTAX");
	for(indx = 0; indx < NumShellCmd; indx++)
	{
		ShellPrintf(&uartIf, HELP_FORMAT_SHELL,
				ShellCmd[indx].cmd,
				ShellCmd[indx].description,
				ShellCmd[indx].syntax);
	}
	ShellPrintf(&uartIf, gdUsrCmdIn.cmdUsrPrompt);
}


/* --------------------------------------------------------------------------------------------
 * @description		Process command line input from user
 * @param[in]		Pointer to array of char
 * @return			None
 */
void ProcessCommandline(char Buffer[])
{
	uint8_t argc;              //Number of argument from command line
	char *argv[MAX_ARGS + 1];  //Argument vector from command line
	uint8_t commandFound_Flag = 0;

	uint8_t buffLen = strlen(Buffer);
	Buffer[buffLen-1] = 0;     //Get ride of the carriage return char "/r"

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
				}
				//End of if(minArg <= argc <= maxArg)
			}//End of if(cmd == argv[0])
		}//End of for()
	}

	if(commandFound_Flag == 0)
		ShellPrintf(&uartIf, "\n\r\tInput Error: Unknown command.\n\r");

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

