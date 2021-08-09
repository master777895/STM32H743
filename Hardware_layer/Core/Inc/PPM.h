//
// Created by Gideon on 2021/8/8.
//

#ifndef STM32H743_PPM_H
#define STM32H743_PPM_H

void MX_TIM2_Init(void);
void TIM2_IRQHandler(void);

extern uint32_t ppm_carry;

#endif //STM32H743_PPM_H
