#include "encode.h"

float s1=1.57, s2=1.57;
float p1=0.0156, p2=0.0156;

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

// this function is put into the GAP_TIM, and it will be called every 10ms(100Hz)
void Brush_Motor_Encode(void)
{
    // motor1
    motor1.direct = __HAL_TIM_IS_TIM_COUNTING_DOWN(&ENCODER_1_TIM);        // 如果向上计数（正转），返回值为0，否则返回值为1
    motor1.totalCount = COUNTERNUM_1 + motor1.overflowNum * RELOADVALUE_1; // 一个周期内的总计数值等于目前计数值加上溢出的计数值
    if (motor1.lastCount - motor1.totalCount > 19000)                      // 在计数值溢出时进行防溢出处理
    {
        motor1.overflowNum++;
        motor1.totalCount = COUNTERNUM_1 + motor1.overflowNum * RELOADVALUE_1; // 一个周期内的总计数值等于目前计数值加上溢出的计数值
    }
    else if (motor1.totalCount - motor1.lastCount > 19000) // 在计数值溢出时进行防溢出处理
    {
        motor1.overflowNum--;
        motor1.totalCount = COUNTERNUM_1 + motor1.overflowNum * RELOADVALUE_1; // 一个周期内的总计数值等于目前计数值加上溢出的计数值
    }
    motor1.speed = s1 * (float)(motor1.totalCount - motor1.lastCount);
    motor1.speed = Speed_Low_Filter(motor1.speed, motor1.speed_Record);
    motor1.position = p1 * (float)(motor1.totalCount - 10000); // 得到当前位置 10000编码器脉冲计数的初始值
    motor1.lastCount = motor1.totalCount;

    // motor2
    motor2.direct = __HAL_TIM_IS_TIM_COUNTING_DOWN(&ENCODER_2_TIM);        // 如果向上计数（正转），返回值为0，否则返回值为1
    motor2.totalCount = COUNTERNUM_2 + motor2.overflowNum * RELOADVALUE_2; // 一个周期内的总计数值等于目前计数值加上溢出的计数值
    if (motor2.lastCount - motor2.totalCount > 19000)                      // 在计数值溢出时进行防溢出处理
    {
        motor2.overflowNum++;
        motor2.totalCount = COUNTERNUM_2 + motor2.overflowNum * RELOADVALUE_2; // 一个周期内的总计数值等于目前计数值加上溢出的计数值
    }
    else if (motor2.totalCount - motor2.lastCount > 19000) // 在计数值溢出时进行防溢出处理
    {
        motor2.overflowNum--;
        motor2.totalCount = COUNTERNUM_2 + motor2.overflowNum * RELOADVALUE_2; // 一个周期内的总计数值等于目前计数值加上溢出的计数值
    }
    motor2.speed = s2 * (float)(motor2.totalCount - motor2.lastCount);
    motor2.speed = Speed_Low_Filter(motor2.speed, motor2.speed_Record);
    motor2.position = p2 * (float)(motor2.totalCount - 10000); // 得到当前位置 10000编码器脉冲计数的初始值
    motor2.lastCount = motor2.totalCount;
}
