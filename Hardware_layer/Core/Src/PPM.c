//
// Created by Gideon on 2021/8/8.
//
#include "main.h"

TIM_HandleTypeDef htim2;



/** PPM数据解析：
 * 使用输入捕获。
 * 至少需要有1us的精度。
 * */


/**
  * @brief This function handles TIM2 global interrupt.
  */
void TIM2_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&htim2);
}



/**
* @brief  Input Capture callback in non-blocking mode
* @param  htim TIM IC handle
* @retval None
* 参考了无名飞控PPM.c程序
*/
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
    static uint8_t ppm_begin_flag=0,ppm_cnt=0;
    static uint64_t ppm_last_time=0;//可以一直累加很久不怕数据溢出了。
    static uint64_t ppm_now_time=0;
    static uint32_t ppm_real_time=0;

    static uint16_t PPM_data_buf[8];//8通道

    if( htim->Instance == TIM2 )
    {
        ppm_last_time = ppm_now_time;
        ppm_now_time = HAL_TIM_ReadCapturedValue(&htim2,TIM_CHANNEL_1) + 4294967295*ppm_carry;

        ppm_real_time = ppm_now_time - ppm_last_time;

        if(ppm_begin_flag == 1)
        {
            if(ppm_real_time > 2010 )/*再次检测，大于2ms,标明是ppm尾*/
            {
                ppm_begin_flag = 1;
                ppm_cnt=0;
            }
            else if(ppm_real_time>=990 && ppm_real_time<=2010)//符合要求
            {
                PPM_data_buf[ppm_cnt++] = ppm_real_time;
                if(ppm_cnt>=8)//一次ppm完成
                {
                    ppm_cnt=0;
                    memcpy(PPM_DATA_BUFF,PPM_data_buf,8*sizeof(uint16_t));//打包数据
                }
            }
            else//数据出错，重新检测
            {
                ppm_begin_flag =0;
            }
        }
        else if(ppm_real_time >= 2010)//表明是ppm数据尾了
        {
            ppm_begin_flag = 1;
            ppm_cnt=0;
        }
    }
}


/**
  * @brief TIM2 PPM
  * @param None
  * @retval None
  */
void MX_TIM2_Init(void)
{


    TIM_MasterConfigTypeDef sMasterConfig = {0};
    TIM_IC_InitTypeDef sConfigIC = {0};

    htim2.Instance = TIM2;
    htim2.Init.Prescaler = 240-1;/** 1us */
    htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim2.Init.Period = 4294967295;
    htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    if (HAL_TIM_IC_Init(&htim2) != HAL_OK)
    {
        Error_Handler();
    }
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
    {
        Error_Handler();
    }
    sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;/** 上升沿*/
    sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
    sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
    sConfigIC.ICFilter = 0;
    if (HAL_TIM_IC_ConfigChannel(&htim2, &sConfigIC, TIM_CHANNEL_1) != HAL_OK)
    {
        Error_Handler();
    }


    HAL_TIM_IC_Start_IT(&htim2,TIM_CHANNEL_1);
}

/************************************************************************/


/**
* @brief TIM_IC MSP Initialization
* This function configures the hardware resources used in this example
* @param htim_ic: TIM_IC handle pointer
* @retval None
*/
void HAL_TIM_IC_MspInit(TIM_HandleTypeDef* htim_ic)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    if(htim_ic->Instance==TIM2)
    {

        /* Peripheral clock enable */
        __HAL_RCC_TIM2_CLK_ENABLE();

        __HAL_RCC_GPIOA_CLK_ENABLE();
        /**TIM2 GPIO Configuration
        PA15 (JTDI)     ------> TIM2_CH1
        */
        GPIO_InitStruct.Pin = GPIO_PIN_15;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_PULLDOWN;//Gideon
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF1_TIM2;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        /* TIM2 interrupt Init */
        HAL_NVIC_SetPriority(TIM2_IRQn, 5, 0);
        HAL_NVIC_EnableIRQ(TIM2_IRQn);

    }
    else if(htim_ic->Instance==TIM12)
    {
        /* USER CODE BEGIN TIM12_MspInit 0 */

        /* USER CODE END TIM12_MspInit 0 */
        /* Peripheral clock enable */
        __HAL_RCC_TIM12_CLK_ENABLE();

        __HAL_RCC_GPIOB_CLK_ENABLE();
        /**TIM12 GPIO Configuration
        PB14     ------> TIM12_CH1
        PB15     ------> TIM12_CH2
        */
        GPIO_InitStruct.Pin = GPIO_PIN_14|GPIO_PIN_15;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF2_TIM12;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

        /* USER CODE BEGIN TIM12_MspInit 1 */

        /* USER CODE END TIM12_MspInit 1 */
    }

}

