//
// Created by Gideon on 2021/8/8.
//

#ifndef STM32H743_BUZZER_H
#define STM32H743_BUZZER_H

void MX_TIM16_Init(void);
void Buzzer_Set(const int8_t switch_flag,const uint16_t para_frequency,const uint16_t para_volume);

enum buzzer_state
{
    _BUZZER_ON=1,
    _BUZZER_OFF,
};

#endif //STM32H743_BUZZER_H
