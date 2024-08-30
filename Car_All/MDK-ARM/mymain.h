#ifndef _MAINMY_H_
#define _MAINMY_H_

#include "main.h"
#include "usart.h"
#include "gpio.h"
#include "stdio.h"
#include "uart.h"
#include "string.h"
#include "stm32f1xx.h"
#include "tim.h"
#include "encode.h"
#include "time.h"

extern void Data_Handle1(void);
extern void mymain(void);
extern void GPIO_init(void);
extern void life_led(void);

#endif
