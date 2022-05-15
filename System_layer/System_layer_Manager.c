//
// Created by Gideon on 2021/8/7.
//
#include "main.h"

/** System layer ȫ�ֱ������ܴ� */
osThreadId defaultTask_Handle;
osThreadId totalControlTask_Handle;
osThreadId ESC_Calibra_Task_Handle;
osThreadId Ultrasonic_Task_Handle;

int8_t NOW_PAGE_NUMBER=1;///������һҳ

void System_layer_Init(void )
{

    /* Create the thread(s) */

    /// �����ȼ�����
    osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 500);
    defaultTask_Handle = osThreadCreate(osThread(defaultTask), NULL);


    ///��������������
    osThreadDef(Ultrasonic_Task, StartUltrasonicTask, osPriorityHigh, 0, 200);
    Ultrasonic_Task_Handle = osThreadCreate(osThread(Ultrasonic_Task), NULL);



#if (ESC_CALIBRATION_MODE_NOW == 0 )///������ǵ��У׼ģʽ,����������

    ///������  StartTotalControlTask
    osThreadDef(totalControlTask, StartTotalControlTask, osPriorityHigh, 0, 3000);
    totalControlTask_Handle=osThreadCreate(osThread(totalControlTask), NULL);

#elif (ESC_CALIBRATION_MODE_NOW == 1)///����ǵ��У׼ģʽ���������У׼����

    osThreadDef(esc_calibra_Task, Start_ESC_Calibra_Task, osPriorityRealtime, 0, 3000);
    ESC_Calibra_Task_Handle=osThreadCreate(osThread(esc_calibra_Task), NULL);

#endif




    /* Start scheduler */
    osKernelStart();
    /* We should never get here as control is now taken by the scheduler */
    while(1);


}
