#ifndef _ENCODE_H_
#define _ENCODE_H_

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

// motor encode define
#define MOTO1_ENCODER1_PORT GPIOA
#define MOTO1_ENCODER1_PIN GPIO_PIN_0
#define MOTO1_ENCODER2_PORT GPIOA
#define MOTO1_ENCODER2_PIN GPIO_PIN_1
#define MOTO2_ENCODER1_PORT GPIOA
#define MOTO2_ENCODER1_PIN GPIO_PIN_8
#define MOTO2_ENCODER2_PORT GPIOA
#define MOTO2_ENCODER2_PIN GPIO_PIN_9

// timer define
#define ENCODER_1_TIM htim2
#define ENCODER_2_TIM htim1
#define PWM_TIM htim3
#define GAP_TIM htim4
#define MOTOR1_TIM htim3
#define MOTOR2_TIM htim3

// motor encode function
#define RELOADVALUE_1 __HAL_TIM_GetAutoreload(&ENCODER_1_TIM) // 获取自动装载值,本例中为20000
#define COUNTERNUM_1 __HAL_TIM_GetCounter(&ENCODER_1_TIM)     // 获取编码器定时器中的计数值
#define RELOADVALUE_2 __HAL_TIM_GetAutoreload(&ENCODER_2_TIM) // 获取自动装载值,本例中为20000
#define COUNTERNUM_2 __HAL_TIM_GetCounter(&ENCODER_2_TIM)     // 获取编码器定时器中的计数值

extern void Brush_Motor_Encode(void);
extern float Speed_Low_Filter(float new_Spe, float *speed_Record);

#endif
