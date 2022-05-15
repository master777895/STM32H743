//
// Created by Gideon on 2021/9/11.
//

#include "main.h"
#include "Ultrasonic_Task.h"

void StartUltrasonicTask(void const * argument)
{

    for(;;)
    {
        osDelay(100);///100ms´¥·¢Ò»´Î
//        LOG("TRIG DONE\n");

        TRIG1_OUTPUT_HIGH;
        delay_us(10);
        TRIG1_OUTPUT_LOW;
    }
}