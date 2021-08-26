//
// Created by Gideon on 2021/8/21.
//
#ifndef STM32H743_TOTAL_CONTROL_TASK_H
#define STM32H743_TOTAL_CONTROL_TASK_H

#include "main.h"

#define UNTIL_DELAY 5

extern osThreadId defaultTaskHandle;
extern osThreadId totalControlTaskHandle;

void StartDefaultTask(void const * argument);
void StartTotalControlTask(void const * argument);








#endif //STM32H743_TOTAL_CONTROL_TASK_H
