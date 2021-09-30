/**
  ******************************************************************************
  * @file    main.c
  * @author  chaiminghuang (chais)
  * @version V1.0
  * @date    03/07/2020
  * @brief   Default main function.
  ******************************************************************************
*/

#include <main.h>



int main(void)
{
	HAL_Init();                    //Mandatory function to enable the system configuration
	SystemClock_Init();            //Configure system clock
	UART_Interface_Init(&uartIf);  //Interface uart for input commands from user

	gbTtlOutEnable = 1;
	//gbTtlFreqKhz = 100;


	gbPrescaleTim = 6400;   //64MHz/6400 = 10KHz
	gbPeriodTim = 10000;    //10K counter for 1 second

	RTC_Init(&rtc_h);              //Real-time clock build-in on stm32
	TIM2_Init(&PeriodicTim, gbPrescaleTim, gbPeriodTim);       //Periodic timer with 1second update



	gdUsrCmdIn.buffIndex = 0;
	gdUsrCmdIn.cmdPrompt = calloc(1, strlen(CMD_PROMPT));
	strncpy(gdUsrCmdIn.cmdPrompt, CMD_PROMPT, strlen(CMD_PROMPT));

	HAL_TIM_Base_Start_IT(&PeriodicTim);

	//Arm the UART Rx Interrupt. Interrupt on a single char input
	HAL_UART_Receive_IT(&uartIf, rxChar, sizeof(rxChar)-1);

	ShellPrintf(&uartIf, "\n\r------------------------------------------\n\r");
	ShellPrintf(&uartIf, "-                                        -\n\r");
	ShellPrintf(&uartIf, "- Welcome to Nucleo-F103RB Board         -\n\r");
	ShellPrintf(&uartIf, "-                  by Team2130 WhisIQey  -\n\r");
	ShellPrintf(&uartIf, "-                                        -\n\r");
	ShellPrintf(&uartIf, "------------------------------------------\n\r");

	HAL_UART_Transmit(&uartIf, gdUsrCmdIn.cmdPrompt, strlen(gdUsrCmdIn.cmdPrompt), UART_TIMEOUT);




	for(;;);
}
