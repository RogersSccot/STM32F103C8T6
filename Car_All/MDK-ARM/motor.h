#ifndef _MOTOR_H_
#define _MOTOR_H_

#include "main.h"
#include "usart.h"
#include "gpio.h"
#include "stdio.h"
#include "uart.h"
#include "string.h"
#include "stm32f1xx.h"
#include "tim.h"
#include "encode.h"
#include "time.h"
#include "pid.h"

#define SPEED_RECORD_NUM 40 // 25Hz

extern void Brush_Motor_Control(void);
extern void Step_Moter_Init(void);
extern void Steer_Engine_Init(void);
extern void Brush_Moter_Init(void);

typedef struct _Motor
{
    int32_t lastCount;     // 上一次计数值
    int32_t totalCount;    // 总计数值
    int16_t overflowNum;   // 溢出次数
    uint8_t direct;        // direct
    float speed;           // 电机转速
    float speed_target;    // 电机转速
    float position;        // motor position
    float position_target; // motor target position
    float speed_Record[SPEED_RECORD_NUM];
    float motor_Out;       // 电机输出
} Motor;

extern Motor motor1;
extern Motor motor2;

#endif
