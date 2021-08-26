//
// Created by Gideon on 2021/8/7.
//

#ifndef STM32H743_HARDWARE_LAYER_MANAGER_H
#define STM32H743_HARDWARE_LAYER_MANAGER_H

#include "main.h"
#include "MPU_DATA_PROCESS.h"

void Hardware_layer_Init(void);





/**  Hardware layer �����ϲ�ĺ���API  */

/** �ɻ����PWM��� */
void PWM_pull_all_down(void);
void PWM_load(uint16_t PWM1, uint16_t PWM2, uint16_t PWM3, uint16_t PWM4);

/** MPU���ݶ�ȡ */
float_t MPU6050_Read_Temp(void);
Vector3f MPU6050_Read_Gyro(void);
Vector3f MPU6050_Read_Accel(void);

/** RGB�� */
void Set_RGB_Brightness(uint16_t R , uint16_t G , uint16_t B );

/** IMU���ȿ��� */
void Set_IMU_temperation_ctrl(uint16_t set_PWM);

/** �ⲿflash */
void W25Qx_Init(void);
void W25Qxx_Write_sector(uint16_t sector_number, uint16_t data_size ,uint8_t *data_buf);
void W25Qxx_Read_sector(uint16_t sector_number, uint16_t data_size ,uint8_t *data_buf);
void W25Qxx_Erase_sector(uint16_t sector_number);


#endif //STM32H743_HARDWARE_LAYER_MANAGER_H
