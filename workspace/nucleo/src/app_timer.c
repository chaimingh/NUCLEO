/*
 * app_timer.c
 *
 *  Created on: Dec 13, 2020
 *      Author: chaiminghuang
 */

#include "stm32f1xx.h"
#include "stm32f1xx_nucleo.h"
#include "main.h"


/*
 * --------------------------------------------------------------------------------------------
 * @description		Timer init for periodic timer
 * @param[in]		Pointers timer handle
 * @return			None
 */
void TIM2_Init(TIM_HandleTypeDef *PeriodicTim, uint32_t prescaleTim, uint32_t periodTim)
{
	/* Periodic Timer*/
	TIM_ClockConfigTypeDef clockConfig = {0};

	PeriodicTim->Instance               = TIM2;
	PeriodicTim->Init.Prescaler         = (prescaleTim-1);                             //(6400-1) -> clock is 64Mhz/6400 =
	PeriodicTim->Init.CounterMode       = TIM_COUNTERMODE_UP;
	PeriodicTim->Init.Period            = (round(periodTim/2)-1);                       //(10000-1);

	if(HAL_TIM_Base_Init(PeriodicTim) != HAL_OK)
	{
		Error_Handler();
	}

	clockConfig.ClockSource   = TIM_CLOCKSOURCE_INTERNAL;
	if(HAL_TIM_ConfigClockSource(PeriodicTim, &clockConfig) != HAL_OK)
	{
		Error_Handler();
	}
}


/* --------------------------------------------------------------------------------------------
 * @description		Configure settings for TIM2 Periodic Timer
 * @param[in]		Timer handle
 * @return			None
 */
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM2)
	{
		__HAL_RCC_TIM2_CLK_ENABLE();

		HAL_NVIC_SetPriority(TIM2_IRQn, 0x0, 0x0);
		HAL_NVIC_EnableIRQ(TIM2_IRQn);       //Enable interrupt vector TIM2_IRQHandler()
	}

	// Enable clock to GPIO port A, B, C
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();

	GPIO_InitTypeDef GPIO_InitStruct = {0};

//	GPIO_InitStruct.Pin 	= D15;
//	GPIO_InitStruct.Mode 	= GPIO_MODE_OUTPUT_PP;
//	GPIO_InitStruct.Pull 	= GPIO_NOPULL;
//	GPIO_InitStruct.Speed 	= GPIO_SPEED_FREQ_HIGH;
//	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);




	GPIO_InitStruct.Pin 	= D15 | D14 | D10;
	GPIO_InitStruct.Mode 	= GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull 	= GPIO_NOPULL;
	GPIO_InitStruct.Speed 	= GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	GPIO_InitStruct.Pin 	= D13 | D12 | D11 | D08 | D07;
	GPIO_InitStruct.Mode 	= GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull 	= GPIO_NOPULL;
	GPIO_InitStruct.Speed 	= GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	GPIO_InitStruct.Pin 	= D09;
	GPIO_InitStruct.Mode 	= GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull 	= GPIO_NOPULL;
	GPIO_InitStruct.Speed 	= GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);





} //End of HAL_TIM_Encoder_MspInit()...


uint8_t outputLatch;
/* --------------------------------------------------------------------------------------------
 * @description		Callback for handler for TIM2
 * @param[in]		Tim2 handle
 * @return			None
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{

	uint8_t onOff = (outputLatch % 2) & gbTtlOutEnable;
	outputLatch++;


	if(htim->Instance == TIM2)
	{
		HAL_GPIO_WritePin(GPIOB, D15, onOff);
		HAL_GPIO_WritePin(GPIOB, D14, onOff);

		HAL_GPIO_WritePin(GPIOA, D13, onOff);
		HAL_GPIO_WritePin(GPIOA, D12, onOff);
		HAL_GPIO_WritePin(GPIOA, D11, onOff);

		HAL_GPIO_WritePin(GPIOB, D10, onOff);

		HAL_GPIO_WritePin(GPIOC, D09, onOff);

		HAL_GPIO_WritePin(GPIOA, D08, onOff);
		HAL_GPIO_WritePin(GPIOA, D07, onOff);

	}


}//End of HAL_TIM_PeriodElapsedCallback()



