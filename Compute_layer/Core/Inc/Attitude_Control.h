//
// Created by Gideon on 2021/8/29.
//

#ifndef STM32H743_ATTITUDE_CONTROL_H
#define STM32H743_ATTITUDE_CONTROL_H
#include "main.h"
#include "Basic_Algorithm.h"
#include "PID.h"


void Attitude_Control(const int16_t* pram_remote, \
                    PID_Controller* parm_Pitch_Angle_Controller,    \
                    PID_Controller* parm_Roll_Angle_Controller,     \
                    PID_Controller* parm_Yaw_Angle_Controller,      \
                                                                    \
                    PID_Controller* para_Pitch_Gyro_Controller,     \
                    PID_Controller* para_Roll_Gyro_Controller,      \
                    PID_Controller* parm_Yaw_Gyro_Controller);

void Angle_Control( const int16_t* pram_remote, \
                    PID_Controller* parm_Pitch_Angle_Controller,    \
                    PID_Controller* parm_Roll_Angle_Controller,     \
                    PID_Controller* parm_Yaw_Angle_Controller,      \
                                                                    \
                    PID_Controller* parm_Yaw_Gyro_Controller);


void Gyro_Control(  PID_Controller* para_Pitch_Gyro_Controller, \
                    PID_Controller* para_Roll_Gyro_Controller,  \
                    PID_Controller* para_Yaw_Gyro_Controller,   \
                                                                \
                    PID_Controller* para_Pitch_Angle_Controller,\
                    PID_Controller* para_Roll_Angle_Controller, \
                    PID_Controller* para_Yaw_Angle_Controller);

#endif //STM32H743_ATTITUDE_CONTROL_H
