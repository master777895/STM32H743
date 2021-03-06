/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "cmsis_os.h"
#include "stm32h7xx_hal.h"
#include "stdio.h"
#include "string.h"
#include "stdint.h"



/** Hardware layer head_file **/
#include "Hardware_layer_Manager.h"
#include "PWM.h"
#include "UART.h"
#include "I2C.h"
#include "ADC.h"
#include "SPI.h"
#include "RGB.h"
#include "BUZZER.h"
#include "PPM.h"
#include "ULTRA.h"
#include "USB.h"
#include "BOTTON.h"
#include "W25Qxx.h"
#include "usb_device.h"
#include "usbd_cdc.h"
#include "usbd_def.h"
#include "usbd_conf.h"
#include "usbd_cdc_if.h"
#include "OLED.h"



/** System layer head_file **/
#include "System_layer_Manager.h"
#include "Total_Control_Task.h"
#include "ESC_Calibration_Task.h"
#include "Ultrasonic_Task.h"
#include "KEY_Process.h"
#include "OLED_Process.h"
#include "printf.h"


/** Compute layer head_file **/
#include "Compute_layer_Manager.h"
#include "Filter.h"
#include "Basic_Algorithm.h"
#include "REMOTE_DATA_PROCESS.h"
#include "AHRS.h"
#include "MPU_DATA_PROCESS.h"
#include "PWM_Output.h"
#include "Attitude_Control.h"
#include "PID.h"
#include "Altitude_Control.h"


/** APP layer head_file **/
#include "APP_layer_Manager.h"



/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

void SystemClock_Config(void);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
