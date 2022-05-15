//
// Created by Gideon on 2021/8/8.
//

#include "main.h"
#include "ULTRA.h"
TIM_HandleTypeDef htim12;
TIM_HandleTypeDef htim7;///用来进行us级别的计数

uint32_t ultra_carry=0;

float_t ULTRA1_DISTANCE=0;




/**
  * @brief This function handles TIM2 global interrupt.
  */
void TIM8_BRK_TIM12_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&htim12);
}




/**
  * @brief TIM12 Initialization Function
  * @param None
  * @retval None
  */
void MX_TIM12_Init(void)
{

    /* USER CODE BEGIN TIM12_Init 0 */

    /* USER CODE END TIM12_Init 0 */

    TIM_IC_InitTypeDef sConfigIC = {0};

    /* USER CODE BEGIN TIM12_Init 1 */

    /* USER CODE END TIM12_Init 1 */
    htim12.Instance = TIM12;
    htim12.Init.Prescaler = 240-1;
    htim12.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim12.Init.Period = 65535;
    htim12.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim12.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    if (HAL_TIM_IC_Init(&htim12) != HAL_OK)
    {
        Error_Handler();
    }
    sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;///先是上升沿
    sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
    sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
    sConfigIC.ICFilter = 0;
    if (HAL_TIM_IC_ConfigChannel(&htim12, &sConfigIC, TIM_CHANNEL_1) != HAL_OK)
    {
        Error_Handler();
    }
    if (HAL_TIM_IC_ConfigChannel(&htim12, &sConfigIC, TIM_CHANNEL_2) != HAL_OK)
    {
        Error_Handler();
    }

    HAL_TIM_IC_Start_IT(&htim12,TIM_CHANNEL_1);

}












/**
 * @brief 占用CPU型延时
 * @param para_us
 */
void delay_us(const uint16_t para_us)
{
    uint16_t differ=(0xffff- 10) - para_us ;///10是偏置

    __HAL_TIM_SET_COUNTER(&htim7,differ);
    HAL_TIM_Base_Start(&htim7);///启动定时器

    do
    {
        differ=__HAL_TIM_GET_COUNTER(&htim7);	///查询计数器的计数值

    }while(differ < 0xffff-10);

    HAL_TIM_Base_Stop(&htim7);
}


/**
  * @brief TIM7 Initialization Function
  * @param None
  * @retval None
  */
void MX_TIM7_Init(void)///us级别的延时实现
{

    /* USER CODE BEGIN TIM7_Init 0 */

    /* USER CODE END TIM7_Init 0 */

    TIM_MasterConfigTypeDef sMasterConfig = {0};

    /* USER CODE BEGIN TIM7_Init 1 */

    /* USER CODE END TIM7_Init 1 */
    htim7.Instance = TIM7;
    htim7.Init.Prescaler = 240-1; ///1us
    htim7.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim7.Init.Period = 0xffff;
    htim7.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
    if (HAL_TIM_Base_Init(&htim7) != HAL_OK)
    {
        Error_Handler();
    }
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(&htim7, &sMasterConfig) != HAL_OK)
    {
        Error_Handler();
    }




}