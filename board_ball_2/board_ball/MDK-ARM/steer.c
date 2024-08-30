#include "steer.h"

STEER steer_x,steer_y;//舵机
BALL ball_now,ball_target,ball_last;//当前球和目标球
float x_pulse,y_pulse;

/// @brief 舵机初始化
/// @param  无
void steer_init(void)
{
	steer_x.angle = 0;
	steer_y.angle = 0;
	steer_x.pid_angle = 0;
	steer_y.pid_angle = 0;
}

/// @brief 舵机x控制
/// @param angle 舵机x角度
void steer_rotate_x(float angle)
{
	x_pulse = 500.0f+(angle+80.0f)*2000.0f/180.0f;
	__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,(int)x_pulse);
}

/// @brief 舵机y控制
/// @param angle 舵机y角度
void steer_rotate_y(float angle)
{
	y_pulse = 500.0f+(90.0f-angle+20.0f)*2000.0f/270.0f;
	__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_2,(int)y_pulse);
}
