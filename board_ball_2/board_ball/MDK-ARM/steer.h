#ifndef _STEER_H
#define _STEER_H

#include "stm32f1xx.h"
#include "gpio.h"
#include "tim.h"

#define steer_x_channel TIM_CHANNEL_1 
#define steer_x_tim htim3 
#define steer_y_channel TIM_CHANNEL_2 
#define steer_y_tim htim3 

/// @brief 舵机结构体
typedef struct _STEER
{
    float angle;
		float pid_angle;
} STEER;

/// @brief 球状态结构体
typedef struct _BALL
{
    float position_x;
		float position_y;
		float velocity_x;
		float velocity_y;
} BALL;

extern STEER steer_x,steer_y;
extern BALL ball_now,ball_target,ball_last;
extern float x_pulse,y_pulse;

void steer_init(void);
void steer_rotate_x(float angle);
void steer_rotate_y(float angle);
	
#endif
