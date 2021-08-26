//
// Created by Gideon on 2021/8/10.
//

#ifndef STM32H743_W25QXX_H
#define STM32H743_W25QXX_H

/*********************************************************************************************************
*
* File                : W25Qx.h
* Hardware Environment:
* Build Environment   : RealView MDK-ARM  Version: 5.15
* Version             : V1.0
* By                  :
*
*                                  (c) Copyright 2005-2015, WaveShare
*                                       http://www.waveshare.net
*                                          All Rights Reserved
*
*********************************************************************************************************/
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __W25Qx_H
#define __W25Qx_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
//#include "stm32f0xx.h"
#include "main.h"
#include "SPI.h"

#define SPI_Handle_W25Qxx hspi2 //spi½Ó¿Ú



/**
 * @brief  W25Q32FV Configuration
 */

#define W25Q32FV_FLASH_SIZE                  0x2000000 /* 32 MBits => 4MBytes */
#define W25Q32FV_BLOCK_SIZE                  0x10000   /* ¿é 64 blocks of 64K Bytes */
#define W25Q32FV_SECTOR_SIZE                0x1000    /* ÉÈÇø  1024 sectors of 4k Bytes */
#define W25Q32FV_PAGE_SIZE                   0x100     /* Ò³ 16384 pages of 256 bytes */

#define W25Q32FV_DUMMY_CYCLES_READ           4
#define W25Q32FV_DUMMY_CYCLES_READ_QUAD      10

#define W25Q32FV_BULK_ERASE_MAX_TIME         250000
#define W25Q32FV_SECTOR_ERASE_MAX_TIME       3000
#define W25Q32FV_SUBSECTOR_ERASE_MAX_TIME    800
#define W25Qx_TIMEOUT_VALUE 1000







/**
  * @brief  W25QxxFV Commands
  */
/* Reset Operations */
#define RESET_ENABLE_CMD                     0x66
#define RESET_MEMORY_CMD                     0x99

#define ENTER_QPI_MODE_CMD                   0x38
#define EXIT_QPI_MODE_CMD                    0xFF

/* Identification Operations */
#define READ_ID_CMD                          0x90
#define DUAL_READ_ID_CMD                     0x92
#define QUAD_READ_ID_CMD                     0x94
#define READ_JEDEC_ID_CMD                    0x9F

/* Read Operations */
#define READ_CMD                             0x03
#define FAST_READ_CMD                        0x0B
#define DUAL_OUT_FAST_READ_CMD               0x3B
#define DUAL_INOUT_FAST_READ_CMD             0xBB
#define QUAD_OUT_FAST_READ_CMD               0x6B
#define QUAD_INOUT_FAST_READ_CMD             0xEB

/* Write Operations */
#define WRITE_ENABLE_CMD                     0x06
#define WRITE_DISABLE_CMD                    0x04

/* Register Operations */
#define READ_STATUS_REG1_CMD                  0x05
#define READ_STATUS_REG2_CMD                  0x35
#define READ_STATUS_REG3_CMD                  0x15

#define WRITE_STATUS_REG1_CMD                 0x01
#define WRITE_STATUS_REG2_CMD                 0x31
#define WRITE_STATUS_REG3_CMD                 0x11


/* Program Operations */
#define PAGE_PROG_CMD                        0x02
#define QUAD_INPUT_PAGE_PROG_CMD             0x32


/* Erase Operations */
#define SECTOR_ERASE_CMD                     0x20
#define CHIP_ERASE_CMD                       0xC7

#define PROG_ERASE_RESUME_CMD                0x7A
#define PROG_ERASE_SUSPEND_CMD               0x75


/* Flag Status Register */
#define W25Q32FV_FSR_BUSY                    ((uint8_t)0x01)    /*!< busy */
#define W25Q32FV_FSR_WREN                    ((uint8_t)0x02)    /*!< write enable */
#define W25Q32FV_FSR_QE                      ((uint8_t)0x02)    /*!< quad enable */


#define W25Qx_Enable() 			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET)
#define W25Qx_Disable() 		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET)

#define W25Qx_OK            ((uint8_t)0x00)
#define W25Qx_ERROR         ((uint8_t)0x01)
#define W25Qx_BUSY          ((uint8_t)0x02)
#define W25Qx_TIMEOUT				((uint8_t)0x03)


uint8_t BSP_W25Qx_Init(void);
static void	BSP_W25Qx_Reset(void);
static uint8_t BSP_W25Qx_GetStatus(void);
uint8_t BSP_W25Qx_WriteEnable(void);
void BSP_W25Qx_Read_ID(uint8_t *ID);
uint8_t BSP_W25Qx_Read(uint8_t* pData, uint32_t ReadAddr, uint32_t Size);
uint8_t BSP_W25Qx_Write(uint8_t* pData, uint32_t WriteAddr, uint32_t Size);
uint8_t BSP_W25Qx_Erase_Block(uint32_t Address);
uint8_t BSP_W25Qx_Erase_Chip(void);

/**
  * @}
  */

/** @defgroup W25Q128FV_Exported_Functions
  * @{
  */
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* __W25Qx_H */




#endif //STM32H743_W25QXX_H
