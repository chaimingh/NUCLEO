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


/* --------------------------------------------------------------------------------------------
 * @description		Callback for UART Rx complete
 * @param[in]		UART handler
 * @return			None
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	// USART2 is interface uart
	if(huart->Instance == USART2)
	{
		HAL_UART_Transmit(&uartIf, RxChar_uartIf, sizeof(RxChar_uartIf)-1, UART_TIMEOUT);   //-1 b/c RxChar_uartIf is 2 byte with CR.
		gdUsrCmdIn.cmd[gdUsrCmdIn.cmdIndx++] = *RxChar_uartIf;

		/*If EnterKey is press, end of user command*/
		if(*RxChar_uartIf == CR_CHAR)
		{
			gdUsrCmdIn.cmdIndx = 0;
			HAL_UART_Transmit(&uartIf, gdUsrCmdIn.cmdUsrPrompt, strlen(gdUsrCmdIn.cmdUsrPrompt), UART_TIMEOUT);

			/* User keeps hitting an enter key on shell, ignore*/
			if(gdUsrCmdIn.cmd[0] != CR_CHAR)
			{
				ProcessCommandline((char*)gdUsrCmdIn.cmd);
			}
			memset(gdUsrCmdIn.cmd, 0, strlen(gdUsrCmdIn.cmd));
		}

		//Re-arm the Rx interrupt
		HAL_UART_Receive_IT(&uartIf, RxChar_uartIf, sizeof(RxChar_uartIf)-1);   //-1 b/c RxChar_uartIf is 2 byte with CR.
		return;
	}
}



void USART2_IRQHandler(void)
{
	HAL_UART_IRQHandler(&uartIf);
}
