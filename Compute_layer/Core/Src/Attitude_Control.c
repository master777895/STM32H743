//
// Created by Gideon on 2021/8/29.
//
#include "main.h"
#include "Attitude_Control.h"




/************��̬�����������Ƕ�+���ٶ�****************/
void Attitude_Control(const int16_t* pram_remote, \
                    PID_Controller* parm_Pitch_Angle_Controller,    \
                    PID_Controller* parm_Roll_Angle_Controller,     \
                    PID_Controller* parm_Yaw_Angle_Controller,      \
                                                                    \
                    PID_Controller* para_Pitch_Gyro_Controller,     \
                    PID_Controller* para_Roll_Gyro_Controller,      \
                    PID_Controller* parm_Yaw_Gyro_Controller)
{
    ///�⻷ �Ƕȿ���
    Angle_Control(pram_remote,
                  parm_Pitch_Angle_Controller,
                  parm_Roll_Angle_Controller,
                  parm_Yaw_Angle_Controller,
                  parm_Yaw_Gyro_Controller);

    ///�ڻ� ���ٶȿ���
    Gyro_Control(para_Pitch_Gyro_Controller,
                 para_Roll_Gyro_Controller,
                 parm_Yaw_Gyro_Controller,
                 parm_Pitch_Angle_Controller,
                 parm_Roll_Angle_Controller,
                 parm_Yaw_Angle_Controller);//���ٶȿ���
}


///�Ƕȿ���
///��ͷģʽ
void Angle_Control( const int16_t* pram_remote, \
                    PID_Controller* parm_Pitch_Angle_Controller,    \
                    PID_Controller* parm_Roll_Angle_Controller,     \
                    PID_Controller* parm_Yaw_Angle_Controller,      \
                                                                    \
                    PID_Controller* parm_Yaw_Gyro_Controller)
{

    static int8_t set_yaw_target_flag=1;

    /** RITCH��ROLL�Ƕȷ��� */
    parm_Pitch_Angle_Controller->FeedBack=PITCH;
    PID_Control(parm_Pitch_Angle_Controller);

    parm_Roll_Angle_Controller->FeedBack =ROLL;
    PID_Control(parm_Roll_Angle_Controller);


    /** YAW�Ƿ��� */
    if(pram_remote[_YAW] == 0) //ƫ����������λ
    {
        if(set_yaw_target_flag == 1)//��ͷ
        {
            parm_Yaw_Angle_Controller->Expect=YAW;
            set_yaw_target_flag = 0;
        }

        parm_Yaw_Angle_Controller->FeedBack=YAW;//ƫ���Ƿ���
        PID_Control(parm_Yaw_Angle_Controller);//ƫ���Ƕȿ���
        //ƫ�����ٶȻ���������Դ��ƫ���Ƕȿ��������
        parm_Yaw_Gyro_Controller->Expect=parm_Yaw_Angle_Controller->Control_OutPut;

    }
    else//����ƫ���ˣ�ֻ�����ڻ����ٶȿ���
    {

        parm_Yaw_Angle_Controller->Expect=0;///ƫ�����ٶ�������0,�����нǶȿ���
        parm_Yaw_Gyro_Controller->Expect=pram_remote[_YAW];///ƫ�����ٶȻ�������ֱ����Դ��ң���������

        set_yaw_target_flag=1;
    }


}

//���ٶȻ�
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

    ///����
    static float Pitch_Roll_Feedforward_Kd=0.0f;
    static float Pitch_Roll_Feedforward_Kp=0.1f;
    static float Yaw_Feedforward_Kp=0.0f;
    static float Yaw_Feedforward_Kd=0.05f;

    if(1)//���������������̬�ڻ����ٶȿ���������PID������
    {
        /***************�ڻ����ٶ�����****************/
        para_Pitch_Gyro_Controller->Expect=para_Pitch_Angle_Controller->Control_OutPut;
        para_Roll_Gyro_Controller->Expect=para_Roll_Angle_Controller->Control_OutPut;

        /***************�ڻ����ٶȷ���****************/
        para_Pitch_Gyro_Controller->FeedBack=PITCH_GYRO;
        para_Roll_Gyro_Controller->FeedBack=ROLL_GYRO;

        /***************�ڻ����ٶȿ���****************/
        //PID_Control_Div_LPF(&Total_Controller.Pitch_Gyro_Control);
        //PID_Control_Div_LPF(&Total_Controller.Roll_Gyro_Control);

        /***************�ڻ����ٶȿ��ƣ�΢�ֲ�����̬����****************/
        PID_Control_Div_LPF_For_Gyro(para_Pitch_Gyro_Controller);
        PID_Control_Div_LPF_For_Gyro(para_Roll_Gyro_Controller);

        /** �õ�һЩ����Ĳ���������ֵ�ĵ�������ǰ������� �����������Ǵ�����.gif*/
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

    else//�����á�����ֻѡ��һ��ģʽ
    {
//        /***************�ڻ����ٶ�����****************/
//        para_Pitch_Gyro_Controller->Expect = para_Pitch_Angle_Controller->Control_OutPut;
//        para_Roll_Gyro_Controller->Expect  = para_Roll_Angle_Controller->Control_OutPut;
//        /***************�ڻ����ٶȷ���****************/
//        para_Pitch_Gyro_Controller->FeedBack=PITCH_GYRO;
//        para_Roll_Gyro_Controller->FeedBack =ROLL_GYRO;
//
//        /***************�ڻ����ٶȿ���****************/
//        PID_Control_Div_LPF(&Total_Controller.Pitch_Gyro_Control);
//        PID_Control_Div_LPF(&Total_Controller.Roll_Gyro_Control);
//
//        ADRC_Control(&ADRC_Pitch_Controller,
//                     Total_Controller.Pitch_Angle_Control.Control_OutPut,//���ٶ�����
//                     Pitch_Gyro);//���ٶ�״̬����
//        ADRC_Control(&ADRC_Roll_Controller,
//                     Total_Controller.Roll_Angle_Control.Control_OutPut,//���ٶ�����
//                     Roll_Gyro);//���ٶ�״̬����
    }

    ///ƫ����ǰ������,ƫ���ǵ������ڽǶȻ�������������
    //Total_Controller.Yaw_Gyro_Control.FeedBack=Yaw_Gyro;
    para_Yaw_Gyro_Controller->FeedBack = YAW_GYRO;//Yaw_Gyro;

    PID_Control_Div_LPF(para_Yaw_Gyro_Controller);

    /**************************ƫ����ǰ������**********************************/
    Yaw_Gyro_Control_Expect_Delta=1000*((para_Yaw_Gyro_Controller->Expect-Last_Yaw_Gyro_Control_Expect)
                                  /para_Yaw_Gyro_Controller->PID_Controller_Dt);

    para_Yaw_Gyro_Controller->Control_OutPut+=Yaw_Feedforward_Kp*para_Yaw_Gyro_Controller->Expect
                                +Yaw_Feedforward_Kd*Yaw_Gyro_Control_Expect_Delta;//ƫ����ǰ������
    para_Yaw_Gyro_Controller->Control_OutPut=constrain_float(para_Yaw_Gyro_Controller->Control_OutPut,
                                                         -para_Yaw_Gyro_Controller->Control_OutPut_Limit,
                                                            para_Yaw_Gyro_Controller->Control_OutPut_Limit);
    Last_Yaw_Gyro_Control_Expect=para_Yaw_Gyro_Controller->Expect;

    /*******ƫ�������쳣����жϣ���ƫ���������ܴ�ʱ��ƫ�����ٶȺ�С�����ʱΪǿ�������š����ŵص�******************************/
//    if(ABS(para_Yaw_Gyro_Controller->Control_OutPut)>para_Yaw_Gyro_Controller->Control_OutPut_Limit/2//ƫ�����������Խϴ�
//       &&ABS(YAW_GYRO)<=30.0)//ƫ�����ٶ���Ժ�С
//    {
//        Yaw_Control_Fault_Cnt++;
//        if(Yaw_Control_Fault_Cnt>=500) Yaw_Control_Fault_Cnt=500;
//    }
//    else Yaw_Control_Fault_Cnt/=2;//�����㣬����������0
//
//    if(Yaw_Control_Fault_Cnt>=400)//����5ms*400=2S,���⴦��
//    {
//        PID_Integrate_Reset(para_Yaw_Gyro_Controller);//���ƫ�����ٶȿ��ƵĻ���
//        PID_Integrate_Reset(para_Yaw_Angle_Controller);//���ƫ���ǿ��ƵĻ���
//        Total_Controller.Yaw_Angle_Control.Expect=YAW;//����ǰƫ���ǣ���Ϊ����ƫ����
//        Yaw_Control_Fault_Cnt=0;
//    }
    /*******ƫ�������쳣�������******************************/
}






























