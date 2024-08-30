#ifndef _STEER_H
#define _STEER_H

#include "stm32f1xx.h"
#include "gpio.h"
#include "tim.h"

#define steer_x_channel TIM_CHANNEL_1 //舵机x通道
#define steer_x_tim htim3 //舵机x定时器
#define steer_y_channel TIM_CHANNEL_2 //舵机y通道
#define steer_y_tim htim3 //舵机y定时器

/// @brief 舵机结构体
typedef struct _STEER
{
    float angle;
} STEER;

extern STEER steer_x,steer_y;

void steer_init(void);
void steer_rotate_x(float angle);
void steer_rotate_y(float angle);
	
#endif
