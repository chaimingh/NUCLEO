/*
 * app_rtc.c
 *
 *  Created on: Dec 13, 2020
 *      Author: chaiminghuang
 */

#include "stm32f1xx.h"
#include "stm32f1xx_nucleo.h"
#include "app_rtc.h"
#include "main.h"


char *Weekday[7] = {"Sun", "Mon", "Tue", "Wed", "Thur", "Fri", "Sat"};

/* --------------------------------------------------------------------------------------------
 * @description		RTC Init
 * @param[in]		Pointers to rtc handler
 * @return			None
 */
void RTC_Init(RTC_HandleTypeDef *hrtc)
{
	hrtc->Instance            = RTC;
	hrtc->Init.AsynchPrediv   = RTC_AUTO_1_SECOND;
	hrtc->Init.OutPut         = RTC_OUTPUTSOURCE_NONE;

	if(HAL_RTC_Init(hrtc) != HAL_OK)
	{
		Error_Handler();
	}
}


/* --------------------------------------------------------------------------------------------
 * @description		Configure settings such as clock and gpio's for rtc
 * @param[in]		Pointers to rtc handler
 * @return			None
 */
void HAL_RTC_MspInit(RTC_HandleTypeDef *hrtc)
{
	RCC_OscInitTypeDef          RCC_OscInitStruct;
	RCC_PeriphCLKInitTypeDef    PeriphClkInitStruct;

	RCC_OscInitStruct.OscillatorType   = RCC_OSCILLATORTYPE_LSE | RCC_OSCILLATORTYPE_LSI;
	RCC_OscInitStruct.PLL.PLLState     = RCC_PLL_NONE;
	RCC_OscInitStruct.LSIState         = RCC_LSI_ON;
	RCC_OscInitStruct.LSEState         = RCC_LSE_OFF;

	if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		Error_Handler();
	}

	PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
	PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
	if(HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
	{
		Error_Handler();
	}

	__HAL_RCC_RTC_ENABLE();
}

/* --------------------------------------------------------------------------------------------
 * @description		Get date from rtc
 * @param[in]		Pointers to rtc handler
 * @return			Pointer to date char
 */
void RTC_GetDate(RTC_HandleTypeDef *hrtc, uint8_t *Date)
{
	RTC_DateTypeDef sDate;
	if(HAL_RTC_GetDate(hrtc, &sDate, RTC_FORMAT_BIN) != HAL_OK)
	{
		Error_Handler();
	}
	sprintf((char*)Date,"%s %02d/%02d/20%02d", Weekday[sDate.WeekDay], sDate.Month, sDate.Date, sDate.Year);
}

/* --------------------------------------------------------------------------------------------
 * @description		Get time from rtc
 * @param[in]		Pointers to rtc handler
 * @return			Pointer to time char
 */
void RTC_GetTime(RTC_HandleTypeDef *hrtc, uint8_t *Time)
{
	RTC_TimeTypeDef sTime;
	if(HAL_RTC_GetTime(hrtc, &sTime, RTC_FORMAT_BIN) != HAL_OK)
	{
		Error_Handler();
	}
	sprintf((char*)Time,"%02d:%02d:%02d",sTime.Hours, sTime.Minutes, sTime.Seconds);
}

/* --------------------------------------------------------------------------------------------
 * @description		Set date on rtc
 * @param[in]		Pointers to rtc handler and rtc date struct
 * @return			None
 */
void RTC_SetDate(RTC_HandleTypeDef *hrtc, RTC_DateTypeDef *sDate)
{
	if(HAL_RTC_SetDate(hrtc, sDate, RTC_FORMAT_BCD) != HAL_OK)
	{
		Error_Handler();
	}
}

/* --------------------------------------------------------------------------------------------
 * @description		Set time on rtc
 * @param[in]		Pointers to rtc handler and rtc time struct
 * @return			None
 */
void RTC_SetTime(RTC_HandleTypeDef *hrtc, RTC_TimeTypeDef *sTime)
{
	if(HAL_RTC_SetTime(hrtc, sTime, RTC_FORMAT_BCD) != HAL_OK)
	{
		Error_Handler();
	}
}


/* --------------------------------------------------------------------------------------------
 * @description		Set date and time on rtc
 * @param[in]		Pointers to rtc handler, rtc time struct, and rtc date struct
 * @return			None
 */
void RTC_SetDateAndTime(RTC_HandleTypeDef *hrtc, RTC_TimeTypeDef *sTime, RTC_DateTypeDef *sDate)
{
	if(HAL_RTC_SetDate(hrtc, sDate, RTC_FORMAT_BCD) != HAL_OK)
	{
		Error_Handler();
	}

	if(HAL_RTC_SetTime(hrtc, sTime, RTC_FORMAT_BCD) != HAL_OK)
	{
		Error_Handler();
	}
}


/* --------------------------------------------------------------------------------------------
 * @description		Set alarm on rtc
 * @param[in]		Pointers to rtc handler and rtc alarm struct
 * @return			None
 */
void RTC_SetAlarm(RTC_HandleTypeDef *hrtc, RTC_AlarmTypeDef *sAlarm)
{
	if(HAL_RTC_SetAlarm(hrtc, sAlarm, RTC_FORMAT_BCD) != HAL_OK)
	{
		Error_Handler();
	}
	else
	{
		gbAlarm = ALARM_ON;
//		if(Clock_Display_Enable)
//			Display_AlarmOnOff(&st7735_Dev, gbAlarm);
	}

}

/* --------------------------------------------------------------------------------------------
 * @description		Deactivate alarm on rtc
 * @param[in]		Pointers to rtc handler
 * @return			None
 */
void RTC_DeactivateAlarm(RTC_HandleTypeDef *hrtc)
{
	if(HAL_RTC_DeactivateAlarm(hrtc, RTC_ALARM_A) != HAL_OK)
	{
		Error_Handler();
	}
	else
	{
		gbAlarm = ALARM_OFF;
//		if(Clock_Display_Enable)
//			Display_AlarmOnOff(&st7735_Dev, gbAlarm);
	}
}

/* --------------------------------------------------------------------------------------------
 * @description		Get alarm on rtc
 * @param[in]		Pointers to rtc handler, rtc alarm struct
 * @return			Pointer to time char
 */
void RTC_GetAlarm(RTC_HandleTypeDef *hrtc, RTC_AlarmTypeDef *sAlarm, uint8_t *Time)
{
	if(HAL_RTC_GetAlarm(hrtc, sAlarm, RTC_ALARM_A, RTC_FORMAT_BCD) != HAL_OK)
	{
		Error_Handler();
	}
	sprintf((char*)Time,"%02x:%02x:%02x",sAlarm->AlarmTime.Hours, sAlarm->AlarmTime.Minutes, sAlarm->AlarmTime.Seconds);
}

/* --------------------------------------------------------------------------------------------
 * @description		Set alarm on/off
 * @param[in]		Pointers to rtc handler, rtc alarm struc, and enum on/off
 * @return			None
 */
void RTC_SetAlarmOnOff(RTC_HandleTypeDef *hrtc, RTC_AlarmTypeDef *sAlarm, Alarm_OnOff OnOff)
{
	switch(OnOff)
	{
	case ALARM_OFF:
		RTC_DeactivateAlarm(hrtc);
		break;

	case ALARM_ON:
		RTC_SetAlarm(hrtc, sAlarm);
		break;
	}
}
