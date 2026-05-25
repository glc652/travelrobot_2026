#ifndef __xunxian_H
#define __xunxian_H

#include "sys.h"
/*判断有无检测到线*/
#define have 0
#define no 1
/*12路灰度引脚定义*/
#define TCRT_OUT1 (GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_8))
#define TCRT_OUT2 (GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9))
#define TCRT_OUT3 (GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_0))
#define TCRT_OUT4 (GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_1))
#define TCRT_OUT5 (GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_2))
#define TCRT_OUT6 (GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_3))
#define TCRT_OUT7 (GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_4))
#define TCRT_OUT8 (GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_5))
#define TCRT_OUT9 (GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_6))
#define TCRT_OUT10 (GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_7))
#define TCRT_OUT11 (GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_10))
#define TCRT_OUT12 (GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11))
extern float Err;
extern int a;//小车基础速度的标志变量，a越小时，基础速度越大
extern int way_flag;
void Xunxian_Init(void);
uint16_t Xunxian_GetState(void);
void xunxian(float LineSpeed);
void Read_Err(void);
//float Read_Err(void);
//void TRCT_MOTOR(uint8_t speed_line);
#endif

