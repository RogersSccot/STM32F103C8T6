#include "pid.h"
#include "tim.h"
#include "main.h"
PID pid_velocity_x, pid_position_x, pid_velocity_y, pid_position_y; // 速度环PID，位置环PID


/**********************************
 * 功能：PID结构体参数初始化
 * 输入：无
 * 返回：无
 * *******************************/
void PID_Init(void) // PID参数初始化
{
    pid_velocity_x.err = 0;
    pid_velocity_x.integral = 0;
    pid_velocity_x.maxIntegral = 1000;
    pid_velocity_x.lastErr = 0;
    pid_velocity_x.output = 0;
    pid_velocity_x.kp = KP_speed_x;
    pid_velocity_x.ki = KI_speed_x;
    pid_velocity_x.kd = KD_speed_x;

		pid_velocity_y.err = 0;
    pid_velocity_y.integral = 0;
    pid_velocity_y.maxIntegral = 1000;
    pid_velocity_y.lastErr = 0;
    pid_velocity_y.output = 0;
    pid_velocity_y.kp = KP_speed_y;
    pid_velocity_y.ki = KI_speed_y;
    pid_velocity_y.kd = KD_speed_y;

		pid_position_x.err = 0;
    pid_position_x.integral = 0;
    pid_position_x.maxIntegral = 1000;
    pid_position_x.lastErr = 0;
    pid_position_x.output = 0;
    pid_position_x.kp = KP_position_x;
    pid_position_x.ki = KI_position_x;
    pid_position_x.kd = KD_position_x;
		
		pid_position_y.err = 0;
    pid_position_y.integral = 0;
    pid_position_y.maxIntegral = 1000;
    pid_position_y.lastErr = 0;
    pid_position_y.output = 0;
    pid_position_y.kp = KP_position_y;
    pid_position_y.ki = KI_position_y;
    pid_position_y.kd = KD_position_y;
}

/****************************************
 * 作用：速度环PID计算
 * 参数：PID参数结构体地址；目标值；反馈值
 * 返回值：无
 * ****************************************/
float Velocity_PID_Realize(PID *pid, float target, float feedback) // 一次PID计算
{
    pid->err = target - feedback;
    if (pid->err < 0.6 && pid->err > -0.6)
        pid->err = 0; // pid死区
    pid->integral += pid->err;

    //if (pid->ki * pid->integral < -pid->maxIntegral)
        //pid->integral = -pid->maxIntegral / pid->ki; // 积分限幅
    //else if (pid->ki * pid->integral > pid->maxIntegral)
        //pid->integral = pid->maxIntegral / pid->ki;

    pid->output = (pid->kp * pid->err) + (pid->ki * pid->integral) + (pid->kd * (pid->err - pid->lastErr)); // 全量式PID

    pid->lastErr = pid->err;

    return pid->output;
}

float Position_PID_Realize(PID *pid, float target, float feedback) // 一次PID计算
{
    if (pid->err < 0.6 && pid->err > -0.6)
        pid->err = 0; // pid死区
    pid->err = target - feedback;
    pid->integral += pid->err;

    //if (pid->ki * pid->integral < -pid->maxIntegral)
        //pid->integral = -pid->maxIntegral / pid->ki; // 积分限幅
    //else if (pid->ki * pid->integral > pid->maxIntegral)
        //pid->integral = pid->maxIntegral / pid->ki;

    pid->output = (pid->kp * pid->err) + (pid->ki * pid->integral) + (pid->kd * (pid->err - pid->lastErr)); // 全量式PID

    pid->lastErr = pid->err;

    return pid->output;
}


