#ifndef _PID_H_
#define _PID_H_

#include "stm32f1xx.h"
#include <stdio.h>
// #include "control.h"

// PID三个参数的值
#define KP_speed_x -1.5
#define KI_speed_x -0.01
#define KD_speed_x -0.2
#define KP_speed_y 1.5
#define KI_speed_y 0.01
#define KD_speed_y 0.2
#define KP_position_x 1
#define KI_position_x 0
#define KD_position_x 0.4
#define KP_position_y 1
#define KI_position_y 0
#define KD_position_y 0.4

typedef struct _PID // PID参数结构体
{
    float kp, ki, kd;
    float err, lastErr;
    float integral, maxIntegral; // 积分值
    float output, maxOutput;
} PID;

extern PID pid_velocity_x, pid_position_x, pid_velocity_y, pid_position_y;

void PID_Init(void);
float Velocity_PID_Realize(PID *pid, float target, float feedback); // 一次PID计算
float Position_PID_Realize(PID *pid, float target, float feedback);

#endif
