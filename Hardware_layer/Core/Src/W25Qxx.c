//
// Created by Gideon on 2021/8/10.
//

#include "main.h"

/*********************************************************************************************************
*
* File                : ws_W25Qx.c
* Hardware Environment:
* Build Environment   : RealView MDK-ARM  Version: 4.20
* Version             : V1.0
* By                  :
*
*                                  (c) Copyright 2005-2011, WaveShare
*                                       http://www.waveshare.net
*                                          All Rights Reserved
*
*********************************************************************************************************/





///测试程序
//uint8_t data1_test[20];
//W25Qxx_Read_sector(_TEST_SECTOR, 10,data1_test);
//printf("Read_sector_1: %X %X %X %X %X %X %X %X %X %X\n",data1_test[0],data1_test[1],data1_test[2],data1_test[3],
//data1_test[4],data1_test[5],data1_test[6],data1_test[7],data1_test[8],data1_test[9]);
//
//uint8_t data_test[]={0x01,0x02,0x03,0x03,0x05,0x06,0x07,0x08,0x09};
//
//W25Qxx_Erase_sector(_TEST_SECTOR);
//W25Qxx_Write_sector(_TEST_SECTOR, 9, data_test);
//
//W25Qxx_Read_sector(_TEST_SECTOR, 10,data1_test);
//printf("Read_sector_2: %X %X %X %X %X %X %X %X %X %X\n",data1_test[0],data1_test[1],data1_test[2],data1_test[3],
//data1_test[4],data1_test[5],data1_test[6],data1_test[7],data1_test[8],data1_test[9]);











/**
 * 初始化
 */
void W25Qx_Init(void)
{
    uint8_t ID[10]={0};
    uint8_t i=0;

    BSP_W25Qx_Init();//初始化W25Q32


    BSP_W25Qx_Read_ID(ID);//读取ID

//    printf("ID: %X  %X\n",ID[0], ID[1]);



    ///ID打印
    if((ID[0] != 0xEF) || (ID[1] != 0x15))
    {
        Error_Handler();//如果 ID不对打印错误
    }
    else//ID正确，打印ID
    {
;
//        printf("W25Q32 ID : ");
//        for(i=0;i<2;i++)
//        {
//            printf("0x%02X ",ID[i]);
//        }
//        printf("\r\n\r\n");
    }

}













/**
 *
 * @param sector_number: 0~1023
 * @param data_size: 1~4096
 * @param data_buf
 */
void W25Qxx_Write_sector(uint16_t sector_number, uint16_t data_size ,uint8_t *data_buf)
{
    if(BSP_W25Qx_Write(data_buf,sector_number*W25Q32FV_SECTOR_SIZE, data_size) == W25Qx_OK)
    {
//        printf("write_success\n");
    }
}





/**
 *
 * @param sector_number :0~1023
 * @param data_size: 0~4096
 * @param data_buf
 */
void W25Qxx_Read_sector(uint16_t sector_number, uint16_t data_size ,uint8_t *data_buf)
{


    if(BSP_W25Qx_Read(data_buf,0+(sector_number*W25Q32FV_SECTOR_SIZE), data_size) == W25Qx_OK)
    {
//      printf("读取扇区数据成功！\n");
    }
    else
        Error_Handler();

}


/**
 *
 * @param sector_number :0~1023
 */
void W25Qxx_Erase_sector(uint16_t sector_number)
{

    if(BSP_W25Qx_Erase_Block(0+sector_number * W25Q32FV_SECTOR_SIZE) == W25Qx_OK)
    {
//        printf(" QSPI Erase sector ok\r\n");
    }
    else
        Error_Handler();

}






/**
  * @brief  Initializes the W25Q128FV interface.
  * @retval None
  */
uint8_t BSP_W25Qx_Init(void)
{
    /* Reset W25Qxxx */
    BSP_W25Qx_Reset();

    return BSP_W25Qx_GetStatus();
}

/**
  * @brief  This function reset the W25Qx.
  * @retval None
  */
static void	BSP_W25Qx_Reset(void)
{
    uint8_t cmd[2] = {RESET_ENABLE_CMD,RESET_MEMORY_CMD};

    int8_t retu=10;

    W25Qx_Enable();

    /* Send the reset command */
    retu = HAL_SPI_Transmit(&SPI_Handle_W25Qxx, cmd, 2, W25Qx_TIMEOUT_VALUE);
    W25Qx_Disable();

}

/**
  * @brief  Reads current status of the W25Q32FV.
  * @retval W25Q128FV memory status
  */
static uint8_t BSP_W25Qx_GetStatus(void)
{
    uint8_t cmd[] = {READ_STATUS_REG1_CMD};//0x05; 0000 0101
    uint8_t status;

    W25Qx_Enable();
    /* Send the read status command */
    HAL_SPI_Transmit(&SPI_Handle_W25Qxx, cmd, 1, W25Qx_TIMEOUT_VALUE);
    /* Reception of the data */
    HAL_SPI_Receive(&SPI_Handle_W25Qxx,&status, 1, W25Qx_TIMEOUT_VALUE);
    W25Qx_Disable();

    /* Check the value of the register */
    if((status & W25Q32FV_FSR_BUSY) != 0)
    {
        return W25Qx_BUSY;
    }
    else
    {
        return W25Qx_OK;
    }
}

/**
  * @brief  This function send a Write Enable and wait it is effective.
  * @retval None
  */
uint8_t BSP_W25Qx_WriteEnable(void)
{
    uint8_t cmd[] = {WRITE_ENABLE_CMD};
    uint32_t tickstart = HAL_GetTick();

    /*Select the FLASH: Chip Select low */
    W25Qx_Enable();
    /* Send the read ID command */
    HAL_SPI_Transmit(&SPI_Handle_W25Qxx, cmd, 1, W25Qx_TIMEOUT_VALUE);
    /*Deselect the FLASH: Chip Select high */
    W25Qx_Disable();

    /* Wait the end of Flash writing */
    while(BSP_W25Qx_GetStatus() == W25Qx_BUSY)
    {
        /* Check for the Timeout */
        if((HAL_GetTick() - tickstart) > W25Qx_TIMEOUT_VALUE)
        {
            return W25Qx_TIMEOUT;
        }
    }

    return W25Qx_OK;
}

/**
  * @brief  Read Manufacture/Device ID.
	* @param  return value address
  * @retval None
  */
void BSP_W25Qx_Read_ID(uint8_t *ID)
{
    uint8_t cmd[4] = {READ_ID_CMD,0x00,0x00,0x00};

    W25Qx_Enable();
    /* Send the read ID command */
    HAL_SPI_Transmit(&SPI_Handle_W25Qxx, cmd, 4, W25Qx_TIMEOUT_VALUE);
    /* Reception of the data */
    HAL_SPI_Receive(&SPI_Handle_W25Qxx,ID, 2, W25Qx_TIMEOUT_VALUE);
    W25Qx_Disable();

}

/**
  * @brief  Reads an amount of data from the QSPI memory.
  * @param  pData: Pointer to data to be read
  * @param  ReadAddr: Read start address
  * @param  Size: Size of data to read
  * @retval QSPI memory status
  */
uint8_t BSP_W25Qx_Read(uint8_t* pData, uint32_t ReadAddr, uint32_t Size)
{
    uint8_t cmd[4];

    /* Configure the command */
    cmd[0] = READ_CMD;
    cmd[1] = (uint8_t)(ReadAddr >> 16);
    cmd[2] = (uint8_t)(ReadAddr >> 8);
    cmd[3] = (uint8_t)(ReadAddr);

    W25Qx_Enable();
    /* Send the read ID command */
    HAL_SPI_Transmit(&SPI_Handle_W25Qxx, cmd, 4, W25Qx_TIMEOUT_VALUE);
    /* Reception of the data */
    if (HAL_SPI_Receive(&SPI_Handle_W25Qxx, pData,Size,W25Qx_TIMEOUT_VALUE) != HAL_OK)
    {
        return W25Qx_ERROR;
    }
    W25Qx_Disable();
    return W25Qx_OK;
}

/**
  * @brief  可以跨页来写，
  * @param  pData: Pointer to data to be written
  * @param  WriteAddr: Write start address，必须是每个扇区的开始地址
  * @param  Size: Size of data to write
  * @retval QSPI memory status
  */
uint8_t BSP_W25Qx_Write( uint8_t* pData, const uint32_t WriteAddr, uint32_t Size)
{

    uint8_t cmd[4];
    uint32_t end_addr, current_size=0, current_addr=0;
    uint16_t factor_addr=0;
    uint32_t tickstart = HAL_GetTick();


    while(Size > 256)
    {
        Size -= 256;

        current_addr = WriteAddr + factor_addr*W25Q32FV_PAGE_SIZE;///递增地址，以页为单位
        current_size = W25Q32FV_PAGE_SIZE;
        pData += factor_addr*W25Q32FV_PAGE_SIZE;///递增数据指针，以页为单位

        /* Configure the command */
        cmd[0] = PAGE_PROG_CMD;
        cmd[1] = (uint8_t)(current_addr >> 16);
        cmd[2] = (uint8_t)(current_addr >> 8);
        cmd[3] = (uint8_t)(current_addr);


        /* Enable write operations */
        if(BSP_W25Qx_WriteEnable() != HAL_OK)
        {
            Error_Handler();
        }
        W25Qx_Enable();
        /* Send the command */
        if (HAL_SPI_Transmit(&SPI_Handle_W25Qxx,cmd, 4, W25Qx_TIMEOUT_VALUE) != HAL_OK)
        {
            return W25Qx_ERROR;
        }

        /* Transmission of the data */
        if (HAL_SPI_Transmit(&SPI_Handle_W25Qxx, pData,current_size, W25Qx_TIMEOUT_VALUE) != HAL_OK)
        {
            return W25Qx_ERROR;
        }
        W25Qx_Disable();
        factor_addr++;
    }

    ///处理余数
    current_addr = WriteAddr + factor_addr*W25Q32FV_PAGE_SIZE;
    pData += factor_addr*W25Q32FV_PAGE_SIZE;
    current_size = Size % W25Q32FV_PAGE_SIZE;

    /* Configure the command */
    cmd[0] = PAGE_PROG_CMD;
    cmd[1] = (uint8_t)(current_addr >> 16);
    cmd[2] = (uint8_t)(current_addr >> 8);
    cmd[3] = (uint8_t)(current_addr);

    BSP_W25Qx_WriteEnable();
    W25Qx_Enable();
    /* Send the command */
    if (HAL_SPI_Transmit(&SPI_Handle_W25Qxx,cmd, 4, W25Qx_TIMEOUT_VALUE) != HAL_OK)
    {
        return W25Qx_ERROR;
    }

    /* Transmission of the data */
    if (HAL_SPI_Transmit(&SPI_Handle_W25Qxx, pData,current_size, W25Qx_TIMEOUT_VALUE) != HAL_OK)
    {
        return W25Qx_ERROR;
    }
    W25Qx_Disable();

    /* Wait the end of Flash writing */
    while(BSP_W25Qx_GetStatus() == W25Qx_BUSY)
    {
        /* Check for the Timeout */
        if((HAL_GetTick() - tickstart) > W25Qx_TIMEOUT_VALUE)
        {
            return W25Qx_TIMEOUT;
        }
    }

    return W25Qx_OK;
}

/**
  * @brief  Erases the specified block of the QSPI memory.
  * @param  BlockAddress: Block address to erase
  * @retval QSPI memory status
  */
uint8_t BSP_W25Qx_Erase_Block(uint32_t Address)
{
    uint8_t cmd[4];
    uint32_t tickstart = HAL_GetTick();
    cmd[0] = SECTOR_ERASE_CMD;
    cmd[1] = (uint8_t)(Address >> 16);
    cmd[2] = (uint8_t)(Address >> 8);
    cmd[3] = (uint8_t)(Address);

    /* Enable write operations */
    BSP_W25Qx_WriteEnable();

    /*Select the FLASH: Chip Select low */
    W25Qx_Enable();
    /* Send the read ID command */
    HAL_SPI_Transmit(&SPI_Handle_W25Qxx, cmd, 4, W25Qx_TIMEOUT_VALUE);
    /*Deselect the FLASH: Chip Select high */
    W25Qx_Disable();

    /* Wait the end of Flash writing */
    while(BSP_W25Qx_GetStatus() == W25Qx_BUSY)
    {
        /* Check for the Timeout */
        if((HAL_GetTick() - tickstart) > W25Q32FV_SECTOR_ERASE_MAX_TIME)
        {
            return W25Qx_TIMEOUT;
        }
    }
    return W25Qx_OK;
}

/**
  * @brief  Erases the entire QSPI memory.This function will take a very long time.
  * @retval QSPI memory status
  */
uint8_t BSP_W25Qx_Erase_Chip(void)
{
    uint8_t cmd[4];
    uint32_t tickstart = HAL_GetTick();
    cmd[0] = SECTOR_ERASE_CMD;

    /* Enable write operations */
    BSP_W25Qx_WriteEnable();

    /*Select the FLASH: Chip Select low */
    W25Qx_Enable();
    /* Send the read ID command */
    HAL_SPI_Transmit(&SPI_Handle_W25Qxx, cmd, 1, W25Qx_TIMEOUT_VALUE);
    /*Deselect the FLASH: Chip Select high */
    W25Qx_Disable();

    /* Wait the end of Flash writing */
    while(BSP_W25Qx_GetStatus() == W25Qx_BUSY)
    {
        /* Check for the Timeout */
        if((HAL_GetTick() - tickstart) > W25Q32FV_BULK_ERASE_MAX_TIME)
        {
            return W25Qx_TIMEOUT;
        }
    }
    return W25Qx_OK;
}



