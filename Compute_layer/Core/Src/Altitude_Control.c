//
// Created by Gideon on 2021/9/23.
//

#include "main.h"
#include "Altitude_Control.h"


void Altitude_Statemachine(void)
{

    static Butter_BufferData Butter_Buffer_HCSR04;
    static Butter_Parameter Butter_2HZ_Parameter_HCSR04;
    float Altitude_Filter=0;


    static uint8_t begin_flag = 0;
    if(begin_flag == 0)
    {
        begin_flag = 1;
        Set_Cutoff_Frequency(10, 4 ,&Butter_2HZ_Parameter_HCSR04);
    }

    /******************* 不使用滤波，直接用超声波的数据来玩 ***********************/

    Altitude_Filter=LPButterworth(ULTRA1_DISTANCE,&Butter_Buffer_HCSR04,&Butter_2HZ_Parameter_HCSR04);

}