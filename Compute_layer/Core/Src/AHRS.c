//
// Created by Gideon on 2021/8/27.
//

#include "main.h"


/******************************************************************************************
������:	void AHRSUpdate_GraDes(float gx, float gy, float gz, float ax, float ay, float az)
˵��:	������+���ٶȼ��ݶ��½���̬�ں��㷨
���:	float gx	������x����
float gy	������y����
float gz	������z����
float ax	���ٶȼ�x����
float ay	���ٶȼ�y����
float az	���ٶȼ�z����
����:	��
��ע:	http://blog.csdn.net/nemol1990
******************************************************************************************/

#define Gyro_Range_Scale (1.0f/65.5f)
#define Beta_Base 0.05f
#define Quad_Num  20
#define IMU_Dt 0.005f //����Ǳ��γ������е�ʱ����
#define TimeSync_Cnt 0

/**
 * @brief ����������ķ�����̬���ó�ŷ����
 * @param pram_accel ��λ�ǣ�cm/s^2 ��
 * @param pram_gyro     degree/s
 * @return ���ɷ�����������ŷ���� PITCH,ROLL,YAW��ȫ�ֱ���
 */
void AHRS_by_Madgwick(const Vector3f* pram_accel, const Vector3f* pram_gyro, const float_t* q_init)
{
    /**********************************************************/

    static _Attitude_Tag att =
            {
                    {1.0f, 0.0f, 0.0f, 0.0f},
                    {0.0f, 0.0f, 0.0f}
            };
    static float BETADEF=0.01f;
    static float Gyro_Delta_Length=0, Gyro_Length=0,Gyro_Length_Filter=0;//������ģ��
    static float Quad_Buf[Quad_Num][4]={0};
    static float Gyro_Delta[3]={0};//���ٶ�����
    static Vector4q quad_buffer[Quad_Num];
    static float Gyro_Record[3][10]={0};
    static Butter_BufferData Butter_Buffer_Gyro_Length;
    static float error_length=0;

    Vector3f retu[2];//����ֵ
    static uint8_t begin_flag0=0;

    static float_t Yaw_Gyro=0,Pitch_Gyro=0,Roll_Gyro=0;
    static float_t Pitch_Gyro_Radian=0, Roll_Gyro_Radian=0, Yaw_Gyro_Radian=0;
    static float_t Yaw_Gyro_Earth_Frame=0;

    static float_t exIntegra=0;



    static const float_t Ki=0.01f, Kp=20.0f;

//    static const float_t Ki=0.00f, Kp=00.0f;

//    static float Acceleration_Length;//�Ǹ���SINS�������ɵ���ֵ



    /***********************************************************/

    float recipNorm;											// ƽ����
    float s0=0, s1=0, s2=0, s3=0;									// �ݶ��½��������������̬
    float qDot1=0, qDot2=0, qDot3=0, qDot4=0;			// ��Ԫ��΢�ַ�����õ���̬
    float _2q0, _2q1, _2q2, _2q3, _4q0, _4q1, _4q2 ,_8q1, _8q2, q0q0, q1q1, q2q2, q3q3;
    float delta_e2;
    static float vx=0,vy=0,vz=0;
    static float ex=0,ey=0,ez=0;
    int16_t i=0;
    static Vector3f accel[Quad_Num];
    static uint16_t sync_cnt=0;


    if(begin_flag0 == 0) // ֻ�ǽ���һ��
    {
        for(i=Quad_Num-1;i>=0;i--)//����ʼ��Ԫ����������,
        {
            quad_buffer[i].q0=q_init[0];
            quad_buffer[i].q1=q_init[1];
            quad_buffer[i].q2=q_init[2];
            quad_buffer[i].q3=q_init[3];
        }

        att.q[0]=q_init[0];
        att.q[1]=q_init[1];
        att.q[2]=q_init[2];
        att.q[3]=q_init[3];

        begin_flag0=1;
    }


    sync_cnt++;
    if(sync_cnt>=4)//4*5=20ms����һ��
    {
        for(i=Quad_Num-1;i>0;i--)//����Ԫ����ʷֵ��������,20*20=400ms
        {
            quad_buffer[i].q0=quad_buffer[i-1].q0;
            quad_buffer[i].q1=quad_buffer[i-1].q1;
            quad_buffer[i].q2=quad_buffer[i-1].q2;
            quad_buffer[i].q3=quad_buffer[i-1].q3;
            accel[i].x=accel[i-1].x;
            accel[i].y=accel[i-1].y;
            accel[i].z=accel[i-1].z;
        }
        quad_buffer[0].q0=att.q[0];
        quad_buffer[0].q1=att.q[1];
        quad_buffer[0].q2=att.q[2];
        quad_buffer[0].q3=att.q[3];
        sync_cnt=0;
    }
    accel[0].x=pram_accel->x;
    accel[0].y=pram_accel->y;
    accel[0].z=pram_accel->z;

    for(i=9;i>0;i--)//����������ʷֵ��������
    {
        Gyro_Record[0][i]=Gyro_Record[0][i-1];
        Gyro_Record[1][i]=Gyro_Record[1][i-1];
        Gyro_Record[2][i]=Gyro_Record[2][i-1];
    }
    Gyro_Record[0][0]=Pitch_Gyro;//��λ:��/��(deg/s)
    Gyro_Record[1][0]=Roll_Gyro;
    Gyro_Record[2][0]=Yaw_Gyro;


    /************���ٶȸ�ֵ��������̬�����ڻ�,���ٶȷ���*************/

    Pitch_Gyro= pram_gyro->x ;
    Roll_Gyro= pram_gyro->y ;
    Yaw_Gyro= pram_gyro->z ;

    ///ȫ�ֱ�����ֵ���Ժ�Ҫ�Ż�һ��
    PITCH_GYRO = -pram_gyro->x;
    ROLL_GYRO = -pram_gyro->y;
    YAW_GYRO = pram_gyro->z;

    Pitch_Gyro_Radian = pram_gyro->x * DEG_TO_RAD;
    Roll_Gyro_Radian = pram_gyro->y * DEG_TO_RAD;
    Yaw_Gyro_Radian = pram_gyro->z * DEG_TO_RAD;

    /** ��ǰ�ĽǼ��ٶ�ֵ��ʱ������5ms */
    Gyro_Delta[0]=(Gyro_Record[0][0]-Gyro_Record[0][1]);
    Gyro_Delta[1]=(Gyro_Record[1][0]-Gyro_Record[1][1]);
    Gyro_Delta[2]=(Gyro_Record[2][0]-Gyro_Record[2][1]);


    /** �Ǽ��ٶ�ģ�� */
    Gyro_Delta_Length=sqrt(Gyro_Delta[0]*Gyro_Delta[0]
                           +Gyro_Delta[1]*Gyro_Delta[1]
                           +Gyro_Delta[2]*Gyro_Delta[2]);

    /** ���ٶ�ģ�� */
    Gyro_Length=sqrt(Yaw_Gyro*Yaw_Gyro
                     +Pitch_Gyro*Pitch_Gyro
                     +Roll_Gyro*Roll_Gyro);//��λdeg/s

    /** ���ٶ�ģ���˲� */
    Gyro_Length_Filter=LPButterworth(Gyro_Length,
                                     &Butter_Buffer_Gyro_Length,&Butter_5HZ_Parameter_Acce);
//    LOG("%f\n",Gyro_Length_Filter);

    /** ���ٶȼ������Чʱ,���ü��ٶȼƲ��������� */
    if(!((pram_accel->x == 0.0f) && (pram_accel->y == 0.0f) && (pram_accel->z == 0.0f)))
    {
        static float_t ax,ay,az,gx,gy,gz;

        /** ѡȡ��20ms * 10 =200msǰ��accel���� */
        ax=accel[TimeSync_Cnt].x;
        ay=accel[TimeSync_Cnt].y;
        az=accel[TimeSync_Cnt].z + Gravity_accel * 100.0f;//����Ӧ�е�����

        /** ���ٶȹ�һ�� */
        recipNorm=invSqrt(ax * ax + ay * ay + az * az);
        ax *= recipNorm;
        ay *= recipNorm;
        az *= recipNorm;

        /** ����������� �����ظ����� */
        _2q0 = 2.0f * quad_buffer[TimeSync_Cnt].q0;
        _2q1 = 2.0f * quad_buffer[TimeSync_Cnt].q1;
        _2q2 = 2.0f * quad_buffer[TimeSync_Cnt].q2;
        _2q3 = 2.0f * quad_buffer[TimeSync_Cnt].q3;
        _4q0 = 4.0f * quad_buffer[TimeSync_Cnt].q0;
        _4q1 = 4.0f * quad_buffer[TimeSync_Cnt].q1;
        _4q2 = 4.0f * quad_buffer[TimeSync_Cnt].q2;
        _8q1 = 8.0f * quad_buffer[TimeSync_Cnt].q1;
        _8q2 = 8.0f * quad_buffer[TimeSync_Cnt].q2;
        q0q0 = quad_buffer[TimeSync_Cnt].q0 * quad_buffer[TimeSync_Cnt].q0;
        q1q1 = quad_buffer[TimeSync_Cnt].q1 * quad_buffer[TimeSync_Cnt].q1;
        q2q2 = quad_buffer[TimeSync_Cnt].q2 * quad_buffer[TimeSync_Cnt].q2;
        q3q3 = quad_buffer[TimeSync_Cnt].q3 * quad_buffer[TimeSync_Cnt].q3;

        /** �ݶ��½��㷨,�����������ݶ� */
        s0 = _4q0 * q2q2 + _2q2 * ax + _4q0 * q1q1 - _2q1 * ay;
        s1 = _4q1 * q3q3 - _2q3 * ax + 4.0f * q0q0 * quad_buffer[TimeSync_Cnt].q1 - _2q0 * ay - _4q1 + _8q1 * q1q1 + _8q1 * q2q2 + _4q1 * az;
        s2 = 4.0f * q0q0 * quad_buffer[TimeSync_Cnt].q2 + _2q0 * ax + _4q2 * q3q3 - _2q3 * ay - _4q2 + _8q2 * q1q1 + _8q2 * q2q2 + _4q2 * az;
        s3 = 4.0f * q1q1 * quad_buffer[TimeSync_Cnt].q3 - _2q1 * ax + 4.0f * q2q2 * quad_buffer[TimeSync_Cnt].q2 - _2q2 * ay;

        /** �ݶȹ�һ��,�õ��ݶȵķ��� */
        recipNorm=invSqrt(s0 * s0 + s1 * s1 + s2 * s2 + s3 * s3);
        s0 *= recipNorm;
        s1 *= recipNorm;
        s2 *= recipNorm;
        s3 *= recipNorm;

//        BETADEF = Beta_Base + (0.0014f * constrain(Gyro_Length_Filter, 0.0f, 400.0f));
//        BETADEF -= 0.01f*(constrain(Acceleration_Length,0,1000)/1000);//��̬����,������ͣ��500����

        /** ���ٶȼƲ����G����ʾ�ģ���Ԫ������Ҳ����ת������ĵ�����*/
        vx=2*(quad_buffer[TimeSync_Cnt].q1*quad_buffer[TimeSync_Cnt].q3 - quad_buffer[TimeSync_Cnt].q0*quad_buffer[TimeSync_Cnt].q2);
        vy=2*(quad_buffer[TimeSync_Cnt].q2*quad_buffer[TimeSync_Cnt].q3 + quad_buffer[TimeSync_Cnt].q0*quad_buffer[TimeSync_Cnt].q1);
        vz=1-2*(quad_buffer[TimeSync_Cnt].q1*quad_buffer[TimeSync_Cnt].q1 + quad_buffer[TimeSync_Cnt].q2*quad_buffer[TimeSync_Cnt].q2);
//        printf("%f, %f, %f\n",vx-ax, vy-ay,vz- az);//���۲��

        /** ��������������ڲ���������Ч�������� */
        error_length = sqrtf((vx-ax) * (vx-ax) + (vy-ay) * (vy-ay) + (vz-az) * (vz-az));//0.3 ~ 1.0
//        printf("%f\n", error_length);
        BETADEF = Beta_Base + 1.0f*(error_length-0.3);
        BETADEF = constrain(BETADEF,Beta_Base,0.4f);

        /** ʹ�ò������������������ʵ�ʲ�����������������������֮������ */
        ex =(ay*vz - az*vy);
        ey =(az*vx - ax*vz);
        ez =(ax*vy - ay*vx);
        /** ����������õ�����������л������㣬���ֵĽ���ۼӵ������ǵ������У������������������ݡ� */
        exIntegra +=ex*Ki* IMU_Dt;
        exIntegra +=ey*Ki* IMU_Dt;
        exIntegra +=ez*Ki* IMU_Dt;

        /** ת��Ϊ�����ƣ�������̬����*/
        gx = pram_gyro->x * DEG_TO_RAD +exIntegra + Kp*ex;
        gy = pram_gyro->y * DEG_TO_RAD +exIntegra + Kp*ey;
        gz = pram_gyro->z * DEG_TO_RAD +exIntegra + Kp*ez;

        /** ��Ԫ��΢�ַ��̼��㱾�δ�������Ԫ��,�����Ԫ����΢��,���ý��ٶ������Ԫ�� */
        /// https://blog.csdn.net/a735148617/article/details/114786861?utm_medium=distribute.pc_relevant.none-task-blog-2~default~baidujs_utm_term~default-0.essearch_pc_relevant&spm=1001.2101.3001.4242
        qDot1 = 0.5f * (-att.q[1] * gx - att.q[2] * gy - att.q[3] * gz);
        qDot2 = 0.5f * ( att.q[0] * gx + att.q[2] * gz - att.q[3] * gy);
        qDot3 = 0.5f * ( att.q[0] * gy - att.q[1] * gz + att.q[3] * gx);
        qDot4 = 0.5f * ( att.q[0] * gz + att.q[1] * gy - att.q[2] * gx);

        qDot1 -= BETADEF * s0;
        qDot2 -= BETADEF * s1;
        qDot3 -= BETADEF * s2;
        qDot4 -= BETADEF * s3;

        /** ��������Ԫ��΢�ַ����������̬��� */
        /** ����Ԫ����̬��������,�õ���ǰ��Ԫ����̬ */
        /** ʹ�ö��ױϿ����΢�ַ��� */
        ///https://blog.csdn.net/zhangyufeikk/article/details/94594646
        /** ע������Ķ��ױϿ�������̬�ں��㷨�ļ��㣬����t��t+1��������⣬��Ҫע�� */
        delta_e2 = (IMU_Dt * gx) * (IMU_Dt * gx) + (IMU_Dt * gy) * (IMU_Dt * gy) + (IMU_Dt * gz) * (IMU_Dt * gz);
        att.q[0] = (1.0f - delta_e2 / 8.0f) * att.q[0] + qDot1 * IMU_Dt;
        att.q[1] = (1.0f - delta_e2 / 8.0f) * att.q[1] + qDot2 * IMU_Dt;
        att.q[2] = (1.0f - delta_e2 / 8.0f) * att.q[2] + qDot3 * IMU_Dt;
        att.q[3] = (1.0f - delta_e2 / 8.0f) * att.q[3] + qDot4 * IMU_Dt;

        /** ��λ����Ԫ�� */
        recipNorm=invSqrt(att.q[0] * att.q[0] + att.q[1] * att.q[1] + att.q[2] * att.q[2] + att.q[3] * att.q[3]);
        att.q[0] *= recipNorm;
        att.q[1] *= recipNorm;
        att.q[2] *= recipNorm;
        att.q[3] *= recipNorm;

        /** ��Ԫ����ŷ����ת��,ת��˳��ΪZ-Y-X,�μ�<Representing Attitude: Euler Angles, Unit Quaternions, and Rotation Vectors>.pdfһ��,P24 */
        /** https://www.cnblogs.com/21207-iHome/p/6894128.html */
        PITCH= -atan2(2.0f * att.q[2] * att.q[3] + 2.0f * att.q[0] * att.q[1], -2.0f * att.q[1] * att.q[1] - 2.0f * att.q[2]* att.q[2] + 1.0f) * RAD_TO_DEG;// Pitch
        ROLL= -asin(2.0f * att.q[0]* att.q[2]-2.0f * att.q[1] * att.q[3]) * RAD_TO_DEG;
        YAW = atan2(2.0f * att.q[1] * att.q[2] + 2.0f * att.q[0] * att.q[3], -2.0f * att.q[3] * att.q[3] - 2.0f * att.q[2] * att.q[2] + 1.0f) * RAD_TO_DEG;// Yaw
        if(YAW<0)
        {
            YAW += 360;
        }


//        printf("%f, %f, %f\n",PITCH, ROLL, YAW);
    }
    else
    {
        PITCH=0;
        ROLL=0;
    }

    imuComputeRotationMatrix();
}



/****************** ���ݳ�ʼ��ŷ���ǳ�ʼ����Ԫ�� *****************************/
/**
 * @param parm_accelx
 * @param parm_accely
 * @param parm_accelz
 * @return ��Ԫ������
 */
float_t* Quaternion_Init(const float_t parm_accelx, const float_t parm_accely,  float_t parm_accelz)
{

    float_t about_roll=0,about_pitch=0, about_yaw=0;
    static float_t q[4]={0};

    parm_accelz += Gravity_accel*100.0f;//���ϸ��е��������ٶ�

    /** �տ�����������һ���Ƕ�ֵ, ������Լ����� */
    about_roll = -57.3*atan(parm_accelx * invSqrt(parm_accely*parm_accely + parm_accelz*parm_accelz));//�����
    about_pitch = 57.3*atan(parm_accely * invSqrt(parm_accelx*parm_accelx + parm_accelz*parm_accelz));//������

//    printf("%f, %f\n",about_roll,about_pitch);

    about_roll *= DEG_TO_RAD;
    about_pitch *= DEG_TO_RAD;
    about_yaw = 180.0f * DEG_TO_RAD;//�Զ����


    /** ���� Z_Y_X ��ת�� */
    q[0] = cos(about_yaw/2)*cos(about_pitch/2)*cos(about_roll/2)+sin(about_yaw/2)*sin(about_pitch/2)*sin(about_roll/2);
    q[1] = cos(about_yaw/2)*cos(about_pitch/2)*sin(about_roll/2)-sin(about_yaw/2)*sin(about_pitch/2)*cos(about_roll/2);
    q[2] = cos(about_yaw/2)*sin(about_pitch/2)*cos(about_roll/2)+sin(about_yaw/2)*cos(about_pitch/2)*sin(about_roll/2);
    q[3] = sin(about_yaw/2)*cos(about_pitch/2)*cos(about_roll/2)-cos(about_yaw/2)*sin(about_pitch/2)*sin(about_roll/2);

    return q;
}




/**
 * @brief ����ŷ����ת����,���Ҳ��������ȫ�ֱ���
 */
void imuComputeRotationMatrix(void)
{
    SIN_PITCH = sin(PITCH* DEG_TO_RAD);
    COS_PITCH = cos(PITCH* DEG_TO_RAD);
    SIN_ROLL  = sin(ROLL* DEG_TO_RAD);
    COS_ROLL  = cos(ROLL* DEG_TO_RAD);
    SIN_YAW   = sin(YAW* DEG_TO_RAD);
    COS_YAW   = cos(YAW* DEG_TO_RAD);



    /** �����321����ϵ�� YAW-ROLL-PITCH ***/
    /**
    X����ת����
    R������
    {1      0        0    }
    {0      cos��    sin��}
    {0    -sin��    cos�� }

    Y����ת����
    R���ȣ�
    {cos��     0        -sin��}
    {0         1        0     }
    {sin��     0        cos��}

    Z����ת����
    R���ȣ�
    {cos��      sin��       0}
    {-sin��     cos��       0}
    {0          0           1 }

    ��Z-Y-X˳����:
    ��������ϵ����������ϵ����ת����R(b2n)
    R(b2n) =R(��)^T*R(��)^T*R(��)^T

    R=
    {cos��*cos��     -cos��*sin��+sin��*sin��*cos��        sin��*sin��+cos��*sin��*cos��}
    {cos��*sin��     cos��*cos�� +sin��*sin��*sin��       -cos��*sin��+cos��*sin��*sin��}
    {-sin��          cos��sin ��                          cos��cos��                   }
    */
    Euler_Rotate_Matrix[0][0] = COS_YAW* COS_ROLL;
    Euler_Rotate_Matrix[0][1] = SIN_PITCH*SIN_ROLL*COS_YAW - COS_PITCH * SIN_YAW;
    Euler_Rotate_Matrix[0][2] = SIN_PITCH * SIN_YAW + COS_PITCH * SIN_ROLL * COS_YAW;

    Euler_Rotate_Matrix[1][0] = SIN_YAW * COS_ROLL;
    Euler_Rotate_Matrix[1][1] = SIN_PITCH * SIN_ROLL * SIN_YAW +COS_PITCH * COS_YAW;
    Euler_Rotate_Matrix[1][2] = COS_PITCH * SIN_ROLL * SIN_YAW - SIN_PITCH * COS_YAW;

    Euler_Rotate_Matrix[2][0] = -SIN_ROLL;
    Euler_Rotate_Matrix[2][1] = SIN_PITCH * COS_ROLL;
    Euler_Rotate_Matrix[2][2] = COS_PITCH * COS_ROLL;
}

































