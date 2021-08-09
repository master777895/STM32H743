//
// Created by Gideon on 2021/8/8.
//

#include "main.h"
#define MPU6050_ADDR 0xD0

/**
 * MPU6050配置寄存器
 */
#define PWR_MGMT_1_REG 0x6B
#define SAMPLE_RATE_DIV_REG 0x19
#define MPU_CONFIG 0x1A
#define GYRO_CONFIG_REG 0x1B
#define ACCEL_CONFIG_REG 0x1c

/**
 * 读取数据寄存器
 */
#define ACCEL_XOUT_H_REG 0x3B
#define TEMP_OUT_H_REG 0x41
#define GYRO_XOUT_H_REG 0x43

/**
 * 设备标识寄存器
 */
#define WHO_AM_I_REG 0x75


I2C_HandleTypeDef hi2c1;
I2C_HandleTypeDef hi2c2;

int16_t Accel_X_RAW = 0;
int16_t Accel_Y_RAW = 0;
int16_t Accel_Z_RAW = 0;

int16_t Gyro_X_RAW = 0;
int16_t Gyro_Y_RAW = 0;
int16_t Gyro_Z_RAW = 0;

int16_t Temp_RAW = 0;

/**
 *
 * @param Accel_buf
 */
void MPU6050_Read_Accel(int16_t *Accel_buf)
{
    uint8_t Rec_Data[6];

    //Read 6 BYTES of data starting from ACCEL_XOUT_H register

    HAL_I2C_Mem_Read (&hi2c1 ,MPU6050_ADDR ,ACCEL_XOUT_H_REG ,1,Rec_Data ,6,1000);

    Accel_X_RAW = (int16_t )(Rec_Data [0] <<8 | Rec_Data [1]);
    Accel_Y_RAW = (int16_t )(Rec_Data [2] <<8 | Rec_Data [3]);
    Accel_Z_RAW = (int16_t )(Rec_Data [4] <<8 | Rec_Data [5]);

    Accel_buf[0] = Accel_X_RAW/4096.0; //8G的量程
    Accel_buf[1] = Accel_Y_RAW/4096.0;
    Accel_buf[2] = Accel_Z_RAW/4096.0;
}


/**
 *
 * @param Gyro_buf
 */
void MPU6050_Read_Gyro(int16_t *Gyro_buf)
{
    uint8_t Rec_Data[6];

    // Read 6 BYTES of data staring from GYRO_XOUT_H register

    HAL_I2C_Mem_Read (&hi2c1, MPU6050_ADDR ,GYRO_XOUT_H_REG ,1,Rec_Data ,6 ,1000);

    Gyro_X_RAW = (int16_t )(Rec_Data [0] << 8 | Rec_Data [1]);
    Gyro_Y_RAW = (int16_t )(Rec_Data [2] << 8 | Rec_Data [3]);
    Gyro_Z_RAW = (int16_t )(Rec_Data [4] << 8 | Rec_Data [5]);

    Gyro_buf[0] = Gyro_X_RAW/65.5; // ± 500 °/s
    Gyro_buf[1] = Gyro_Y_RAW/65.5;
    Gyro_buf[2] = Gyro_Z_RAW/65.5;
}

/**
 *
 * @param Temp_buf
 */
void MPU6050_Read_Temp(int16_t *Temp_buf)
{
    uint8_t Rec_Data[2];

    HAL_I2C_Mem_Read (&hi2c1 ,MPU6050_ADDR ,TEMP_OUT_H_REG ,1 ,Rec_Data  ,2 ,1000);

    Temp_RAW = (int16_t )(Rec_Data [0]<<8)|Rec_Data [1];
    *Temp_buf = 36.53 + (Temp_RAW ) / 340;
}




/**
 * @brief MPU6050初始化
 */
void MPU6050_Init(void )
{
    uint8_t check,Data;

    // check device ID WHO_AM_I

    HAL_I2C_Mem_Read (&hi2c1 ,MPU6050_ADDR,WHO_AM_I_REG,1,&check ,1,1000);

    if(check == 104)		//if the device is present
    {
        //power management register 0x6B we should write all 0's to wake the sensor up
        Data = 0x00;
        HAL_I2C_Mem_Write (&hi2c1 ,MPU6050_ADDR ,PWR_MGMT_1_REG ,1,&Data ,1,1000);

        Data = 0x00;
        HAL_I2C_Mem_Write (&hi2c1 ,MPU6050_ADDR ,SAMPLE_RATE_DIV_REG ,1 ,&Data,1,1000);

        /** 内部低通滤波频率，加速度计94hz,陀螺仪98hz */
        Data = 0x02;
        HAL_I2C_Mem_Write (&hi2c1 ,MPU6050_ADDR ,MPU_CONFIG ,1 ,&Data,1,1000);


        // Set Gyroscopic configuration in GYRO_CONFIG Register
        // XG_ST=0,YG_ST=0,
        Data = 0x08;/** 0000 1000   FS_SEL=1 ->± 500 °/s*/
        HAL_I2C_Mem_Write (&hi2c1 ,MPU6050_ADDR, GYRO_CONFIG_REG, 1, &Data, 1, 1000);


        // Set accelerometer configuration in ACCEL_CONFIG Register
        // XA_ST=0,YA_ST=0,ZA_ST=0,
        Data = 0x10;/** 0001 0000    FS_SEL=2 ->±8g*/
        HAL_I2C_Mem_Write (&hi2c1 ,MPU6050_ADDR, ACCEL_CONFIG_REG, 1, &Data, 1, 1000);

    }
    else
    {
        Error_Handler();
    }
}


/**
  * 检测I2C地址的方法
  */
//for(uint8_t i=0 ;i<255;i++)
//{
//    if(HAL_I2C_IsDeviceReady(&hi2c1,i,1,1000) == HAL_OK)
//    {
//        printf("I2C1 addr = %X",i);
//        break;
//    }
//}






/*********************************************************/

/**
  * @brief I2C1 外接板的I2C接口
  * @param None
  * @retval None
  */
void MX_I2C1_Init(void)
{


    hi2c1.Instance = I2C1;
    hi2c1.Init.Timing = 0x00B03FDB;
    hi2c1.Init.OwnAddress1 = 0;
    hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    hi2c1.Init.OwnAddress2 = 0;
    hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
    hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
    if (HAL_I2C_Init(&hi2c1) != HAL_OK)
    {
        Error_Handler();
    }
    /** Configure Analogue filter
    */
    if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
    {
        Error_Handler();
    }
    /** Configure Digital filter
    */
    if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
    {
        Error_Handler();
    }

}

/**
  * @brief I2C2 预留I2C
  * @param None
  * @retval None
  */
void MX_I2C2_Init(void)
{

    /* USER CODE BEGIN I2C2_Init 0 */

    /* USER CODE END I2C2_Init 0 */

    /* USER CODE BEGIN I2C2_Init 1 */

    /* USER CODE END I2C2_Init 1 */
    hi2c2.Instance = I2C2;
    hi2c2.Init.Timing = 0x307075B1;
    hi2c2.Init.OwnAddress1 = 0;
    hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    hi2c2.Init.OwnAddress2 = 0;
    hi2c2.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
    hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
    if (HAL_I2C_Init(&hi2c2) != HAL_OK)
    {
        Error_Handler();
    }
    /** Configure Analogue filter
    */
    if (HAL_I2CEx_ConfigAnalogFilter(&hi2c2, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
    {
        Error_Handler();
    }
    /** Configure Digital filter
    */
    if (HAL_I2CEx_ConfigDigitalFilter(&hi2c2, 0) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN I2C2_Init 2 */

    /* USER CODE END I2C2_Init 2 */

}

/************************************************************************/


/**
* @brief I2C MSP Initialization
* This function configures the hardware resources used in this example
* @param hi2c: I2C handle pointer
* @retval None
*/
void HAL_I2C_MspInit(I2C_HandleTypeDef* hi2c)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    if(hi2c->Instance==I2C1)
    {
        /* USER CODE BEGIN I2C1_MspInit 0 */

        /* USER CODE END I2C1_MspInit 0 */

        __HAL_RCC_GPIOB_CLK_ENABLE();
        /**I2C1 GPIO Configuration
        PB6     ------> I2C1_SCL
        PB7     ------> I2C1_SDA
        */
        GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

        /* Peripheral clock enable */
        __HAL_RCC_I2C1_CLK_ENABLE();

        /* I2C1 interrupt Init */
        HAL_NVIC_SetPriority(I2C1_EV_IRQn, 5, 0);
        HAL_NVIC_EnableIRQ(I2C1_EV_IRQn);
        HAL_NVIC_SetPriority(I2C1_ER_IRQn, 5, 0);
        HAL_NVIC_EnableIRQ(I2C1_ER_IRQn);

    }
    else if(hi2c->Instance==I2C2)
    {
        /* USER CODE BEGIN I2C2_MspInit 0 */

        /* USER CODE END I2C2_MspInit 0 */

        __HAL_RCC_GPIOB_CLK_ENABLE();
        /**I2C2 GPIO Configuration
        PB10     ------> I2C2_SCL
        PB11     ------> I2C2_SDA
        */
        GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_11;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF4_I2C2;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

        /* Peripheral clock enable */
        __HAL_RCC_I2C2_CLK_ENABLE();
        /* USER CODE BEGIN I2C2_MspInit 1 */

        /* USER CODE END I2C2_MspInit 1 */
    }

}
