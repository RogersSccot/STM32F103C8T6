/*
 * Copyright (c) 2021, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "ti/driverlib/dl_gpio.h"
#include "ti_msp_dl_config.h"
#include "stdio.h"
#include <stdint.h>
#include "string.h"
#include "empty.h"

#define DELAY (16000000)
//串口用
uint8_t gEchoData;
uint8_t gEchoData_0[127];
uint8_t gEchoData_1[127];
uint8_t gEchoData_2[127];
uint8_t UART_Count_0 = 0;
uint8_t UART_Count_1 = 0;
uint8_t UART_Count_2 = 0;

//状态机用
uint8_t mission_flag = 0;
uint8_t mission1_end_flag = 0;
uint8_t mission2_end_flag = 0;
uint8_t mission3_end_flag = 0;
uint8_t mission4_end_flag = 0;
uint8_t car_position_flag;

//PID用
int velocity_left = 0;
int velocity_right = 0;
int velocity_left_read = 0;
int velocity_right_read = 0;
int velocity_left_filtered = 0;
int velocity_right_filtered = 0;
uint8_t Target_Speed_Left,Target_Speed_Right;
uint8_t Target_Speed_Line_Fix_Left,Target_Speed_Line_Fix_Right;
uint8_t Target_Position_Left,Target_Position_Right;
uint8_t Now_Speed_Left,Now_Speed_Right;
uint8_t Now_Position_Left,Now_Position_Right;

LINE line;

volatile uint32_t cnt1 = 0;
volatile uint32_t cnt2 = 0;
volatile uint32_t cnt1_last = 0;
volatile uint32_t cnt2_last = 0;
bool dir1=false;
bool dir2=false;

float speed_Record_Left[SPEED_RECORD_NUM] = {0};
float speed_Record_Right[SPEED_RECORD_NUM] = {0};

float angle_z,angle_z_last,angle_z_all,angle_error,Target_Angle;
uint8_t jy901_of_count;

int fputc(int c,FILE* stream)
{
    DL_UART_Main_transmitDataBlocking(UART_0_INST,c);
    return c;
}

int fputs(const char* restrict s,FILE* restrict stream)
{
    uint16_t i,len;
    len = strlen(s);
    for(i=0;i<len;i++)
    {
        DL_UART_Main_transmitDataBlocking(UART_0_INST,s[i]);
    }
    return len;
}

int puts(const char* _ptr)
{
    int count = fputs(_ptr,stdout);
    count += fputs("\n",stdout);
    return count;
}

int main(void)
{
    SYSCFG_DL_init();
    NVIC_EnableIRQ(TIMER_0_INST_INT_IRQN);
    DL_TimerG_startCounter(TIMER_0_INST);
    DL_TimerG_startCounter(PWM_0_INST);

    NVIC_ClearPendingIRQ(UART_0_INST_INT_IRQN);
        NVIC_EnableIRQ(UART_0_INST_INT_IRQN);
        NVIC_ClearPendingIRQ(UART_1_INST_INT_IRQN);
        NVIC_EnableIRQ(UART_1_INST_INT_IRQN);
        NVIC_ClearPendingIRQ(UART_2_INST_INT_IRQN);
        NVIC_EnableIRQ(UART_2_INST_INT_IRQN);

        NVIC_EnableIRQ(CAPTURE_0_INST_INT_IRQN);
        DL_TimerG_startCounter(CAPTURE_0_INST);
        NVIC_EnableIRQ(CAPTURE_1_INST_INT_IRQN);
        DL_TimerG_startCounter(CAPTURE_1_INST);

    while (1)
    {
        printf("cnt1:%d\n",cnt1);
        printf("cnt2:%d\n",cnt2);
        printf("mission:%d",mission_flag);
        delay_cycles(DELAY/4);

//        if (DL_GPIO_readPins(GPIO_BUTTON_PORT, GPIO_BUTTON_PIN_BUTTON_1_PIN))
//                {
//                     DL_GPIO_setPins(GPIO_LED_PORT, GPIO_LED_PIN_LED_1_PIN);
//                     DL_TimerA_setCaptureCompareValue(PWM_0_INST, 249, DL_TIMER_CC_0_INDEX);
//                     DL_TimerA_setCaptureCompareValue(PWM_0_INST, 1000, DL_TIMER_CC_1_INDEX);
//                }
//        else
//        {
//            DL_TimerA_setCaptureCompareValue(PWM_0_INST, 1000, DL_TIMER_CC_0_INDEX);
//            DL_TimerA_setCaptureCompareValue(PWM_0_INST, 249, DL_TIMER_CC_1_INDEX);
//        }
        //delay_cycles(DELAY);
        //printf("Hello%d",100);
                //DL_GPIO_togglePins(GPIO_LED_PORT, GPIO_LED_PIN_LED_1_PIN);
//        if (DL_GPIO_readPins(GPIO_BUTTON_PORT, GPIO_BUTTON_PIN_BUTTON_1_PIN))
//        {
//             DL_GPIO_setPins(GPIO_LED_PORT, GPIO_LED_PIN_LED_1_PIN);
//        }
//        else
//        {
//             DL_GPIO_clearPins(GPIO_LED_PORT, GPIO_LED_PIN_LED_1_PIN);
//        }
    }
}

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

//间隔定时器，执行PID和获取巡线参数
void TIMER_0_INST_IRQHandler(void)
{
    switch (DL_TimerG_getPendingInterrupt(TIMER_0_INST)) {
        case DL_TIMER_IIDX_ZERO:
            //在这里写你间隔定时器的程序
            line.line_1 = DL_GPIO_readPins(GPIO_LINE_PIN_LINE_1_PORT, GPIO_LINE_PIN_LINE_1_PIN );
            line.line_2 = DL_GPIO_readPins(GPIO_LINE_PIN_LINE_2_PORT, GPIO_LINE_PIN_LINE_1_PIN );
            line.line_3 = DL_GPIO_readPins(GPIO_LINE_PIN_LINE_3_PORT, GPIO_LINE_PIN_LINE_1_PIN );
            line.line_4 = DL_GPIO_readPins(GPIO_LINE_PIN_LINE_4_PORT, GPIO_LINE_PIN_LINE_1_PIN );
            line.line_5 = DL_GPIO_readPins(GPIO_LINE_PIN_LINE_5_PORT, GPIO_LINE_PIN_LINE_1_PIN );
            //DL_GPIO_togglePins(GPIO_LED_PORT, GPIO_LED_PIN_LED_1_PIN);

            velocity_left_read = cnt1 - cnt1_last;
            velocity_right_read = cnt2 - cnt2_last;
            cnt1_last = cnt1;
            cnt2_last = cnt2;
            velocity_left_filtered = Speed_Low_Filter(velocity_left_read,speed_Record_Left);
            velocity_right_filtered = Speed_Low_Filter(velocity_right_read,speed_Record_Right);
            break;
        default:
            break;
    }
}

//编码器左
void TIMG7_IRQHandler(void) {
  switch (DL_TimerG_getPendingInterrupt(CAPTURE_0_INST)) {
  case DL_TIMERG_IIDX_CC0_DN:
    dir1 = DL_GPIO_readPins(GPIO_Encoder_PORT, GPIO_Encoder_PIN_ENCODER_LEFT_PIN);
    if(dir1 ==1)
    {
        cnt1++;
    }
    else if(dir1 ==0)
        {
            cnt1--;
        }
    break;
  default:
    break;
  }
}

//编码器右
void TIMA1_IRQHandler(void) {
  switch (DL_TimerG_getPendingInterrupt(CAPTURE_1_INST)) {
  case DL_TIMERG_IIDX_CC0_DN:
    dir2 = DL_GPIO_readPins(GPIO_Encoder_PORT, GPIO_Encoder_PIN_ENCODER_RIGHT_PIN);
    if(dir2 ==1)
        {
            cnt2++;
        }
        else if(dir2 ==0)
            {
                cnt2--;
            }
    break;
  default:
    break;
  }
}

void motor_left_run_velocity(float velocity)
{
    DL_TimerA_setCaptureCompareValue(PWM_0_INST, 1250*(0.5-0.01*velocity), DL_TIMER_CC_0_INDEX);
    DL_TimerA_setCaptureCompareValue(PWM_0_INST, 1250*0.5, DL_TIMER_CC_1_INDEX);
}

void motor_right_run_velocity(float velocity)
{
    DL_TimerA_setCaptureCompareValue(PWM_0_INST, 1250*(0.5-0.01*velocity), DL_TIMER_CC_2_INDEX);
    DL_TimerA_setCaptureCompareValue(PWM_0_INST, 1250*0.5, DL_TIMER_CC_3_INDEX);
}

void Go_Straight_Velocity(float velocity)
{
    motor_left_run_velocity(velocity);
    motor_right_run_velocity(velocity);
    //printf("V");
}

void Turn_Left_Velocity(float velocity)
{
    motor_left_run_velocity(-velocity);
    motor_right_run_velocity(velocity);
}

void Turn_Right_Velocity(float velocity)
{
    motor_left_run_velocity(velocity);
    motor_right_run_velocity(-velocity);
}

//串口0（上位机）控制函数
void Buffer_Handle_0(void)
{
    if(gEchoData_0[0] == 'V')
       {
           velocity_left = 10*(gEchoData_0[1]-'0')+(gEchoData_0[2]-'0');
           Go_Straight_Velocity(velocity_left);
           //printf("V");
       }
       if(gEchoData_0[0] == 'L')
       {
           velocity_left = 10*(gEchoData_0[1]-'0')+(gEchoData_0[2]-'0');
           Turn_Left_Velocity(velocity_left);
       }
       if(gEchoData_0[0] == 'R')
       {
           velocity_right = 10*(gEchoData_0[1]-'0')+(gEchoData_0[2]-'0');
           Turn_Left_Velocity(velocity_right);
       }
}

void Buffer_Handle_1(void)
{
    if(gEchoData_1[0] == '1')
       {
           mission_flag = 1;
       }
    if(gEchoData_1[0] == '2')
           {
               mission_flag = 2;
           }
    if(gEchoData_1[0] == '3')
           {
               mission_flag = 3;
           }
    if(gEchoData_1[0] == '4')
           {
               mission_flag = 4;
           }
    if(gEchoData_1[0] == 'R')
           {
               mission_flag = 0;
           }
}

void Buffer_Handle_2(void)
{

    if (gEchoData_2[0] == 85 && gEchoData_2[1] == 83)
      {
        angle_z = (float)((gEchoData_2[7] << 8) | gEchoData_2[6]) / 32768.0 * 180.0;
      }
      if (angle_z > 300 && angle_z_last < 50)
      {
        jy901_of_count--;
      }
      else if (angle_z < 50 && angle_z_last > 300)
      {
        jy901_of_count++;
      }
      angle_z_all = angle_z + jy901_of_count * 360;
      angle_z_last = angle_z;
      angle_error = angle_z_all - Target_Angle;
      // 调节角度环的时候开始，分别为目标位置，实际角度减去目标角度
      // /*
//      printf("%.2f,%d,%.2f,%.2f,%.2f\n",
//             angle_z,
//             jy901_of_count,
//             angle_z_all,
//             Target_Angle,
//             angle_error

}

void UART_0_INST_IRQHandler(void)
{
    switch (DL_UART_Main_getPendingInterrupt(UART_0_INST)) {
        case DL_UART_MAIN_IIDX_RX:
            //DL_GPIO_togglePins(GPIO_LED_PORT, GPIO_LED_PIN_LED_1_PIN);
            gEchoData_0[UART_Count_0] = DL_UART_Main_receiveData(UART_0_INST);
            if(gEchoData_0[UART_Count_0] !='@')
            {
            DL_UART_Main_transmitData(UART_0_INST, gEchoData_0[UART_Count_0]);
            UART_Count_0++;
            }

            else if(gEchoData_0[UART_Count_0] == '@')
            {
                Buffer_Handle_0();
                UART_Count_0 = 0;
            }

            break;
        default:
            break;
    }
}

void UART_1_INST_IRQHandler(void)
{
    switch (DL_UART_Main_getPendingInterrupt(UART_1_INST)) {
            case DL_UART_MAIN_IIDX_RX:
                //DL_GPIO_togglePins(GPIO_LED_PORT, GPIO_LED_PIN_LED_1_PIN);
                gEchoData_1[UART_Count_1] = DL_UART_Main_receiveData(UART_1_INST);
                if(gEchoData_1[UART_Count_1] !='@')
                {
                DL_UART_Main_transmitData(UART_1_INST, gEchoData_1[UART_Count_1]);
                UART_Count_1++;
                }

                else if(gEchoData_1[UART_Count_1] == '@')
                {
                    Buffer_Handle_1();
                    UART_Count_1 = 0;
                }

                break;
            default:
                break;
        }
}

void UART_2_INST_IRQHandler(void)
{
    switch (DL_UART_Main_getPendingInterrupt(UART_2_INST)) {
                case DL_UART_MAIN_IIDX_RX:
                    //DL_GPIO_togglePins(GPIO_LED_PORT, GPIO_LED_PIN_LED_1_PIN);
                    gEchoData_2[UART_Count_2] = DL_UART_Main_receiveData(UART_2_INST);
                    if(gEchoData_2[UART_Count_2] !='@')
                    {
                    DL_UART_Main_transmitData(UART_2_INST, gEchoData_2[UART_Count_1]);
                    UART_Count_2++;
                    }

                    else if(gEchoData_2[UART_Count_2] == '@')
                    {
                        Buffer_Handle_1();
                        UART_Count_2 = 0;
                    }

                    break;
                default:
                    break;
            }
}

void Light_Sound(void)
{
    DL_GPIO_setPins(GPIO_LED_PORT, GPIO_LED_PIN_LED_1_PIN);
    DL_GPIO_setPins(GPIO_LED_PORT, GPIO_LED_PIN_BEEP_PIN);
    delay_cycles(DELAY);
    DL_GPIO_clearPins(GPIO_LED_PORT, GPIO_LED_PIN_LED_1_PIN);
    DL_GPIO_clearPins(GPIO_LED_PORT, GPIO_LED_PIN_BEEP_PIN);
}

void Line_Fix(LINE linex)
{
    if((linex.line_1 == 0)&&(linex.line_2 == 0)&&(linex.line_3 == 0)&&(linex.line_4 == 0)&&(linex.line_5 == 0))
    {
        Target_Speed_Line_Fix_Left = 0;
        Target_Speed_Line_Fix_Right = 0;
    }
    if((linex.line_1 == 0)&&(linex.line_2 == 0)&&(linex.line_3 == 1)&&(linex.line_4 == 0)&&(linex.line_5 == 0))
        {
            Target_Speed_Line_Fix_Left = 0;
            Target_Speed_Line_Fix_Right = 0;
        }
    if((linex.line_1 == 0)&&(linex.line_2 == 1)&&(linex.line_3 == 0)&&(linex.line_4 == 0)&&(linex.line_5 == 0))
        {
            Target_Speed_Line_Fix_Left = 0;
            Target_Speed_Line_Fix_Right = 5;
        }
    if((linex.line_1 == 0)&&(linex.line_2 == 0)&&(linex.line_3 == 0)&&(linex.line_4 == 1)&&(linex.line_5 == 0))
        {
            Target_Speed_Line_Fix_Left = 5;
            Target_Speed_Line_Fix_Right = 0;
        }
    if((linex.line_1 == 0)&&(linex.line_2 == 0)&&(linex.line_3 == 0)&&(linex.line_4 == 0)&&(linex.line_5 == 1))
        {
            Target_Speed_Line_Fix_Left = 10;
            Target_Speed_Line_Fix_Right = 0;
        }
    if((linex.line_1 == 1)&&(linex.line_2 == 0)&&(linex.line_3 == 0)&&(linex.line_4 == 0)&&(linex.line_5 == 0))
        {
            Target_Speed_Line_Fix_Left = 0;
            Target_Speed_Line_Fix_Right = 10;
        }
}

void State_Machine(void)
{
    switch(mission_flag)
    {
    case 1:
        Target_Position_Left += Dis_Straight;
        Target_Position_Right += Dis_Straight;
        if(mission1_end_flag)
        {
            Light_Sound();
        }
        break;
    case 2:
        Target_Position_Left += Dis_Straight;
        Target_Position_Right += Dis_Straight;
        delay_cycles(DELAY);
        if(mission1_end_flag)
        {
             Light_Sound();
        }
        break;
    case 3:
        if(mission1_end_flag)
        {
             Light_Sound();
        }
        break;
    case 4:
        if(mission1_end_flag)
        {
             Light_Sound();
        }
        break;
    default:
        break;
    }
}


