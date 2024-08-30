#ifndef _UART_H_
#define _UART_H_

#include "main.h"
#include "usart.h"
#include "gpio.h"
#include "stdio.h"
#include "mymain.h"
#include "string.h"
#include "stm32f1xx_hal_dma.h"

#define BUFF_NUM 100

extern uint8_t receive_Buff1[BUFF_NUM]; //接收数据直接区
extern uint8_t handle_Buff1[BUFF_NUM]; //接收数据缓冲区

extern void uart_init(void); //串口初始化函数
extern void USAR_UART_IDLECallback(void); //串口中断回调函数

#endif
