//
// Created by Gideon on 2021/8/7.
//

#ifndef STM32H743_HARDWARE_LAYER_MANAGER_H
#define STM32H743_HARDWARE_LAYER_MANAGER_H

void Hardware_layer_Init(void);





/**  Hardware layer 传给上层的函数API  */

/** 飞机电机PWM输出 */
void PWM_pull_all_down(void);
void PWM_load(uint16_t PWM1, uint16_t PWM2, uint16_t PWM3, uint16_t PWM4);

/** MPU数据读取 */
void MPU6050_Read_Temp(int16_t *Temp_buf);
void MPU6050_Read_Gyro(int16_t *Gyro_buf);
void MPU6050_Read_Accel(int16_t *Accel_buf);

/** RGB灯 */
void Set_RGB_Brightness(uint16_t R , uint16_t G , uint16_t B );

/** IMU加热控制 */
void Set_IMU_temperation_ctrl(uint16_t set_PWM);


#endif //STM32H743_HARDWARE_LAYER_MANAGER_H
