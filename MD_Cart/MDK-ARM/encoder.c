#include "encoder.h"
#include "tim.h"
#include "pid.h"

float speed_Record_1[SPEED_RECORD_NUM] = {0};
float speed_Record_2[SPEED_RECORD_NUM] = {0};
float motor1_Out, motor2_Out;
float Now_Position_1, Now_Position_2;
float speed, position;
int ITjishu;
float Target_Speed_1, Target_Speed_2;
float Target_Speed_fixed_1, Target_Speed_fixed_2;
float Target_Position_1, Target_Position_2;
float last_error_1 = 0, last_error_2 = 0;

void Motor_Init(void)
{
    HAL_TIM_Encoder_Start(&ENCODER_1_TIM, TIM_CHANNEL_ALL); // 开启编码器定时器
    __HAL_TIM_ENABLE_IT(&ENCODER_1_TIM, TIM_IT_UPDATE);     // 开启编码器定时器更新中断,防溢出处理
    HAL_TIM_Encoder_Start(&ENCODER_2_TIM, TIM_CHANNEL_ALL); // 开启编码器定时器
    __HAL_TIM_ENABLE_IT(&ENCODER_2_TIM, TIM_IT_UPDATE);     // 开启编码器定时器更新中断,防溢出处理
    HAL_TIM_Base_Start_IT(&GAP_TIM);                        // 开启100ms定时器中断
    HAL_TIM_PWM_Start(&PWM_TIM, TIM_CHANNEL_2);             // 开启PWM
    HAL_TIM_PWM_Start(&PWM_TIM, TIM_CHANNEL_1);             // 开启PWM
    HAL_TIM_PWM_Start(&PWM_TIM, TIM_CHANNEL_3);             // 开启PWM
    HAL_TIM_PWM_Start(&PWM_TIM, TIM_CHANNEL_4);             // 开启PWM
    __HAL_TIM_SET_COUNTER(&ENCODER_1_TIM, 10000);           // 编码器定时器初始值设定为10000
    __HAL_TIM_SET_COUNTER(&ENCODER_2_TIM, 10000);
    motor1.lastCount = 0; // 结构体内容初始化
    motor1.totalCount = 0;
    motor1.overflowNum = 0;
    motor1.speed = 0;
    motor1.direct = 0;
    motor2.lastCount = 0; // 结构体内容初始化
    motor2.totalCount = 0;
    motor2.overflowNum = 0;
    motor2.speed = 0;
    motor2.direct = 0;
}

// 速度滤波
float Speed_Low_Filter(float new_Spe, float *speed_Record)
{
    float sum = 0.0f;
    for (uint8_t i = SPEED_RECORD_NUM - 1; i > 0; i--) // 将现有数据后移一位
    {
        speed_Record[i] = speed_Record[i - 1];
        sum += speed_Record[i - 1];
    }
    speed_Record[0] = new_Spe; // 第一位是新的数据
    sum += new_Spe;
    return sum / SPEED_RECORD_NUM; // 返回均值
}

// 步进电机

void motor1_run(float velocity)
{
    if (velocity < 0)
    {
        //__HAL_TIM_SetAutoreload(&htim3,velocity);		//设置速度快慢
        //	__HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_1, 1.158*velocity);//设置脉冲
        //	__HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_2, 0);//设置脉冲
        Target_Speed_1 = velocity;
    }
    else if (velocity > 0)
    {
        //__HAL_TIM_SetAutoreload(&htim3,-velocity);		//设置速度快慢
        //	__HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_2, -1.158*velocity);//设置脉冲
        //	__HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_1, 0);//设置脉冲
        Target_Speed_1 = velocity;
    }

    else if (velocity == 0)
    {
        //__HAL_TIM_SetAutoreload(&htim3,velocity);		//设置速度快慢
        //	__HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_2, 0);//设置脉冲
        //	__HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_1, 0);//设置脉冲
        Target_Speed_1 = 0;
    }
}

void motor2_run(/*int direction,*/ float velocity)
{
    if (velocity > 0)
    {
        //__HAL_TIM_SetAutoreload(&htim3,velocity);		//设置速度快慢
        //	__HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_3, velocity);//设置脉冲
        //	__HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_4, 0);//设置脉冲
        Target_Speed_2 = velocity;
    }
    else if (velocity < 0)
    {
        //__HAL_TIM_SetAutoreload(&htim3,-velocity);		//设置速度快慢
        //	__HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_4, velocity);//设置脉冲
        //	__HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_3, 0);//设置脉冲
        Target_Speed_2 = velocity;
    }
    else if (velocity == 0)
    {
        //__HAL_TIM_SetAutoreload(&htim3,velocity);		//设置速度快慢
        //	__HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_3, 0);//设置脉冲
        //	__HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_4, 0);//设置脉冲
        Target_Speed_2 = 0;
    }
}


//位置环控制器
void motor_foward(float distance, float velocity)
{
    Target_Position_1 = Target_Position_1 - distance * 200;
    Target_Position_2 = Target_Position_2 - distance * 200;
}

void motor_turnback(float angle, float velocity)
{
    Target_Position_1 = Target_Position_1 - angle * 200;
    Target_Position_2 = Target_Position_2 + angle * 200;
}

// 无陀螺仪左右转
void motor_turnleft(float angle, float velocity)
{
    Target_Speed_1 = -angle * 50;
    Target_Speed_2 = angle * 50;
}

void motor_turnright(float angle, float velocity)
{
    Target_Speed_1 = angle * 50;
    Target_Speed_2 = -angle * 50;
}

// 原来的只有P的巡线
void fix_error(float error, float velocity)
{
    if (velocity > 0)
    {
        Target_Speed_fixed_1 = -error * 20;
        Target_Speed_fixed_2 = 0;
    }
    if (velocity < 0)
    {
        Target_Speed_fixed_2 = -error * 20;
        Target_Speed_fixed_1 = 0;
    }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) // 定时器回调函数，用于计算速度
{
    // Motor_Control(&htim3,speed,position);
    if (htim->Instance == GAP_TIM.Instance) // 间隔定时器中断，是时候计算速度了
    {
        motor1.direct = __HAL_TIM_IS_TIM_COUNTING_DOWN(&ENCODER_1_TIM);        // 如果向上计数（正转），返回值为0，否则返回值为1
        motor1.totalCount = COUNTERNUM_1 + motor1.overflowNum * RELOADVALUE_1; // 一个周期内的总计数值等于目前计数值加上溢出的计数值

        if (motor1.lastCount - motor1.totalCount > 19000) // 在计数值溢出时进行防溢出处理
        {
            motor1.overflowNum++;
            motor1.totalCount = COUNTERNUM_1 + motor1.overflowNum * RELOADVALUE_1; // 一个周期内的总计数值等于目前计数值加上溢出的计数值
        }
        else if (motor1.totalCount - motor1.lastCount > 19000) // 在计数值溢出时进行防溢出处理
        {
            motor1.overflowNum--;
            motor1.totalCount = COUNTERNUM_1 + motor1.overflowNum * RELOADVALUE_1; // 一个周期内的总计数值等于目前计数值加上溢出的计数值
        }

        motor1.speed = (float)(motor1.totalCount - motor1.lastCount) / (4 * MOTOR_SPEED_RERATIO * PULSE_PRE_ROUND) * 3000; // 算得每秒多少转,除以4是因为4倍频
        /*******************在这里添加滤波函数************************/
        motor1.speed = 1.4*Speed_Low_Filter(motor1.speed, speed_Record_1);
        /**********************************************************/
        motor1.lastCount = motor1.totalCount; // 记录这一次的计数值
        /***************************PID速度环**********************************/
        //////////////////////////////////////PID位置环////////////////////////////////////////////////////////
        // Now_Position_1 = (float)(motor1.totalCount-10000);// 得到当前位置 10000编码器脉冲计数的初始值
        Target_Speed_fixed_1 = Line_PID_Realize(&pid_line,error);
        // Target_Speed_1 = Location_PID_Realize(&pid_position,Target_Position_1,Now_Position_1)/3;//位置环 Target_Position是目标位置，自行定义即可
        motor1_Out = Target_Speed_1 + Target_Speed_fixed_1 + Speed_PID_Realize(&pid_speed, Target_Speed_1 + Target_Speed_fixed_1, motor1.speed); // 速度环
        if (motor1_Out >= 0)
        {
            __HAL_TIM_SetCompare(&MOTOR1_TIM, MOTOR1_CHANNEL_FORWARD, 10000);
            __HAL_TIM_SetCompare(&MOTOR1_TIM, MOTOR1_CHANNEL_BACKWARD, 10000 - motor1_Out);
        }
        else
        {
            __HAL_TIM_SetCompare(&MOTOR1_TIM, MOTOR1_CHANNEL_BACKWARD, 10000);
            __HAL_TIM_SetCompare(&MOTOR1_TIM, MOTOR1_CHANNEL_FORWARD, 10000 + motor1_Out);
        }
         //printf("speed1 = %f\r\n",motor1.speed);
         
        ///////////////////////////////////////////////////////////////////////////////////////////
        motor2.direct = __HAL_TIM_IS_TIM_COUNTING_DOWN(&ENCODER_2_TIM);        // 如果向上计数（正转），返回值为0，否则返回值为1
        motor2.totalCount = COUNTERNUM_2 + motor2.overflowNum * RELOADVALUE_2; // 一个周期内的总计数值等于目前计数值加上溢出的计数值

        if (motor2.lastCount - motor2.totalCount > 19000) // 在计数值溢出时进行防溢出处理
        {
            motor2.overflowNum++;
            motor2.totalCount = COUNTERNUM_2 + motor2.overflowNum * RELOADVALUE_2; // 一个周期内的总计数值等于目前计数值加上溢出的计数值
        }
        else if (motor2.totalCount - motor2.lastCount > 19000) // 在计数值溢出时进行防溢出处理
        {
            motor2.overflowNum--;
            motor2.totalCount = COUNTERNUM_2 + motor2.overflowNum * RELOADVALUE_2; // 一个周期内的总计数值等于目前计数值加上溢出的计数值
        }

        motor2.speed = (float)(motor2.totalCount - motor2.lastCount) / (4 * MOTOR_SPEED_RERATIO * PULSE_PRE_ROUND) * 3000; // 算得每秒多少转,除以4是因为4倍频
        /*******************在这里添加滤波函数************************/
        motor2.speed = 2.25*Speed_Low_Filter(motor2.speed, speed_Record_2);
        /**********************************************************/
        motor2.lastCount = motor2.totalCount; // 记录这一次的计数值
        /***************************PID速度环**********************************/
        ///////////////////////////////////PID位置环//////////////////////////////////////////
        // Now_Position_2 = -(float)(motor2.totalCount-10000);// 得到当前位置 10000编码器脉冲计数的初始值
        // Target_Speed_2 = Location_PID_Realize(&pid_position,Target_Position_2,Now_Position_2)/3;//位置环 Target_Position是目标位置，自行定义即可
        Target_Speed_fixed_2 = Line_PID_Realize(&pid_line,error);
        motor2_Out = Target_Speed_2 + Target_Speed_fixed_2+Speed_PID_Realize(&pid_speed, Target_Speed_2 + Target_Speed_fixed_2, motor2.speed); // 速度环
        if (motor2_Out >= 0)
        {
            __HAL_TIM_SetCompare(&MOTOR2_TIM, MOTOR2_CHANNEL_FORWARD, 10000);
            __HAL_TIM_SetCompare(&MOTOR2_TIM, MOTOR2_CHANNEL_BACKWARD, 10000 - motor2_Out);
        }
        else
        {
            __HAL_TIM_SetCompare(&MOTOR2_TIM, MOTOR2_CHANNEL_BACKWARD, 10000);
            __HAL_TIM_SetCompare(&MOTOR2_TIM, MOTOR2_CHANNEL_FORWARD, 10000 + motor2_Out);
        }
				//printf("speed2 = %f\r\n",motor2.speed);
        // printf("channels: %f,%f,%f,%f\n",Now_Position_1,Target_Position_1,Now_Position_2,Target_Position_2);
				printf("channels: %f,%f\n",motor1.speed,Target_Speed_1+Target_Speed_fixed_1);
    }
}
