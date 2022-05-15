/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  *
  *     分为四层设计：分别是：
  *                     APP_layer
  *                     Compute_layer
  *                     System_layer
  *                     Hardware_layer
  *
  * 助记：取值范围
  * uint8_t  0~255
  * uint16_t 0~6,5535
  * uint32_t 0~42,9496,7295
  *
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"




int main(void)
{



    Hardware_layer_Init();/** 硬件层初始化 **/

    Compute_layer_Init();/** 计算层初始化 **/

    System_layer_Init();/** 系统层初始化 必须放在后面**/

    while (1);
}




/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  __disable_irq();/**  关闭所有中断 */
  while (1)
  {
      PWM_pull_all_down();
      printf("Error_Handler\n");
  }
}



#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
