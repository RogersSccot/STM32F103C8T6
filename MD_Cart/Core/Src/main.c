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

float velocity_set = 1500;
Motor motor1, motor2;     // 左右电机
uint8_t receive_buff[50]; // 从树莓派（串�?1）接受的指令
uint8_t USART1_buffer[50];
uint8_t rx_buffer[50]; // 从陀螺仪（串�?3）接受的角度数据
uint8_t USART3_buffer[50];

float foward_distance = 0;
float left_angle = 0;
float back_angle = 0;
float right_angle = 0;
float Target_Angle = 0;
float error = 0;   // 巡线误差

int button_flag = 0;
int last_flag_button = 0;
int medicine_flag=0;
int last_flag=0;
uint32_t count;

char last_order = '0';
int s1, s2, s3, s4; // 灰度传感器参数

// 901S初始化指令
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
int fputc(int ch, FILE *f) // printf
{
  HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xffff);
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
  MX_DMA_Init();
  MX_USART1_UART_Init();
  MX_USART3_UART_Init();
  MX_TIM1_Init();
  /* USER CODE BEGIN 2 */
  Motor_Init();
  PID_Init();
  HAL_Delay(500);
  DMA_USART_Init(&huart1);
  DMA_USART_Init(&huart3);
  HAL_Delay(500);
  // 初始化
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    // printf("OK");//测试串口

    // 启动按钮检测
    last_flag_button = button_flag;
    button_flag = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_12);
    if ((!button_flag) && last_flag_button)
    {
      printf("S");
    }

     if (USART3_buffer[0] != 0) // 接受到串口屏信号
    {
      buffer_hadle_USART3(USART3_buffer);
      memset(USART3_buffer, 0, 50);
    }
    // 接受树莓派指令，OKOK=亮灯，OFFF=灭灯，V=设置速度，L，R=左右转，B=掉头
    if (USART1_buffer[0] != 0) // 接受到信号
    {
      buffer_hadle_USART1(USART1_buffer);
      memset(USART1_buffer, 0, 50);
    }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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

/// @brief 串口1（树莓派或者电脑）信息处理
/// @param buffer 
void buffer_hadle_USART1(uint8_t* buffer)
{
  if ((buffer[0] == 'N') && (buffer[1] == 'O') && (buffer[2] == 'N') && (buffer[3] == 'O')) // NONO
  {
    led2_off();
  }
  else if ((buffer[0] == 'R') && (buffer[1] == 'S') && (buffer[2] == 'E') && (buffer[3] == 'T')) // RSET
  {
    NVIC_SystemReset(); // 复位函数
  }
  else if ((buffer[0] == '9') && (buffer[1] == '0') && (buffer[2] == '1') && (buffer[3] == 'S')) // 901S角度初始化
  {
    HAL_UART_Transmit(&huart3, data1, sizeof(data1), 0xffff);
    HAL_Delay(200);
    HAL_UART_Transmit(&huart3, data2, sizeof(data2), 0xffff);
    HAL_Delay(3000);
    HAL_UART_Transmit(&huart3, data3, sizeof(data3), 0xffff);
    printf("9");
  }
  else if ((buffer[0] == 'O') && (buffer[1] == 'K') && (buffer[2] == 'O') && (buffer[3] == 'K')) // OKOK
  {
    led1_on();
    // printf("o");
  }
	else if ((buffer[0] == 'O') && (buffer[1] == 'F') && (buffer[2] == 'F') && (buffer[3] == 'F')) // OKOK
  {
    led1_off();
    // printf("o");
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
    // printf("s");
  }

  else
  {
    switch (buffer[0])
    {
    case 'L': // 左转-
      if (buffer[1] == '+')
      {
        left_angle = (buffer[2] - '0') * 100 + (buffer[3] - '0')*10 + (buffer[4] - '0');
      }
      else if (receive_buff[1] == '-')
      {
        left_angle = -(buffer[2] - '0') * 100 - (buffer[3] - '0')*10 - (buffer[4] - '0');
      }
      Target_Angle += left_angle;
      last_order = 'L';
      break;

    case 'R': // 右转
      if (buffer[1] == '+')
      {
        right_angle = (buffer[2] - '0') * 100 + (buffer[3] - '0')*10 + (buffer[4] - '0');
      }
      else if (buffer[1] == '-')
      {
        right_angle = -(buffer[2] - '0') * 100 - (buffer[3] - '0')*10 - (buffer[4] - '0');
      }
      Target_Angle -= right_angle;
      last_order = 'R';
      break;

    case 'E': // 纠正
      if (buffer[1] == '+')
      {
        error = (buffer[2] - '0') * 100 + (buffer[3] - '0')*10 + (buffer[4] - '0');
        fix_error(error, 300);
      }
      else if (buffer[1] == '-')
      {
        error = (buffer[2] - '0') * 100 + (buffer[3] - '0')*10 + (buffer[4] - '0');
        fix_error(error, -300);
      }
      last_order = 'E';
      break;

    case 'V': // 前进
    // 
      if (buffer[1] == '+')
      {
        velocity_set = (buffer[2] - '0') * 100 + (buffer[3] - '0')*10 + (buffer[4] - '0');
        Target_Speed_1 = velocity_set * 50;
        Target_Speed_2 = velocity_set * 50;
        Target_Speed_fixed_2 = 0;
        Target_Speed_fixed_1 = 0;
      }
      else if (buffer[1] == '-')
      {
        velocity_set = (buffer[2] - '0') * 100 + (buffer[3] - '0')*10 + (buffer[4] - '0');
        Target_Speed_1 = -velocity_set * 50;
        Target_Speed_2 = -velocity_set * 50;
        Target_Speed_fixed_2 = 0;
        Target_Speed_fixed_1 = 0;
      }
      last_order = 'V';
      break;
    }
  }
}

/// @brief 串口3信号处理（串口屏或陀螺仪）
/// @param buffer 
void buffer_hadle_USART3(uint8_t* buffer)
{
  if ((buffer[0] == '1') && (buffer[1] == '1') && (buffer[2] == '1') && (buffer[3] == '1'))
  {
    printf("1");
  }
  if ((buffer[0] == '2') && (buffer[1] == '2') && (buffer[2] == '2') && (buffer[3] == '2'))
  {
    printf("2");
  }
  if ((buffer[0] == '3') && (buffer[1] == '3') && (buffer[2] == '3') && (buffer[3] == '3'))
  {
    printf("3");
  }
  if ((buffer[0] == '4') && (buffer[1] == '4') && (buffer[2] == '4') && (buffer[3] == '4'))
  {
    printf("4");
  }
  if ((buffer[0] == 'T') && (buffer[1] == 'E') && (buffer[2] == 'S') && (buffer[3] == 'T'))
  {
    printf("ERROR:%.2f",error);
  }
}
void DMA_USART_Init(UART_HandleTypeDef *huart)
{
  if (huart->Instance == USART1)
  {
    __HAL_UART_ENABLE_IT(huart, UART_IT_IDLE);
    HAL_UART_Receive_DMA(huart, receive_buff, 50);
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
    // 把接收缓冲中的数据复制到处理缓冲中
    memcpy(USART1_buffer, receive_buff, 50);
     //printf("%s\r\n",receive_buff);

    // 清零接收缓冲区
    memset(receive_buff, 0, data_length);
    data_length = 0;

    // 重启开始DMA传输 每次255字节数据
    HAL_UART_Receive_DMA(huart, (uint8_t *)receive_buff, 50);
  }

  if (huart->Instance == USART3)
  {
    //    led_Big.LED_Flip();
    // 停止本次DMA传输
    HAL_UART_DMAStop(huart);

    // 计算接收到的数据长度
    uint8_t data_length = 50 - __HAL_DMA_GET_COUNTER(&hdma_usart3_rx);

    // 测试函数：将接收到的数据打印出去
     //printf("Receive Data(length = %d): ",data_length);
    //    HAL_UART_Transmit(huart,rx_buffer,data_length,0x200);
    // 把接收缓冲中的数据复制到处理缓冲中
    memcpy(USART3_buffer, rx_buffer, 50);
     //printf("%s",rx_buffer);

    // 清零接收缓冲区
    memset(rx_buffer, 0, data_length);
    data_length = 0;

    // 重启开始DMA传输 每次255字节数据
    HAL_UART_Receive_DMA(huart, (uint8_t *)rx_buffer, 50);
  }
}

/*
 * 蓝牙串口回调函数，需要放到stm32f4xx_it.c中
 */
void Bluetooth_UART_IRQHandler(UART_HandleTypeDef *huart)
{ // 判断是否是串口1
  if (huart->Instance == USART1)
  { // 判断是否是空闲中断
    if (RESET != __HAL_UART_GET_FLAG(huart, UART_FLAG_IDLE))
    { // 清除空闲中断标志（否则会一直不断进入中断）
      __HAL_UART_CLEAR_IDLEFLAG(huart);
      // 调用中断处理函数
      USAR_UART_IDLECallback(&huart1);
    }
  }

  if (huart->Instance == USART3)
  { // 判断是否是空闲中断
    if (RESET != __HAL_UART_GET_FLAG(huart, UART_FLAG_IDLE))
    { // 清除空闲中断标志（否则会一直不断进入中断）
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
