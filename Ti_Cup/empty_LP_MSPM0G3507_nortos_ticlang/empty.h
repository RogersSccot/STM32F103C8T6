/*
 * empty.h
 *
 *  Created on: 2024年7月29日
 *      Author: 86177
 */

#ifndef EMPTY_H_
#define EMPTY_H_

int fputc(int c,FILE* stream);
int fputs(const char* restrict s,FILE* restrict stream);
int puts(const char* _ptr);
void motor_left_run_velocity(float velocity);
void motor_right_run_velocity(float velocity);
void Go_Straight_Velocity(float velocity);
void Turn_Left_Velocity(float velocity);
void Turn_Right_Velocity(float velocity);
void Light_Sound(void);
float Speed_Low_Filter(float new_Spe, float *speed_Record);
void Buffer_Handle_0(void);
void Buffer_Handle_1(void);


#define Dis_Straight 0
#define SPEED_RECORD_NUM 10

typedef struct _LINE// 五路灰度循迹
{
    uint8_t line_1;
    uint8_t line_2;
    uint8_t line_3;
    uint8_t line_4;
    uint8_t line_5;

} LINE;

extern LINE line;
void Line_Fix(LINE line);

#endif /* EMPTY_H_ */
