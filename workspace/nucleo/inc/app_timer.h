/*
 * app_timer.h
 *
 *  Created on: Dec 13, 2020
 *      Author: chaiminghuang
 */

#ifndef APP_TIMER_H_
#define APP_TIMER_H_

void TIM2_Init(TIM_HandleTypeDef *PeriodicTim, uint32_t prescaleTim, uint32_t periodTim);
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim);


#define D15 GPIO_PIN_8   //PortB
#define D14 GPIO_PIN_9   //PortB

#define D13 GPIO_PIN_5   //PortA
#define D12 GPIO_PIN_6   //PortA
#define D11 GPIO_PIN_7   //PortA

#define D10 GPIO_PIN_6   //PortB
#define D09 GPIO_PIN_7   //PortC

#define D08 GPIO_PIN_9   //PortA
#define D07 GPIO_PIN_8   //PortA


#endif /* APP_TIMER_H_ */
