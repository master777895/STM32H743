//
// Created by Gideon on 2021/8/22.
//

#ifndef STM32H743_MPU_DATA_PROCESS_H
#define STM32H743_MPU_DATA_PROCESS_H

#include "main.h"

#define Gravity_accel 9.78833f //广州的
#define DEG_TO_RAD (PI / 180.0f)
#define RAD_TO_DEG (180.0f / PI)

//全局变量
extern float_t SIN_PITCH, COS_PITCH;
extern float_t SIN_ROLL, COS_ROLL;
extern float_t SIN_YAW, COS_YAW;
extern float_t PITCH, ROLL, YAW;
extern float_t Euler_Rotate_Matrix[3][3];



enum imu_data
{
    _X=0,
    _Y,
    _Z,
};

typedef struct
{
    float x;
    float y;
    float z;
}Vector3f;

typedef struct
{
    float q0;
    float q1;
    float q2;
    float q3;
}Vector4q;

typedef struct
{
    float q[4];
    float angle[3];
}_Attitude_Tag;

Vector3f* GET_MPU_DATA(void);

#endif //STM32H743_MPU_DATA_PROCESS_H
