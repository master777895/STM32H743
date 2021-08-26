//
// Created by Gideon on 2021/8/27.
//

#ifndef STM32H743_AHRS_H
#define STM32H743_AHRS_H
#include "main.h"

Vector3f* AHRS_by_Madgwick(const Vector3f* pram_accel, const Vector3f* pram_gyro, const float_t* q_init);
float_t* Quaternion_Init(const float_t parm_accelx, const float_t parm_accely, const float_t parm_accelz);
void imuComputeRotationMatrix(void);








#endif //STM32H743_AHRS_H
