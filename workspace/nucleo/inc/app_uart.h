/*
 * app_uart.h
 *
 *  Created on: Mar 7, 2020
 *      Author: chaiminghuang
 */

#ifndef APP_UART_H_
#define APP_UART_H_


#define UART2_GPIO_PORT 	GPIOA
#define UART2_TX_PIN		GPIO_PIN_2
#define UART2_RX_PIN		GPIO_PIN_3


#define UART1_GPIO_PORT 	GPIOA
#define UART1_TX_PIN		GPIO_PIN_9	//remap USART1 to GPIO_PIN_6
#define UART1_RX_PIN		GPIO_PIN_10	//remap USART1 to GPIO_PIN_7

void UART_Interface_Init(UART_HandleTypeDef* huart);
void HAL_UART_MspInit(UART_HandleTypeDef* huart);



#define CR_CHAR 		0x0D
#define BACKSPACE_CHAR	0x08
#define DELETE_CHAR		0x7F

#endif /* APP_UART_H_ */
