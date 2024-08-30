/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "encoder.h"
#include <stdio.h>
#include "pid.h"
#include "led.h"
#include "medicine.h"
#include "string.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
// Motor motor1;
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/////////////////////////////////////////////////////////////参数初始化//////////////////////////////////////////////////////////
float velocity_set = 1500;
Motor motor1, motor2; // 左右电机
// 对应接口B6(TX)，B7(RX)，一般用于蓝牙，树莓派的连接，(通信波特率：115200)
uint8_t receive_buff[50];
uint8_t USART1_buffer[50];

// 对应接口A2(TX)，A3(RX)，一般用于陀螺仪的连接，(通信波特率：115200)
uint8_t receive_buff_2[50];
uint8_t USART2_buffer[50];

// 对应接口B10(TX)，B11(RX)，一般用于显示屏的连接（现用于陀螺仪的角度读取），(通信波特率：115200)
uint8_t rx_buffer[50];
uint8_t USART3_buffer[50];

float foward_distance = 0;
float left_angle = 0;
float back_angle = 0;
float right_angle = 0;
float Target_Angle = 0;

int button_flag = 0;
int last_flag_button = 0;
int medicine_flag = 0;
int last_flag = 0;
// 是否开启位置环参数
int pos_pid_flag = 1;
// 复位901S的参数
int _901S_flag;
uint32_t count;

// angle_z从陀螺仪读到的参数
float angle_z = 0;
// 上一次的角度
float angle_z_last = 0;
// 整体的角度
float angle_z_all = 0;
// 旋转的圈数
int jy901_of_count = 0;
// 角度误差
float angle_error = 0;

char last_order;
int s1, s2, s3, s4; // 灰度传感器参

// 901S初始化指
uint8_t data1[] = {0xFF, 0xAA, 0x69, 0x88, 0xB5};
uint8_t data2[] = {0xFF, 0xAA, 0x01, 0x04, 0x00};
uint8_t data3[] = {0xFF, 0xAA, 0x00, 0x00, 0x00};

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
// 开启printf的使用，定位到串口2
int fputc(int ch, FILE *f) // printf
{
  HAL_UART_Transmit(&huart3, (uint8_t *)&ch, 1, 0xffff);
  return ch;
}

int fgetc(FILE *f) // getchar
{
  uint8_t ch = 0;
  HAL_UART_Receive(&huart1, &ch, 1, 0xffff);
  return ch;
}
/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_TIM1_Init();
  MX_DMA_Init();
  MX_USART1_UART_Init();
  MX_USART3_UART_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
  // 上面一定注意，DMA的初始化一定要放在串口初始化的后面，否则会出现串口接收不到数据的情况
  Motor_Init();

  HAL_Delay(500);
  DMA_USART_Init(&huart1);
  DMA_USART_Init(&huart2);
  DMA_USART_Init(&huart3);
  pos_pid_flag = 1;
  HAL_Delay(1000);
  PID_Init();
  printf("OK Init\n");
  // 初始
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
    // HAL_UART_Transmit(&huart3,"FFFF",4,0xff);
    /* USER CODE BEGIN 3 */
    // 驱动32唯一的主程序，用于复位901S
    ///////////////////////////////////////////////////////////主函数////////////////////////////////////////////////////////
    if (_901S_flag)
    {
      // 因为此时陀螺仪暂时接到串口2，所以是往串口2发送复位信号
      HAL_UART_Transmit(&huart2, data1, sizeof(data1), 0xffff);
      HAL_Delay(200);
      HAL_UART_Transmit(&huart2, data2, sizeof(data2), 0xffff);
      HAL_Delay(3000);
      HAL_UART_Transmit(&huart2, data3, sizeof(data3), 0xffff);
      HAL_Delay(200);
      HAL_UART_Transmit(&huart1, "901S", 4, 0xffff);
      _901S_flag = 0;
    }
    // printf("OKOK\n");
  }
  /* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
   * in the RCC_OscInitTypeDef structure.
   */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
   */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/// @brief 串口1（树莓派或电脑）信息处理
/// @param buffer

// bad_order用来记录之前的不完整命令，0表示没有不完整命令，1表示有不完整命令
uint8_t bad_order = 0;
uint8_t bad_begin = 0;
uint8_t buffer1[4];

void chelk_buffer(uint8_t *buffer)
{
  // 首先判断之前是否有不完整指令
  if (bad_order == 0)
  {
    // 此时没有不完整指令，接下来判断这一条指令是否完整
    if (buffer[3] == 00)
    {
      // 进入这里说明这一条指令为不完整指令
      bad_order = 1;
      for (int i = 0; i < 4; i++)
      {
        if (buffer[i] != 00)
        {
          // 这一位还是正常的
          buffer1[i] = buffer[i];
        }
        else
        {
          // 这一位是不对的
          bad_begin = i;
          break;
        }
      }
    }
    else
    {
      // 进入这里说明这一条指令为完整指令
      bad_order = 0;
    }
  }
  // 这里就说明已经存在不完整指令
  else
  {
    // 此时我们需要将两条指令拼接起来
    for (int i = bad_begin; i < 4; i++)
    {
      buffer1[i] = buffer[i - bad_begin];
    }
    // 结束后将buffer设为新的buffer1
    for (int i = 0; i < 4; i++)
    {
      buffer[i] = buffer1[i];
    }
    bad_order = 0;
    bad_begin = 0;
  }
}

// 串口中断处理函数
// 串口中断处理函数
// 串口中断处理函数
// 串口中断处理函数
// 串口中断处理函数
// 串口中断处理函数
// 串口中断处理函数
// 串口中断处理函数
// 串口中断处理函数
// 串口中断处理函数
// 串口中断处理函数
// 串口中断处理函数
// 串口中断处理函数
// 串口中断处理函数
// 串口中断处理函数
// 串口中断处理函数
// 串口中断处理函数
// 串口中断处理函数
// 串口中断处理函数
// 串口中断处理函数
// 串口中断处理函数
// 串口中断处理函数
// 串口中断处理函数
// 串口中断处理函数
// 串口中断处理函数
// 串口中断处理函数
// 串口中断处理函数
// 串口中断处理函数
// 串口中断处理函数
// 串口中断处理函数
// 串口中断处理函数

/// @brief ////////////////////////////////////////////串口1中断处理函数/////////////////////////////////////////////////
// 对应接口B6(TX)，B7(RX)，一般用于蓝牙，树莓派的连接，(通信波特率：115200)
/// @param buffer
void buffer_hadle_USART1(uint8_t *buffer)
{
  // 首先需要判断这个指令是否为完整指令
  chelk_buffer(buffer);
  if (bad_order == 0)
  {
    // 将收到的命令从这里打回去
    HAL_UART_Transmit(&huart1, "urt1", 4, 0xff);
    HAL_UART_Transmit(&huart1, buffer, 4, 0xff);
    if ((buffer[0] == 'N') && (buffer[1] == 'O') && (buffer[2] == 'N') && (buffer[3] == 'O')) // NONO
    {
      led2_off();
    }
    else if ((buffer[0] == 'R') && (buffer[1] == 'S') && (buffer[2] == 'E') && (buffer[3] == 'T')) // RSET
    {
      NVIC_SystemReset(); // 复位函数
    }
    else if ((buffer[0] == '9') && (buffer[1] == '0') && (buffer[2] == '1') && (buffer[3] == 'S')) // 901S角度初始
    {
      _901S_flag = 1;
    }
    else if ((buffer[0] == 'O') && (buffer[1] == 'K') && (buffer[2] == 'O') && (buffer[3] == 'K')) // OKOK
    {
      led1_on();
    }
    else if ((buffer[0] == 'O') && (buffer[1] == 'F') && (buffer[2] == 'F') && (buffer[3] == 'F')) //
    {
      led1_off();
    }
    else if ((buffer[0] == 'G') && (buffer[1] == 'O') && (buffer[2] == 'O') && (buffer[3] == 'D')) // GOOD
    {
      led2_on();
    }
    else if ((buffer[0] == 'S') && (buffer[1] == 'T') && (buffer[2] == 'O') && (buffer[3] == 'P')) // OKOK
    {
      Target_Speed_1 = 0;
      Target_Speed_2 = 0;
      Target_Speed_fixed_2 = 0;
      Target_Speed_fixed_1 = 0;
    }
    else
    {
      switch (buffer[0])
      {
      case 'L': // 左转
        // printf("jinruLcas\n");
        pos_pid_flag = 0;
        // pid_speed_1.integral = 0;
        // pid_speed_2.integral = 0;
        if (buffer[1] == '+')
        {
          left_angle = (buffer[2] - '0') * 10 + (buffer[3] - '0');
        }
        else if (receive_buff[1] == '-')
        {
          left_angle = -(buffer[2] - '0') * 10 - (buffer[3] - '0');
        }
        // Target_Position_1 -= left_angle*5;
        // Target_Position_2 -= left_angle*5;
        Target_Angle += left_angle;
        last_order = 'L';
        break;
      case 'R': // 右转
        pos_pid_flag = 0;
        // pid_speed_1.integral = 0;
        // pid_speed_2.integral = 0;
        if (buffer[1] == '+')
        {
          right_angle = (buffer[2] - '0') * 10 + (buffer[3] - '0');
        }
        else if (buffer[1] == '-')
        {
          right_angle = -(buffer[2] - '0') * 10 - (buffer[3] - '0');
        }
        // Target_Position_1 += right_angle*5;
        // Target_Position_2 += right_angle*5;
        Target_Angle -= right_angle;
        last_order = 'R';
        break;
      case 'V': // 前进
        if (buffer[1] == '+')
        {
          // printf("V function\n");
          pos_pid_flag = 0;
          velocity_set = (buffer[2] - '0') * 100 + (buffer[3] - '0') * 10 + (buffer[4] - '0');
          Target_Speed_1 = velocity_set;
          Target_Speed_2 = velocity_set;
          Target_Speed_fixed_2 = 0;
          Target_Speed_fixed_1 = 0;
        }
        else if (buffer[1] == '-')
        {
          pos_pid_flag = 0;
          velocity_set = (buffer[2] - '0') * 100 + (buffer[3] - '0') * 10 + (buffer[4] - '0');
          Target_Speed_1 = -velocity_set;
          Target_Speed_2 = -velocity_set;
          Target_Speed_fixed_2 = 0;
          Target_Speed_fixed_1 = 0;
        }
        last_order = 'V';
        break;
      case 'F':
        // HAL_UART_Transmit(&huart1,"FFFF",4,0xff);
        Target_Speed_1 = 0;
        Target_Speed_2 = 0;
        // pid_speed_1.integral = 0;
        // pid_speed_2.integral = 0;
        Target_Speed_fixed_1 = 0;
        Target_Speed_fixed_2 = 0;
        Target_Angle = angle_z_all;
        angle_error = 0;
        pos_pid_flag = 1;
        velocity_set = (buffer[2] - '0') * 10 + (buffer[3] - '0');
        motor_foward(velocity_set, 0);
        _901S_flag = 1;
        last_order = 'F';
        break;
      }
    }
  }
}

// /// @brief 串口2信号处理（串口屏或陀螺仪/////////////////////////////////////////////////////////////////////////////////////
// 对应接口A2(TX)，A3(RX)，一般用于陀螺仪的连接，(通信波特率：115200)
void buffer_hadle_USART2(uint8_t *buffer)
{
  // 测试程序，将受到的东西打回去
  // HAL_UART_Transmit(&huart2, "urt2", 4, 0xff);
  // HAL_UART_Transmit(&huart2, buffer, 4, 0xff);

  if (buffer[0] == 85 && buffer[1] == 83)
  {
    angle_z = (float)((buffer[7] << 8) | buffer[6]) / 32768.0 * 180.0;
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
  printf("%.2f,%.2f,%.2f\n", angle_z_all, Target_Angle, angle_error);
  // printf("angle:%.2f  ",angle_z_all);
  // printf("Target_angle:%.2f\n",Target_Angle);
  // printf("error:%.2f\n",angle_error);

  if ((last_order == 'L') || (last_order == 'R'))
  {
    // printf("L+90");
    pos_pid_flag = 0;
    Target_Position_1 = Now_Position_1;
    Target_Position_2 = Now_Position_2;
    Target_Speed_1 = 0;
    Target_Speed_2 = 0;
    Target_Speed_fixed_1 = Angle_PID_Realize(&pid_angle, angle_error);
    Target_Speed_fixed_2 = -Target_Speed_fixed_1;
  }
  else
  {
    Target_Speed_fixed_1 = 0;
    Target_Speed_fixed_2 = 0;
    Target_Angle = angle_z_all;
    pos_pid_flag = 1;
  }
}
/// @brief 串口3信号处理（串口屏或陀螺仪)/////////////////////////////////////////////////////////////////////////////////////
// 对应接口B10(TX)，B11(RX)，一般用于显示屏的连接（现用于陀螺仪的角度读取），(通信波特率：115200)
/// @param buffer
void buffer_hadle_USART3(uint8_t *buffer)
{
  // 测试程序，将受到的东西打回去
  HAL_UART_Transmit(&huart3, "urt3", 4, 0xff);
  HAL_UART_Transmit(&huart3, buffer, 4, 0xff);
}

// 下面的函数能正常使用，千万不要再去修改
// 下面的函数能正常使用，千万不要再去修改
// 下面的函数能正常使用，千万不要再去修改
// 下面的函数能正常使用，千万不要再去修改
// 下面的函数能正常使用，千万不要再去修改
// 下面的函数能正常使用，千万不要再去修改
// 下面的函数能正常使用，千万不要再去修改
// 下面的函数能正常使用，千万不要再去修改
// 下面的函数能正常使用，千万不要再去修改
// 下面的函数能正常使用，千万不要再去修改
// 下面的函数能正常使用，千万不要再去修改
// 下面的函数能正常使用，千万不要再去修改
// 下面的函数能正常使用，千万不要再去修改
// 下面的函数能正常使用，千万不要再去修改
// 下面的函数能正常使用，千万不要再去修改
// 下面的函数能正常使用，千万不要再去修改
// 下面的函数能正常使用，千万不要再去修改
// 下面的函数能正常使用，千万不要再去修改
// 下面的函数能正常使用，千万不要再去修改
// 下面的函数能正常使用，千万不要再去修改
// 下面的函数能正常使用，千万不要再去修改
// 下面的函数能正常使用，千万不要再去修改
// 下面的函数能正常使用，千万不要再去修改
// 下面的函数能正常使用，千万不要再去修改
// 下面的函数能正常使用，千万不要再去修改
// 下面的函数能正常使用，千万不要再去修改
// 下面的函数能正常使用，千万不要再去修改
// 下面的函数能正常使用，千万不要再去修改
// 下面的函数能正常使用，千万不要再去修改
// 下面的函数能正常使用，千万不要再去修改
// 下面的函数能正常使用，千万不要再去修改
// 下面的函数能正常使用，千万不要再去修改
// 下面的函数能正常使用，千万不要再去修改
// 下面的函数能正常使用，千万不要再去修改
// 下面的函数能正常使用，千万不要再去修改
// 下面的函数能正常使用，千万不要再去修改
// 下面的函数能正常使用，千万不要再去修改
// 下面的函数能正常使用，千万不要再去修改
// 下面的函数能正常使用，千万不要再去修改

void DMA_USART_Init(UART_HandleTypeDef *huart)
{
  if (huart->Instance == USART1)
  {
    __HAL_UART_ENABLE_IT(huart, UART_IT_IDLE);
    HAL_UART_Receive_DMA(huart, receive_buff, 50);
  }

  if (huart->Instance == USART2)
  {
    __HAL_UART_ENABLE_IT(huart, UART_IT_IDLE);
    HAL_UART_Receive_DMA(huart, receive_buff_2, 50);
  }

  if (huart->Instance == USART3)
  {
    __HAL_UART_ENABLE_IT(huart, UART_IT_IDLE);
    HAL_UART_Receive_DMA(huart, rx_buffer, 50);
  }
}

void USAR_UART_IDLECallback(UART_HandleTypeDef *huart)
{
  if (huart->Instance == USART1)
  {
    // 停止本次DMA传输
    HAL_UART_DMAStop(huart);

    // 计算接收到的数据长度
    uint8_t data_length = 50 - __HAL_DMA_GET_COUNTER(&hdma_usart1_rx);

    // 测试函数：将接收到的数据打印出去
    // printf("Receive Data(length = %d): ",data_length);
    //    HAL_UART_Transmit(huart,receive_Buff,data_length,0x200);
    // 把接收缓冲中的数据复制到处理缓冲
    memcpy(USART1_buffer, receive_buff, 50);
    // printf("%s\r\n",receive_buff);
    buffer_hadle_USART1(USART1_buffer);
    // 清零接收缓冲
    memset(receive_buff, 0, data_length);
    data_length = 0;

    // 重启始DMA传输 每次255字节数据
    HAL_UART_Receive_DMA(huart, (uint8_t *)receive_buff, 50);
  }

  if (huart->Instance == USART2)
  {
    // 停止本次DMA传输
    HAL_UART_DMAStop(huart);

    // 计算接收到的数据长度
    uint8_t data_length = 50 - __HAL_DMA_GET_COUNTER(&hdma_usart2_rx);

    // 测试函数：将接收到的数据打印出去
    // printf("Receive Data(length = %d): ",data_length);
    //    HAL_UART_Transmit(huart,receive_Buff,data_length,0x200);
    // 把接收缓冲中的数据复制到处理缓冲
    memcpy(USART2_buffer, receive_buff_2, 50);
    buffer_hadle_USART2(USART2_buffer);
    // 清零接收缓冲
    memset(receive_buff_2, 0, data_length);
    data_length = 0;

    // 重启始DMA传输 每次255字节数据
    HAL_UART_Receive_DMA(huart, (uint8_t *)receive_buff_2, 50);
  }

  if (huart->Instance == USART3)
  {
    //    led_Big.LED_Flip();
    // 停止本次DMA传输
    HAL_UART_DMAStop(huart);

    // 计算接收到的数据长度
    uint8_t data_length = 50 - __HAL_DMA_GET_COUNTER(&hdma_usart3_rx);

    // 测试函数：将接收到的数据打印出去
    // printf("Receive Data(length = %d): ",data_length);
    //    HAL_UART_Transmit(huart,rx_buffer,data_length,0x200);
    // 把接收缓冲中的数据复制到处理缓冲
    memcpy(USART3_buffer, rx_buffer, 50);
    // printf("%s",rx_buffer);
    buffer_hadle_USART3(USART3_buffer);
    // 清零接收缓冲
    memset(rx_buffer, 0, data_length);
    data_length = 0;

    // 重启始DMA传输 每次255字节数据
    HAL_UART_Receive_DMA(huart, (uint8_t *)rx_buffer, 50);
  }
}

/*
 * 蓝牙串口回调函数，需要放到stm32f4xx_it.c
 */
void Bluetooth_UART_IRQHandler(UART_HandleTypeDef *huart)
{ // 判断是否是串1
  if (huart->Instance == USART1)
  { // 判断是否是空闲中
    if (RESET != __HAL_UART_GET_FLAG(huart, UART_FLAG_IDLE))
    { // 清除空闲中断标志（否则会直不断进入中断）
      __HAL_UART_CLEAR_IDLEFLAG(huart);
      // 调用中断处理函数
      USAR_UART_IDLECallback(&huart1);
    }
  }

  if (huart->Instance == USART2)
  { // 判断是否是空闲中
    if (RESET != __HAL_UART_GET_FLAG(huart, UART_FLAG_IDLE))
    { // 清除空闲中断标志（否则会直不断进入中断）
      __HAL_UART_CLEAR_IDLEFLAG(huart);
      // 调用中断处理函数
      USAR_UART_IDLECallback(&huart2);
    }
  }

  if (huart->Instance == USART3)
  { // 判断是否是空闲中
    if (RESET != __HAL_UART_GET_FLAG(huart, UART_FLAG_IDLE))
    { // 清除空闲中断标志（否则会直不断进入中断）
      __HAL_UART_CLEAR_IDLEFLAG(huart);
      // 调用中断处理函数
      USAR_UART_IDLECallback(&huart3);
    }
  }
}
/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
