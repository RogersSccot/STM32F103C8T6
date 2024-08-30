#ifndef _MYMAIN_H
#define _MYMAIN_H

#include "main.h"
#include <stdio.h>
#include "stm32f1xx.h"
#include "stm32f1xx_hal.h"
#include "dma.h"
#include "usart.h"
#include "gpio.h"
#include "string.h"

typedef struct _SQUARE
{
    uint8_t x;
		uint8_t y;
		char name;
} SQUARE;

typedef struct _CART
{
    uint8_t x;
	uint8_t y;
	int posision_see;
	int position_now;
	int position_target;
	int direct_now;
	int direct_target;
} CART;

#define FOWARD_ANGLE 1
#define FOWARD_DIRECT 0
#define RIGHT_ANGLE 4
#define RIGHT_DIRECT 270
#define BACK_ANGLE -1
#define BACK_DIRECT 180
#define LEFT_ANGLE -4
#define LEFT_DIRECT 90


void mymain(void);
void Init_car(void);
void Beep(void);
void Cart_Run(uint8_t dis_X,uint8_t dis_Y);
void Square_Run(CART cart1);
void Turn_Left(void);
void Turn_Right(void);
void Turn_Around(void);
void Go_Foward(uint8_t distance);
void Direct_Change(char angle);
void Square_Init(void);
void State_Machine(void);
void buffer_hadle_USART1(uint8_t *buffer);
void buffer_hadle_USART2(uint8_t *buffer);
void buffer_hadle_USART3(uint8_t *buffer);

#endif
