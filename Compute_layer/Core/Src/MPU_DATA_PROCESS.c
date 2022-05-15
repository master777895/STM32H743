//
// Created by Gideon on 2021/8/22.
//

#include "main.h"

static Vector3f Accel_Quantizer(const Vector3f* imudata);
static Vector3f Accel_Filter(const Vector3f* Accel);
static Vector3f Gyro_Filer(const Vector3f* gyro);
static Vector3f Gyro_Quantizer(const Vector3f* imudata);
static Vector3f Accel_calibration(const Vector3f* parm_Accel);
static Vector3f Gyro_calibration(const Vector3f* parm_Gyro);

//200_30z
Butter_Parameter Accel_Parameter={
        1,  -0.7477891782585,    0.272214937925,
        0.1311064399166,   0.2622128798333,   0.1311064399166
};
Butter_Parameter Gyro_Parameter={
        //200hz---51hz
        1,  0.03680751639284,   0.1718123812701,
        0.3021549744157,   0.6043099488315,   0.3021549744157
};
Butter_Parameter Butter_1HZ_Parameter_Acce={
        //200hz---1hz
        1,   -1.955578240315,   0.9565436765112,
        0.000241359049042, 0.000482718098084, 0.000241359049042
};

/**
 *
 * @return 两个Vector3f的数，并且单位是：cm/s^2 和 degree/s
 */
Vector3f* GET_MPU_DATA(void)
{
    static Vector3f retu[2]={{0}};
    static Vector3f retu_null[2] = {{0}};//无效返回值，始终为0
    static uint8_t calibrated_flag=0;
    static uint8_t start_colibrate_flag=0;
    static uint32_t time_cnt=0, time_cnt2=0;

    static Vector3f temp_accel={0}, temp_gyro={0};
    static Vector3f Calibrated_Accel, Calibrated_Gyro;


    static Vector3f Accel0,Accel1,Accel2,Accel3;
    static Vector3f Gyro0,Gyro1,Gyro2,Gyro3;
    float_t temperature=0;

    /** 读取数据 **/
    Accel0 = MPU6050_Read_Accel();
    Gyro0 = MPU6050_Read_Gyro();
    temperature = MPU6050_Read_Temp();

    /** 修正坐标极性 非常重要*/
    Accel0.x = Accel0.x;
    Accel0.y = Accel0.y;
    Accel0.z = -Accel0.z;

    Gyro0.x = -Gyro0.x;
    Gyro0.y = -Gyro0.y;
    Gyro0.z = Gyro0.z;

//    LOG("%f, %f, %f || %f, %f, %f\n", \
//                Accel0.x,Accel0.y,Accel0.z, \
//                Gyro0.x,Gyro0.y,Gyro0.z);

/** 加速度处理 **/
    Accel1 = Accel_Filter(&Accel0);///升级
/** 角速度处理 **/
    Gyro1 = Gyro_Filer(&Gyro0);

//    LOG("%f, %f, %f || %f, %f, %f\n", \
//                Accel1.x,Accel1.y,Accel1.z, \
//                Gyro1.x,Gyro1.y,Gyro1.z);

    /** 获取修正值 */
    if(calibrated_flag == 0)
    {
        time_cnt++;
        if(time_cnt >= 3000/UNTIL_DELAY)//3s过后,结束矫正
        {
            calibrated_flag = 1;
        }

        time_cnt2++;
        if(time_cnt >= 100/UNTIL_DELAY)//100ms后面的数据才要.
        {
            time_cnt2--;///防止数据溢出

            temp_accel = Accel_calibration(&Accel1); //加速度校正
            temp_gyro = Gyro_calibration(&Gyro1); //角速度校正
        }


        return retu_null;
    }
    else if(calibrated_flag == 1)//校正好了
    {
        Calibrated_Accel = temp_accel;
        Calibrated_Gyro = temp_gyro;

        /** 单位量化 */
        Accel2 = Accel_Quantizer(&Accel1);
        Gyro2 = Gyro_Quantizer(&Gyro1);

        /** 修正 ,后面的数字是人工看出来的，后面需要优化*/
        Accel2.x = Accel2.x - Calibrated_Accel.x;
        Accel2.y = Accel2.y - Calibrated_Accel.y;
        Accel2.z = Accel2.z - Calibrated_Accel.z;

        Gyro2.x = Gyro2.x - Calibrated_Gyro.x ;
        Gyro2.y = Gyro2.y - Calibrated_Gyro.y ;
        Gyro2.z = Gyro2.z - Calibrated_Gyro.z ;

//        LOG("%f, %f, %f || %f, %f, %f\n", \
//                Calibrated_Accel.x,Calibrated_Accel.y, Calibrated_Accel.z, \
//                Calibrated_Gyro.x,Calibrated_Gyro.y,Calibrated_Gyro.z);

//        LOG(" %f, %f, %f, %f, %f, %f\n", \
//                Accel2.x,Accel2.y,Accel2.z, \
//                Gyro2.x,Gyro2.y,Gyro2.z);

        /** 返回值 **/
        memcpy(retu, &Accel2, 1*sizeof(Vector3f));
        memcpy(retu+1, &Gyro2, 1*sizeof(Vector3f));

        return retu;
    }
    return retu_null;
}



Vector3f Gyro_Filer(const Vector3f* gyro)
{
    static Butter_BufferData Gyro_BufferData_BPF[3];
    static Vector3f retu;

    retu.x=Butterworth_Filter(gyro->x,&Gyro_BufferData_BPF[0],&Bandstop_Filter_Parameter_30_98);
    retu.y=Butterworth_Filter(gyro->y,&Gyro_BufferData_BPF[1],&Bandstop_Filter_Parameter_30_98);
    retu.z=Butterworth_Filter(gyro->z,&Gyro_BufferData_BPF[2],&Bandstop_Filter_Parameter_30_98);

    return retu;
}


/**
 * @brief
 * @param imudata
 */
Vector3f Gyro_Quantizer(const Vector3f* imudata)
{
    static Vector3f retu;

    retu.x = imudata->x /65.5f;
    retu.y = imudata->y /65.5f;
    retu.z = imudata->z /65.5f;

    return retu;
}



/**
 * @brief 单位变成 cm/s^2
 * @param imudata
 */
Vector3f Accel_Quantizer(const Vector3f* imudata)
{
    static Vector3f retu;

    retu.x = imudata->x /4096.0f * Gravity_accel * 100.0f;
    retu.y = imudata->y /4096.0f * Gravity_accel * 100.0f;
    retu.z = imudata->z /4096.0f * Gravity_accel * 100.0f;

    retu.z -= Gravity_accel * 100.0f;/** 减去地球重力加速度 */

    return retu;
}


Vector3f Accel_Filter(const Vector3f* Accel)
{
    static Butter_BufferData Accel_BufferData_BPF[3];
    static Butter_BufferData Accel_BufferData[3];
    static float X_g_av_bpf,Y_g_av_bpf,Z_g_av_bpf;
    static Vector3f retu;


//    Acce_Control_Filter();//加速度滤波，用于惯导、加速度控制反馈量
    /* 加速度计Butterworth滤波 */
    X_g_av_bpf=Butterworth_Filter(Accel->x,&Accel_BufferData_BPF[0],&Bandstop_Filter_Parameter_30_94);
    Y_g_av_bpf=Butterworth_Filter(Accel->y,&Accel_BufferData_BPF[1],&Bandstop_Filter_Parameter_30_94);
    Z_g_av_bpf=Butterworth_Filter(Accel->z,&Accel_BufferData_BPF[2],&Bandstop_Filter_Parameter_30_94);

    retu.x=Butterworth_Filter(X_g_av_bpf,&Accel_BufferData[0],&Accel_Parameter);
    retu.y=Butterworth_Filter(Y_g_av_bpf,&Accel_BufferData[1],&Accel_Parameter);
    retu.z=Butterworth_Filter(Z_g_av_bpf,&Accel_BufferData[2],&Accel_Parameter);

    return retu;
}


/**
 * @brief 本函数要持续收集数值进行校准.用在开机时候。用的是平均值较真
 * @param parm_Accelx
 * @return
 */
Vector3f Accel_calibration(const Vector3f* parm_Accel)
{

    static Vector3f retu={0};
    float buff[3][2] = {{0}};
    static uint32_t count=0;

    static Vector3f accel1;

    accel1 = Accel_Quantizer(parm_Accel);

    buff[_X][0] = accel1.x - 0.0f ;///误差
    buff[_X][1] = (float)((buff[_X][1] * ((double)count / ((double)count+1.0)))
                        + (buff[_X][0] * (1.0/((double)count+1.0)))); ///平均误差

    buff[_Y][0] = accel1.y - 0.0f ;///误差
    buff[_Y][1] = (float)((buff[_Y][1] * ((double)count / ((double)count+1.0)))
                          + (buff[_Y][0] * (1.0/((double)count+1.0)))); ///平均误差

    buff[_Z][0] = accel1.z - 0.0f ;///误差
    buff[_Z][1] = (float)((buff[_Z][1] * ((double)count / ((double)count+1.0)))
                          + (buff[_Z][0] * (1.0/((double)count+1.0)))); ///平均误差

    count++;

    retu.x = buff[_X][1];
    retu.y = buff[_Y][1];
    retu.z = buff[_Z][1];

    return retu;
}


Vector3f Gyro_calibration(const Vector3f* parm_Gyro)
{
    static Vector3f retu={0};
    float buff[3][2] = {{0}};
    static uint32_t count=0;

    static Vector3f gyro1;

    gyro1 = Gyro_Quantizer(parm_Gyro);

    buff[_X][0] = gyro1.x - 0.0f ;//误差
    buff[_X][1] = (float)((buff[_X][1] * ((double)count / ((double)count+1.0)))
                          + (buff[_X][0] * (1.0/((double)count+1.0)))); ///平均误差

    buff[_Y][0] = gyro1.y - 0.0f ;//误差
    buff[_Y][1] = (float)((buff[_Y][1] * ((double)count / ((double)count+1.0)))
                          + (buff[_Y][0] * (1.0/((double)count+1.0)))); ///平均误差

    buff[_Z][0] = gyro1.z - 0.0f ;//误差
    buff[_Z][1] = (float)((buff[_Z][1] * ((double)count / ((double)count+1.0)))
                          + (buff[_Z][0] * (1.0/((double)count+1.0)))); ///平均误差

    retu.x = buff[_X][1];
    retu.y = buff[_Y][1];
    retu.z = buff[_Z][1];

    return retu;
}






















































