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
    uint8_t Err_Limit_Flag ;//ƫ���޷���־
    uint8_t Integrate_Limit_Flag ;//�����޷���־
    uint8_t Integrate_Separation_Flag ;//���ַ����־
    float Expect;//����
    float FeedBack;//����ֵ
    float Err;//ƫ��
    float Last_Err;//�ϴ�ƫ��
    float Err_Max;//ƫ���޷�ֵ
    float Integrate_Separation_Err;//���ַ���ƫ��ֵ
    float Integrate;//����ֵ
    float Integrate_Max;//�����޷�ֵ
    float Kp;//���Ʋ���Kp
    float Ki;//���Ʋ���Ki
    float Kd;//���Ʋ���Kd
    float Control_OutPut;//�����������
    float Last_Control_OutPut;//�ϴο����������
    float Control_OutPut_Limit;//����޷�
    /***************************************/
    float Pre_Last_Err;//���ϴ�ƫ��
    float Adaptable_Kd;//����Ӧ΢�ֲ���
    float Last_FeedBack;//�ϴη���ֵ
    float Dis_Err;//΢����
    float Dis_Error_History[5];//��ʷ΢����
    float Err_LPF;
    float Last_Err_LPF;
    float Dis_Err_LPF;
    float Last_Dis_Err_LPF;
    float Pre_Last_Dis_Err_LPF;
    Butter_BufferData Control_Device_LPF_Buffer;//��������ͨ�����������
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
