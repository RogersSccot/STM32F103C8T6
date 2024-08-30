#include "motor.h"

Motor motor1;
Motor motor2;

// Step Moter init
void Step_Moter_Init(void)
{
    // open the time3 PWM output
    HAL_TIM_PWM_Stop_IT(&htim3, TIM_CHANNEL_1);
    HAL_TIM_PWM_Stop_IT(&htim3, TIM_CHANNEL_2);
    HAL_TIM_PWM_Stop_IT(&htim3, TIM_CHANNEL_3);
    HAL_TIM_PWM_Stop_IT(&htim3, TIM_CHANNEL_4);
    // set the speed of moter, f higher, speed higher
    __HAL_TIM_SetAutoreload(&htim3, 1000 - 1);
    // PWM will be open in the Data_Handle1 function
}

// Step Moter init
void Steer_Engine_Init(void)
{
    // set the load num, f = 50Hz, T = 20ms
    __HAL_TIM_SetAutoreload(&htim3, 20000 - 1);
    // open the PWM
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
    // close the PWM
    HAL_TIM_PWM_Stop_IT(&htim3, TIM_CHANNEL_3);
    HAL_TIM_PWM_Stop_IT(&htim3, TIM_CHANNEL_4);
}

// Brush Moter Init
void Brush_Moter_Init(void)
{
    // open the encoder timer
    HAL_TIM_Encoder_Start(&htim1, TIM_CHANNEL_ALL); // 开启编码器定时器
    __HAL_TIM_ENABLE_IT(&htim1, TIM_IT_UPDATE);     // 开启编码器定时器更新中断,防溢出处理
    HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL); // 开启编码器定时器
    __HAL_TIM_ENABLE_IT(&htim2, TIM_IT_UPDATE);     // 开启编码器定时器更新中断,防溢出处理
    __HAL_TIM_SET_COUNTER(&ENCODER_1_TIM, 10000);   // 编码器定时器初始值设定为
    __HAL_TIM_SET_COUNTER(&ENCODER_2_TIM, 10000);
    motor1.lastCount = COUNTERNUM_1 + motor1.overflowNum * RELOADVALUE_1; // 结构体内容初始化
    motor1.totalCount = COUNTERNUM_1 + motor1.overflowNum * RELOADVALUE_1;
    motor2.lastCount = COUNTERNUM_2 + motor2.overflowNum * RELOADVALUE_2; // 结构体内容初始化
    motor2.totalCount = COUNTERNUM_2 + motor2.overflowNum * RELOADVALUE_2;
    // set frequent to 100Hz
    __HAL_TIM_SetAutoreload(&htim3, 10000 - 1);
    // open the PWM
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_4);
}

// this function is put into the GAP_TIM, and it will be called every 10ms(100Hz)
void Brush_Motor_Control(void)
{
    if (last_order == 'X')
    {
        motor1.speed_target = PID_Realize(&pid_position_1, motor1.position_target, motor1.position); // 位置环
        motor2.speed_target = PID_Realize(&pid_position_2, motor2.position_target, motor2.position); // 位置环
        motor1.motor_Out = PID_Realize(&pid_speed_1, motor1.speed_target, motor1.speed); // 速度环
        motor2.motor_Out = PID_Realize(&pid_speed_2, motor2.speed_target, motor2.speed); // 速度环
    }
    if (last_order == 'V')
    {
        motor1.motor_Out = PID_Realize(&pid_speed_1, motor1.speed_target, motor1.speed); // 速度环
        motor2.motor_Out = PID_Realize(&pid_speed_2, motor2.speed_target, motor2.speed); // 速度环
    }
    if (motor1.motor_Out >= 0)
    {
        __HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_1, 10000 - motor1.motor_Out);
        __HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_2, 10000);
    }
    else
    {
        __HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_1, 10000);
        __HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_2, 10000 + motor1.motor_Out);
    }
    if (motor2.motor_Out >= 0)
    {
        __HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_4, 10000 - motor2.motor_Out);
        __HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_3, 10000);
    }
    else
    {
        __HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_4, 10000);
        __HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_3, 10000 + motor2.motor_Out);
    }
}
