//
// Created by Gideon on 2021/8/29.
//
#include "main.h"
#include "Attitude_Control.h"




/************姿态环控制器：角度+角速度****************/
void Attitude_Control(const int16_t* pram_remote, \
                    PID_Controller* parm_Pitch_Angle_Controller,    \
                    PID_Controller* parm_Roll_Angle_Controller,     \
                    PID_Controller* parm_Yaw_Angle_Controller,      \
                                                                    \
                    PID_Controller* para_Pitch_Gyro_Controller,     \
                    PID_Controller* para_Roll_Gyro_Controller,      \
                    PID_Controller* parm_Yaw_Gyro_Controller)
{
    ///外环 角度控制
    Angle_Control(pram_remote,
                  parm_Pitch_Angle_Controller,
                  parm_Roll_Angle_Controller,
                  parm_Yaw_Angle_Controller,
                  parm_Yaw_Gyro_Controller);

    ///内环 角速度控制
    Gyro_Control(para_Pitch_Gyro_Controller,
                 para_Roll_Gyro_Controller,
                 parm_Yaw_Gyro_Controller,
                 parm_Pitch_Angle_Controller,
                 parm_Roll_Angle_Controller,
                 parm_Yaw_Angle_Controller);//角速度控制
}


///角度控制
///无头模式
void Angle_Control( const int16_t* pram_remote, \
                    PID_Controller* parm_Pitch_Angle_Controller,    \
                    PID_Controller* parm_Roll_Angle_Controller,     \
                    PID_Controller* parm_Yaw_Angle_Controller,      \
                                                                    \
                    PID_Controller* parm_Yaw_Gyro_Controller)
{

    static int8_t set_yaw_target_flag=1;

    /** RITCH和ROLL角度反馈 */
    parm_Pitch_Angle_Controller->FeedBack=PITCH;
    PID_Control(parm_Pitch_Angle_Controller);

    parm_Roll_Angle_Controller->FeedBack =ROLL;
    PID_Control(parm_Roll_Angle_Controller);


    /** YAW角反馈 */
    if(pram_remote[_YAW] == 0) //偏航杆置于中位
    {
        if(set_yaw_target_flag == 1)//锁头
        {
            parm_Yaw_Angle_Controller->Expect=YAW;
            set_yaw_target_flag = 0;
        }

        parm_Yaw_Angle_Controller->FeedBack=YAW;//偏航角反馈
        PID_Control(parm_Yaw_Angle_Controller);//偏航角度控制
        //偏航角速度环期望，来源于偏航角度控制器输出
        parm_Yaw_Gyro_Controller->Expect=parm_Yaw_Angle_Controller->Control_OutPut;

    }
    else//拨动偏航杆，只进行内环角速度控制
    {

        parm_Yaw_Angle_Controller->Expect=0;///偏航角速度期望给0,不进行角度控制
        parm_Yaw_Gyro_Controller->Expect=pram_remote[_YAW];///偏航角速度环期望，直接来源于遥控器打杆量

        set_yaw_target_flag=1;
    }


}

//角速度环
void Gyro_Control(  PID_Controller* para_Pitch_Gyro_Controller, \
                    PID_Controller* para_Roll_Gyro_Controller,  \
                    PID_Controller* para_Yaw_Gyro_Controller,   \
                                                                \
                    PID_Controller* para_Pitch_Angle_Controller,\
                    PID_Controller* para_Roll_Angle_Controller, \
                    PID_Controller* para_Yaw_Angle_Controller)
{

    static float Pitch_Gyro_Control_Expect_Delta=0;
    static float Roll_Gyro_Control_Expect_Delta=0;
    static float Yaw_Gyro_Control_Expect_Delta=0;
    static float Last_Pitch_Gyro_Control_Expect=0;
    static float Last_Roll_Gyro_Control_Expect=0;
    static float Last_Yaw_Gyro_Control_Expect=0;

    ///参数
    static float Pitch_Roll_Feedforward_Kd=0.0f;
    static float Pitch_Roll_Feedforward_Kp=0.1f;
    static float Yaw_Feedforward_Kp=0.0f;
    static float Yaw_Feedforward_Kd=0.05f;

    if(1)//俯仰、横滚方向姿态内环角速度控制器采用PID控制器
    {
        /***************内环角速度期望****************/
        para_Pitch_Gyro_Controller->Expect=para_Pitch_Angle_Controller->Control_OutPut;
        para_Roll_Gyro_Controller->Expect=para_Roll_Angle_Controller->Control_OutPut;

        /***************内环角速度反馈****************/
        para_Pitch_Gyro_Controller->FeedBack=PITCH_GYRO;
        para_Roll_Gyro_Controller->FeedBack=ROLL_GYRO;

        /***************内环角速度控制****************/
        //PID_Control_Div_LPF(&Total_Controller.Pitch_Gyro_Control);
        //PID_Control_Div_LPF(&Total_Controller.Roll_Gyro_Control);

        /***************内环角速度控制：微分参数动态调整****************/
        PID_Control_Div_LPF_For_Gyro(para_Pitch_Gyro_Controller);
        PID_Control_Div_LPF_For_Gyro(para_Roll_Gyro_Controller);

        /** 用到一些神奇的操作，期望值的导数，来前馈到输出 看不懂，但是大受震撼.gif*/
        Pitch_Gyro_Control_Expect_Delta=1000*((para_Pitch_Gyro_Controller->Expect - Last_Pitch_Gyro_Control_Expect)
                        /para_Pitch_Gyro_Controller->PID_Controller_Dt);

        Roll_Gyro_Control_Expect_Delta=1000*((para_Roll_Gyro_Controller->Expect - Last_Roll_Gyro_Control_Expect)
                        /para_Roll_Gyro_Controller->PID_Controller_Dt);

        Last_Pitch_Gyro_Control_Expect=para_Pitch_Gyro_Controller->Expect;
        Last_Roll_Gyro_Control_Expect=para_Roll_Gyro_Controller->Expect;

        para_Pitch_Gyro_Controller->Control_OutPut+=Pitch_Roll_Feedforward_Kd * Pitch_Gyro_Control_Expect_Delta
                                                            +Pitch_Roll_Feedforward_Kp * para_Pitch_Gyro_Controller->Expect;
        para_Pitch_Gyro_Controller->Control_OutPut=constrain_float(para_Pitch_Gyro_Controller->Control_OutPut,
                                                                    -para_Pitch_Gyro_Controller->Control_OutPut_Limit,
                                                                    para_Pitch_Gyro_Controller->Control_OutPut_Limit);

        para_Roll_Gyro_Controller->Control_OutPut+=Pitch_Roll_Feedforward_Kd * Roll_Gyro_Control_Expect_Delta
                                                           +Pitch_Roll_Feedforward_Kp * para_Roll_Gyro_Controller->Expect;
        para_Roll_Gyro_Controller->Control_OutPut=constrain_float(para_Roll_Gyro_Controller->Control_OutPut,
                                                                    -para_Roll_Gyro_Controller->Control_OutPut_Limit,
                                                                    para_Roll_Gyro_Controller->Control_OutPut_Limit);

    }

    else//测试用、正常只选择一种模式
    {
//        /***************内环角速度期望****************/
//        para_Pitch_Gyro_Controller->Expect = para_Pitch_Angle_Controller->Control_OutPut;
//        para_Roll_Gyro_Controller->Expect  = para_Roll_Angle_Controller->Control_OutPut;
//        /***************内环角速度反馈****************/
//        para_Pitch_Gyro_Controller->FeedBack=PITCH_GYRO;
//        para_Roll_Gyro_Controller->FeedBack =ROLL_GYRO;
//
//        /***************内环角速度控制****************/
//        PID_Control_Div_LPF(&Total_Controller.Pitch_Gyro_Control);
//        PID_Control_Div_LPF(&Total_Controller.Roll_Gyro_Control);
//
//        ADRC_Control(&ADRC_Pitch_Controller,
//                     Total_Controller.Pitch_Angle_Control.Control_OutPut,//角速度期望
//                     Pitch_Gyro);//角速度状态反馈
//        ADRC_Control(&ADRC_Roll_Controller,
//                     Total_Controller.Roll_Angle_Control.Control_OutPut,//角速度期望
//                     Roll_Gyro);//角速度状态反馈
    }

    ///偏航角前馈控制,偏航角的期望在角度环函数中设立了
    //Total_Controller.Yaw_Gyro_Control.FeedBack=Yaw_Gyro;
    para_Yaw_Gyro_Controller->FeedBack = YAW_GYRO;//Yaw_Gyro;

    PID_Control_Div_LPF(para_Yaw_Gyro_Controller);

    /**************************偏航角前馈控制**********************************/
    Yaw_Gyro_Control_Expect_Delta=1000*((para_Yaw_Gyro_Controller->Expect-Last_Yaw_Gyro_Control_Expect)
                                  /para_Yaw_Gyro_Controller->PID_Controller_Dt);

    para_Yaw_Gyro_Controller->Control_OutPut+=Yaw_Feedforward_Kp*para_Yaw_Gyro_Controller->Expect
                                +Yaw_Feedforward_Kd*Yaw_Gyro_Control_Expect_Delta;//偏航角前馈控制
    para_Yaw_Gyro_Controller->Control_OutPut=constrain_float(para_Yaw_Gyro_Controller->Control_OutPut,
                                                         -para_Yaw_Gyro_Controller->Control_OutPut_Limit,
                                                            para_Yaw_Gyro_Controller->Control_OutPut_Limit);
    Last_Yaw_Gyro_Control_Expect=para_Yaw_Gyro_Controller->Expect;

    /*******偏航控制异常情况判断，即偏航控制量很大时，偏航角速度很小，如此时为强外力干扰、已着地等******************************/
//    if(ABS(para_Yaw_Gyro_Controller->Control_OutPut)>para_Yaw_Gyro_Controller->Control_OutPut_Limit/2//偏航控制输出相对较大
//       &&ABS(YAW_GYRO)<=30.0)//偏航角速度相对很小
//    {
//        Yaw_Control_Fault_Cnt++;
//        if(Yaw_Control_Fault_Cnt>=500) Yaw_Control_Fault_Cnt=500;
//    }
//    else Yaw_Control_Fault_Cnt/=2;//不满足，快速削减至0
//
//    if(Yaw_Control_Fault_Cnt>=400)//持续5ms*400=2S,特殊处理
//    {
//        PID_Integrate_Reset(para_Yaw_Gyro_Controller);//清空偏航角速度控制的积分
//        PID_Integrate_Reset(para_Yaw_Angle_Controller);//清空偏航角控制的积分
//        Total_Controller.Yaw_Angle_Control.Expect=YAW;//将当前偏航角，作为期望偏航角
//        Yaw_Control_Fault_Cnt=0;
//    }
    /*******偏航控制异常处理结束******************************/
}






























