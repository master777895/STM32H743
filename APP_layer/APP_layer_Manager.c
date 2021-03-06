//
// Created by Gideon on 2021/8/27.
//

#include "main.h"

///全局变量
/**
    1偏差限幅标志     2积分限幅标志     3积分分离标志     4期望
    5反馈           6偏差；          7上次偏差；       8偏差限幅值；
    9积分分离偏差值；  10积分值         11积分限幅值；     12控制参数Kp；
    13控制参数Ki；   14控制参数Kd；    15控制器总输出；   16上次控制器总输出
    17总输出限幅度；  18：Scale_Kp     19：Scale_Ki    20:Scale_Kd
    21控制名称
*/
const float PID_CONTROL_UNIT[19][21]=
{

/** 需要对应上这个枚举量
    typedef enum
    {
        _NO_TYPE=0,

        _PID_PITCH_ANGLE,
        _PID_ROLL_ANGLE,
        _PID_YAW_ANGLE,

        _PID_PITCH_GYRO,
        _PID_ROLL_GYRO,
        _PID_YAW_GYRO,

    }PID_control_type;
*/


    {0,0,0,0,  0,0,0,0,  0,0,0,0,  0,0,0,0,  0,0,0,0},//_NO_TYPE

    ///3S锂电池，好盈乐天20A默认固件，NC330机架，定制电机KV1400，乾丰8045螺旋桨
    /*                                          Kp      Ki      Kd            */
    /*1  2  3  4  5  6   7  8   9    10  11     12      13      14     15  16   17   18 19 20        21*/
    {1  ,1 ,0 ,0 ,0 ,0 , 0 ,30  ,0  ,0 , 80,   4.50f  ,0.00f  ,0.00f  ,0  ,0 , 450,  1 ,1 ,1 ,_PID_PITCH_ANGLE},//Pitch_Angle;偏航角度
    {1  ,1 ,0 ,0 ,0 ,0 , 0 ,30  ,0  ,0 , 80,   4.50f  ,0.00f  ,0.00f  ,0  ,0 , 450,  1 ,1 ,1 ,_PID_ROLL_ANGLE},//Roll_Angle;横滚角
    {1  ,1 ,0 ,0 ,0 ,0 , 0 ,45  ,0  ,0 , 150 , 0.00f  ,0.00f  ,0.00f  ,0  ,0 , 450,  1 ,1 ,1 ,_PID_YAW_ANGLE},//Yaw_Angle;偏航角

    {1  ,1 ,0 ,0 ,0 ,0 , 0 ,500 ,0  ,0 , 300,  0.75f  ,2.50f  ,0.00f  ,0  ,0 , 500,  1 ,1 ,1 ,_PID_PITCH_GYRO},//Pitch_Gyro;偏航角速度
    {1  ,1 ,0 ,0 ,0 ,0 , 0 ,500 ,0  ,0 , 300,  0.75f  ,2.50f  ,0.00f  ,0  ,0 , 500,  1 ,1 ,1 ,_PID_ROLL_GYRO},//Roll_Gyro;横滚角速度
    {1  ,1 ,0 ,0 ,0 ,0 , 0 ,300 ,0  ,0 , 200,  0.00f  ,0.00f  ,0.00f  ,0  ,0 , 500,  1 ,1 ,1 ,_PID_YAW_GYRO},//Yaw_Gyro;偏航角速度

    ///原始参数，备份一下
//    {1  ,1 ,0 ,0 ,0 ,0 , 0 ,30  ,0  ,0 , 80,   4.50f  ,0.00f  ,0.00f  ,0  ,0 , 450,  1 ,1 ,1 ,_PID_PITCH_ANGLE},//Pitch_Angle;偏航角度
//    {1  ,1 ,0 ,0 ,0 ,0 , 0 ,30  ,0  ,0 , 80,   4.50f  ,0.00f  ,0.00f  ,0  ,0 , 450,  1 ,1 ,1 ,_PID_ROLL_ANGLE},//Roll_Angle;横滚角
//    {1  ,1 ,0 ,0 ,0 ,0 , 0 ,45  ,0  ,0 , 150 , 4.50f  ,0.00f  ,0.00f  ,0  ,0 , 450,  1 ,1 ,1 ,_PID_YAW_ANGLE},//Yaw_Angle;偏航角
//
//    {1  ,1 ,0 ,0 ,0 ,0 , 0 ,500 ,0  ,0 , 300,  0.75f  ,2.50f  ,3.00f  ,0  ,0 , 500,  1 ,1 ,1 ,_PID_PITCH_GYRO},//Pitch_Gyro;偏航角速度
//    {1  ,1 ,0 ,0 ,0 ,0 , 0 ,500 ,0  ,0 , 300,  0.75f  ,2.50f  ,3.00f  ,0  ,0 , 500,  1 ,1 ,1 ,_PID_ROLL_GYRO},//Roll_Gyro;横滚角速度
//    {1  ,1 ,0 ,0 ,0 ,0 , 0 ,300 ,0  ,0 , 200,  1.20f  ,0.50f  ,0.00f  ,0  ,0 , 500,  1 ,1 ,1 ,_PID_YAW_GYRO},//Yaw_Gyro;偏航角速度




//    ///定高参数
//    //高度单项比例控制，有偏差限幅、总输出即为最大攀升、下降速度400cm/s
//    //Z轴速度比例+积分控制，无偏差限幅
//    /*                                          Kp      Ki      Kd            */
//    /*1  2  3  4  5  6   7   8   9  10   11     12      13      14    15  16   17   18*/
//    {1  ,1 ,0 ,0 ,0 ,0 , 0 ,200 ,0  ,0 ,100 ,  1.00f  ,0.000f  ,0     ,0  ,0 ,500,  1 ,  1 ,  1 },//High_Position;海拔高度位置
//    {1  ,1 ,0 ,0 ,0 ,0 , 0 ,600 ,0  ,0 ,500 ,  5.00f  ,0.000f  ,0     ,0  ,0 ,1000,  1 ,  1 ,  1 },//High_Speed;海拔攀升速度
//
//
//    /*                                        Kp        Ki        Kd              */
//    /*1  2  3  4  5  6   7  8   9   10  11    12        13        14     15   16  17*/
//    {1  ,1 ,0 ,0 ,0 ,0 , 0 ,180 ,0  ,0 ,8,   0.200f    ,0.000f    ,0    ,0    ,0 ,300,  1 ,  1 ,  1 },//Longitude_Position;水平经度位置
//    {1  ,1 ,0 ,0 ,0 ,0 , 0 ,500 ,0  ,0 ,450, 1.800f    ,0.450f    ,0    ,0    ,0 ,800,  1 ,  1 ,  1 },//Longitude_Speed;水平经度速度
//    {1  ,1 ,0 ,0 ,0 ,0 , 0 ,180 ,0  ,0 ,8,   0.200f    ,0.000f    ,0    ,0    ,0 ,300,  1 ,  1 ,  1 },//Latitude_Position;水平纬度位置
//    {1  ,1 ,0 ,0 ,0 ,0 , 0 ,500 ,0  ,0 ,450, 1.800f    ,0.450f    ,0    ,0    ,0 ,800,  1 ,  1 ,  1 },//Latitude_Speed;水平纬度速度
//    /*************加速度控制器****************/
//    //期望最大加速度500cm/s^2
//    {1  ,1 ,0 ,0 ,0 ,0 , 0 ,1500 ,0  ,0 ,750,0.15f    ,1.2000f    ,0.0f  ,0   ,0 ,1000,  1 ,  1 ,  1 },//垂直加速度控制器
//
//
//    {1  ,1 ,0 ,0 ,0 ,0 , 0 ,100  ,0  ,0 ,3,  0.32f    ,0.0000f    ,0    ,0   ,0 ,150,  1 ,  1 ,  1 },//水平经度方向加速度控制器
//    {1  ,1 ,0 ,0 ,0 ,0 , 0 ,100  ,0  ,0 ,15, 0.45f    ,0.0000f    ,0.0f  ,0   ,0 ,25 ,  1 ,  1 ,  1 },//水平维度方向加速度控制器
//
//    /*************光流位置、速度控制器****************/
//    /*                                            Kp      Ki      Kd            */
//    /*1  2  3  4  5  6   7   8     9   10  11     12      13      14     15   16   17   18*/
//    {1  ,1 ,0 ,0 ,0 ,0 , 0 ,100   ,15  ,0 ,15,  0.25f   ,0.00f   ,0      ,0   ,0  ,30,   1 ,  1 ,  1 },//光流水平位置控制器
//    {1  ,1 ,0 ,0 ,0 ,0 , 0 ,100   ,30  ,0 ,200, 4.50f   ,0.10f   ,0.0f   ,0   ,0  ,450 , 1 ,  1 ,  1 },//光流水平速度控制器
//    {1  ,1 ,0 ,0 ,0 ,0 , 0 ,30    ,100 ,0 ,10,  2.50f   ,0       ,150.0f ,0   ,0  ,300 , 1 ,  1 ,  1 },//水平维度方向加速度控制器
};