//
// Created by Gideon on 2021/9/9.
//
#include "main.h"
#include "KEY_Process.h"

/*
 * Copyright (c) 2016 Zibin Zheng <znbin@qq.com>
 * All rights reserved
 */

//链接：https://github.com/0x1abin/MultiButton
//记得添加一个设置一个5ms间隔的定时器循环调用后台处理函数，（在task里面添加也行）


#include "main.h"
#include "KEY_Process.h"



#define EVENT_CB(ev)   if(handle->cb[ev])handle->cb[ev]((Button*)handle)

//button handle list head.
static struct Button* head_handle = NULL;



//Gideon Code begin-----------------------------------//

//这里的callback函数到时候会传一个参数btn（按键句柄）进去，这是例子里面写的，记住就行。
//看看后面的使用就知道了
struct Button button0;
uint8_t read_button0_gpio(void);
void button0_callback(void *btn);

struct Button button1;
uint8_t read_button1_gpio(void);
void button1_callback(void *btn);


void key_init() ///初始化multibutton，记得调用这个函数
{

    button_init(&button1, read_button1_gpio, 	0);//这里面的read_button1_gpio需要绑定电平读取接口
    button_attach(&button1, PRESS_DOWN,       button1_callback);
    button_attach(&button1, PRESS_UP,         button1_callback);
    button_attach(&button1, PRESS_REPEAT,     button1_callback);
    button_attach(&button1, SINGLE_CLICK,     button1_callback);
    button_attach(&button1, DOUBLE_CLICK,     button1_callback);
    button_attach(&button1, LONG_PRESS_START, button1_callback);
    button_attach(&button1, LONG_PRESS_HOLD,  button1_callback);
    button_start(&button1);

    button_init(&button0, read_button0_gpio, 	0);
    button_attach(&button0, SINGLE_CLICK,     button0_callback);
    button_attach(&button0, DOUBLE_CLICK,     button0_callback);
    button_attach(&button0, LONG_PRESS_START, button0_callback);
    button_start(&button0);
}

//绑定电平读取接口
uint8_t read_button1_gpio(void)
{
    return HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_11);
}

uint8_t read_button0_gpio(void)
{
    return HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_10);
}

//callback函数，在这里面再做按键类型的判断
void button1_callback(void *btn)
{
    switch(get_button_event((struct Button*)btn))
    {
        case PRESS_DOWN :
//            printf("KEY1 PRESS_DOWN! \n");
            break;
        case PRESS_UP:
//            printf("KEY1 PRESS_UP! \n");
            break;
        case PRESS_REPEAT:
//            printf("KEY1 PRESS_REPEAT! \n");
            break;
        case SINGLE_CLICK:
//            printf("KEY1 SINGLE_CLICK! \n");
            NOW_PAGE_NUMBER ++;
            break;
        case DOUBLE_CLICK:
//            printf("KEY1 DOUBLE_CLICK! \n");
            break;
        case LONG_PRESS_START:
//            printf("KEY1 LONG_PRESS_START! \n");
            break;
        case LONG_PRESS_HOLD:
//            printf("KEY1 LONG_PRESS_HOLD! \n");
            break;
        default:
            break;

    }

}



void button0_callback(void *btn)
{
    switch(get_button_event((struct Button*)btn))
    {

        case SINGLE_CLICK:
//            printf("KEY2 SINGLE_CLICK! \n");
            NOW_PAGE_NUMBER --;
            break;
        case DOUBLE_CLICK:
//            printf("KEY2 DOUBLE_CLICK! \n");
            break;
        case LONG_PRESS_START:
//            printf("KEY2 LONG_PRESS_START! \n");
            break;
        default:
            break;

    }

}

/*Gideon Code END-----------------------------------*/



/**
  * @brief  Initializes the button struct handle.
  * @param  handle: the button handle strcut.
  * @param  pin_level: read the HAL GPIO of the connet button level.
  * @param  active_level: pressed GPIO level.
  * @retval None
  */
void button_init(struct Button* handle, uint8_t(*pin_level)(), uint8_t active_level)
{
    memset(handle, 0, sizeof(struct Button));
    handle->event = (uint8_t)NONE_PRESS;
    handle->hal_button_Level = pin_level;
    handle->button_level = handle->hal_button_Level();
    handle->active_level = active_level;
}

/**
  * @brief  Attach the button event callback function.
  * @param  handle: the button handle strcut.
  * @param  event: trigger event type.
  * @param  cb: callback function.
  * @retval None
  */
void button_attach(struct Button* handle, PressEvent event, BtnCallback cb)
{
    handle->cb[event] = cb;
}

/**
  * @brief  Inquire the button event happen.
  * @param  handle: the button handle strcut.
  * @retval button event.
  */
PressEvent get_button_event(struct Button* handle)
{
    return (PressEvent)(handle->event);
}

/**
  * @brief  Button driver core function, driver state machine.
  * @param  handle: the button handle strcut.
  * @retval None
  */
void button_handler(struct Button* handle)
{
    uint8_t read_gpio_level = handle->hal_button_Level();

    //ticks counter working..
    if((handle->state) > 0) handle->ticks++;

    /*------------button debounce handle---------------*/
    if(read_gpio_level != handle->button_level) { //not equal to prev one
        //continue read 3 times same new level change
        if(++(handle->debounce_cnt) >= DEBOUNCE_TICKS) {
            handle->button_level = read_gpio_level;
            handle->debounce_cnt = 0;
        }
    } else { //leved not change ,counter reset.
        handle->debounce_cnt = 0;
    }

    /*-----------------State machine-------------------*/
    switch (handle->state) {
        case 0:
            if(handle->button_level == handle->active_level) {	//start press down
                handle->event = (uint8_t)PRESS_DOWN;
                EVENT_CB(PRESS_DOWN);
                handle->ticks = 0;
                handle->repeat = 1;
                handle->state = 1;
            } else {
                handle->event = (uint8_t)NONE_PRESS;
            }
            break;

        case 1:
            if(handle->button_level != handle->active_level) { //released press up
                handle->event = (uint8_t)PRESS_UP;
                EVENT_CB(PRESS_UP);
                handle->ticks = 0;
                handle->state = 2;

            } else if(handle->ticks > LONG_TICKS) {
                handle->event = (uint8_t)LONG_PRESS_START;
                EVENT_CB(LONG_PRESS_START);
                handle->state = 5;
            }
            break;

        case 2:
            if(handle->button_level == handle->active_level) { //press down again
                handle->event = (uint8_t)PRESS_DOWN;
                EVENT_CB(PRESS_DOWN);
                handle->repeat++;
                EVENT_CB(PRESS_REPEAT); // repeat hit
                handle->ticks = 0;
                handle->state = 3;
            } else if(handle->ticks > SHORT_TICKS) { //released timeout
                if(handle->repeat == 1) {
                    handle->event = (uint8_t)SINGLE_CLICK;
                    EVENT_CB(SINGLE_CLICK);
                } else if(handle->repeat == 2) {
                    handle->event = (uint8_t)DOUBLE_CLICK;
                    EVENT_CB(DOUBLE_CLICK); // repeat hit
                }
                handle->state = 0;
            }
            break;

        case 3:
            if(handle->button_level != handle->active_level) { //released press up
                handle->event = (uint8_t)PRESS_UP;
                EVENT_CB(PRESS_UP);
                if(handle->ticks < SHORT_TICKS) {
                    handle->ticks = 0;
                    handle->state = 2; //repeat press
                } else {
                    handle->state = 0;
                }
            }
            break;

        case 5:
            if(handle->button_level == handle->active_level) {
                //continue hold trigger
                handle->event = (uint8_t)LONG_PRESS_HOLD;
                EVENT_CB(LONG_PRESS_HOLD);

            } else { //releasd
                handle->event = (uint8_t)PRESS_UP;
                EVENT_CB(PRESS_UP);
                handle->state = 0; //reset
            }
            break;
    }
}

/**
  * @brief  Start the button work, add the handle into work list.
  * @param  handle: target handle strcut.
  * @retval 0: succeed. -1: already exist.
  */
int button_start(struct Button* handle)
{
    struct Button* target = head_handle;
    while(target) {
        if(target == handle) return -1;	//already exist.
        target = target->next;
    }
    handle->next = head_handle;
    head_handle = handle;
    return 0;
}

/**
  * @brief  Stop the button work, remove the handle off work list.
  * @param  handle: target handle strcut.
  * @retval None
  */
void button_stop(struct Button* handle)
{
    struct Button** curr;
    for(curr = &head_handle; *curr; ) {
        struct Button* entry = *curr;
        if (entry == handle) {
            *curr = entry->next;
//			free(entry);
        } else
            curr = &entry->next;
    }
}

/**
  * @brief  background ticks, timer repeat invoking interval 5ms.
  * @param  None.
  * @retval None
  */
void button_ticks()
{
    static struct Button* target;
    for(target=head_handle; target; target=target->next) {
        button_handler(target);
    }
}












