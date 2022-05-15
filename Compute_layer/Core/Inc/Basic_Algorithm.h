//
// Created by Gideon on 2021/8/25.
//

#ifndef STM32H743_BASIC_ALGORITHM_H
#define STM32H743_BASIC_ALGORITHM_H

#include "main.h"
#include "Attitude_Control.h"

#define M_PI_F 3.141592653589793f
#define PI 3.1415926535898

#define ABS(X)  (((X)>0)?(X):-(X))
//#define MAX(a,b)  ((a)>(b)?(a):(b))
//#define MIN(a,b)  ((a)<(b)?(a):(b))




float invSqrt(float x);
float_t constrain(float value, float_t min_val, float max_val);
int16_t constrain_int16_t(int16_t amt, int16_t low, int16_t high);
float constrain_float(float amt, float low, float high);
uint16_t Value_Limit(uint16_t Min,uint16_t Max,uint16_t Data);



#endif //STM32H743_BASIC_ALGORITHM_H
