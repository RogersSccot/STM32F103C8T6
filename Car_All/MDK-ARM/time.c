#include "time.h"

// initial my timer here
void My_TIME_Init(void)
{
    // open the time3 PWM output
    HAL_TIM_PWM_Stop_IT(&htim3, TIM_CHANNEL_1);
    HAL_TIM_PWM_Stop_IT(&htim3, TIM_CHANNEL_2);
    HAL_TIM_PWM_Stop_IT(&htim3, TIM_CHANNEL_3);
    HAL_TIM_PWM_Stop_IT(&htim3, TIM_CHANNEL_4);
    // start the timer4
    HAL_TIM_Base_Start_IT(&htim3); // 开启100ms定时器中断
    HAL_TIM_Base_Start_IT(&htim4); // 开启100ms定时器中断
}

// timer interrupt callback function
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == PWM_TIM.Instance)
    {
        // pulse4_flag1 only used to stepper motor
        if (pulse4_flag1 == 1)
        {
            pulse4_1++;
            if (pulse4_1 == pulse4_set1)
            {
                pulse4_1 = 0;
                pulse4_flag1 = 0;
                pulse4_set1 = 0;
                HAL_TIM_PWM_Stop_IT(&htim3, TIM_CHANNEL_1); // 关闭PWM
            }
        }
    }
    if (htim->Instance == htim4.Instance)
    {
        // make sure the tick_time is not overflow
        // every tick_time = 10ms
        tick_time++;
        if ((last_order == 'V') || (last_order == 'X'))
        {
            // measure the speed
            Brush_Motor_Encode();
            // control the brush motor
            Brush_Motor_Control();
        }
    }
}
