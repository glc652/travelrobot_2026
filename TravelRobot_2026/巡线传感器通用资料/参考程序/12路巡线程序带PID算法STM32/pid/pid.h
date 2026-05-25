#ifndef __pid_H
#define __pid_H

#include "sys.h"
extern float Moto1,Moto2;
extern float PID_sd;//小车差速PID控制器的PWM输出值
extern float PID_jc;//小车基础速度PID控制器的PWM输出值

void TIM5_IRQHandler(void);
int BaseSpeed_PD(int Actual,int Target,int speed_base);
float Position_PID(float Actual,int Target);
void PID_xunxian(void);
#endif

