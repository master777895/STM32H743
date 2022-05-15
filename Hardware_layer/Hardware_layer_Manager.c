//
// Created by Gideon on 2021/8/7.
//
#include "main.h"

/** Hardware layer ȫ�ֱ������ܴ� */
UART_HandleTypeDef huart1;
SPI_HandleTypeDef hspi1;
SPI_HandleTypeDef hspi2;
SPI_HandleTypeDef hspi4;
uint32_t ppm_carry=0;//�Ժ��Ż������
uint16_t PPM_DATA_BUFF[8];//ppmͨ�����ݡ�
/* USB Device Core handle declaration. */
//USBD_HandleTypeDef hUsbDeviceFS;

/**
 * Ӳ�����ʼ��
 */
void Hardware_layer_Init(void)
{
    /** Reset of all peripherals, Initializes the Flash interface and the Systick. */
    HAL_Init();

    /** Configure the system clock */
    SystemClock_Config();

    /** Initialize all configured peripherals */
    MX_GPIO_Init();

    MX_USART1_UART_Init();
//    MX_USART2_UART_Init();
//    MX_USART3_UART_Init();
//    MX_UART4_Init();
//    MX_UART5_Init();
//    MX_USART6_UART_Init();
//    MX_UART7_Init();

    MX_TIM1_Init();
    MX_TIM2_Init();
//    MX_TIM4_Init();
    MX_TIM5_Init();
    MX_TIM7_Init();
    MX_TIM12_Init();
    MX_TIM16_Init();

    MX_I2C1_Init();
//    MX_I2C2_Init();

//    MX_SPI1_Init();
    MX_SPI2_Init();
    MX_SPI4_Init();

//    MX_ADC1_Init();

//    MX_USB_OTG_FS_PCD_Init();


    MX_USB_DEVICE_Init();



    /** Initialize all devices */
    HAL_Delay(100);

    MPU6050_Init();

    W25Qx_Init();

    bsp_OLED_INIT();

    key_init();

///���������ó�ʼ��


    Set_RGB_Brightness(0,0,0);



//    uint16_t freq0 = 100;

//    while(1)
//    {
//        Buzzer_Set(_BUZZER_ON, freq0, 1);
//        freq0 += 100;
//        HAL_Delay(1000);
//        CDC_Transmit_FS("hello word\n",12);
//        printf("send end\n\n");


//    }



}


