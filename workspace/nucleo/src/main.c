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


	gdUsrCmdIn.cmdIndx = 0;
	gdUsrCmdIn.cmdUsrPrompt = calloc(1, strlen(USER_PROMPT));
	strncpy(gdUsrCmdIn.cmdUsrPrompt, USER_PROMPT, strlen(USER_PROMPT));
	UART_Interface_Init(&uartIf);  //Interface uart for input commands from user

	HAL_UART_Receive_IT(&uartIf, RxChar_uartIf, sizeof(RxChar_uartIf)-1);
	HAL_UART_Transmit(&uartIf, gdUsrCmdIn.cmdUsrPrompt, strlen(gdUsrCmdIn.cmdUsrPrompt), UART_TIMEOUT);




	for(;;);
}
