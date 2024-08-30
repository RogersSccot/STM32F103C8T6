#ifndef _PID_H_
#define _PID_H_

#include "main.h"
#include "usart.h"
#include "gpio.h"
#include "stdio.h"
#include "uart.h"
#include "string.h"
#include "stm32f1xx.h"
#include "tim.h"
#include "time.h"
#include "motor.h"

typedef struct _PID // PID参数结构体
{
    float kp, ki, kd;
    float err, lastErr;
    float integral, maxIntegral; // 积分值
    float output, maxOutput;
    float diezone;
} PID;

extern PID pid_speed_1,pid_speed_2,pid_position_1,pid_position_2,pid_line,pid_angle;

extern void PID_Init(void);
extern float PID_Realize(PID *pid, float target, float feedback); // 一次PID计算

#endif
