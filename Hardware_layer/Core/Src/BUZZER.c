//
// Created by Gideon on 2021/8/8.
//

#include "main.h"

TIM_HandleTypeDef htim16;




/**
 *
 * @param switch_flag
 * @param para_frequency 0~10000
 * @param para_volume 2
 */
void Buzzer_Set(const int8_t switch_flag,const uint16_t para_frequency,const uint16_t para_volume)
{

    if(switch_flag == _BUZZER_ON)
    {
//        HAL_TIM_PWM_Start(&htim16,TIM_CHANNEL_1);
        __HAL_TIM_SET_AUTORELOAD(&htim16,(100000/para_frequency)-1);
        __HAL_TIM_SET_COMPARE(&htim16,TIM_CHANNEL_1,(100000/para_frequency)/2);

    }
    else if(switch_flag == _BUZZER_OFF)
    {
        __HAL_TIM_SET_COMPARE(&htim16,TIM_CHANNEL_1,0);
    }
}
















/**
  * @brief TIM16 蜂鸣器
  * @param None
  * @retval None
  */
void MX_TIM16_Init(void)
{

    /* USER CODE BEGIN TIM16_Init 0 */

    /* USER CODE END TIM16_Init 0 */

    TIM_OC_InitTypeDef sConfigOC = {0};
    TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

    /* USER CODE BEGIN TIM16_Init 1 */

    /* USER CODE END TIM16_Init 1 */
    htim16.Instance = TIM16;
    htim16.Init.Prescaler = 2400-1; ///100000hz
    htim16.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim16.Init.Period = 100-1; ///0~10000 , 100000/100 = 1000Hz
    htim16.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim16.Init.RepetitionCounter = 0;
    htim16.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    if (HAL_TIM_Base_Init(&htim16) != HAL_OK)
    {
        Error_Handler();
    }
    if (HAL_TIM_PWM_Init(&htim16) != HAL_OK)
    {
        Error_Handler();
    }
    sConfigOC.OCMode = TIM_OCMODE_PWM1;
    sConfigOC.Pulse = 0;
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
    sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
    if (HAL_TIM_PWM_ConfigChannel(&htim16, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
    {
        Error_Handler();
    }
    sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
    sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
    sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
    sBreakDeadTimeConfig.DeadTime = 0;
    sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
    sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
    sBreakDeadTimeConfig.BreakFilter = 0;
    sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
    if (HAL_TIMEx_ConfigBreakDeadTime(&htim16, &sBreakDeadTimeConfig) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN TIM16_Init 2 */

    /* USER CODE END TIM16_Init 2 */
    HAL_TIM_MspPostInit(&htim16);

    HAL_TIM_PWM_Start(&htim16,TIM_CHANNEL_1);





    __HAL_TIM_SET_COMPARE(&htim16,TIM_CHANNEL_1,0);



}

/************************************************************************/


/**
* @brief TIM_Base MSP Initialization
* This function configures the hardware resources used in this example
* @param htim_base: TIM_Base handle pointer
* @retval None
*/
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* htim_base)
{
    if(htim_base->Instance==TIM7)///us级别的延时定时器
    {
        /* USER CODE BEGIN TIM7_MspInit 0 */

        /* USER CODE END TIM7_MspInit 0 */
        /* Peripheral clock enable */
        __HAL_RCC_TIM7_CLK_ENABLE();
        /* USER CODE BEGIN TIM7_MspInit 1 */

        /* USER CODE END TIM7_MspInit 1 */
    }
    else if(htim_base->Instance==TIM16)
    {
        /* USER CODE BEGIN TIM16_MspInit 0 */

        /* USER CODE END TIM16_MspInit 0 */
        /* Peripheral clock enable */
        __HAL_RCC_TIM16_CLK_ENABLE();
        /* USER CODE BEGIN TIM16_MspInit 1 */

        /* USER CODE END TIM16_MspInit 1 */
    }

}

/**
* @brief TIM_Base MSP De-Initialization
* This function freeze the hardware resources used in this example
* @param htim_base: TIM_Base handle pointer
* @retval None
*/
void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* htim_base)
{
    if(htim_base->Instance==TIM16)
    {
        /* USER CODE BEGIN TIM16_MspDeInit 0 */

        /* USER CODE END TIM16_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_TIM16_CLK_DISABLE();
        /* USER CODE BEGIN TIM16_MspDeInit 1 */

        /* USER CODE END TIM16_MspDeInit 1 */
    }

}