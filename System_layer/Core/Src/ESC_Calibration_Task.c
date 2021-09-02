//
// Created by Gideon on 2021/8/27.
//

#include "main.h"

void Start_ESC_Calibra_Task(void const * argument)
{
    static portTickType PreviousWakeTime;
    PreviousWakeTime = xTaskGetTickCount();
    for (;;)
    {
        osDelayUntil(&PreviousWakeTime,5);
//        printf("ESC_calibration_mode!!!  ");
//        printf("ppmdata: %d\n",PPM_DATA_BUFF[_THROTTLE]);

        PWM_load( PPM_DATA_BUFF[_THROTTLE],PPM_DATA_BUFF[_THROTTLE], \
            PPM_DATA_BUFF[_THROTTLE], PPM_DATA_BUFF[_THROTTLE]);
    }
}