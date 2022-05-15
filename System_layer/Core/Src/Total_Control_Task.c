//
// Created by Gideon on 2021/8/21.
//
#include "main.h"




void StartTotalControlTask(void const * argument)
{
    static portTickType PreviousWakeTime;
    PreviousWakeTime = xTaskGetTickCount();

    static int8_t begin_flag=0;

    static int16_t Target_From_Remote[8] ={0};

    static int8_t temp_int8t[10];
    static int16_t temp_int16t[10];

    static float_t q_init[4];
    static Vector3f imu_data[2];

    for(;;)
    {

        osDelayUntil(&PreviousWakeTime,UNTIL_DELAY);

        /** PPM ���ݴ��� */
        memcpy(Target_From_Remote, Remote_Controller(PPM_DATA_BUFF), 8*sizeof(int16_t));

//        LOG("remote : %d, %d, %d, %d,|| %d, %d, %d, %d\n",Target_From_Remote[_ROLL],Target_From_Remote[_PITCH],
//                Target_From_Remote[_THROTTLE],Target_From_Remote[_YAW],Target_From_Remote[_CHANNEL5],
//                Target_From_Remote[_CHANNEL6],Target_From_Remote[_CHANNEL7],Target_From_Remote[_CHANNEL8]);

        /** �ж��Ƿ�������Լ�ͨ����ģʽ */
        memcpy(temp_int8t, Remote_State_Analyse(Target_From_Remote), 2*sizeof(int8_t));

//        LOG("Remote_state: %d, %d\n",temp_int8t[_MODE],temp_int8t[_LOCK_FLAG]);

        /** IMU���ݶ�ȡ */
        memcpy(imu_data, GET_MPU_DATA(), 2*sizeof(Vector3f));

        if(imu_data[0].x == 0 && imu_data[0].y == 0 && imu_data[0].z == 0)
        {
            ;/** ��ʾ����IMUУ׼�� */
//            LOG("IMU_calibrating\n");
        }
        else /** У׼��� **/
        {
            if (begin_flag == 0)
            {
                /** ��ʼ����Ԫ��������̬���㺯��ʹ�� */
                memcpy(q_init, Quaternion_Init( imu_data[0].x,imu_data[0].y,imu_data[0].z), 4*sizeof(float_t));
                begin_flag = 1;
            }

//            LOG("%.3f, %.3f, %.3f, %.3f, %.3f, %.3f, %.3f, %.3f, %.3f\n",imu_data[1].x, imu_data[1].y, imu_data[1].z\
//            ,imu_data[1].x, imu_data[1].y, imu_data[1].z\
//            ,imu_data[1].x, imu_data[1].y, imu_data[1].z);


            /** ��̬����  һ���ݶ��½���̬�ں� */
            AHRS_by_Madgwick(&imu_data[0], &imu_data[1], q_init);
//            LOG("Angle: %.3f, %.3f, %.3f\n",PITCH,ROLL,YAW);

            /** �߶ȹߵ��ں� */
            Altitude_Statemachine();


            /** ��̬���� */
            Attitude_Control(   Target_From_Remote,
                                &CONTROLLER_BUS.PITCH_ANGLE_CONTROLLER,
                                &CONTROLLER_BUS.ROLL_ANGLE_CONTROLLER,
                                &CONTROLLER_BUS.YAW_ANGLE_CONTROLLER,

                                &CONTROLLER_BUS.PITCH_GYRO_CONTROLLER,
                                &CONTROLLER_BUS.ROLL_GYRO_CONTROLLER,
                                &CONTROLLER_BUS.YAW_GYRO_CONTROLLER);

            /** PWM����� */
            PWM_Output( temp_int8t[_LOCK_FLAG],
                        temp_int8t[_MODE],
                        Target_From_Remote[_THROTTLE]+1000,
                        &CONTROLLER_BUS);

        }
    }
}























































