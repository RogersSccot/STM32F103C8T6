#ifndef _MYMAIN_H
#define _MYMAIN_H

#include "main.h"
#include "stdio.h"
#include "stm32f1xx.h"
#include "stm32f1xx_hal.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "steer.h"
#include "string.h"

void mymain(void);
void buffer_hadle_USART1(uint8_t *buffer);
void buffer_hadle_USART3(uint8_t *buffer);

#endif
