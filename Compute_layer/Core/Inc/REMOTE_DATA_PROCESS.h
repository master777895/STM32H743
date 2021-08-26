//
// Created by Gideon on 2021/8/25.
//

#ifndef STM32H743_REMOTE_DATA_PROCESS_H
#define STM32H743_REMOTE_DATA_PROCESS_H
#include "main.h"


enum base_type
{
    _ROLL=0,
    _PITCH,
    _THROTTLE,
    _YAW,

    _CHANNEL5,
    _CHANNEL6,
    _CHANNEL7,
    _CHANNEL8,
};

enum channel_state
{
    _INVALID=0,
    _HIGH,
    _LOW,

};

enum lock_state
{
    _LOCK = 1,
    _UNLOCK = 2,

};


#define MAX_ANGLE_FOR_PITCH_AND_ROLL 35
#define MAX_ANGLE_FOR_YAW 200


int8_t Remote_State_Analyse(int16_t* target_from_remote);
uint16_t* RC_Data_LPF_Filter(uint16_t* ppm);
int16_t* Remote_Controller(uint16_t* PPM_DATA_BUFF_0);


#endif //STM32H743_REMOTE_DATA_PROCESS_H
