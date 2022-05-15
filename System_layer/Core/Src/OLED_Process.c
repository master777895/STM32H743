//
// Created by Gideon on 2021/9/9.
//

#include "main.h"
#include "OLED_Process.h"







void OLED_Update(void)
{

    OLED_Clear();

    static uint8_t temp_uint8[40]={0};

#if (ESC_CALIBRATION_MODE_NOW == 1)
///如果是电调模式

    sprintf(temp_uint8, "ESC_CALIBRATION_MODE");
    OLED_ShowString(0,0,temp_uint8,16);

    sprintf(temp_uint8, "Throttle : %d", PPM_DATA_BUFF[_THROTTLE]);
    OLED_ShowString(0,44,temp_uint8,12);


#elif (ESC_CALIBRATION_MODE_NOW == 0)


    switch (NOW_PAGE_NUMBER)
    {
        case 1:///第一页
        {
            sprintf(temp_uint8, "Pitch: %.2f",PITCH);
            OLED_ShowString(0,0,temp_uint8,12);

            sprintf(temp_uint8, "Roll : %.2f",ROLL);
            OLED_ShowString(0,11,temp_uint8,12);

            sprintf(temp_uint8, "Yaw  : %.2f",YAW);
            OLED_ShowString(0,22,temp_uint8,12);

            sprintf(temp_uint8, "%.2f",PITCH_GYRO);
            OLED_ShowString(90,0,temp_uint8,12);

            sprintf(temp_uint8, "%.2f",ROLL_GYRO);
            OLED_ShowString(90,11,temp_uint8,12);

            sprintf(temp_uint8, "%.2f",YAW_GYRO);
            OLED_ShowString(90,22,temp_uint8,12);

            sprintf(temp_uint8, "Ultra: %.2f",ULTRA1_DISTANCE);
            OLED_ShowString(0,33,temp_uint8,12);

            break;
        }

        case 2:///第2页
        {

            sprintf(temp_uint8,"Ch1: %d",PPM_DATA_BUFF[_ROLL]);
            OLED_ShowString(0,0,temp_uint8,12);

            sprintf(temp_uint8,"Ch2: %d",PPM_DATA_BUFF[_PITCH]);
            OLED_ShowString(0,11,temp_uint8,12);

            sprintf(temp_uint8,"Ch3: %d",PPM_DATA_BUFF[_THROTTLE]);
            OLED_ShowString(0,22,temp_uint8,12);

            sprintf(temp_uint8,"Ch4: %d",PPM_DATA_BUFF[_YAW]);
            OLED_ShowString(0,33,temp_uint8,12);



            sprintf(temp_uint8,"Ch5: %d",PPM_DATA_BUFF[_CHANNEL5]);
            OLED_ShowString(64,0,temp_uint8,12);

            sprintf(temp_uint8,"Ch6: %d",PPM_DATA_BUFF[_CHANNEL6]);
            OLED_ShowString(64,11,temp_uint8,12);

            sprintf(temp_uint8,"Ch7: %d",PPM_DATA_BUFF[_CHANNEL7]);
            OLED_ShowString(64,22,temp_uint8,12);

            sprintf(temp_uint8,"Ch8: %d",PPM_DATA_BUFF[_CHANNEL8]);
            OLED_ShowString(64,33,temp_uint8,12);


            break;
        }

        case 3:
        {


            break;
        }

        default:
        {

            break;
        }
    }

#endif

    OLED_Refresh();

}

