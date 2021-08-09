//
// Created by Gideon on 2021/8/8.
//

#include "main.h"

TIM_HandleTypeDef htim5;




/**
 * @brief RGB颜色设置
 * @note 数值范围：0~100；越大越亮
 * @param R
 * @param G
 * @param B
 */
void Set_RGB_Brightness(uint16_t R , uint16_t G , uint16_t B )
{
    __HAL_TIM_SET_COMPARE(&htim5,TIM_CHANNEL_1,1000-R*10);
    __HAL_TIM_SET_COMPARE(&htim5,TIM_CHANNEL_2,1000-G*10);
    __HAL_TIM_SET_COMPARE(&htim5,TIM_CHANNEL_3,1000-B*10);
}

/**
 * @note 数值范围：0~100
 * @param PWM_set
 */
void Set_IMU_temperation_ctrl(uint16_t set_PWM)
{
    __HAL_TIM_SET_COMPARE(&htim5,TIM_CHANNEL_4,set_PWM*10);
}


/**
  * @brief TIM5 RGB的PWM1,2,3的初始化，也有温控PWM4的初始化。
  * @param None
  * @retval None
  */
void MX_TIM5_Init(void)
{

    TIM_MasterConfigTypeDef sMasterConfig = {0};
    TIM_OC_InitTypeDef sConfigOC = {0};


    htim5.Instance = TIM5;/** 挂载在APB1时钟线上面 240M */
    htim5.Init.Prescaler = 24-1; //10mHZ
    htim5.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim5.Init.Period = 1000; //10000HZ 周期
    htim5.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim5.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    if (HAL_TIM_PWM_Init(&htim5) != HAL_OK)
    {
        Error_Handler();
    }
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(&htim5, &sMasterConfig) != HAL_OK)
    {
        Error_Handler();
    }
    sConfigOC.OCMode = TIM_OCMODE_PWM1;
    sConfigOC.Pulse = 0;
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    if (HAL_TIM_PWM_ConfigChannel(&htim5, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
    {
        Error_Handler();
    }
    if (HAL_TIM_PWM_ConfigChannel(&htim5, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
    {
        Error_Handler();
    }
    if (HAL_TIM_PWM_ConfigChannel(&htim5, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
    {
        Error_Handler();
    }
    if (HAL_TIM_PWM_ConfigChannel(&htim5, &sConfigOC, TIM_CHANNEL_4) != HAL_OK)
    {
        Error_Handler();
    }

    HAL_TIM_MspPostInit(&htim5);

    HAL_TIM_PWM_Start(&htim5,TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim5,TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim5,TIM_CHANNEL_3);
    HAL_TIM_PWM_Start(&htim5,TIM_CHANNEL_4);

    __HAL_TIM_SET_COMPARE(&htim5,TIM_CHANNEL_1,0);
    __HAL_TIM_SET_COMPARE(&htim5,TIM_CHANNEL_2,0);
    __HAL_TIM_SET_COMPARE(&htim5,TIM_CHANNEL_3,0);
    __HAL_TIM_SET_COMPARE(&htim5,TIM_CHANNEL_4,0);

}

/************************************************************************/


/**
* @brief TIM_OC MSP De-Initialization
* This function freeze the hardware resources used in this example
* @param htim_oc: TIM_OC handle pointer
* @retval None
*/
void HAL_TIM_OC_MspDeInit(TIM_HandleTypeDef* htim_oc)
{
    if(htim_oc->Instance==TIM5)
    {
        /* USER CODE BEGIN TIM5_MspDeInit 0 */

        /* USER CODE END TIM5_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_TIM5_CLK_DISABLE();
        /* USER CODE BEGIN TIM5_MspDeInit 1 */

        /* USER CODE END TIM5_MspDeInit 1 */
    }

}