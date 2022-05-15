//
// Created by Gideon on 2021/8/27.
//

#ifndef STM32H743_PWM_OUTPUT_H
#define STM32H743_PWM_OUTPUT_H
#include "main.h"



#define THROTTLE_START          1100 //起转油门
#define THROTTLE_HOVER_DEFAULT  1500 //悬停油门
#define THR_MIN                 1000 //最低油门
#define Thr_Idle 1100 //油门怠速




#define MOTOR1_THR_SCALE 1.0f
#define MOTOR2_THR_SCALE 1.0f
#define MOTOR3_THR_SCALE 1.0f
#define MOTOR4_THR_SCALE 1.0f

#define MOTOR1_ROLL_SCALE -1.0f
#define MOTOR2_ROLL_SCALE 1.0f
#define MOTOR3_ROLL_SCALE 1.0f
#define MOTOR4_ROLL_SCALE -1.0f

#define MOTOR1_PITCH_SCALE -1.0f
#define MOTOR2_PITCH_SCALE 1.0f
#define MOTOR3_PITCH_SCALE -1.0f
#define MOTOR4_PITCH_SCALE 1.0f

#define MOTOR1_YAW_SCALE 1.0f
#define MOTOR2_YAW_SCALE 1.0f
#define MOTOR3_YAW_SCALE -1.0f
#define MOTOR4_YAW_SCALE -1.0f

void PWM_Output(int8_t pram_lockflag,
                int8_t pram_mode,
                uint16_t parm_throttle,
                ALL_PID_CONTROLLER* para_controller_bus);


#endif //STM32H743_PWM_OUTPUT_H
