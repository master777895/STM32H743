//
// Created by Gideon on 2021/9/11.
//

#ifndef STM32H743_ULTRASONIC_TASK_H
#define STM32H743_ULTRASONIC_TASK_H


#define TRIG1_OUTPUT_LOW  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_RESET)
#define TRIG1_OUTPUT_HIGH HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_SET)











void StartUltrasonicTask(void const * argument);



#endif //STM32H743_ULTRASONIC_TASK_H
