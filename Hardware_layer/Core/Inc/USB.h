//
// Created by Gideon on 2021/8/8.
//

#ifndef STM32H743_USB_H
#define STM32H743_USB_H
#include "main.h"
#include "usbd_def.h"

extern PCD_HandleTypeDef hpcd_USB_OTG_FS;
extern USBD_HandleTypeDef hUsbDeviceFS;

void MX_USB_OTG_FS_PCD_Init(void);


#endif //STM32H743_USB_H
