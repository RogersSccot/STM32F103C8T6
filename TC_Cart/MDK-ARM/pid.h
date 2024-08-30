#ifndef _PID_H_
#define _PID_H_

#include "stm32f1xx.h"
#include "encoder.h"
#include <stdio.h>
// #include "control.h"

// PID三个参数的值
#define KP_speed 100.0
#define KI_speed 7.0
#define KD_speed 5.0
#define KP_position 0.5
#define KI_position 0
#define KD_position 0.3
#define KP_line 10
#define KI_line 0
#define KD_line 0
#define KP_angle 0.2
#define KI_angle 0
#define KD_angle 0.05

typedef struct _PID // PID参数结构体
{
    float kp, ki, kd;
    float err, lastErr;
    float integral, maxIntegral; // 积分值
    float output, maxOutput;
} PID;

extern PID pid_speed_1,pid_speed_2,pid_position_1,pid_position_2,pid_line,pid_angle;

void PID_Init(void);
float Speed_PID_Realize(PID *pid, float target, float feedback); // 一次PID计算
float Angle_PID_Realize(PID *pid, float error);
float Location_PID_Realize(PID *pid, float target, float feedback);
float Line_PID_Realize(PID *pid, float line_error);
#endif
