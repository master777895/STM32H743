//
// Created by Gideon on 2021/8/29.
//
#include "main.h"
#include "PID.h"




static Butter_Parameter Control_Device_Div_LPF_Parameter_For_Gyro;
static Butter_Parameter Control_Device_Div_LPF_Parameter;
static Butter_Parameter Control_Device_Err_LPF_Parameter;
static Butter_Parameter Control_Device_SDK_Err_LPF_Parameter;

/**
 * @brief PID��ʼ��
 */
void PID_Init(void)
{
    PID_init_parameter(&CONTROLLER_BUS.PITCH_ANGLE_CONTROLLER, _PID_PITCH_ANGLE);
    PID_init_parameter(&CONTROLLER_BUS.ROLL_ANGLE_CONTROLLER, _PID_ROLL_ANGLE);
    PID_init_parameter(&CONTROLLER_BUS.YAW_ANGLE_CONTROLLER, _PID_YAW_ANGLE);

    PID_init_parameter(&CONTROLLER_BUS.PITCH_GYRO_CONTROLLER, _PID_PITCH_GYRO);
    PID_init_parameter(&CONTROLLER_BUS.ROLL_GYRO_CONTROLLER, _PID_ROLL_GYRO);
    PID_init_parameter(&CONTROLLER_BUS.YAW_GYRO_CONTROLLER, _PID_YAW_GYRO);


    Set_Cutoff_Frequency(50, 5 ,&Control_Device_SDK_Err_LPF_Parameter);//100 5
    Set_Cutoff_Frequency(Sampling_Freq, 5 ,&Control_Device_Err_LPF_Parameter);//5
    Set_Cutoff_Frequency(Sampling_Freq, 20,&Control_Device_Div_LPF_Parameter);
    Set_Cutoff_Frequency(Sampling_Freq, 30,&Control_Device_Div_LPF_Parameter_For_Gyro);//30
}



/**
 * @brief ��ʼ��PID�������Ĳ���
 * @param para_controller
 */
void PID_init_parameter(PID_Controller* para_controller, PID_control_type para_label)
{
    para_controller->Err_Limit_Flag         =(uint8_t)(PID_CONTROL_UNIT[para_label][0]);//1ƫ���޷���־
    para_controller->Integrate_Limit_Flag   =(uint8_t)(PID_CONTROL_UNIT[para_label][1]);//2�����޷���־
    para_controller->Integrate_Separation_Flag=(uint8_t)(PID_CONTROL_UNIT[para_label][2]);//3���ַ����־
    para_controller->Expect                 =PID_CONTROL_UNIT[para_label][3];//4����
    para_controller->FeedBack               =PID_CONTROL_UNIT[para_label][4];//5����ֵ
    para_controller->Err                    =PID_CONTROL_UNIT[para_label][5];//6ƫ��
    para_controller->Last_Err               =PID_CONTROL_UNIT[para_label][6];//7�ϴ�ƫ��
    para_controller->Err_Max                =PID_CONTROL_UNIT[para_label][7];//8ƫ���޷�ֵ
    para_controller->Integrate_Separation_Err=PID_CONTROL_UNIT[para_label][8];//9���ַ���ƫ��ֵ
    para_controller->Integrate              =PID_CONTROL_UNIT[para_label][9];//10����ֵ
    para_controller->Integrate_Max          =PID_CONTROL_UNIT[para_label][10];//11�����޷�ֵ
    para_controller->Kp                     =PID_CONTROL_UNIT[para_label][11];//12���Ʋ���Kp
    para_controller->Ki                     =PID_CONTROL_UNIT[para_label][12];//13���Ʋ���Ki
    para_controller->Kd                     =PID_CONTROL_UNIT[para_label][13];//14���Ʋ���Ki
    para_controller->Control_OutPut         =PID_CONTROL_UNIT[para_label][14];//15�����������
    para_controller->Last_Control_OutPut    =PID_CONTROL_UNIT[para_label][15];//16�ϴο����������
    para_controller->Control_OutPut_Limit   =PID_CONTROL_UNIT[para_label][16];//17�޷�
    para_controller->Scale_Kp               =PID_CONTROL_UNIT[para_label][17];
    para_controller->Scale_Ki               =PID_CONTROL_UNIT[para_label][18];
    para_controller->Scale_Kd               =PID_CONTROL_UNIT[para_label][19];
    para_controller->PID_type               =PID_CONTROL_UNIT[para_label][20];//����������

    para_controller->PID_Controller_Dt = 0.005f;///��������

    ///�ж��Ƿ�ͬ��
    if(para_label != (uint32_t)PID_CONTROL_UNIT[para_label][20])
    {
        Error_Handler();
    }
}

float PID_Control(PID_Controller *pram_controler)
{
    const float controller_dt=0.005f;

    /*******ƫ�����*********************/
    pram_controler->Last_Err =pram_controler->Err;//�����ϴ�ƫ��
    pram_controler->Err      =pram_controler->Expect-pram_controler->FeedBack;//������ȥ�����õ�ƫ��

    if(pram_controler->PID_type == _PID_YAW_ANGLE)
    {
        /***********************ƫ����ƫ���+-180����*****************************/
//        if(pram_controler->Err<-180)
//            pram_controler->Err=pram_controler->Err+360;
//        if(pram_controler->Err>180)
//            pram_controler->Err=pram_controler->Err-360;
//
//        if(pram_controler->Err<-360)
//        {
//
//        }
    }

    if(pram_controler->Err_Limit_Flag==1)///ƫ���޷��ȱ�־λ
    {
        if(pram_controler->Err>=pram_controler->Err_Max)
            pram_controler->Err= pram_controler->Err_Max;
        if(pram_controler->Err<=-pram_controler->Err_Max)
            pram_controler->Err=-pram_controler->Err_Max;
    }

    /*******���ּ���*********************/
    if(pram_controler->Integrate_Separation_Flag==1)///���ַ����־λ
    {
        if(ABS(pram_controler->Err)<=pram_controler->Integrate_Separation_Err)
            pram_controler->Integrate+=pram_controler->Scale_Ki*pram_controler->Ki*
                                        pram_controler->Err*controller_dt;
    }
    else
    {
        pram_controler->Integrate+=pram_controler->Scale_Ki*pram_controler->Ki*pram_controler->Err*controller_dt;
    }

    /*******�����޷�*********************/
    if(pram_controler->Integrate_Limit_Flag==1)//�������Ʒ��ȱ�־
    {
        if(pram_controler->Integrate>=pram_controler->Integrate_Max)
            pram_controler->Integrate=pram_controler->Integrate_Max;
        if(pram_controler->Integrate<=-pram_controler->Integrate_Max)
            pram_controler->Integrate=-pram_controler->Integrate_Max ;
    }

    /*******���������*********************/
    pram_controler->Last_Control_OutPut=pram_controler->Control_OutPut;//���ֵ����
    pram_controler->Control_OutPut=pram_controler->Scale_Kp*pram_controler->Kp*pram_controler->Err//����
                                   +pram_controler->Integrate//����
                                   +pram_controler->Kd*(pram_controler->Err-pram_controler->Last_Err);//΢��

    /*******������޷�*********************/
    if(pram_controler->Control_OutPut>=pram_controler->Control_OutPut_Limit)
        pram_controler->Control_OutPut=pram_controler->Control_OutPut_Limit;
    if(pram_controler->Control_OutPut<=-pram_controler->Control_OutPut_Limit)
        pram_controler->Control_OutPut=-pram_controler->Control_OutPut_Limit;

    /*******���������*********************/
    return pram_controler->Control_OutPut;
}


/**
 * @brief �ڻ����ٶȿ��ƣ�΢�ֲ�����̬����
 * @param Controler
 * @return
 */
float PID_Control_Div_LPF_For_Gyro(PID_Controller *Controler)
{
    int16_t i=0;

    const float controller_dt=0.005f;

    /*******ƫ�����*********************/
    Controler->Pre_Last_Err=Controler->Last_Err;//���ϴ�ƫ��
    Controler->Last_Err=Controler->Err;//�����ϴ�ƫ��
    Controler->Err=Controler->Expect-Controler->FeedBack;//������ȥ�����õ�ƫ��
    Controler->Dis_Err=(Controler->Err-Controler->Pre_Last_Err);//�����һ�β�����΢��
    /******************************************/
    //��ֵ�˲�����֤�õ����ݲ����䣬����������Ծʱ��΢������쳣
//  float tempa,tempb,tempc,max,min;//���ڷ������˲�
//	tempa=Controler->Pre_Last_Dis_Err_LPF;
//  tempb=Controler->Last_Dis_Err_LPF;
//  tempc=Controler->Dis_Err;
//  max = tempa > tempb ? tempa:tempb;
//  max = max > tempc ? max:tempc;
//  min = tempa < tempb ? tempa:tempb;
//  min = min < tempc ? min:tempc;
//  if(tempa > min && tempa < max)    Controler->Dis_Err = tempa;
//  if(tempb > min  && tempb < max )  Controler->Dis_Err = tempb;
//  if(tempc > min  &&  tempc < max)  Controler->Dis_Err = tempc;
//  Controler->Pre_Last_Dis_Err_LPF = Controler->Last_Dis_Err_LPF;
//  Controler->Last_Dis_Err_LPF = Controler->Dis_Err;
    /*****************************************/
    for(i=4;i>0;i--)//���ֵ�ͨ��΢�����
    {
        Controler->Dis_Error_History[i]=Controler->Dis_Error_History[i-1];
    }
    Controler->Dis_Error_History[0]=Control_Device_LPF(Controler->Dis_Err,
                                                       &Controler->Control_Device_LPF_Buffer,
                                                       &Control_Device_Div_LPF_Parameter_For_Gyro);//������˹��ͨ��õ���΢����,30hz
    Controler->Dis_Error_History[0]=constrain_float(Controler->Dis_Error_History[0],-500,500);//΢�����޷�
    Controler->Adaptable_Kd=Controler->Kd*(1+ABS(Controler->Dis_Error_History[0])/500.0f);//����Ӧ΢�ֲ���

    if(Controler->Err_Limit_Flag==1)//ƫ���޷��ȱ�־λ
    {
        if(Controler->Err>=Controler->Err_Max)   Controler->Err= Controler->Err_Max;
        if(Controler->Err<=-Controler->Err_Max)  Controler->Err=-Controler->Err_Max;
    }

    /*******���ּ���*********************/
    if(Controler->Integrate_Separation_Flag==1)//���ַ����־λ
    {
        if(ABS(Controler->Err)<=Controler->Integrate_Separation_Err)
            Controler->Integrate+=Controler->Scale_Ki*Controler->Ki*Controler->Err*controller_dt;
    }
    else
    {
        Controler->Integrate+=Controler->Scale_Ki*Controler->Ki*Controler->Err*controller_dt;
    }

    /*******�����޷�*********************/
    if(Controler->Integrate_Limit_Flag==1)//�������Ʒ��ȱ�־
    {
        if(Controler->Integrate>=Controler->Integrate_Max)
            Controler->Integrate=Controler->Integrate_Max;
        if(Controler->Integrate<=-Controler->Integrate_Max)
            Controler->Integrate=-Controler->Integrate_Max ;
    }
    /*******���������*********************/
    Controler->Last_Control_OutPut=Controler->Control_OutPut;//���ֵ����
    Controler->Control_OutPut=Controler->Scale_Kp*Controler->Kp*Controler->Err//����
                              +Controler->Integrate//����
                              +Controler->Adaptable_Kd*Controler->Dis_Err;//΢��



    //+Controler->Adaptable_Kd*Controler->Dis_Error_History[0];//΢������Դ�ڰ�����˹��ͨ�˲���
    /*******������޷�*********************/
    if(Controler->Control_OutPut>=Controler->Control_OutPut_Limit)
        Controler->Control_OutPut=Controler->Control_OutPut_Limit;
    if(Controler->Control_OutPut<=-Controler->Control_OutPut_Limit)
        Controler->Control_OutPut=-Controler->Control_OutPut_Limit;
    /*******���������*********************/
    return Controler->Control_OutPut;
}


float PID_Control_Div_LPF(PID_Controller *Controler)
{
    int16_t  i=0;

    float controller_dt=0.005f;

    /*******ƫ�����*********************/
    Controler->Last_Err=Controler->Err;//�����ϴ�ƫ��
    Controler->Err=Controler->Expect-Controler->FeedBack;//������ȥ�����õ�ƫ��
    Controler->Dis_Err=Controler->Err-Controler->Last_Err;//ԭʼ΢��

    /******************************************/
    //��ֵ�˲�����֤�õ����ݲ����䣬����������Ծʱ��΢������쳣
//  float tempa,tempb,tempc,max,min;//���ڷ������˲�
//  tempa=Controler->Pre_Last_Dis_Err_LPF;
//  tempb=Controler->Last_Dis_Err_LPF;
//  tempc=Controler->Dis_Err;
//  max = tempa > tempb ? tempa:tempb;
//  max = max > tempc ? max:tempc;
//  min = tempa < tempb ? tempa:tempb;
//  min = min < tempc ? min:tempc;
//  if(tempa > min && tempa < max)    Controler->Dis_Err = tempa;
//  if(tempb > min  && tempb < max )  Controler->Dis_Err = tempb;
//  if(tempc > min  &&  tempc < max)  Controler->Dis_Err = tempc;
//  Controler->Pre_Last_Dis_Err_LPF = Controler->Last_Dis_Err_LPF;
//  Controler->Last_Dis_Err_LPF = Controler->Dis_Err;
    /*****************************************/

    for(i=4;i>0;i--)//���ֵ�ͨ��΢�����
    {
        Controler->Dis_Error_History[i]=Controler->Dis_Error_History[i-1];
    }
    Controler->Dis_Error_History[0]=Control_Device_LPF(Controler->Dis_Err,
                                                       &Controler->Control_Device_LPF_Buffer,
                                                       &Control_Device_Div_LPF_Parameter);//������˹��ͨ��õ���΢����,20hz

    if(Controler->Err_Limit_Flag==1)//ƫ���޷��ȱ�־λ
    {
        if(Controler->Err>=Controler->Err_Max)   Controler->Err= Controler->Err_Max;
        if(Controler->Err<=-Controler->Err_Max)  Controler->Err=-Controler->Err_Max;
    }
    /*******���ּ���*********************/
    if(Controler->Integrate_Separation_Flag==1)//���ַ����־λ
    {
        if(ABS(Controler->Err)<=Controler->Integrate_Separation_Err)
            Controler->Integrate+=Controler->Scale_Ki*Controler->Ki*Controler->Err*controller_dt;
    }
    else
    {
        Controler->Integrate+=Controler->Scale_Ki*Controler->Ki*Controler->Err*controller_dt;
    }
    /*******�����޷�*********************/
    if(Controler->Integrate_Limit_Flag==1)//�������Ʒ��ȱ�־
    {
        if(Controler->Integrate>=Controler->Integrate_Max)
            Controler->Integrate=Controler->Integrate_Max;
        if(Controler->Integrate<=-Controler->Integrate_Max)
            Controler->Integrate=-Controler->Integrate_Max ;
    }
    /*******���������*********************/
    Controler->Last_Control_OutPut=Controler->Control_OutPut;//���ֵ����
    Controler->Control_OutPut=Controler->Scale_Kp*Controler->Kp*Controler->Err//����
                              +Controler->Integrate//����
                              //+Controler->Kd*Controler->Dis_Err;//΢��
                              +Controler->Kd*Controler->Dis_Error_History[0];//΢������Դ�ڰ�����˹��ͨ�˲���
    /*******������޷�*********************/
    if(Controler->Control_OutPut>=Controler->Control_OutPut_Limit)
        Controler->Control_OutPut=Controler->Control_OutPut_Limit;
    if(Controler->Control_OutPut<=-Controler->Control_OutPut_Limit)
        Controler->Control_OutPut=-Controler->Control_OutPut_Limit;
    /*******���������*********************/
    return Controler->Control_OutPut;
}



/** ���ٶȼ�Butterworth�˲� */
float Control_Device_LPF(float curr_inputer,Butter_BufferData *Buffer,Butter_Parameter *Parameter)
{
    /* ��ȡ����x(n) */
    Buffer->Input_Butter[2]=curr_inputer;
    /* Butterworth�˲� */
    Buffer->Output_Butter[2]=
            Parameter->b[0] * Buffer->Input_Butter[2]
            +Parameter->b[1] * Buffer->Input_Butter[1]
            +Parameter->b[2] * Buffer->Input_Butter[0]
            -Parameter->a[1] * Buffer->Output_Butter[1]
            -Parameter->a[2] * Buffer->Output_Butter[0];
    /* x(n) ���б��� */
    Buffer->Input_Butter[0]=Buffer->Input_Butter[1];
    Buffer->Input_Butter[1]=Buffer->Input_Butter[2];
    /* y(n) ���б��� */
    Buffer->Output_Butter[0]=Buffer->Output_Butter[1];
    Buffer->Output_Butter[1]=Buffer->Output_Butter[2];
    return (Buffer->Output_Butter[2]);
}

void  PID_Integrate_Reset(PID_Controller *para_Controller)
{
    para_Controller->Integrate=0.0f;
}

/**
 * @brief ���ǰreset����
 * @param para_AllController
 */
void Preflight_ResetIntegrate(ALL_PID_CONTROLLER* para_AllController)
{
    ///���ǰ���λ���
    PID_Integrate_Reset(&para_AllController->ROLL_GYRO_CONTROLLER);
    PID_Integrate_Reset(&para_AllController->PITCH_GYRO_CONTROLLER);
    PID_Integrate_Reset(&para_AllController->YAW_GYRO_CONTROLLER);

    PID_Integrate_Reset(&para_AllController->ROLL_ANGLE_CONTROLLER);
    PID_Integrate_Reset(&para_AllController->PITCH_ANGLE_CONTROLLER);
    PID_Integrate_Reset(&para_AllController->YAW_ANGLE_CONTROLLER);



//    PID_Integrate_Reset(&Total_Controller.Longitude_Speed_Control);//λ�ÿ����ٶȻ�������
//    PID_Integrate_Reset(&Total_Controller.Latitude_Speed_Control);
//    OpticalFlow_Ctrl_Reset();
    //OpticalFlow_SINS_Reset();
}









































