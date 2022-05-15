//
// Created by Gideon on 2021/9/9.
//

#ifndef STM32H743_KEY_PROCESS_H
#define STM32H743_KEY_PROCESS_H

/*
 * Copyright (c) 2016 Zibin Zheng <znbin@qq.com>
 * All rights reserved
 */


#include "stdint.h"
#include "string.h"

//According to your need to modify the constants.
#define TICKS_INTERVAL    10	//ms
#define DEBOUNCE_TICKS    3	//MAX 8
#define SHORT_TICKS       (50 /TICKS_INTERVAL)
#define LONG_TICKS        (1000 /TICKS_INTERVAL)


typedef void (*BtnCallback)(void*);

typedef enum {
    PRESS_DOWN = 0,
    PRESS_UP,
    PRESS_REPEAT,
    SINGLE_CLICK,
    DOUBLE_CLICK,
    LONG_PRESS_START,
    LONG_PRESS_HOLD,
    number_of_event,
    NONE_PRESS
}PressEvent;

typedef struct Button {
    uint16_t ticks;
    uint8_t  repeat : 4;
    uint8_t  event : 4;
    uint8_t  state : 3;
    uint8_t  debounce_cnt : 3;
    uint8_t  active_level : 1;
    uint8_t  button_level : 1;
    uint8_t  (*hal_button_Level)(void);
    BtnCallback  cb[number_of_event];
    struct Button* next;
}Button;

#ifdef __cplusplus
extern "C" {
#endif

void button_init(struct Button* handle, uint8_t(*pin_level)(), uint8_t active_level);
void button_attach(struct Button* handle, PressEvent event, BtnCallback cb);
PressEvent get_button_event(struct Button* handle);
int  button_start(struct Button* handle);
void button_stop(struct Button* handle);
void button_ticks(void);
void key_init(void); //初始化multibutton，记得在main初始化里面调用这个函数



#ifdef __cplusplus
}
#endif






#endif //STM32H743_KEY_PROCESS_H
