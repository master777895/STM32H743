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
 * @brief PID初始化
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
 * @brief 初始化PID控制器的参数
 * @param para_controller
 */
void PID_init_parameter(PID_Controller* para_controller, PID_control_type para_label)
{
    para_controller->Err_Limit_Flag         =(uint8_t)(PID_CONTROL_UNIT[para_label][0]);//1偏差限幅标志
    para_controller->Integrate_Limit_Flag   =(uint8_t)(PID_CONTROL_UNIT[para_label][1]);//2积分限幅标志
    para_controller->Integrate_Separation_Flag=(uint8_t)(PID_CONTROL_UNIT[para_label][2]);//3积分分离标志
    para_controller->Expect                 =PID_CONTROL_UNIT[para_label][3];//4期望
    para_controller->FeedBack               =PID_CONTROL_UNIT[para_label][4];//5反馈值
    para_controller->Err                    =PID_CONTROL_UNIT[para_label][5];//6偏差
    para_controller->Last_Err               =PID_CONTROL_UNIT[para_label][6];//7上次偏差
    para_controller->Err_Max                =PID_CONTROL_UNIT[para_label][7];//8偏差限幅值
    para_controller->Integrate_Separation_Err=PID_CONTROL_UNIT[para_label][8];//9积分分离偏差值
    para_controller->Integrate              =PID_CONTROL_UNIT[para_label][9];//10积分值
    para_controller->Integrate_Max          =PID_CONTROL_UNIT[para_label][10];//11积分限幅值
    para_controller->Kp                     =PID_CONTROL_UNIT[para_label][11];//12控制参数Kp
    para_controller->Ki                     =PID_CONTROL_UNIT[para_label][12];//13控制参数Ki
    para_controller->Kd                     =PID_CONTROL_UNIT[para_label][13];//14控制参数Ki
    para_controller->Control_OutPut         =PID_CONTROL_UNIT[para_label][14];//15控制器总输出
    para_controller->Last_Control_OutPut    =PID_CONTROL_UNIT[para_label][15];//16上次控制器总输出
    para_controller->Control_OutPut_Limit   =PID_CONTROL_UNIT[para_label][16];//17限幅
    para_controller->Scale_Kp               =PID_CONTROL_UNIT[para_label][17];
    para_controller->Scale_Ki               =PID_CONTROL_UNIT[para_label][18];
    para_controller->Scale_Kd               =PID_CONTROL_UNIT[para_label][19];
    para_controller->PID_type               =PID_CONTROL_UNIT[para_label][20];//控制器类型

    para_controller->PID_Controller_Dt = 0.005f;///控制周期

    ///判定是否同步
    if(para_label != (uint32_t)PID_CONTROL_UNIT[para_label][20])
    {
        Error_Handler();
    }
}

float PID_Control(PID_Controller *pram_controler)
{
    const float controller_dt=0.005f;

    /*******偏差计算*********************/
    pram_controler->Last_Err =pram_controler->Err;//保存上次偏差
    pram_controler->Err      =pram_controler->Expect-pram_controler->FeedBack;//期望减去反馈得到偏差

    if(pram_controler->PID_type == _PID_YAW_ANGLE)
    {
        /***********************偏航角偏差超过+-180处理*****************************/
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

    if(pram_controler->Err_Limit_Flag==1)///偏差限幅度标志位
    {
        if(pram_controler->Err>=pram_controler->Err_Max)
            pram_controler->Err= pram_controler->Err_Max;
        if(pram_controler->Err<=-pram_controler->Err_Max)
            pram_controler->Err=-pram_controler->Err_Max;
    }

    /*******积分计算*********************/
    if(pram_controler->Integrate_Separation_Flag==1)///积分分离标志位
    {
        if(ABS(pram_controler->Err)<=pram_controler->Integrate_Separation_Err)
            pram_controler->Integrate+=pram_controler->Scale_Ki*pram_controler->Ki*
                                        pram_controler->Err*controller_dt;
    }
    else
    {
        pram_controler->Integrate+=pram_controler->Scale_Ki*pram_controler->Ki*pram_controler->Err*controller_dt;
    }

    /*******积分限幅*********************/
    if(pram_controler->Integrate_Limit_Flag==1)//积分限制幅度标志
    {
        if(pram_controler->Integrate>=pram_controler->Integrate_Max)
            pram_controler->Integrate=pram_controler->Integrate_Max;
        if(pram_controler->Integrate<=-pram_controler->Integrate_Max)
            pram_controler->Integrate=-pram_controler->Integrate_Max ;
    }

    /*******总输出计算*********************/
    pram_controler->Last_Control_OutPut=pram_controler->Control_OutPut;//输出值递推
    pram_controler->Control_OutPut=pram_controler->Scale_Kp*pram_controler->Kp*pram_controler->Err//比例
                                   +pram_controler->Integrate//积分
                                   +pram_controler->Kd*(pram_controler->Err-pram_controler->Last_Err);//微分

    /*******总输出限幅*********************/
    if(pram_controler->Control_OutPut>=pram_controler->Control_OutPut_Limit)
        pram_controler->Control_OutPut=pram_controler->Control_OutPut_Limit;
    if(pram_controler->Control_OutPut<=-pram_controler->Control_OutPut_Limit)
        pram_controler->Control_OutPut=-pram_controler->Control_OutPut_Limit;

    /*******返回总输出*********************/
    return pram_controler->Control_OutPut;
}


/**
 * @brief 内环角速度控制：微分参数动态调整
 * @param Controler
 * @return
 */
float PID_Control_Div_LPF_For_Gyro(PID_Controller *Controler)
{
    int16_t i=0;

    const float controller_dt=0.005f;

    /*******偏差计算*********************/
    Controler->Pre_Last_Err=Controler->Last_Err;//上上次偏差
    Controler->Last_Err=Controler->Err;//保存上次偏差
    Controler->Err=Controler->Expect-Controler->FeedBack;//期望减去反馈得到偏差
    Controler->Dis_Err=(Controler->Err-Controler->Pre_Last_Err);//间隔了一次采样的微分
    /******************************************/
    //均值滤波，保证得到数据不跳变，避免期望阶跃时，微分输出异常
//  float tempa,tempb,tempc,max,min;//用于防跳变滤波
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
    for(i=4;i>0;i--)//数字低通后微分项保存
    {
        Controler->Dis_Error_History[i]=Controler->Dis_Error_History[i-1];
    }
    Controler->Dis_Error_History[0]=Control_Device_LPF(Controler->Dis_Err,
                                                       &Controler->Control_Device_LPF_Buffer,
                                                       &Control_Device_Div_LPF_Parameter_For_Gyro);//巴特沃斯低通后得到的微分项,30hz
    Controler->Dis_Error_History[0]=constrain_float(Controler->Dis_Error_History[0],-500,500);//微分项限幅
    Controler->Adaptable_Kd=Controler->Kd*(1+ABS(Controler->Dis_Error_History[0])/500.0f);//自适应微分参数

    if(Controler->Err_Limit_Flag==1)//偏差限幅度标志位
    {
        if(Controler->Err>=Controler->Err_Max)   Controler->Err= Controler->Err_Max;
        if(Controler->Err<=-Controler->Err_Max)  Controler->Err=-Controler->Err_Max;
    }

    /*******积分计算*********************/
    if(Controler->Integrate_Separation_Flag==1)//积分分离标志位
    {
        if(ABS(Controler->Err)<=Controler->Integrate_Separation_Err)
            Controler->Integrate+=Controler->Scale_Ki*Controler->Ki*Controler->Err*controller_dt;
    }
    else
    {
        Controler->Integrate+=Controler->Scale_Ki*Controler->Ki*Controler->Err*controller_dt;
    }

    /*******积分限幅*********************/
    if(Controler->Integrate_Limit_Flag==1)//积分限制幅度标志
    {
        if(Controler->Integrate>=Controler->Integrate_Max)
            Controler->Integrate=Controler->Integrate_Max;
        if(Controler->Integrate<=-Controler->Integrate_Max)
            Controler->Integrate=-Controler->Integrate_Max ;
    }
    /*******总输出计算*********************/
    Controler->Last_Control_OutPut=Controler->Control_OutPut;//输出值递推
    Controler->Control_OutPut=Controler->Scale_Kp*Controler->Kp*Controler->Err//比例
                              +Controler->Integrate//积分
                              +Controler->Adaptable_Kd*Controler->Dis_Err;//微分



    //+Controler->Adaptable_Kd*Controler->Dis_Error_History[0];//微分项来源于巴特沃斯低通滤波器
    /*******总输出限幅*********************/
    if(Controler->Control_OutPut>=Controler->Control_OutPut_Limit)
        Controler->Control_OutPut=Controler->Control_OutPut_Limit;
    if(Controler->Control_OutPut<=-Controler->Control_OutPut_Limit)
        Controler->Control_OutPut=-Controler->Control_OutPut_Limit;
    /*******返回总输出*********************/
    return Controler->Control_OutPut;
}


float PID_Control_Div_LPF(PID_Controller *Controler)
{
    int16_t  i=0;

    float controller_dt=0.005f;

    /*******偏差计算*********************/
    Controler->Last_Err=Controler->Err;//保存上次偏差
    Controler->Err=Controler->Expect-Controler->FeedBack;//期望减去反馈得到偏差
    Controler->Dis_Err=Controler->Err-Controler->Last_Err;//原始微分

    /******************************************/
    //均值滤波，保证得到数据不跳变，避免期望阶跃时，微分输出异常
//  float tempa,tempb,tempc,max,min;//用于防跳变滤波
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

    for(i=4;i>0;i--)//数字低通后微分项保存
    {
        Controler->Dis_Error_History[i]=Controler->Dis_Error_History[i-1];
    }
    Controler->Dis_Error_History[0]=Control_Device_LPF(Controler->Dis_Err,
                                                       &Controler->Control_Device_LPF_Buffer,
                                                       &Control_Device_Div_LPF_Parameter);//巴特沃斯低通后得到的微分项,20hz

    if(Controler->Err_Limit_Flag==1)//偏差限幅度标志位
    {
        if(Controler->Err>=Controler->Err_Max)   Controler->Err= Controler->Err_Max;
        if(Controler->Err<=-Controler->Err_Max)  Controler->Err=-Controler->Err_Max;
    }
    /*******积分计算*********************/
    if(Controler->Integrate_Separation_Flag==1)//积分分离标志位
    {
        if(ABS(Controler->Err)<=Controler->Integrate_Separation_Err)
            Controler->Integrate+=Controler->Scale_Ki*Controler->Ki*Controler->Err*controller_dt;
    }
    else
    {
        Controler->Integrate+=Controler->Scale_Ki*Controler->Ki*Controler->Err*controller_dt;
    }
    /*******积分限幅*********************/
    if(Controler->Integrate_Limit_Flag==1)//积分限制幅度标志
    {
        if(Controler->Integrate>=Controler->Integrate_Max)
            Controler->Integrate=Controler->Integrate_Max;
        if(Controler->Integrate<=-Controler->Integrate_Max)
            Controler->Integrate=-Controler->Integrate_Max ;
    }
    /*******总输出计算*********************/
    Controler->Last_Control_OutPut=Controler->Control_OutPut;//输出值递推
    Controler->Control_OutPut=Controler->Scale_Kp*Controler->Kp*Controler->Err//比例
                              +Controler->Integrate//积分
                              //+Controler->Kd*Controler->Dis_Err;//微分
                              +Controler->Kd*Controler->Dis_Error_History[0];//微分项来源于巴特沃斯低通滤波器
    /*******总输出限幅*********************/
    if(Controler->Control_OutPut>=Controler->Control_OutPut_Limit)
        Controler->Control_OutPut=Controler->Control_OutPut_Limit;
    if(Controler->Control_OutPut<=-Controler->Control_OutPut_Limit)
        Controler->Control_OutPut=-Controler->Control_OutPut_Limit;
    /*******返回总输出*********************/
    return Controler->Control_OutPut;
}



/** 加速度计Butterworth滤波 */
float Control_Device_LPF(float curr_inputer,Butter_BufferData *Buffer,Butter_Parameter *Parameter)
{
    /* 获取最新x(n) */
    Buffer->Input_Butter[2]=curr_inputer;
    /* Butterworth滤波 */
    Buffer->Output_Butter[2]=
            Parameter->b[0] * Buffer->Input_Butter[2]
            +Parameter->b[1] * Buffer->Input_Butter[1]
            +Parameter->b[2] * Buffer->Input_Butter[0]
            -Parameter->a[1] * Buffer->Output_Butter[1]
            -Parameter->a[2] * Buffer->Output_Butter[0];
    /* x(n) 序列保存 */
    Buffer->Input_Butter[0]=Buffer->Input_Butter[1];
    Buffer->Input_Butter[1]=Buffer->Input_Butter[2];
    /* y(n) 序列保存 */
    Buffer->Output_Butter[0]=Buffer->Output_Butter[1];
    Buffer->Output_Butter[1]=Buffer->Output_Butter[2];
    return (Buffer->Output_Butter[2]);
}

void  PID_Integrate_Reset(PID_Controller *para_Controller)
{
    para_Controller->Integrate=0.0f;
}

/**
 * @brief 起飞前reset积分
 * @param para_AllController
 */
void Preflight_ResetIntegrate(ALL_PID_CONTROLLER* para_AllController)
{
    ///起飞前屏蔽积分
    PID_Integrate_Reset(&para_AllController->ROLL_GYRO_CONTROLLER);
    PID_Integrate_Reset(&para_AllController->PITCH_GYRO_CONTROLLER);
    PID_Integrate_Reset(&para_AllController->YAW_GYRO_CONTROLLER);

    PID_Integrate_Reset(&para_AllController->ROLL_ANGLE_CONTROLLER);
    PID_Integrate_Reset(&para_AllController->PITCH_ANGLE_CONTROLLER);
    PID_Integrate_Reset(&para_AllController->YAW_ANGLE_CONTROLLER);



//    PID_Integrate_Reset(&Total_Controller.Longitude_Speed_Control);//位置控制速度环积分项
//    PID_Integrate_Reset(&Total_Controller.Latitude_Speed_Control);
//    OpticalFlow_Ctrl_Reset();
    //OpticalFlow_SINS_Reset();
}









































