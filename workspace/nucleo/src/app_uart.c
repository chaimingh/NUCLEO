/*
 * app_uart.c
 *
 *  Created on: Mar 7, 2020
 *      Author: chaiminghuang
 */


#include <main.h>
#include "stm32f1xx.h"
#include "stm32f1xx_nucleo.h"
#include "app_uart.h"


/* --------------------------------------------------------------------------------------------
 * @description		UART Init
 * @param[in]		Pointers to interface uart handler
 * @return			None
 */
void UART_Interface_Init(UART_HandleTypeDef* huart)
{
	/* Configure USART2 module */
	__HAL_RCC_USART2_CLK_ENABLE();

	huart->Instance			= USART2;
	huart->Init.BaudRate	= 115200;
	huart->Init.WordLength 	= UART_WORDLENGTH_8B;
	huart->Init.StopBits	= UART_STOPBITS_1;
	huart->Init.Parity		= UART_PARITY_NONE;
	huart->Init.HwFlowCtl	= UART_HWCONTROL_NONE;
	huart->Init.Mode		= UART_MODE_TX_RX;

	if(HAL_UART_Init(huart) != HAL_OK)
	{
		Error_Handler();
	}
	HAL_NVIC_EnableIRQ(USART2_IRQn);


}

/* --------------------------------------------------------------------------------------------
 * @description		Configure settings such as clock and gpio's for UART
 * @param[in]		Pointers to uart handler
 * @return			None
 */
void HAL_UART_MspInit(UART_HandleTypeDef* huart)
{

	/*Interface uart*/
	if(huart->Instance == USART2)
	{
		__HAL_RCC_GPIOA_CLK_ENABLE();
		GPIO_InitTypeDef GPIO_InitStruct = {0};

		// Configure GPIO pin : PA9 for USART_TX for USART1
		GPIO_InitStruct.Pin  	= UART2_TX_PIN;
		GPIO_InitStruct.Mode 	= GPIO_MODE_AF_PP;   //PP mode to read ODR (RM0008 9.1.9)
		GPIO_InitStruct.Speed 	= GPIO_SPEED_FREQ_HIGH;   //Use on-board pull up resistor
		HAL_GPIO_Init(UART1_GPIO_PORT, &GPIO_InitStruct);

		// Configure GPIO pin : PA10 for USART_RX for USART1
		GPIO_InitStruct.Pin  	= UART2_RX_PIN;
		GPIO_InitStruct.Mode 	= GPIO_MODE_INPUT;   //OD mode to read IDR (RM0008 9.1.9)
		GPIO_InitStruct.Pull 	= GPIO_NOPULL;
		HAL_GPIO_Init(UART1_GPIO_PORT, &GPIO_InitStruct);
	}
}

/* --------------------------------------------------------------------------------------------
 * @description		Callback for UART Rx complete
 * @param[in]		UART handler
 * @return			None
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	uint8_t rtnVal;
	uint8_t *pTempBuff;
	uint8_t tempBuffSize;
	uint8_t i;

	// USART2 is interface uart
	if(huart->Instance == USART2)
	{
		HAL_UART_Transmit(&uartIf, rxChar, sizeof(rxChar)-1, UART_TIMEOUT);   //Echo back for each char received.

		//Buffer is not empty and use pressed ENTER to activate command input
		if((rxChar[0] == CR_CHAR) && (gdUsrCmdIn.buffIndex > 0))
		{
#if 0
			debug_printf(&uartIf, "\nuser entered: %s (%d char)", gdUsrCmdIn.buff, gdUsrCmdIn.buffIndex);
			HAL_UART_Transmit(&uartIf, gdUsrCmdIn.cmdPrompt, strlen(gdUsrCmdIn.cmdPrompt), UART_TIMEOUT);
#endif
			gdUsrCmdIn.buffIndex = 0;
			rtnVal = ProcessCommandline((char*)gdUsrCmdIn.buff);
			if(rtnVal)
			{
				HAL_UART_Transmit(&uartIf, gdUsrCmdIn.cmdPrompt, strlen(gdUsrCmdIn.cmdPrompt), UART_TIMEOUT);
			}
			memset(gdUsrCmdIn.buff, 0, strlen(gdUsrCmdIn.buff));
		}
		//Backspace to delete
		else if((rxChar[0] == BACKSPACE_CHAR) && (gdUsrCmdIn.buffIndex > 0))
		{
			gdUsrCmdIn.buffIndex--;  //-1 for last char to be deleted
			gdUsrCmdIn.buff[gdUsrCmdIn.buffIndex] = '\0';
			if(gdUsrCmdIn.buffIndex != 0)
			{

				for(i = 0; i < gdUsrCmdIn.buffIndex+sizeof(CMD_PREFIX); i++)
					HAL_UART_Transmit(&uartIf, tx_backspace, 1, UART_TIMEOUT);

				tempBuffSize = sizeof(CMD_PREFIX) + gdUsrCmdIn.buffIndex+2;
				pTempBuff = calloc(1, tempBuffSize);

				strcpy(pTempBuff, CMD_PREFIX);
				strncat(pTempBuff, gdUsrCmdIn.buff, gdUsrCmdIn.buffIndex);
				strncat(pTempBuff, tx_endOfText, 1);    //End of text: this ascii replaces deleted char
				strncat(pTempBuff, tx_leftArrowKey, 1); //Left arrow: send cursor 1 to left.

				HAL_UART_Transmit(&uartIf, pTempBuff, tempBuffSize, UART_TIMEOUT);

				free(pTempBuff);

			}
			else
			{
				pTempBuff = calloc(1, 2);

				strncat(pTempBuff, tx_endOfText, 1);    //End of text
				strncat(pTempBuff, tx_leftArrowKey, 1); //Left arrow
				HAL_UART_Transmit(&uartIf, pTempBuff, 2, UART_TIMEOUT);

				free(pTempBuff);
				//HAL_UART_Transmit(&uartIf, gdUsrCmdIn.cmdPrompt, sizeof(gdUsrCmdIn.cmdPrompt), UART_TIMEOUT);
			}

		}
		//Each char input is being buffered
		else if(rxChar[0] != CR_CHAR && rxChar[0] != BACKSPACE_CHAR)
		{
			gdUsrCmdIn.buff[gdUsrCmdIn.buffIndex++] = rxChar[0];
		}
		//ENTER key is pressed
		else
		{
			HAL_UART_Transmit(&uartIf, gdUsrCmdIn.cmdPrompt, sizeof(gdUsrCmdIn.cmdPrompt), UART_TIMEOUT);
		}




		//if(gdUsrCmdIn.buffIndex == 5)
		//	debug_printf(&uartIf, "\nUser command: %s", gdUsrCmdIn.buff);
#if 0
		/*If EnterKey is press, end of user command*/
		if(rxChar[0] == CR_CHAR)
		{
			gdUsrCmdIn.buffIndex = 0;
			HAL_UART_Transmit(&uartIf, gdUsrCmdIn.cmdPrompt, strlen(gdUsrCmdIn.cmdPrompt), UART_TIMEOUT);

			/* User keeps hitting an enter key on shell, ignore*/
			if(gdUsrCmdIn.buff[0] != CR_CHAR)
			{
				ProcessCommandline((char*)gdUsrCmdIn.buff);
			}
			memset(gdUsrCmdIn.buff, 0, strlen(gdUsrCmdIn.buff));
		}
#endif


		//Re-arm the Rx interrupt
		HAL_UART_Receive_IT(&uartIf, rxChar, sizeof(rxChar)-1);   //-1 b/c rxChar is 2 byte with CR.
		return;
	}
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == USART2)
	{
		HAL_UART_Receive_IT(&uartIf, rxChar, sizeof(rxChar)-1);    //Re-arm the Rx interrupt
	}
}

void USART2_IRQHandler(void)
{
	HAL_UART_IRQHandler(&uartIf);
}
