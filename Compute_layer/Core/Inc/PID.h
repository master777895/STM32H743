//
// Created by Gideon on 2021/8/29.
//

#ifndef STM32H743_PID_H
#define STM32H743_PID_H
#include "main.h"


typedef enum
{
    _NO_TYPE=0,

    _PID_PITCH_ANGLE,
    _PID_ROLL_ANGLE,
    _PID_YAW_ANGLE,

    _PID_PITCH_GYRO,
    _PID_ROLL_GYRO,
    _PID_YAW_GYRO,

}PID_control_type;

typedef struct
{
    PID_control_type PID_type;
    uint8_t Err_Limit_Flag ;//偏差限幅标志
    uint8_t Integrate_Limit_Flag ;//积分限幅标志
    uint8_t Integrate_Separation_Flag ;//积分分离标志
    float Expect;//期望
    float FeedBack;//反馈值
    float Err;//偏差
    float Last_Err;//上次偏差
    float Err_Max;//偏差限幅值
    float Integrate_Separation_Err;//积分分离偏差值
    float Integrate;//积分值
    float Integrate_Max;//积分限幅值
    float Kp;//控制参数Kp
    float Ki;//控制参数Ki
    float Kd;//控制参数Kd
    float Control_OutPut;//控制器总输出
    float Last_Control_OutPut;//上次控制器总输出
    float Control_OutPut_Limit;//输出限幅
    /***************************************/
    float Pre_Last_Err;//上上次偏差
    float Adaptable_Kd;//自适应微分参数
    float Last_FeedBack;//上次反馈值
    float Dis_Err;//微分量
    float Dis_Error_History[5];//历史微分量
    float Err_LPF;
    float Last_Err_LPF;
    float Dis_Err_LPF;
    float Last_Dis_Err_LPF;
    float Pre_Last_Dis_Err_LPF;
    Butter_BufferData Control_Device_LPF_Buffer;//控制器低通输入输出缓冲
    float PID_Controller_Dt;//0.005
    float Scale_Kp;
    float Scale_Ki;
    float Scale_Kd;
}PID_Controller;


typedef struct
{
    PID_Controller PITCH_ANGLE_CONTROLLER;
    PID_Controller ROLL_ANGLE_CONTROLLER;
    PID_Controller YAW_ANGLE_CONTROLLER;

    PID_Controller PITCH_GYRO_CONTROLLER;
    PID_Controller ROLL_GYRO_CONTROLLER;
    PID_Controller YAW_GYRO_CONTROLLER;

}ALL_PID_CONTROLLER;



extern ALL_PID_CONTROLLER CONTROLLER_BUS;
extern const float PID_CONTROL_UNIT[19][21];

float PID_Control(PID_Controller *pram_controler);
float Control_Device_LPF(float curr_inputer,Butter_BufferData *Buffer,Butter_Parameter *Parameter);
void PID_Init(void);
float PID_Control_Div_LPF_For_Gyro(PID_Controller *Controler);
float PID_Control_Div_LPF(PID_Controller *Controler);
void PID_init_parameter(PID_Controller* para_controller, PID_control_type para_label);
void  PID_Integrate_Reset(PID_Controller *para_Controller);
void Preflight_ResetIntegrate(ALL_PID_CONTROLLER* para_AllController);

#endif //STM32H743_PID_H
