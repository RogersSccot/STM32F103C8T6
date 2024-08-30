#include "steer.h"

STEER steer_x,steer_y;//舵机对象

/// @brief 初始化舵机
/// @param  无
void steer_init(void)
{
	steer_x.angle = 0;
	steer_y.angle = 0;
}

/// @brief 设置舵机x角度
/// @param angle 角度
void steer_rotate_x(float angle)
{
	__HAL_TIM_SET_COMPARE(&steer_x_tim,steer_x_channel,500+angle*11);
}

/// @brief 设置舵机x角度
/// @param angle 角度
void steer_rotate_y(float angle)
{
	__HAL_TIM_SET_COMPARE(&steer_y_tim,steer_y_channel,500+angle*11);
}
