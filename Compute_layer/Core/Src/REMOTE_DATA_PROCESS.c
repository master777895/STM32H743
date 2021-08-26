//
// Created by Gideon on 2021/8/25.
//
#include "main.h"


int8_t Remote_State_Analyse(int16_t* target_from_remote)
{



    static int8_t lock_flag=_LOCK;

/** 解锁 */

    static portTickType unlock_tick=0;
    static int8_t unlock_start=0, unlock_started = 0;

    if(target_from_remote[_THROTTLE] == 0 \
            && target_from_remote[_YAW] >= 199 \
            && target_from_remote[_ROLL] == 0 \
            && target_from_remote[_PITCH] == 0 )
    {
        unlock_start = 1;
    }
    else unlock_start = 0;


    if(unlock_start == 1 && unlock_tick == 0)
    {
        unlock_tick = xTaskGetTickCount();
    }
    else if(unlock_start == 0)
    {
        unlock_tick = 0;
    }


    if(unlock_tick != 0 \
        && (xTaskGetTickCount() - unlock_tick) >= 1500 )
    {
        unlock_tick = 0;
        lock_flag = _UNLOCK;
    }



    /** 上锁 */

    static portTickType inlock_tick=0;
    static int8_t inlock_start=0, inlock_started = 0;

    if(target_from_remote[_THROTTLE] == 0 \
            && target_from_remote[_YAW] <= -195 \
            && target_from_remote[_ROLL] == 0 \
            && target_from_remote[_PITCH] == 0 )
    {
        inlock_start = 1;
    }
    else inlock_start = 0;


    if(inlock_start == 1 && inlock_tick == 0)
    {
        inlock_tick = xTaskGetTickCount();
    }
    else if(inlock_start == 0)
    {
        inlock_tick = 0;
    }


    if(inlock_tick != 0 \
        && (xTaskGetTickCount() - inlock_tick) >= 500 )
    {
        inlock_tick = 0;
        lock_flag = _LOCK;
    }




//    printf("lock_flag : %d\n", lock_flag);

    return lock_flag;
}




/**
 * @param
 * @return 八通道数组，量化了单位。
 * @brief   通道一：横滚：-35~35
 *          通道二：俯仰：-35~35
 *          通道三：油门：0~1000
 *          通道四：偏航：-200~200
 *          通道5~通道8 ：_HIGH, _LOW, _INVALID,
 */
int16_t* Remote_Controller(uint16_t* PPM_DATA_BUFF_0)
{


    static uint16_t RC_Buttom=1000,RC_Top=2000,RC_Middle=1500,RC_Deadband=200,\
         RC_Deadzone_Buttom=1400,RC_Deadzone_Top=1600,Buttom_Safe_Deadband=50;


    /** 通道分别是：
     * 0：通道一：横滚
     * 1：通道二：俯仰
     * 2：通道三：油门
     * 3：通道四：偏航
     * */

    static int16_t PPM_DATA_BUFF_1[8]={0};

    static int16_t Target_from_remote[8] ={0};


    /** 不开遥控器的判断 */
    if (PPM_DATA_BUFF_0[0] == 0 && \
            PPM_DATA_BUFF_0[1] == 0 &&\
            PPM_DATA_BUFF_0[2] == 0 &&\
            PPM_DATA_BUFF_0[3] == 0 &&\
            PPM_DATA_BUFF_0[4] == 0 &&\
            PPM_DATA_BUFF_0[5] == 0 &&\
            PPM_DATA_BUFF_0[6] == 0 &&\
            PPM_DATA_BUFF_0[7] == 0 )
    {
        for (int i = 0; i < 8; i++)
        {
            Target_from_remote[i]=0;
        }

        return Target_from_remote;
    }



    memcpy(PPM_DATA_BUFF_1,   RC_Data_LPF_Filter(PPM_DATA_BUFF_0),   8*sizeof(uint16_t));
//    memcpy(PPM_DATA_BUFF_1,   PPM_DATA_BUFF_0,   8*sizeof(uint16_t));


    /** 通道一 */

    if(PPM_DATA_BUFF_1[0] < 1400)
    {
        Target_from_remote[_ROLL] = (1400 - PPM_DATA_BUFF_1[0]) * MAX_ANGLE_FOR_PITCH_AND_ROLL/400;
    }
    else if (PPM_DATA_BUFF_1[0] > 1600)
    {
        Target_from_remote[_ROLL] = - (PPM_DATA_BUFF_1[0]-1600) * MAX_ANGLE_FOR_PITCH_AND_ROLL/400;
    }
    else
    {
        Target_from_remote[_ROLL] = 0;
    }

    Target_from_remote[_ROLL] = constrain_int16_t(Target_from_remote[_ROLL], \
    -MAX_ANGLE_FOR_PITCH_AND_ROLL, MAX_ANGLE_FOR_PITCH_AND_ROLL);

    /** 通道二 */

    if(PPM_DATA_BUFF_1[1] < 1400)
    {
        Target_from_remote[_PITCH] = (1400 - PPM_DATA_BUFF_1[1]) * MAX_ANGLE_FOR_PITCH_AND_ROLL/400;
    }
    else if (PPM_DATA_BUFF_1[1] > 1600)
    {
        Target_from_remote[_PITCH] = - (PPM_DATA_BUFF_1[1]-1600) * MAX_ANGLE_FOR_PITCH_AND_ROLL/400;
    }
    else
    {
        Target_from_remote[_PITCH] = 0;
    }

    Target_from_remote[_PITCH] = constrain_int16_t(Target_from_remote[_PITCH], \
    -MAX_ANGLE_FOR_PITCH_AND_ROLL, MAX_ANGLE_FOR_PITCH_AND_ROLL);

    /** 通道三 */

    Target_from_remote[_THROTTLE] = PPM_DATA_BUFF_1[2] - (RC_Buttom+Buttom_Safe_Deadband);
    Target_from_remote[_THROTTLE] = constrain_int16_t(Target_from_remote[_THROTTLE], 0, \
                                                                1000);


    /** 通道四 */

    if(PPM_DATA_BUFF_1[3] < 1400)
    {
        Target_from_remote[_YAW] = (1400 - PPM_DATA_BUFF_1[3]) * MAX_ANGLE_FOR_YAW/400;
    }
    else if (PPM_DATA_BUFF_1[3] > 1600)
    {
        Target_from_remote[_YAW] = - (PPM_DATA_BUFF_1[3]-1600) * MAX_ANGLE_FOR_YAW/400;
    }
    else
    {
        Target_from_remote[_YAW] = 0;
    }

    Target_from_remote[_YAW] = constrain_int16_t(Target_from_remote[_YAW], \
    -MAX_ANGLE_FOR_YAW, MAX_ANGLE_FOR_YAW);

/** 通道5 */

    if(PPM_DATA_BUFF_1[4]<1050)
    {
        Target_from_remote[_CHANNEL5] = _LOW ;
    }
    else if (PPM_DATA_BUFF_1[4]>1950)
    {
        Target_from_remote[_CHANNEL5] = _HIGH ;
    }
    else
    {
        Target_from_remote[_CHANNEL5] = _INVALID ;
    }

/** 通道6 */
    if(PPM_DATA_BUFF_1[5]<1050)
    {
        Target_from_remote[_CHANNEL6] = _LOW ;
    }
    else if (PPM_DATA_BUFF_1[5]>1950)
    {
        Target_from_remote[_CHANNEL6] = _HIGH ;
    }
    else
    {
        Target_from_remote[_CHANNEL6] = _INVALID;
    }

/** 通道7 */

    if(PPM_DATA_BUFF_1[6]<1050)
    {
        Target_from_remote[_CHANNEL7] = _LOW ;
    }
    else if (PPM_DATA_BUFF_1[6]>1950)
    {
        Target_from_remote[_CHANNEL7] = _HIGH ;
    }
    else
    {
        Target_from_remote[_CHANNEL7] = _INVALID;
    }

/** 通道8 */

    if(PPM_DATA_BUFF_1[7]<1050)
    {
        Target_from_remote[_CHANNEL8] = _LOW ;
    }
    else if (PPM_DATA_BUFF_1[7]>1950)
    {
        Target_from_remote[_CHANNEL8] = _HIGH ;
    }
    else
    {
        Target_from_remote[_CHANNEL8] = _INVALID;
    }

    return Target_from_remote;

}



/**
 * @brief 对遥控器前四个通道滤波处理
 * @param ppm 8个ppm通道输入
 * @return 滤波后的数据
 */
uint16_t* RC_Data_LPF_Filter(uint16_t* ppm)
{
    static uint16_t retu[8];
    static Butter_BufferData RC_LPF_Buffer[4];//ppm专属的巴特滤波buffer

    uint16_t i=0;
    for(i=0;i<4;i++)//对遥控器前四个通道滤波处理
    {
        retu[i]=(uint16_t)(LPButterworth(ppm[i],&RC_LPF_Buffer[i],&Butter_5HZ_Parameter_RC));
    }

    for(i=4;i<8;i++)//后四位保留
    {
        retu[i] = ppm[i];
    }


    return retu;
}

