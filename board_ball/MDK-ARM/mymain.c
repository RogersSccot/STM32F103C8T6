#include "mymain.h"

void mymain(void)
{
	__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,500);
	HAL_Delay(3000);
	__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,2500);
	HAL_Delay(3000);
	//printf("hello,world");
}

void buffer_hadle_USART1(uint8_t *buffer)
{
     printf("USART1 Buffer: %s\r\n", buffer);
}

void buffer_hadle_USART3(uint8_t *buffer)
{
     printf("USART3 Buffer: %s\r\n", buffer);
}
