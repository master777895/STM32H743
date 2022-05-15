//
// Created by Gideon on 2021/8/8.
//

#ifndef STM32H743_ULTRA_H
#define STM32H743_ULTRA_H

void MX_TIM12_Init(void);
void delay_us(const uint16_t para_us);
void MX_TIM7_Init(void);

extern TIM_HandleTypeDef htim12;
extern uint32_t ultra_carry;
extern float_t ULTRA1_DISTANCE;

#endif //STM32H743_ULTRA_H
