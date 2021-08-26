//
// Created by Gideon on 2021/8/21.
//
#include "main.h"




void StartTotalControlTask(void const * argument)
{
    static portTickType PreviousWakeTime;
    PreviousWakeTime = xTaskGetTickCount();

    static int8_t begin_flag=0;
    static int16_t temp[8];
    static int16_t Target_From_Remote[8] ={0};
    static float_t q_init[4];
    static Vector3f imu_data[2];

    for(;;)
    {
        osDelayUntil(&PreviousWakeTime,UNTIL_DELAY);

        /** PPM 数据处理 */
        memcpy(Target_From_Remote, Remote_Controller(PPM_DATA_BUFF), 8*sizeof(int16_t));
        Remote_State_Analyse(Target_From_Remote);//判断是否解锁

        /** IMU数据读取 */
        memcpy(imu_data, GET_MPU_DATA(), 2*sizeof(Vector3f));

        if(imu_data[0].x == 0 && imu_data[0].y == 0 && imu_data[0].z == 0)
        {
            ;/** 表示还在IMU校准。 */
        }
        else /** 校准完毕 **/
        {
            if (begin_flag == 0)
            {
                /** 初始化四元数，给姿态解算函数使用 */
                memcpy(q_init, Quaternion_Init( imu_data[0].x,imu_data[0].y,imu_data[0].z), 4*sizeof(float_t));
                begin_flag = 1;
            }

//            printf("%f, %f, %f\n",imu_data[1].x, imu_data[1].y, imu_data[1].z);
            /** 姿态解算  一阶梯度下降姿态融合 */
            AHRS_by_Madgwick(&imu_data[0], &imu_data[1], q_init);




        }



    }




}























































