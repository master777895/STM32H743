//
// Created by Gideon on 2021/8/27.
//

#include "main.h"
#include "PWM_Output.h"

uint16_t Throttle_Angle_Compensate(uint16_t pram_throttle);




void PWM_Output(const int8_t pram_lockflag,
                const int8_t pram_mode,
                const uint16_t parm_throttle,
                ALL_PID_CONTROLLER* para_controller_bus)
{
    const ALL_PID_CONTROLLER const_controller_bus = *para_controller_bus;


    uint16_t Throttle0,Throttle1,Throttle2,Throttle3;
    uint16_t Motor_PWM_1,Motor_PWM_2,Motor_PWM_3,Motor_PWM_4;

//    Throttle0 = Throttle_Angle_Compensate(parm_throttle);///油门倾角补偿


    if(pram_lockflag == _UNLOCK && pram_mode != _INVALID)
    {
        Set_RGB_Brightness(100,0,0);

        if(pram_mode == _MANUAL_MODE)
        {
            if(parm_throttle >= THROTTLE_START)
            {
                Motor_PWM_1=(uint16_t)(
                         MOTOR1_THR_SCALE   *(float)parm_throttle
                        +MOTOR1_ROLL_SCALE  *const_controller_bus.ROLL_GYRO_CONTROLLER.Control_OutPut
                        +MOTOR1_PITCH_SCALE *const_controller_bus.PITCH_GYRO_CONTROLLER.Control_OutPut
                        +MOTOR1_YAW_SCALE   *const_controller_bus.YAW_GYRO_CONTROLLER.Control_OutPut);

                Motor_PWM_2=(uint16_t)(
                         MOTOR2_THR_SCALE   *(float)parm_throttle
                        +MOTOR2_ROLL_SCALE  *const_controller_bus.ROLL_GYRO_CONTROLLER.Control_OutPut
                        +MOTOR2_PITCH_SCALE *const_controller_bus.PITCH_GYRO_CONTROLLER.Control_OutPut
                        +MOTOR2_YAW_SCALE   *const_controller_bus.YAW_GYRO_CONTROLLER.Control_OutPut);

                Motor_PWM_3=(uint16_t)(
                         MOTOR3_THR_SCALE   *(float)parm_throttle
                        +MOTOR3_ROLL_SCALE  *const_controller_bus.ROLL_GYRO_CONTROLLER.Control_OutPut
                        +MOTOR3_PITCH_SCALE *const_controller_bus.PITCH_GYRO_CONTROLLER.Control_OutPut
                        +MOTOR3_YAW_SCALE   *const_controller_bus.YAW_GYRO_CONTROLLER.Control_OutPut);

                Motor_PWM_4=(uint16_t)(
                         MOTOR4_THR_SCALE   *(float)parm_throttle
                        +MOTOR4_ROLL_SCALE  *const_controller_bus.ROLL_GYRO_CONTROLLER.Control_OutPut
                        +MOTOR4_PITCH_SCALE *const_controller_bus.PITCH_GYRO_CONTROLLER.Control_OutPut
                        +MOTOR4_YAW_SCALE   *const_controller_bus.YAW_GYRO_CONTROLLER.Control_OutPut);

//                LOG("PWM: %d, %d, %d, %d\n",Motor_PWM_1,Motor_PWM_2,Motor_PWM_3,Motor_PWM_4);
            }
            else ///没有起飞
            {
                Motor_PWM_1=(uint16_t)(parm_throttle);
                Motor_PWM_2=(uint16_t)(parm_throttle);
                Motor_PWM_3=(uint16_t)(parm_throttle);
                Motor_PWM_4=(uint16_t)(parm_throttle);

                ///清积分
                Preflight_ResetIntegrate(para_controller_bus);

            }

            //总输出限幅
            Motor_PWM_1=Value_Limit(Thr_Idle,2000,Motor_PWM_1);
            Motor_PWM_2=Value_Limit(Thr_Idle,2000,Motor_PWM_2);
            Motor_PWM_3=Value_Limit(Thr_Idle,2000,Motor_PWM_3);
            Motor_PWM_4=Value_Limit(Thr_Idle,2000,Motor_PWM_4);

            //Throttle_Control_Reset();
        }

        else if(pram_mode == _ALTITUDE_HOLD_MODE \
                || pram_mode == _POS_HOLD_MODE  \
                || pram_mode == _SDK_MODE       \
                || pram_mode == _LAND_MODE)
        {
            Motor_PWM_1=(uint16_t)(
                    MOTOR1_THR_SCALE   *(float)parm_throttle
                    +MOTOR1_ROLL_SCALE  *const_controller_bus.ROLL_GYRO_CONTROLLER.Control_OutPut
                    +MOTOR1_PITCH_SCALE *const_controller_bus.PITCH_GYRO_CONTROLLER.Control_OutPut
                    +MOTOR1_YAW_SCALE   *const_controller_bus.YAW_GYRO_CONTROLLER.Control_OutPut);

            Motor_PWM_2=(uint16_t)(
                    MOTOR2_THR_SCALE   *(float)parm_throttle
                    +MOTOR2_ROLL_SCALE  *const_controller_bus.ROLL_GYRO_CONTROLLER.Control_OutPut
                    +MOTOR2_PITCH_SCALE *const_controller_bus.PITCH_GYRO_CONTROLLER.Control_OutPut
                    +MOTOR2_YAW_SCALE   *const_controller_bus.YAW_GYRO_CONTROLLER.Control_OutPut);

            Motor_PWM_3=(uint16_t)(
                    MOTOR3_THR_SCALE   *(float)parm_throttle
                    +MOTOR3_ROLL_SCALE  *const_controller_bus.ROLL_GYRO_CONTROLLER.Control_OutPut
                    +MOTOR3_PITCH_SCALE *const_controller_bus.PITCH_GYRO_CONTROLLER.Control_OutPut
                    +MOTOR3_YAW_SCALE   *const_controller_bus.YAW_GYRO_CONTROLLER.Control_OutPut);

            Motor_PWM_4=(uint16_t)(
                    MOTOR4_THR_SCALE    *(float)parm_throttle
                    +MOTOR4_ROLL_SCALE  *const_controller_bus.ROLL_GYRO_CONTROLLER.Control_OutPut
                    +MOTOR4_PITCH_SCALE *const_controller_bus.PITCH_GYRO_CONTROLLER.Control_OutPut
                    +MOTOR4_YAW_SCALE   *const_controller_bus.YAW_GYRO_CONTROLLER.Control_OutPut);

            ///总输出限幅
            Motor_PWM_1=Value_Limit(Thr_Idle,2000,Motor_PWM_1);
            Motor_PWM_2=Value_Limit(Thr_Idle,2000,Motor_PWM_2);
            Motor_PWM_3=Value_Limit(Thr_Idle,2000,Motor_PWM_3);
            Motor_PWM_4=Value_Limit(Thr_Idle,2000,Motor_PWM_4);
        }
        else///不会来到这里
        {
            Motor_PWM_1=THR_MIN;
            Motor_PWM_2=THR_MIN;
            Motor_PWM_3=THR_MIN;
            Motor_PWM_4=THR_MIN;
            Error_Handler();
        }
    }
    else ///飞机没有解锁 或者 模式错误
    {
        Set_RGB_Brightness(0,100,0);

        Motor_PWM_1=THR_MIN;
        Motor_PWM_2=THR_MIN;
        Motor_PWM_3=THR_MIN;
        Motor_PWM_4=THR_MIN;

        ///清积分
        Preflight_ResetIntegrate(para_controller_bus);

        //Throttle_Control_Reset();
    }

    Motor_PWM_1=Value_Limit(0,2000,Motor_PWM_1);
    Motor_PWM_2=Value_Limit(0,2000,Motor_PWM_2);
    Motor_PWM_3=Value_Limit(0,2000,Motor_PWM_3);
    Motor_PWM_4=Value_Limit(0,2000,Motor_PWM_4);

//    LOG("PWM_Output: %d, %d, %d, %d, %.3f, %.3f, %.3f\n",Motor_PWM_1,Motor_PWM_2,Motor_PWM_3,Motor_PWM_4,
//        const_controller_bus.ROLL_GYRO_CONTROLLER.Control_OutPut,
//        const_controller_bus.PITCH_GYRO_CONTROLLER.Control_OutPut,
//        const_controller_bus.YAW_GYRO_CONTROLLER.Control_OutPut);


    PWM_load(Motor_PWM_1,Motor_PWM_2,Motor_PWM_3,Motor_PWM_4);
}



/**
 * @brief 油门倾角补偿函数
 * @param pram_throttle 油门值
 * @return 补偿后的油门数值
 */
uint16_t Throttle_Angle_Compensate(uint16_t pram_throttle)//油门倾角补偿
{
    float CosPitch_CosRoll=ABS(COS_PITCH*COS_ROLL);
    float Throttle_Makeup=0;
    float Temp=0;
    uint16_t retu=0;

    if(CosPitch_CosRoll>=0.999999)  CosPitch_CosRoll=0.999999;
    if(CosPitch_CosRoll<=0.000001)  CosPitch_CosRoll=0.000001;
    if(CosPitch_CosRoll<=0.50)  CosPitch_CosRoll=0.50;//Pitch,Roll约等于30度
    if(pram_throttle>=THROTTLE_START)//大于起转油门量
    {
        Temp=(uint16_t)(MAX(ABS(100*PITCH),ABS(100*ROLL)));
        Temp=constrain_float(9000-Temp,0,3000)/(3000*CosPitch_CosRoll);

        Throttle_Makeup=(pram_throttle-THROTTLE_START)*Temp;//油门倾角补偿

        retu=(uint16_t)(THROTTLE_START+Throttle_Makeup);
        retu=(uint16_t)(constrain_float(retu,THROTTLE_START,2000));

        return retu;
    }
    else retu = pram_throttle;
}