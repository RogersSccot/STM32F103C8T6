#include "pid.h"
#include "tim.h"
PID pid_speed_1, pid_speed_2, pid_position_1, pid_position_2, pid_line, pid_angle;

/**********************************
 * 功能：PID结构体参数初始化
 * 输入：无
 * 返回：无
 * *******************************/
void PID_Init(void) // PID参数初始化
{
    pid_speed_1.err = 0;
    pid_speed_1.integral = 0;
    pid_speed_1.maxIntegral = 100000;
    pid_speed_1.maxOutput = 10000; 
    pid_speed_1.lastErr = 0;
    pid_speed_1.output = 0;
    pid_speed_1.kp = KP_speed;
    pid_speed_1.ki = KI_speed;
    pid_speed_1.kd = KD_speed;

    pid_position_1.err = 0;
    pid_position_1.integral = 0;
    pid_position_1.maxIntegral = 80000;
    pid_position_1.maxOutput = 10000; 
    pid_position_1.lastErr = 0;
    pid_position_1.output = 0;
    pid_position_1.kp = KP_position; // 这几个宏定义要自己补充
    pid_position_1.ki = KI_position;
    pid_position_1.kd = KD_position;

    pid_speed_2.err = 0;
    pid_speed_2.integral = 0;
    pid_speed_2.maxIntegral = 100000;
    pid_speed_2.maxOutput = 10000; 
    pid_speed_2.lastErr = 0;
    pid_speed_2.output = 0;
    pid_speed_2.kp = KP_speed;
    pid_speed_2.ki = KI_speed;
    pid_speed_2.kd = KD_speed;

    pid_position_2.err = 0;
    pid_position_2.integral = 0;
    pid_position_2.maxIntegral = 80000;
    pid_position_2.maxOutput = 10000;
    pid_position_2.lastErr = 0;
    pid_position_2.output = 0;
    pid_position_2.kp = KP_position; // 这几个宏定义要自己补充
    pid_position_2.ki = KI_position;
    pid_position_2.kd = KD_position;

    pid_line.err = 0;
    pid_line.integral = 0;
    pid_line.maxIntegral = 100000;
    pid_line.maxOutput = 10000;
    pid_line.lastErr = 0;
    pid_line.output = 0;
    pid_line.kp = KP_line;
    pid_line.ki = KI_line;
    pid_line.kd = KD_line;

    pid_angle.err = 0;
    pid_angle.integral = 0;
    pid_angle.maxIntegral = 100;
    pid_angle.lastErr = 0;
    pid_angle.output = 0;
    pid_angle.maxOutput = 3000;
    pid_angle.kp = KP_angle;
    pid_angle.ki = KI_angle;
    pid_angle.kd = KD_angle;
}

/****************************************
 * 作用：速度环PID计算
 * 参数：PID参数结构体地址；目标值；反馈值
 * 返回值：无
 * ****************************************/
float Speed_PID_Realize(PID *pid, float target, float feedback) // 一次PID计算
{
    pid->err = target - feedback;
    if (pid->err < 0.5 && pid->err > -0.5)
    {
        pid->err = 0; // pid死区
        //pid->integral = 0;
    }
    pid->integral += pid->err;

    pid->output = (pid->kp * pid->err) + (pid->ki * pid->integral) + (pid->kd * (pid->err - pid->lastErr)); // 全量式PID

    // 输出限幅
    if (target >= 0) // 正转时
    {
        if (pid->output < 0)
            pid->output = 0;
        else if (pid->output > pid->maxOutput)
            pid->output = pid->maxOutput;
    }
    else if (target < 0) // 反转时
    {
        if (pid->output < -pid->maxOutput)
            pid->output = -pid->maxOutput;
        else if (pid->output > 0)
            pid->output = 0;
    }

    pid->lastErr = pid->err;
    if (target == 0)
        pid->output = 0; // 刹车时直接输出0
    return pid->output;
}

float Location_PID_Realize(PID *pid, float target, float feedback) // 一次PID计算
{
    pid->err = target - feedback;
    if (pid->err < 0.3 && pid->err > -0.3)
    {
        pid->err = 0;
    } // pid死区

    pid->integral += pid->err;

    if (pid->ki * pid->integral < -pid->maxIntegral)
        pid->integral = -pid->maxIntegral / pid->ki; // 积分限幅
    else if (pid->ki * pid->integral > pid->maxIntegral)
        pid->integral = pid->maxIntegral / pid->ki;

    pid->output = (pid->kp * pid->err) + (pid->ki * pid->integral) + (pid->kd * (pid->err - pid->lastErr)); // 全量式PID

    // 输出限幅
    if (pid->output > pid->maxOutput)
        pid->output = pid->maxOutput;
    if (pid->output < -pid->maxOutput)
        pid->output = -pid->maxOutput;
    if (pid->err < 0.3 && pid->err > -0.3)
    {
        pid->output = 0;
    }
    pid->lastErr = pid->err;
    return pid->output;
}

float Angle_PID_Realize(PID *pid, float error) // 一次PID计算
{
    pid->err = error;
    if (pid->err < 1 && pid->err > -1)
    {
        pid->err = 0;
    } // pid死区

    pid->integral += pid->err;

    if (pid->ki * pid->integral < -pid->maxIntegral)
        pid->integral = -pid->maxIntegral / pid->ki; // 积分限幅
    else if (pid->ki * pid->integral > pid->maxIntegral)
        pid->integral = pid->maxIntegral / pid->ki;

    pid->output = (pid->kp * pid->err) + (pid->ki * pid->integral) + (pid->kd * (pid->err - pid->lastErr)); // 全量式PID

    // 输出限幅
    if (pid->output > pid->maxOutput)
    {
        pid->output = pid->maxOutput;
    }
    if (pid->output < -pid->maxOutput)
    {
        pid->output = -pid->maxOutput;
    }

    if (pid->err < 3 && pid->err > -3)
    {
        pid->err = 0;
        pid->output = 0;
        pid->integral = 0;
    }
    pid->lastErr = pid->err;

    return pid->output;
}

float Line_PID_Realize(PID *pid, float line_error) // 一次PID计算
{
    pid->err = line_error;
    if (pid->err < 0.3 && pid->err > -0.3)
        pid->err = 0; // pid死区
    pid->integral += pid->err;

    if (pid->ki * pid->integral < -pid->maxIntegral)
        pid->integral = -pid->maxIntegral / pid->ki; // 积分限幅
    else if (pid->ki * pid->integral > pid->maxIntegral)
        pid->integral = pid->maxIntegral / pid->ki;

    if (line_error == 0)
        pid->integral = 0; // 刹车时清空i

    pid->output = (pid->kp * pid->err) + (pid->ki * pid->integral) + (pid->kd * (pid->err - pid->lastErr)); // 全量式PID

    // 输出限幅
    if (line_error >= 0) // 正转时
    {
        if (pid->output < 0)
            pid->output = 0;
        else if (pid->output > pid->maxOutput)
            pid->output = pid->maxOutput;
    }
    else if (line_error < 0) // 反转时
    {
        if (pid->output < -pid->maxOutput)
            pid->output = -pid->maxOutput;
        else if (pid->output > 0)
            pid->output = 0;
    }

    pid->lastErr = pid->err;
    if (line_error == 0)
        pid->output = 0; // 刹车时直接输出0
    return pid->output;
}
