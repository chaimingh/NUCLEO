/*
 * app_sysclk.c
 *
 *  Created on: Mar 7, 2020
 *      Author: chaiminghuang
 */


#include <main.h>        // To use Error_Handler()
#include "stm32f1xx.h"
#include "stm32f1xx_nucleo.h"
#include "app_sysclk.h"



void SystemClock_Init(void)
{
	RCC_OscInitTypeDef RCC_OscInitStruct = {0};   //Oscillator type
	RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

	/* Initializes the CPU, AHB and APB buses clocks */

	// Configure oscillator to use HSI 8MHz
	RCC_OscInitStruct.OscillatorType		= RCC_OSCILLATORTYPE_HSI;   //Mulitple clock sources can be configured
	RCC_OscInitStruct.HSIState				= RCC_HSI_ON;
	RCC_OscInitStruct.HSICalibrationValue   = RCC_HSICALIBRATION_DEFAULT;

	// Configure PLL to use HSI and increase it from 8MHz to 64MHz
	RCC_OscInitStruct.PLL.PLLState			= RCC_PLL_ON;               //Turn ON the PLL
	RCC_OscInitStruct.PLL.PLLSource			= RCC_PLLSOURCE_HSI_DIV2;   //RCC_OSCILLATORTYPE_HSI;
	RCC_OscInitStruct.PLL.PLLMUL			= RCC_PLL_MUL16;			//RCC_PLL_MUL16

	if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		Error_Handler();
	}

	// Configure system clock to use PLL's output as a source
	RCC_ClkInitStruct.ClockType			= RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
										| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource		= RCC_SYSCLKSOURCE_PLLCLK;   //Set SYSCLK to use PLL's output of 64MHz
	RCC_ClkInitStruct.AHBCLKDivider		= RCC_SYSCLK_DIV1;           //64MHz
	RCC_ClkInitStruct.APB1CLKDivider	= RCC_HCLK_DIV2;             //32MHz
	RCC_ClkInitStruct.APB2CLKDivider	= RCC_HCLK_DIV1;             //64MHz

	if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
	{
		Error_Handler();
	}

} //End of SystemClockInit()...
