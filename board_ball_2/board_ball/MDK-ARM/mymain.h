#ifndef _MYMAIN_H
#define _MYMAIN_H

#include "main.h"
#include <stdio.h>
#include "stm32f1xx.h"
#include "stm32f1xx_hal.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "steer.h"
#include "string.h"
#include "pid.h"

#define GAP_TIM htim4 //间隔定时器
#define LOCATION_RECORD_NUM 3 //滤波记录个数
#define VELOCITY_RECORD_NUM 7 //滤波记录个数
#define TIME_INTERVAL 0.01f //位置更新时间
#define Area1_x  13.59f
#define Area1_y  22.81f
#define Area2_x  12.33f
#define Area2_y  40.55f
#define Area3_x  31.68f
#define Area3_y  23.52f
#define Area4_x  30.70f
#define Area4_y  41.69f
#define Area5_x  21.42f
#define Area5_y  32.59f
#define Area6_x  26.55f
#define Area6_y  28.05f

extern float Position_X,Position_Y;//当前位置(滤波前)
extern float Velocity_X,Velocity_Y;

void mymain(void);
void State_Machine(void);
void PID_controll(void);
float Location_Low_Filter(float new_Loc, float *location_Record);
float Velocity_Low_Filter(float new_Spe, float *velocity_Record);
void buffer_hadle_USART1(uint8_t *buffer);
void buffer_hadle_USART3(uint8_t *buffer);
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);

#endif
