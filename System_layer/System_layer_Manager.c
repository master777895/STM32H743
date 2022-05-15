//
// Created by Gideon on 2021/8/7.
//
#include "main.h"

/** System layer 全局变量汇总处 */
osThreadId defaultTask_Handle;
osThreadId totalControlTask_Handle;
osThreadId ESC_Calibra_Task_Handle;
osThreadId Ultrasonic_Task_Handle;

int8_t NOW_PAGE_NUMBER=1;///开机第一页

void System_layer_Init(void )
{

    /* Create the thread(s) */

    /// 低优先级任务
    osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 500);
    defaultTask_Handle = osThreadCreate(osThread(defaultTask), NULL);


    ///超声波触发任务
    osThreadDef(Ultrasonic_Task, StartUltrasonicTask, osPriorityHigh, 0, 200);
    Ultrasonic_Task_Handle = osThreadCreate(osThread(Ultrasonic_Task), NULL);



#if (ESC_CALIBRATION_MODE_NOW == 0 )///如果不是电调校准模式,创建主任务

    ///主任务  StartTotalControlTask
    osThreadDef(totalControlTask, StartTotalControlTask, osPriorityHigh, 0, 3000);
    totalControlTask_Handle=osThreadCreate(osThread(totalControlTask), NULL);

#elif (ESC_CALIBRATION_MODE_NOW == 1)///如果是电调校准模式，创建电调校准任务

    osThreadDef(esc_calibra_Task, Start_ESC_Calibra_Task, osPriorityRealtime, 0, 3000);
    ESC_Calibra_Task_Handle=osThreadCreate(osThread(esc_calibra_Task), NULL);

#endif




    /* Start scheduler */
    osKernelStart();
    /* We should never get here as control is now taken by the scheduler */
    while(1);


}
