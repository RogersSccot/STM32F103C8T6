#ifndef _PID_H_
#define _PID_H_

#include "stm32f1xx.h"
#include "encoder.h"
#include <stdio.h>
// #include "control.h"

// PID三个参数的值
//#define KP_speed 1.0
//#define KI_speed 0.2
//#define KD_speed 0
#define KP_position 3
#define KI_position 0
#define KD_position 1
#define KP_line 12
#define KI_line 0.1
#define KD_line 1

typedef struct _PID // PID参数结构体
{
    float kp, ki, kd;
    float err, lastErr;
    float integral, maxIntegral; // 积分值
    float output, maxOutput;
} PID;

extern PID pid_speed, pid_position,pid_line;
extern float KP_speed,KI_speed,KD_speed;
void PID_Init(void);
float Speed_PID_Realize(PID *pid, float target, float feedback); // 一次PID计算
float Location_PID_Realize(PID *pid, float target, float feedback);
float Line_PID_Realize(PID *pid, float line_error);
#endif
