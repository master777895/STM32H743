//
// Created by Gideon on 2021/8/22.
//

#include "main.h"


//全局变量,节约运算量
float_t PITCH=0, ROLL=0, YAW=0;
float_t PITCH_GYRO=0, ROLL_GYRO=0, YAW_GYRO=0;

float_t SIN_PITCH=0, COS_PITCH=0;
float_t SIN_ROLL=0, COS_ROLL=0;
float_t SIN_YAW =0, COS_YAW=0;
float_t Euler_Rotate_Matrix[3][3] = {{0}};

ALL_PID_CONTROLLER CONTROLLER_BUS;


void Compute_layer_Init(void)
{

    Butterworth_Parameter_Init();
    PID_Init();

}