#include "pid.h"

PID pid_speed_1, pid_speed_2, pid_position_1, pid_position_2, pid_line, pid_angle;

void PID_Init(void) // PID参数初始化
{
    pid_speed_1.err = 0;
    pid_speed_1.integral = 0;
    pid_speed_1.maxIntegral = 100000;
    pid_speed_1.maxOutput = 10000;
    pid_speed_1.lastErr = 0;
    pid_speed_1.output = 0;
    pid_speed_1.kp = 80;
    pid_speed_1.ki = 3.7;
    pid_speed_1.kd = 500;
    pid_speed_1.diezone = 0.05;

    pid_position_1.err = 0;
    pid_position_1.integral = 0;
    pid_position_1.maxIntegral = 100000;
    pid_position_1.maxOutput = 10000;
    pid_position_1.lastErr = 0;
    pid_position_1.output = 0;
    pid_position_1.kp = 10; // 这几个宏定义要自己补充
    pid_position_1.ki = 0;
    pid_position_1.kd = 20;
    pid_position_1.diezone = 0.1;

    pid_speed_2.err = 0;
    pid_speed_2.integral = 0;
    pid_speed_2.maxIntegral = 100000;
    pid_speed_2.maxOutput = 10000;
    pid_speed_2.lastErr = 0;
    pid_speed_2.output = 0;
    pid_speed_2.kp = 80;
    pid_speed_2.ki = 3.7;
    pid_speed_2.kd = 500;
    pid_speed_2.diezone = 0.05;

    pid_position_2.err = 0;
    pid_position_2.integral = 0;
    pid_position_2.maxIntegral = 80000;
    pid_position_2.maxOutput = 10000;
    pid_position_2.lastErr = 0;
    pid_position_2.output = 0;
    pid_position_2.kp = 10; // 这几个宏定义要自己补充
    pid_position_2.ki = 0;
    pid_position_2.kd = 1;
    pid_position_2.diezone = 0.1;

    pid_line.err = 0;
    pid_line.integral = 0;
    pid_line.maxIntegral = 100000;
    pid_line.maxOutput = 10000;
    pid_line.lastErr = 0;
    pid_line.output = 0;
    pid_line.kp = 0;
    pid_line.ki = 0;
    pid_line.kd = 0;

    pid_angle.err = 0;
    pid_angle.integral = 0;
    pid_angle.maxIntegral = 100;
    pid_angle.lastErr = 0;
    pid_angle.output = 0;
    pid_angle.maxOutput = 3000;
    pid_angle.kp = 0;
    pid_angle.ki = 0;
    pid_angle.kd = 0;

    motor1.lastCount = 0; // 结构体内容初始化
    motor1.totalCount = 0;
    motor1.overflowNum = 0;
    motor1.direct = 0;
    motor1.speed = 0;
    motor1.speed_target = 0;
    motor1.position = 0;
    motor1.position_target = 0;
    motor1.motor_Out = 0;
    motor2.lastCount = 0; // 结构体内容初始化
    motor2.totalCount = 0;
    motor2.overflowNum = 0;
    motor2.direct = 0;
    motor2.speed = 0;
    motor2.speed_target = 0;
    motor2.position = 0;
    motor2.position_target = 0;
    motor2.motor_Out = 0;

    for (uint8_t i = 0; i < SPEED_RECORD_NUM; i++)
    {
        motor1.speed_Record[i] = 0;
        motor2.speed_Record[i] = 0;
    }

}

float PID_Realize(PID *pid, float target, float feedback) // 一次PID计算
{
    pid->err = target - feedback;
    if (pid->err < pid->diezone && pid->err > -pid->diezone) // 死区处理
    {
        pid->err = 0; // pid死区
        // pid->integral = 0;
    }
    pid->integral += pid->err;

    pid->output = (pid->kp * pid->err) + (pid->ki * pid->integral) + (pid->kd * (pid->err - pid->lastErr)); // 全量式PID

    // 输出限幅
    if (target >= 0) // 正转时
    {
        // if (pid->output < 0)
        //     pid->output = 0;
         if (pid->output > pid->maxOutput)
            pid->output = pid->maxOutput;
    }
     if (target < 0) // 反转时
    {
        if (pid->output < -pid->maxOutput)
            pid->output = -pid->maxOutput;
        // else if (pid->output > 0)
        //     pid->output = 0;
    }

    pid->lastErr = pid->err;
    if ((target == 0)&&(last_order=='V'))
        pid->output = 0; // 刹车时直接输出0
    return pid->output;
}
