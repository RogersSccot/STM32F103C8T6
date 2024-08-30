#include "mymain.h"

uint8_t mission; // 题号
uint8_t mission2_flag;
uint8_t mission3_flag;
uint8_t mission4_flag;
uint8_t mission5_flag;
uint32_t time_flag;
float Position_X, Position_Y;
float Velocity_X, Velocity_Y;
float target_A, target_B, target_C, target_D;
float Position_Record_X[LOCATION_RECORD_NUM] = {0};
float Position_Record_Y[LOCATION_RECORD_NUM] = {0};
float Velocity_Record_X[VELOCITY_RECORD_NUM] = {0};
float Velocity_Record_Y[VELOCITY_RECORD_NUM] = {0};
uint8_t mission6_path[4] = {0};
uint8_t m6_point=0;
uint8_t m7_point=0;
float Areap_X[6]={13.59f, 12.33f, 31.68f, 30.70f, 21.42f, 26.55f};
float Areap_Y[6]={22.81f, 40.55f, 23.52f, 41.69f, 32.59f, 28.05f};
float Mission7_Circle_X[28] = {13.59,21.42, 25.66,27.42, 25.66, 21.42, 17.17, 15.42, 17.17,21.42, 25.66,27.42, 25.66, 21.42, 17.17, 15.42, 17.17,21.42, 25.66,27.42, 25.66, 21.42, 17.17, 15.42,17.17,21.42,25.66,30.70};
float Mission7_Circle_Y[28] = {22.81,26.59, 28.34,32.59, 36.83, 38.59, 36.83, 32.59, 28.34,26.59, 28.34,32.59, 36.83, 38.59, 36.83, 32.59, 28.34,26.59, 28.34,32.59, 36.83, 38.59, 36.83, 32.59,28.34,26.59,28.34,41.69};
uint8_t data[1] = "1";

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

/// @brief 主程序
/// @param
void mymain(void)
{
	PID_controll();
	State_Machine();
}

/// @brief 串口1信息处理，计算滤波后位置
/// @param buffer
void buffer_hadle_USART1(uint8_t *buffer)
{
	HAL_UART_Transmit(&huart1, data, 1, 0xff);
	switch (buffer[0])
	{
	case 'X':
		ball_last.position_x = ball_now.position_x;
		Position_X = (buffer[1] - '0') * 10.0f + (buffer[2] - '0') * 1.0f + (buffer[4] - '0') * 0.1f + (buffer[5] - '0') * 0.01f;
		ball_now.position_x = Location_Low_Filter(Position_X, Position_Record_X);
		Velocity_X = (ball_now.position_x - ball_last.position_x) / TIME_INTERVAL;
		ball_now.velocity_x = Velocity_Low_Filter(Velocity_X, Velocity_Record_X);

		ball_last.position_y = ball_now.position_y;
		Position_Y = (buffer[7] - '0') * 10.0f + (buffer[8] - '0') * 1.0f + (buffer[10] - '0') * 0.1f + (buffer[11] - '0') * 0.01f;
		ball_now.position_y = Location_Low_Filter(Position_Y, Position_Record_Y);
		Velocity_Y = (ball_now.position_y - ball_last.position_y) / TIME_INTERVAL;
		ball_now.velocity_y = Velocity_Low_Filter(Velocity_Y, Velocity_Record_Y);
		break;

	case 'T':
		// HAL_UART_Transmit(&huart1,data,1,0xff);
		steer_x.angle = (buffer[1] - '0') * 10.0f + (buffer[2] - '0') * 1.0f + (buffer[4] - '0') * 0.1f + (buffer[5] - '0') * 0.01f;
		steer_y.angle = (buffer[7] - '0') * 10.0f + (buffer[8] - '0') * 1.0f + (buffer[10] - '0') * 0.1f + (buffer[11] - '0') * 0.01f;
		steer_rotate_x(steer_x.angle);
		steer_rotate_y(steer_y.angle);
		break;

	case 'M':
		ball_target.position_x = (buffer[1] - '0') * 10.0f + (buffer[2] - '0') * 1.0f + (buffer[4] - '0') * 0.1f + (buffer[5] - '0') * 0.01f;
		ball_target.position_y = (buffer[7] - '0') * 10.0f + (buffer[8] - '0') * 1.0f + (buffer[10] - '0') * 0.1f + (buffer[11] - '0') * 0.01f;
		break;
	}
}

/// @brief 串口3信息处理，获取题号
/// @param buffer
void buffer_hadle_USART3(uint8_t *buffer)
{
	if (buffer[0] == '1')
	{
		mission = 1;
	}
	if (buffer[0] == '2')
	{
		mission = 2;
	}
	if (buffer[0] == '3')
	{
		mission = 3;
	}
	if (buffer[0] == '4')
	{
		mission = 4;
	}
	if (buffer[0] == '5')
	{
		mission = 5;
	}
	if (buffer[0] == '6')
	{
		// mission = 6;
		
	}
	if (buffer[0] == '7')
	{
		mission = 7;
	}

	if (buffer[0] == 'R')
	{
		mission = 0;
		mission2_flag = 0;
		mission3_flag = 0;
		mission4_flag = 0;
		mission5_flag = 0;
		m6_point = 0;
		m7_point = 0;
	}

	if (buffer[0] == 'A')
	{
		mission6_path[0] = buffer[1] - '0' - 1;
	}
	if (buffer[0] == 'B')
	{
		mission6_path[1] = buffer[1] - '0' - 1;
	}
	if (buffer[0] == 'C')
	{
		mission6_path[2] = buffer[1] - '0' - 1;
	}
	if (buffer[0] == 'D')
	{
		mission6_path[3] = buffer[1] - '0' - 1;
		mission = 6;
		time_flag=0;
	}
	if (buffer[0] == 'T')
	{
		// HAL_UART_Transmit(&huart1,data,1,0xff);
		steer_x.angle = (buffer[1] - '0') * 10.0f + (buffer[2] - '0') * 1.0f + (buffer[4] - '0') * 0.1f + (buffer[5] - '0') * 0.01f;
		steer_y.angle = (buffer[7] - '0') * 10.0f + (buffer[8] - '0') * 1.0f + (buffer[10] - '0') * 0.1f + (buffer[11] - '0') * 0.01f;
		steer_rotate_x(steer_x.angle);
		steer_rotate_y(steer_y.angle);
	}

	if ((buffer[0] == 'P') && (buffer[1] == 'X') && (buffer[2] == 'V'))
	{
		pid_velocity_x.kp = ((buffer[3] - '0') * 1.0 + (buffer[5] - '0') * 0.1 + (buffer[6] - '0') * 0.01) * (-1.0f);
	}
	if (buffer[0] == 'I' && (buffer[1] == 'X') && (buffer[2] == 'V'))
	{
		pid_velocity_x.ki = ((buffer[3] - '0') * 1.0 + (buffer[5] - '0') * 0.1 + (buffer[6] - '0') * 0.01) * (-1.0f);
	}
	if (buffer[0] == 'D' && (buffer[1] == 'X') && (buffer[2] == 'V'))
	{
		pid_velocity_x.kd = ((buffer[3] - '0') * 1.0 + (buffer[5] - '0') * 0.1 + (buffer[6] - '0') * 0.01) * (-1.0f);
	}

	if ((buffer[0] == 'P') && (buffer[1] == 'Y') && (buffer[2] == 'V'))
	{
		pid_velocity_y.kp = (buffer[3] - '0') * 1.0 + (buffer[5] - '0') * 0.1 + (buffer[6] - '0') * 0.01;
	}
	if (buffer[0] == 'I' && (buffer[1] == 'Y') && (buffer[2] == 'V'))
	{
		pid_velocity_y.ki = (buffer[3] - '0') * 1.0 + (buffer[5] - '0') * 0.1 + (buffer[6] - '0') * 0.01;
	}
	if (buffer[0] == 'D' && (buffer[1] == 'Y') && (buffer[2] == 'V'))
	{
		pid_velocity_y.kd = (buffer[3] - '0') * 1.0 + (buffer[5] - '0') * 0.1 + (buffer[6] - '0') * 0.01;
	}

	if ((buffer[0] == 'P') && (buffer[1] == 'X') && (buffer[2] == 'L'))
	{
		pid_position_x.kp = (buffer[3] - '0') * 1.0 + (buffer[5] - '0') * 0.1 + (buffer[6] - '0') * 0.01;
	}
	if (buffer[0] == 'I' && (buffer[1] == 'X') && (buffer[2] == 'L'))
	{
		pid_position_x.ki = (buffer[3] - '0') * 1.0 + (buffer[5] - '0') * 0.1 + (buffer[6] - '0') * 0.01;
	}
	if (buffer[0] == 'D' && (buffer[1] == 'X') && (buffer[2] == 'L'))
	{
		pid_position_x.kd = (buffer[3] - '0') * 1.0 + (buffer[5] - '0') * 0.1 + (buffer[6] - '0') * 0.01;
	}

	if ((buffer[0] == 'P') && (buffer[1] == 'Y') && (buffer[2] == 'L'))
	{
		pid_position_y.kp = (buffer[3] - '0') * 1.0 + (buffer[5] - '0') * 0.1 + (buffer[6] - '0') * 0.01;
	}
	if (buffer[0] == 'I' && (buffer[1] == 'Y') && (buffer[2] == 'L'))
	{
		pid_position_y.ki = (buffer[3] - '0') * 1.0 + (buffer[5] - '0') * 0.1 + (buffer[6] - '0') * 0.01;
	}
	if (buffer[0] == 'D' && (buffer[1] == 'Y') && (buffer[2] == 'L'))
	{
		pid_position_y.kd = (buffer[3] - '0') * 1.0 + (buffer[5] - '0') * 0.1 + (buffer[6] - '0') * 0.01;
	}
}

/// @brief 位置滤波
/// @param new_Loc 当前位置
/// @param location_Record 位置记录数组
/// @return
float Location_Low_Filter(float new_Loc, float *location_Record)
{
	float sum = 0.0f;
	for (uint8_t i = LOCATION_RECORD_NUM - 1; i > 0; i--)
	{
		location_Record[i] = location_Record[i - 1];
		sum += location_Record[i - 1];
	}
	location_Record[0] = new_Loc;
	sum += new_Loc;
	return sum / LOCATION_RECORD_NUM;
}

/// @brief 速度滤波
/// @param new_Spe 当次速度
/// @param velocity_Record 速度记录数组
/// @return
float Velocity_Low_Filter(float new_Spe, float *velocity_Record)
{
	float sum = 0.0f;
	for (uint8_t i = VELOCITY_RECORD_NUM - 1; i > 0; i--)
	{
		velocity_Record[i] = velocity_Record[i - 1];
		sum += velocity_Record[i - 1];
	}
	velocity_Record[0] = new_Spe;
	sum += new_Spe;
	return sum / VELOCITY_RECORD_NUM;
}

/// @brief PID控制
/// @param
void PID_controll(void)
{
	// PID计算
	ball_target.velocity_x = Position_PID_Realize(&pid_position_x, ball_target.position_x, ball_now.position_x);
	ball_target.velocity_y = Position_PID_Realize(&pid_position_y, ball_target.position_y, ball_now.position_y);
	steer_x.pid_angle = Velocity_PID_Realize(&pid_velocity_x, ball_target.velocity_x, ball_now.velocity_x);
	steer_y.pid_angle = Velocity_PID_Realize(&pid_velocity_y, ball_target.velocity_y, ball_now.velocity_y);
	// 舵机角度上下限
	steer_x.pid_angle = steer_x.pid_angle + 30.4;
	steer_y.pid_angle = steer_y.pid_angle + 27.9;

	if (steer_x.pid_angle > 90)
	{
		steer_x.pid_angle = 90;
	}
	else if (steer_x.pid_angle < 0)
	{
		steer_x.pid_angle = 0;
	}
	if (steer_y.pid_angle > 90)
	{
		steer_y.pid_angle = 90;
	}
	else if (steer_y.pid_angle < 0)
	{
		steer_y.pid_angle = 0;
	}
	// 舵机执行
	steer_rotate_x(steer_x.pid_angle);
	steer_rotate_y(steer_y.pid_angle);

	// printf("P:%.2f I:%.2f D:%.2f\r\n",pid_velocity_x.kp,pid_velocity_x.ki,pid_velocity_x.kd);
	printf("%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.1f,%.1f\r\n", ball_now.position_x, ball_now.position_y, ball_target.position_x, ball_target.position_y, ball_now.velocity_x, ball_now.velocity_y, ball_target.velocity_x, ball_target.velocity_y, steer_x.pid_angle, steer_y.pid_angle);
}

/// @brief 状态机
/// @param
void State_Machine(void)
{
	switch (mission)
	{
	case 0:
		ball_target.position_x = Area5_x;
		ball_target.position_y = Area5_y;
		printf("0");
		break;
	case 1:
		ball_target.position_x = Area2_x;
		ball_target.position_y = Area2_y;
		printf("1");
		break;
	case 2:
	if ((ball_now.position_x < Area1_x + 1.0f) && (ball_now.position_x > Area1_x - 1.0f) && (ball_now.position_y < Area1_y + 1.0f) && (ball_now.position_y > Area1_y - 1.0f) && (ball_now.velocity_x < 0.5) && (ball_now.velocity_x > -0.5) && (ball_now.velocity_y < 0.5) && (ball_now.velocity_y > -0.5))
		{
			time_flag++;
			if (time_flag >= 50)
			{
				mission2_flag = 1;
				time_flag = 0;
			}
		}
		if (mission2_flag == 0)
		{
			ball_target.position_x = Area1_x;
			ball_target.position_y = Area1_y;
		}
		if (mission2_flag == 1)
		{
			ball_target.position_x = Area5_x;
			ball_target.position_y = Area5_y;
		}
		break;
	case 3:
		if ((ball_now.position_x < Area1_x + 1.0f) && (ball_now.position_x > Area1_x - 1.0f) && (ball_now.position_y < Area1_y + 1.0f) && (ball_now.position_y > Area1_y - 1.0f) && (ball_now.velocity_x < 0.5) && (ball_now.velocity_x > -0.5) && (ball_now.velocity_y < 0.5) && (ball_now.velocity_y > -0.5))
		{
			time_flag++;
			if (time_flag >= 30)
			{
				mission3_flag = 1;
				time_flag = 0;
			}
		}
		if ((ball_now.position_x < Area5_x + 1.0f) && (ball_now.position_x > Area5_x - 1.0f) && (ball_now.position_y < Area5_y + 1.0f) && (ball_now.position_y > Area5_y - 1.0f) && (ball_now.velocity_x < 0.5) && (ball_now.velocity_x > -0.5) && (ball_now.velocity_y < 0.5) && (ball_now.velocity_y > -0.5))
		{
			time_flag++;
			if (time_flag >= 30)
			{
				mission3_flag = 2;
				time_flag = 0;
			}
		}
		if (mission3_flag == 0)
		{
			ball_target.position_x = Area1_x;
			ball_target.position_y = Area1_y;
		}
		if (mission3_flag == 1)
		{
			ball_target.position_x = Area5_x;
			ball_target.position_y = Area5_y;
		}
		else if (mission3_flag == 2)
		{
			ball_target.position_x = Area4_x;
			ball_target.position_y = Area4_y;
		}
		break;
	case 4:
		if ((ball_now.position_x < Area1_x + 1.0f) && (ball_now.position_x > Area1_x - 1.0f) && (ball_now.position_y < Area1_y + 1.0f) && (ball_now.position_y > Area1_y - 1.0f) && (ball_now.velocity_x < 0.5) && (ball_now.velocity_x > -0.5) && (ball_now.velocity_y < 0.5) && (ball_now.velocity_y > -0.5))
		{
			time_flag++;
			if (time_flag >= 50)
			{
				mission4_flag = 1;
				time_flag = 0;
			}
		}
		if ((ball_now.position_x < 28.55) && (ball_now.position_x > 24.55) && (ball_now.position_y < 30.05) && (ball_now.position_y > 26.05) && (ball_now.velocity_x < 0.5) && (ball_now.velocity_x > -0.5) && (ball_now.velocity_y < 0.5) && (ball_now.velocity_y > -0.5))
		{
			time_flag++;
			if (time_flag >= 10)
			{
				mission4_flag = 2;
				time_flag = 0;
			}
		}
		if (mission4_flag == 0)
		{
			ball_target.position_x = Area1_x;
			ball_target.position_y = Area1_y;
		}
		if (mission4_flag == 1)
		{
			ball_target.position_x = Area6_x;
			ball_target.position_y = Area6_y;
		}
		else if (mission4_flag == 2)
		{
			ball_target.position_x = Area4_x;
			ball_target.position_y = Area4_y;
		}
		break;
	case 5:
		if ((ball_now.position_x < Area1_x + 1.0f) && (ball_now.position_x > Area1_x - 1.0f) && (ball_now.position_y < Area1_y + 1.0f) && (ball_now.position_y > Area1_y - 1.0f) && (ball_now.velocity_x < 0.5) && (ball_now.velocity_x > -0.5) && (ball_now.velocity_y < 0.5) && (ball_now.velocity_y > -0.5))
		{
			time_flag++;
			if (time_flag >= 30)
			{
				mission5_flag = 1;
				time_flag = 0;
			}
		}
		if ((ball_now.position_x < Area2_x + 1.0f) && (ball_now.position_x > Area2_x - 1.0f) && (ball_now.position_y < Area2_y + 1.0f) && (ball_now.position_y > Area2_y - 1.0f) && (ball_now.velocity_x < 0.5) && (ball_now.velocity_x > -0.5) && (ball_now.velocity_y < 0.5) && (ball_now.velocity_y > -0.5))
		{
			time_flag++;
			if (time_flag >= 30)
			{
				mission5_flag = 2;
				time_flag = 0;
			}
		}
		if ((ball_now.position_x < Area5_x + 1.0f) && (ball_now.position_x > Area5_x - 1.0f) && (ball_now.position_y < Area5_y + 1.0f) && (ball_now.position_y > Area5_y - 1.0f) && (ball_now.velocity_x < 0.5) && (ball_now.velocity_x > -0.5) && (ball_now.velocity_y < 0.5) && (ball_now.velocity_y > -0.5))
		{
			time_flag++;
			if (time_flag >= 30)
			{
				mission5_flag = 3;
				time_flag = 0;
			}
		}
		if (mission5_flag == 0)
		{
			ball_target.position_x = Area1_x;
			ball_target.position_y = Area1_y;
		}
		if (mission5_flag == 1)
		{
			ball_target.position_x = Area2_x;
			ball_target.position_y = Area2_y;
		}
		else if (mission5_flag == 2)
		{
			ball_target.position_x = Area5_x;
			ball_target.position_y = Area5_y;
		}
		else if (mission5_flag == 3)
		{
			ball_target.position_x = Area4_x;
			ball_target.position_y = Area4_y;
		}
		break;
	case 6:
		ball_target.position_x = Areap_X[mission6_path[m6_point]];
		ball_target.position_y = Areap_Y[mission6_path[m6_point]];
		if ((ball_now.position_x < Areap_X[mission6_path[m6_point]] + 1.5f) && (ball_now.position_x > Areap_X[mission6_path[m6_point]] - 1.5f) && (ball_now.position_y < Areap_Y[mission6_path[m6_point]] + 1.0f) && (ball_now.position_y > Areap_Y[mission6_path[m6_point]] - 1.0f) && (ball_now.velocity_x < 0.5) && (ball_now.velocity_x > -0.5) && (ball_now.velocity_y < 0.5) && (ball_now.velocity_y > -0.5))
		{
			time_flag++;
			if((time_flag>=50)&(m6_point<4))
			{
				m6_point++;
				time_flag=0;
			}
			if(m6_point == 4)
			{
				mission = 9;
			}
		}
		break;
	case 7:
		ball_target.position_x = Mission7_Circle_X[m7_point];
		ball_target.position_y = Mission7_Circle_Y[m7_point];
		if ((ball_now.position_x < Mission7_Circle_X[m7_point] + 1.5f) && (ball_now.position_x > Mission7_Circle_X[m7_point] - 1.5f) && (ball_now.position_y < Mission7_Circle_Y[m7_point] + 1.5f) && (ball_now.position_y > Mission7_Circle_Y[m7_point] - 1.5f) && (ball_now.velocity_x < 2.5) && (ball_now.velocity_x > -2.5) && (ball_now.velocity_y < 2.5) && (ball_now.velocity_y > -2.5))
		{
			time_flag++;
			if((time_flag>=5)&(m7_point<28))
			{
				m7_point++;
				time_flag=0;
			}
			if(m7_point == 28)
			{
				mission = 8;
			}
		}
		break;
	case 8:
		ball_target.position_x = Area4_x;
		ball_target.position_y = Area4_y;
		break;
	case 9:
		ball_target.position_x = Areap_X[mission6_path[3]];
		ball_target.position_y = Areap_Y[mission6_path[3]];
		break;
	default:
		break;
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	printf("2");
	if (htim->Instance == GAP_TIM.Instance)
	{
		printf("1");
	}
}
