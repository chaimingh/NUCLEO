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

