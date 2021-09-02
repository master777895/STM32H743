//
// Created by Gideon on 2021/8/25.
//
#include "main.h"


/**
 * @brief Å£¶Ùµü´ú·¨Çó 1.0/sqrt(x)
 * @param x
 * @return
 */
float invSqrt(float x)
{
    float halfx = 0.5f * x;
    float y = x;
    long i = *(long*)&y;

    i = 0x5f3759df - (i>>1);
    y = *(float*)&i;
    y = y * (1.5f - (halfx * y * y));

    return y;
}



float_t constrain(float value, const float min_val, const float max_val)
{
    if(value>=max_val)  value=(float_t )max_val;
    if(value<=min_val)  value=(float_t )min_val;
    return value;
}


int16_t constrain_int16_t(int16_t amt, int16_t low, int16_t high)
{
    return ((amt)<(low)?(low):((amt)>(high)?(high):(amt)));
}

float constrain_float(float amt, float low, float high)
{
    return ((amt)<(low)?(low):((amt)>(high)?(high):(amt)));
}

uint16_t Value_Limit(uint16_t Min,uint16_t Max,uint16_t Data)
{
    if(Data>=Max) Data=Max;
    else if(Data<=Min) Data=Min;
    return Data;
}




