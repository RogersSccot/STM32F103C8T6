#include "mymain.h"

int m1 = 1;
int m2 = 1;

void life_led(void)
{
  if (tick_time > 72000000)
  {
    // limit the range
    tick_time = 0;
  }
  if (tick_time % 200 < 100)
  {
    // led on
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
  }
  if (tick_time % 200 > 100)
  {
    // led off
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
  }
}

void mymain(void)
{
}

// This function is used to handle the data from the uart1
// TIM_CHANNEL_1 = PA6, TIM_CHANNEL_2 = PA7
// TIM_CHANNEL_3 = PB0, TIM_CHANNEL_4 = PB1

// The communication protocol :
// brush motor:
// B2V±xxx : 2 motors move forward/backward at a speed of xxx
// B2D±xxx : 2 motors turn left/right at a speed of xxx
// B2X±xxx : 2 motors move forward/backward at a distance of xxx
// B2T±xxx : 2 motors turn left/right at a distance of xxx
// BLV±xxx : left motor move forward/backward at a speed of xxx
// BRV±xxx : right motor move forward/backward at a speed of xxx
// BLX±xxx : left motor move forward/backward at a distance of xxx
// BRX±xxx : right motor move forward/backward at a distance of xxx

// steering engine:
// A2X±xxx : 2 steering engines move to xxx degree
// ALX±xxx : left steering engine move to xxx degree
// ARX±xxx : right steering engine move to xxx degree

// stepper motor:
// S2V±xxx : 2 motors move forward/backward at a speed of xxx
// S2D±xxx : 2 motors turn left/right at a speed of xxx
// S2X±xxx : 2 motors move forward/backward at a distance of xxx
// S2T±xxx : 2 motors turn left/right at a distance of xxx
// SLV±xxx : left motor move forward/backward at a speed of xxx
// SRV±xxx : right motor move forward/backward at a speed of xxx
// SLX±xxx : left motor move forward/backward at a distance of xxx
// SRX±xxx : right motor move forward/backward at a distance of xxx

// '+'=43
// '-'=45

void Data_Handle1(void)
{
  // reset order
  if ((handle_Buff1[0] == '0') && (handle_Buff1[1] == '0') && (handle_Buff1[2] == '0'))
  {
    NVIC_SystemReset(); // 复位函数
  }
  // calculate the num given
  float num = 100 * (handle_Buff1[4] - '0') + 10 * (handle_Buff1[5] - '0') + (handle_Buff1[6] - '0');

  // decide the motor to move, and control the direction
  if ((handle_Buff1[1] == '2') && ((handle_Buff1[2] == 'V') || (handle_Buff1[2] == 'X')))
  {
    m1 = -1 * (handle_Buff1[3] - 44);
    m2 = -1 * (handle_Buff1[3] - 44);
  }
  if ((handle_Buff1[1] == '2') && ((handle_Buff1[2] == 'D') || (handle_Buff1[2] == 'T')))
  {
    m1 = -1 * (handle_Buff1[3] - 44);
    m2 = 1 * (handle_Buff1[3] - 44);
  }
  if (handle_Buff1[1] == 'L')
  {
    m1 = -1 * (handle_Buff1[3] - 44);
    m2 = 0;
  }
  if (handle_Buff1[1] == 'R')
  {
    m1 = 0;
    m2 = -1 * (handle_Buff1[3] - 44);
  }

  // decide the last order
  if ((handle_Buff1[2] == 'V') || (handle_Buff1[2] == 'D'))
  {
    last_order = 'V';
  }
  if ((handle_Buff1[2] == 'X') || (handle_Buff1[2] == 'T'))
  {
    last_order = 'X';
  }

  // Put into action, Brush motor
  if (handle_Buff1[0] == 'B')
  {
    if (last_motor != 'B')
    {
      Brush_Moter_Init();
    }
    if (last_order == 'V')
    {
      motor1.speed_target = num * m1;
      motor2.speed_target = num * m2;
    }
    if (last_order == 'X')
    {
      motor1.position_target = num * m1;
      motor2.position_target = num * m2;
    }
    last_motor = 'B';
  }

  // move the steering engine1
  if (handle_Buff1[0] == 'A')
  {
    if (last_motor != 'A')
    {
      // init the steering engine
      Steer_Engine_Init();
    }
    // calculate the compare value
    int compare1 = 1000 * num / 180;
    // print this to see
    // printf("compare1: %d", compare1);

    // set the compare value
    __HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_1, compare1 * m1);
    __HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_2, compare1 * m2);
    last_motor = 'A';
  }

  // move the stepper motor1
  if (handle_Buff1[0] == 'S')
  {
    if (last_motor != 'S')
    {
      // init the stepper motor
      Step_Moter_Init();
    }
    // set the direction
    if (m1 == 1)
    {
      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
    }
    if (m1 == -1)
    {
      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
    }
    if (m2 == 1)
    {
      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_SET);
    }
    if (m2 == -1)
    {
      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_RESET);
    }
    if ((m1 == 0) || ((num == 0) && (last_order == 'V')))
    {
      HAL_TIM_PWM_Stop_IT(&htim3, TIM_CHANNEL_1);
    }
    else
    {
      if (last_order == 'V')
      {
        pulse4_flag1 = 1;
        pulse4_set1 = -1;
        __HAL_TIM_SetAutoreload(&htim3, 20000 / num);
        __HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_1, 20);
        HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
      }
      if (last_order == 'X')
      {
        pulse4_flag1 = 1;
        pulse4_set1 = num * 1;
        __HAL_TIM_SetAutoreload(&htim3, 1000);
        __HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_1, 20);
        HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
      }
    }
    if ((m2 == 0) || ((num == 0) && (last_order == 'V')))
    {
      HAL_TIM_PWM_Stop_IT(&htim3, TIM_CHANNEL_2);
    }
    else
    {
      if (last_order == 'V')
      {
        pulse4_flag2 = 1;
        pulse4_set2 = -1;
        __HAL_TIM_SetAutoreload(&htim3, 20000 / num);
        __HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_2, 20);
        HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
      }
      if (last_order == 'X')
      {
        pulse4_flag2 = 1;
        pulse4_set2 = num * 1;
        __HAL_TIM_SetAutoreload(&htim3, 1000);
        __HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_2, 20);
        HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
      }
    }
    last_motor = 'S';
  }
}
