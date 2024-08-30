#include "mymain.h"

uint8_t mission;
uint8_t mission2_flag;
uint8_t mission3_path[4];
uint8_t mission4_flag;
uint8_t P_flag;
uint8_t mission5_flag;
uint8_t mission5_path[16];
uint8_t mission5_path_length;
uint8_t mission5_start_flag;
uint8_t mission5_end_flag;
uint8_t mission6_end_flag;
uint8_t i_X,i_Y;
uint8_t data_forward[4] = "F+30";
uint8_t data_right[4] = "R+90";
uint8_t data_left[4] = "L+90";
uint8_t data_dash[4] = "F+00";
uint8_t data_stop[4] = "F+00";
uint8_t data_beep[4] = "OKOK";
uint8_t data_beep2[4] = "OFFF";
char map[16];

SQUARE square[17];
CART cart;

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

/// @brief ???
/// @param
void mymain(void)
{
	//HAL_UART_Transmit(&huart2,"OKOK",4,0xff);
	//HAL_Delay(1000);
	State_Machine();
}

/// @brief 树莓派
/// @param buffer
void buffer_hadle_USART1(uint8_t *buffer)
{
	if(buffer[0] == 'P')
	{
		cart.position_target = (buffer[1]-'0')*10 + (buffer[2]-'0');
		//printf("P=%d\n",cart.position_target);
		P_flag = 1;
		//Square_Run(cart);
	}
	if(buffer[0] == 'G')
	{
		for(int i=0;i<16;i++)
		{
			if(buffer[i+1] != 'Z')
			{
				if((buffer[i+1] != 'A')&&(buffer[i+1] != 'B')&&(buffer[i+1] != 'C')&&(buffer[i+1] != 'D')&&(buffer[i+1] != 'E')&&(buffer[i+1] != 'F'))
				{
					mission5_path[i] = (buffer[i+1]-'0');
				}
				else if(buffer[i+1] == 'A')
				{
					mission5_path[i] = 10;
				}
				else if(buffer[i+1] == 'B')
				{
					mission5_path[i] = 11;
				}
				else if(buffer[i+1] == 'C')
				{
					mission5_path[i] = 12;
				}
				else if(buffer[i+1] == 'D')
				{
					mission5_path[i] = 13;
				}
				else if(buffer[i+1] == 'E')
				{
					mission5_path[i] = 14;
				}
				else if(buffer[i+1] == 'F')
				{
					mission5_path[i] = 15;
				}
				mission5_path_length++;
			}
			else
			{
				mission5_start_flag = 1;
				break;
			}
			//printf("mission5flag%d:%d",i,mission5_path[i]);
		}
	}
	if((buffer[0] == 'F')||(buffer[0] == 'R')||(buffer[0] == 'L'))
	{
		HAL_UART_Transmit(&huart2, buffer, 4, 0xffff);
	}
	if(buffer[0] == 'C')
	{
		cart.posision_see = (buffer[1]-'0')*10 + (buffer[2]-'0');
		if(cart.posision_see <10){
		printf("t1.txt=\"%d\"\xff\xff\xff",cart.posision_see);
		}
		else
		{
			if(cart.posision_see == 10)
			{
		printf("t1.txt=\"A\"\xff\xff\xff");
			}
			if(cart.posision_see == 11)
			{
		printf("t1.txt=\"B\"\xff\xff\xff");
			}
			if(cart.posision_see == 12)
			{
		printf("t1.txt=\"C\"\xff\xff\xff");
			}
			if(cart.posision_see == 13)
			{
		printf("t1.txt=\"D\"\xff\xff\xff");
			}
			if(cart.posision_see == 14)
			{
		printf("t1.txt=\"E\"\xff\xff\xff");
			}
			if(cart.posision_see == 15)
			{
		printf("t1.txt=\"F\"\xff\xff\xff");
			}
		}
	}
	if((buffer[0] == 'S')&&(buffer[1] == 'T')&&(buffer[2] == 'O')&&(buffer[3] == 'P'))
	{
		mission5_end_flag = 1;
		mission6_end_flag = 1;
	}
	if(buffer[0] == 'M')
	{
		for(int i=0;i<16;i++)
		{
			map[i] = buffer[i+1];
			
		}
		for(int i=0;i<16;i++)
		{
			printf("t%d.txt=\"%c\"\xff\xff\xff",i+2,map[i]);
		}
		//HAL_UART_Transmit(&huart3,map, 16, 0xffff);
	}
}

/// @brief 蓝牙->小车
/// @param buffer 
void buffer_hadle_USART2(uint8_t *buffer)
{
	
}

/// @brief 串口屏
/// @param buffer 
void buffer_hadle_USART3(uint8_t *buffer)
{
	if(buffer[0] == 'R')
	{
		mission = 0;
		mission2_flag = 0;
		for(int i=0;i<4;i++)
		{
		    mission3_path[i] = 0;
		}
		mission4_flag = 0;
		mission5_end_flag = 0;
		mission5_start_flag = 0;
		mission6_end_flag = 0;
		P_flag =0;
		mission5_path_length = 0;
		for(int i=0;i<16;i++)
		{
		    mission5_path[i] = 0;
		}
		cart.direct_now = 0;
		cart.position_now = -1;
		HAL_UART_Transmit(&huart2, "RSET", 4, 0xffff);
	}
	if((buffer[0] == '1')&&(buffer[1] == '1')&&(buffer[2] == '1')&&(buffer[3] == '1'))
	{
		mission = 1;
		HAL_UART_Transmit(&huart1, "P11", 3, 0xffff);
	}
	if((buffer[0] == '2')&&(buffer[1] == '2')&&(buffer[2] == '2')&&(buffer[3] == '2'))
	{
		mission = 2;
		HAL_UART_Transmit(&huart1, "P12", 3, 0xffff);
	}
	if((buffer[0] == '3')&&(buffer[1] == '3')&&(buffer[2] == '3')&&(buffer[3] == '3'))
	{
		mission = 3;
		HAL_UART_Transmit(&huart1, "P13", 3, 0xffff);
	}
	if((buffer[0] == '4')&&(buffer[1] == '4')&&(buffer[2] == '4')&&(buffer[3] == '4'))
	{
		mission = 4;
		HAL_UART_Transmit(&huart1, "P21", 3, 0xffff);
	}
	if((buffer[0] == '5')&&(buffer[1] == '5')&&(buffer[2] == '5')&&(buffer[3] == '5'))
	{
		mission = 5;
		HAL_UART_Transmit(&huart1, "P22", 3, 0xffff);
	}
	if((buffer[0] == '6')&&(buffer[1] == '6')&&(buffer[2] == '6')&&(buffer[3] == '6'))
	{
		mission = 6;
		HAL_UART_Transmit(&huart1, "P23", 3, 0xffff);
	}
	if((buffer[0] == '7')&&(buffer[1] == '7')&&(buffer[2] == '7')&&(buffer[3] == '7'))
	{
		mission = 7;
		HAL_UART_Transmit(&huart1, "7", 1, 0xffff);
	}

	if((buffer[0]=='M')&&(buffer[1]=='2')&&(buffer[2]=='C'))
	{
		mission2_flag = 12;
	}
	if((buffer[0]=='M')&&(buffer[1]=='2')&&(buffer[2]=='D'))
	{
		mission2_flag = 13;
	}
	if((buffer[0]=='M')&&(buffer[1]=='2')&&(buffer[2]=='E'))
	{
		mission2_flag = 14;
	}
	if((buffer[0]=='M')&&(buffer[1]=='2')&&(buffer[2]=='F'))
	{
		mission2_flag = 15;
	}

	if((buffer[0]=='M')&&(buffer[1]=='3')&&(buffer[2]=='A'))
	{
		if(buffer[3] != 'C')
		{
			mission3_path[0] = (buffer[3] - '0');
		}
		else if(buffer[3] == 'C')
		{
			mission3_path[0] = 10;
		}
		//printf("mission3_path[0] = %d\n", mission3_path[0]);
	}
	if((buffer[0]=='M')&&(buffer[1]=='3')&&(buffer[2]=='B'))
	{
		if(buffer[3] != 'C')
		{
			mission3_path[1] = (buffer[3] - '0');
		}
		else if(buffer[3] == 'C')
		{
			mission3_path[0] = 10;
		}
		//printf("mission3_path[1] = %d\n", mission3_path[1]);
	}
	if((buffer[0]=='M')&&(buffer[1]=='3')&&(buffer[2]=='C'))
	{
		if(buffer[3] != 'C')
		{
			mission3_path[2] = (buffer[3] - '0');
		}
		else if(buffer[3] == 'C')
		{
			mission3_path[0] = 10;
		}
		//printf("mission3_path[2] = %d\n", mission3_path[2]);
	}
	if((buffer[0]=='M')&&(buffer[1]=='3')&&(buffer[2]=='D'))
	{
		if(buffer[3] != 'C')
		{
			mission3_path[3] = (buffer[3] - '0');
		}
		else if(buffer[3] == 'C')
		{
			mission3_path[0] = 10;
		}
		//printf("mission3_path[3] = %d\n", mission3_path[3]);
	}

	if((buffer[0]=='M')&&(buffer[1]=='4')&&(buffer[2]!='A')&&(buffer[2]!='B')&&(buffer[2]!='C')&&(buffer[2]!='D')&&(buffer[2]!='E')&&(buffer[2]!='F'))
	{
		mission4_flag = (buffer[2] - '0');
	}
	if((buffer[0]=='M')&&(buffer[1]=='4')&&(buffer[2]=='A'))
	{
		mission4_flag = 10;
	}
	if((buffer[0]=='M')&&(buffer[1]=='4')&&(buffer[2]=='B'))
	{
		mission4_flag = 11;
	}
	if((buffer[0]=='M')&&(buffer[1]=='4')&&(buffer[2]=='C'))
	{
		mission4_flag = 12;
	}
	if((buffer[0]=='M')&&(buffer[1]=='4')&&(buffer[2]=='D'))
	{
		mission4_flag = 13;
	}
	if((buffer[0]=='M')&&(buffer[1]=='4')&&(buffer[2]=='E'))
	{
		mission4_flag = 14;
	}
	if((buffer[0]=='M')&&(buffer[1]=='4')&&(buffer[2]=='F'))
	{
		mission4_flag = 15;
	}

	if((buffer[0]=='M')&&(buffer[1]=='5')&&(buffer[2]!='A')&&(buffer[2]!='B')&&(buffer[2]!='C')&&(buffer[2]!='D')&&(buffer[2]!='E')&&(buffer[2]!='F'))
	{
		mission5_flag = (buffer[2] - '0');
		buffer[0] = 'B';
		buffer[1] = 'B';
		HAL_UART_Transmit(&huart1,buffer,3,0xff);
	}
	if((buffer[0]=='M')&&(buffer[1]=='5')&&(buffer[2]=='A'))
	{
		mission5_flag = 10;
		buffer[0] = 'B';
		buffer[1] = 'B';
		HAL_UART_Transmit(&huart1,buffer,3,0xff);
	}
	if((buffer[0]=='M')&&(buffer[1]=='5')&&(buffer[2]=='B'))
	{
		mission5_flag = 11;
		buffer[0] = 'B';
		buffer[1] = 'B';
		HAL_UART_Transmit(&huart1,buffer,3,0xff);
	}
	if((buffer[0]=='M')&&(buffer[1]=='5')&&(buffer[2]=='C'))
	{
		mission5_flag = 12;
		buffer[0] = 'B';
		buffer[1] = 'B';
		HAL_UART_Transmit(&huart1,buffer,3,0xff);
	}
	if((buffer[0]=='M')&&(buffer[1]=='5')&&(buffer[2]=='D'))
	{
		mission5_flag = 13;
		buffer[0] = 'B';
		buffer[1] = 'B';
		HAL_UART_Transmit(&huart1,buffer,3,0xff);
	}
	if((buffer[0]=='M')&&(buffer[1]=='5')&&(buffer[2]=='E'))
	{
		mission5_flag = 14;
		buffer[0] = 'B';
		buffer[1] = 'B';
		HAL_UART_Transmit(&huart1,buffer,3,0xff);
	}
	if((buffer[0]=='M')&&(buffer[1]=='5')&&(buffer[2]=='F'))
	{
		mission5_flag = 15;
		buffer[0] = 'B';
		buffer[1] = 'B';
		HAL_UART_Transmit(&huart1,buffer,3,0xff);
	}

	if((buffer[0]=='M')&&(buffer[1]=='6')&&(buffer[2]!='A')&&(buffer[2]!='B')&&(buffer[2]!='C')&&(buffer[2]!='D')&&(buffer[2]!='E')&&(buffer[2]!='F'))
	{
		mission5_flag = (buffer[2] - '0');
		buffer[0] = 'C';
		buffer[1] = 'C';
		HAL_UART_Transmit(&huart1,buffer,3,0xff);
	}
	if((buffer[0]=='M')&&(buffer[1]=='6')&&(buffer[2]=='A'))
	{
		mission5_flag = 10;
		buffer[0] = 'C';
		buffer[1] = 'C';
		HAL_UART_Transmit(&huart1,buffer,3,0xff);
	}
	if((buffer[0]=='M')&&(buffer[1]=='6')&&(buffer[2]=='B'))
	{
		mission5_flag = 11;
		buffer[0] = 'C';
		buffer[1] = 'C';
		HAL_UART_Transmit(&huart1,buffer,3,0xff);
	}
	if((buffer[0]=='M')&&(buffer[1]=='6')&&(buffer[2]=='C'))
	{
		mission5_flag = 12;
		buffer[0] = 'C';
		buffer[1] = 'C';
		HAL_UART_Transmit(&huart1,buffer,3,0xff);
	}
	if((buffer[0]=='M')&&(buffer[1]=='6')&&(buffer[2]=='D'))
	{
		mission5_flag = 13;
		buffer[0] = 'C';
		buffer[1] = 'C';
		HAL_UART_Transmit(&huart1,buffer,3,0xff);
	}
	if((buffer[0]=='M')&&(buffer[1]=='6')&&(buffer[2]=='E'))
	{
		mission5_flag = 14;
		buffer[0] = 'C';
		buffer[1] = 'C';
		HAL_UART_Transmit(&huart1,buffer,3,0xff);
	}
	if((buffer[0]=='M')&&(buffer[1]=='6')&&(buffer[2]=='F'))
	{
		mission5_flag = 15;
		buffer[0] = 'C';
		buffer[1] = 'C';
		HAL_UART_Transmit(&huart1,buffer,3,0xff);
	}
}

/// @brief 小车一次到位，用于前两问
/// @param dis_X 行距离
/// @param dis_Y 列距离
void Cart_Run(uint8_t dis_X,uint8_t dis_Y)
{
	Go_Foward(dis_Y);
	if(dis_X>0)
	{Turn_Right();
	Go_Foward(dis_X);}
	//printf("%d,%d\n",dis_X,dis_Y);
}

/// @brief 小车一格格前进，用于后四问
/// @param cart1 小车句柄，包含小车的位置和姿态
void Square_Run(CART cart1)
{
	
	//printf("cart1.position_now:%d\n",cart1.position_now);
	//printf("cart1.position_target:%d\n",cart1.position_target);
	if(cart1.position_target-cart1.position_now == FOWARD_ANGLE)
	{
		cart1.direct_target = FOWARD_DIRECT;
		//printf("FOWARD\n");
	}
	if(cart1.position_target-cart1.position_now == BACK_ANGLE)
	{
		cart1.direct_target = BACK_DIRECT;
		//printf("BACK\n");
	}
	if(cart1.position_target-cart1.position_now == LEFT_ANGLE)
	{
		cart1.direct_target = LEFT_DIRECT;
		//printf("LEFT\n");
	}
	if(cart1.position_target-cart1.position_now == RIGHT_ANGLE)
	{
		cart1.direct_target = RIGHT_DIRECT;
		//printf("RIGHT\n");
	}
	//printf("cart1.direct_target:%d\n",cart1.direct_target);
	//printf("cart1.direct_now:%d\n",cart1.direct_now);
	int direct_error = cart1.direct_target - cart1.direct_now;
	if(direct_error < 0){direct_error += 360;}
	//printf("direct_error:%d\n",direct_error);
	if(direct_error == FOWARD_DIRECT)
	{
		
	}
	else if(direct_error == BACK_DIRECT)
	{
		Turn_Around();
	}
	else if(direct_error == RIGHT_DIRECT)
	{
		Turn_Right();
	}
	else if(direct_error == LEFT_DIRECT)
	{
		Turn_Left();
	}
	Go_Foward(1);
	//printf("step done\n");
	cart.position_now = cart.position_target;
}

void Square_Init(void)
{
	square[0].name = '0';
	square[0].x = 1;
	square[0].y = 1;
	square[1].name = '1';
	square[1].x = 1;
	square[1].y = 2;
	square[2].name = '2';
	square[2].x = 1;
	square[2].y = 3;
	square[3].name = '3';
	square[3].x = 1;
	square[3].y = 4;
	square[4].name = '4';
	square[4].x = 2;
	square[4].y = 1;
	square[5].name = '5';
	square[5].x = 2;
	square[5].y = 2;
	square[6].name = '6';
	square[6].x = 2;
	square[6].y = 3;
	square[7].name = '7';
	square[7].x = 2;
	square[7].y = 4;
	square[8].name = '8';
	square[8].x = 3;
	square[8].y = 1;
	square[9].name = '9';
	square[9].x = 3;
	square[9].y = 2;
	square[10].name = 'A';
	square[10].x = 3;
	square[10].y = 3;
	square[11].name = 'B';
	square[11].x = 3;
	square[11].y = 4;
	square[12].name = 'C';
	square[12].x = 4;
	square[12].y = 1;
	square[13].name = 'D';
	square[13].x = 4;
	square[13].y = 2;
	square[14].name = 'E';
	square[14].x = 4;
	square[14].y = 3;
	square[15].name = 'F';
	square[15].x = 4;
	square[15].y = 4;
	square[16].name = 'S';
	square[16].x = 1;
	square[16].y = 0;
	cart.x = 1;
	cart.y = 0;
	cart.position_now = -1;
	cart.position_target = 0;
	cart.direct_now = FOWARD_DIRECT;
}

/// @brief ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param  
void State_Machine(void)
{
	switch(mission)
	{
		case 1:
			Cart_Run(square[3].x-cart.x,square[3].y-cart.y);
			HAL_UART_Transmit(&huart1, "000", 3, 0xffff);
			//HAL_UART_Transmit(&huart2,data_dash, 4, 0xffff);
			Beep();
			mission = 0;
			Init_car();
			break;
		case 2:
			//printf("mission2flag:%d\n", mission2_flag);
			if(mission2_flag != 0)
			{
			Cart_Run(square[mission2_flag].x-cart.x,square[mission2_flag].y-cart.y);
			HAL_UART_Transmit(&huart1, "000", 3, 0xffff);
			//HAL_UART_Transmit(&huart2,data_dash, 4, 0xffff);
			Beep();
			mission = 0;
				Init_car();
			}
			break;
		case 3:
			if(mission3_path[3] != 0)
			{
			//Cart_Run(square[mission3_path[0]].x-cart.x,square[mission3_path[0]].y-cart.y);
			for(int i=0;i<4;i++){cart.position_target = mission3_path[i];Square_Run(cart);}//printf("mission3_path[%d]:%d",i,mission3_path[i]);}
			HAL_UART_Transmit(&huart1, "000", 3, 0xffff);
			//HAL_UART_Transmit(&huart2,data_dash, 4, 0xffff);
			Beep();
			mission = 0;
			Init_car();
			}
			break;
		case 4:
		if(mission4_flag != 0)
		{
			Cart_Run(square[mission4_flag].x-cart.x,square[mission4_flag].y-cart.y);
			HAL_UART_Transmit(&huart1, "000", 3, 0xffff);
			Beep();
			mission = 0;
			Init_car();
		}
			break;
		case 5:
		case 6:
		//if(P_flag != 0){P_flag = 0; Square_Run(cart);}
			//if(mission5_end_flag == 1){HAL_UART_Transmit(&huart1, "000", 3, 0xffff);Beep();mission = 0;}
			if(mission5_start_flag != 0)
			{
			//Cart_Run(square[mission3_path[0]].x-cart.x,square[mission3_path[0]].y-cart.y);
			for(int i=0;i<mission5_path_length;i++){cart.position_target = mission5_path[i];Square_Run(cart);}//printf("mission5_path[%d]:%d",i,mission5_path[i]);}
			HAL_UART_Transmit(&huart1, "000", 3, 0xffff);
			//HAL_UART_Transmit(&huart2,data_dash, 4, 0xffff);
			Beep();
			mission = 0;
			Init_car();
			}
			break;
		default:
			break;
	}
}

void Direct_Change(char angle)
{
	if(angle == 'l'){cart.direct_now +=90;}
	if(angle == 'r'){cart.direct_now -=90;}
}

void Turn_Left(void)
{
	HAL_UART_Transmit(&huart2,data_left,4,0x200);
	Direct_Change('l');
	HAL_Delay(3000);
	//HAL_UART_Transmit(&huart2,data_dash,4,0x200);
	//HAL_Delay(1500);
}

void Turn_Right(void)
{
	HAL_UART_Transmit(&huart2,data_right,4,0x200);
	Direct_Change('r');
	HAL_Delay(3000);
	//HAL_UART_Transmit(&huart2,data_dash,4,0x200);
	//HAL_Delay(1500);
}

void Go_Foward(uint8_t distance)
{
	for(uint8_t i=0;i<distance;i++)
	{
		HAL_UART_Transmit(&huart2,data_forward,4,0x200);
		HAL_Delay(1000);

	}
	//printf("HALDELAY");
	HAL_Delay(3000);
}

void Turn_Around(void){
	HAL_UART_Transmit(&huart2,data_left,4,0x200);
	Direct_Change('l');
	HAL_Delay(1000);
	HAL_UART_Transmit(&huart2,data_left,4,0x200);
	Direct_Change('l');
	HAL_Delay(4000);
	//HAL_UART_Transmit(&huart2,data_dash,4,0x200);
	//HAL_Delay(2000);
	}

void Beep(void)
{
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14,GPIO_PIN_RESET);
		HAL_Delay(1000);
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14,GPIO_PIN_SET);
}

void Init_car(void)
{
	mission = 0;
		mission2_flag = 0;
		for(int i=0;i<4;i++)
		{
		    mission3_path[i] = 0;
		}
		mission4_flag = 0;
		mission5_end_flag = 0;
		mission5_start_flag = 0;
		mission6_end_flag = 0;
		P_flag =0;
		mission5_path_length = 0;
		for(int i=0;i<16;i++)
		{
		    mission5_path[i] = 0;
		}
		cart.direct_now = 0;
		cart.position_now = -1;
}
