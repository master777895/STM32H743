//
// Created by Gideon on 2021/8/7.
//

#ifndef STM32H743_UART_H
#define STM32H743_UART_H

extern UART_HandleTypeDef huart1;

void MX_UART4_Init(void);
void MX_UART5_Init(void);
void MX_UART7_Init(void);
void MX_USART1_UART_Init(void);
void MX_USART2_UART_Init(void);
void MX_USART3_UART_Init(void);
void MX_USART6_UART_Init(void);

#endif //STM32H743_UART_H
