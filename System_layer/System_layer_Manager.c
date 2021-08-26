//
// Created by Gideon on 2021/8/7.
//
#include "main.h"

/** System layer 全局变量汇总处 */
osThreadId defaultTask_Handle;
osThreadId totalControlTask_Handle;






void System_layer_Init(void )
{

    /* Create the thread(s) */
    /** definition and creation of defaultTask */
    osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 200);
    defaultTask_Handle = osThreadCreate(osThread(defaultTask), NULL);

    osThreadDef(totalControlTask, StartTotalControlTask, osPriorityHigh, 0, 3000);
    totalControlTask_Handle=osThreadCreate(osThread(totalControlTask), NULL);

    /* Start scheduler */
    osKernelStart();
    /* We should never get here as control is now taken by the scheduler */
    while(1);


}
