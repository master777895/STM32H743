//
// Created by Gideon on 2021/8/25.
//

#ifndef STM32H743_BASIC_ALGORITHM_H
#define STM32H743_BASIC_ALGORITHM_H

#include "main.h"

float invSqrt(float x);
float_t constrain(float value, float_t min_val, float max_val);
int16_t constrain_int16_t(int16_t amt, int16_t low, int16_t high);

#endif //STM32H743_BASIC_ALGORITHM_H
