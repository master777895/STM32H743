//
// Created by Gideon on 2021/8/8.
//



#ifndef STM32H743_SPI_H
#define STM32H743_SPI_H

#include "main.h"

extern SPI_HandleTypeDef hspi1;
extern SPI_HandleTypeDef hspi2;
extern SPI_HandleTypeDef hspi4;

void MX_SPI1_Init(void);
void MX_SPI2_Init(void);
void MX_SPI4_Init(void);
#endif //STM32H743_SPI_H
