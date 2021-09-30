/*
 * app_rtc.h
 *
 *  Created on: Dec 13, 2020
 *      Author: chaiminghuang
 */

#ifndef APP_RTC_H_
#define APP_RTC_H_




typedef struct{
	uint8_t Weekday;
	uint8_t Month;
	uint8_t Date;
	uint8_t Year;
} RTC_Date_t;

typedef struct{
	uint8_t HH;
	uint8_t MM;
	uint8_t SS;
} RTC_Time_t;

typedef enum{
	ALARM_OFF,
	ALARM_ON
} Alarm_OnOff;

void RTC_Init(RTC_HandleTypeDef *hrtc);
void HAL_RTC_MspInit(RTC_HandleTypeDef *hrtc);


void RTC_SetDate(RTC_HandleTypeDef *hrtc, RTC_DateTypeDef *sDate);
void RTC_SetTime(RTC_HandleTypeDef *hrtc, RTC_TimeTypeDef *sTime);
void RTC_SetDateAndTime(RTC_HandleTypeDef *hrtc, RTC_TimeTypeDef *sTime, RTC_DateTypeDef *sDate);
void RTC_GetDate(RTC_HandleTypeDef *hrtc, uint8_t *Date);
void RTC_GetTime(RTC_HandleTypeDef *hrtc, uint8_t *Time);


void RTC_SetAlarm(RTC_HandleTypeDef *hrtc, RTC_AlarmTypeDef *sAlarm);
void RTC_DeactivateAlarm(RTC_HandleTypeDef *hrtc);
void RTC_GetAlarm(RTC_HandleTypeDef *hrtc, RTC_AlarmTypeDef *sAlarm, uint8_t *Time);
void RTC_SetAlarmOnOff(RTC_HandleTypeDef *hrtc, RTC_AlarmTypeDef *sAlarm, Alarm_OnOff OnOff);


#endif /* APP_RTC_H_ */
