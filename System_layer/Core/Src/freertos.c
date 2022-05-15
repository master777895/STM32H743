/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */


/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"


void vApplicationStackOverflowHook( TaskHandle_t xTask, char *pcTaskName );


/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */

void StartDefaultTask(void const * argument)
{
    static double_t buff[3]={0};
    static uint8_t count=0;
    static portTickType PreviousWakeTime;
    PreviousWakeTime = xTaskGetTickCount();

    for(;;)///没有比这个函数优先级更低的函数
    {
        osDelayUntil(&PreviousWakeTime,10);


        button_ticks();///按键扫描,不使用中断

        count++;
        if(count >= 5)
        {
            count=0;
            OLED_Update();///LED更新数据
        }



    }
}





/**
 * @note 任务栈溢出的回调函数
 * @param xTask
 * @param pcTaskName
 */
void vApplicationStackOverflowHook( TaskHandle_t xTask, char *pcTaskName )
{

    for(;;)
    {
        printf("Stack overflown! \n");
    }

}




/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );


static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
