#include "xunxian.h"

float Err = 0.0;
int a=0;//小车基础速度的标志变量，a越小时，基础速度越大
int way_flag=0;
/****************************
函数名称：Xunxian_Init
函数作用：12路循迹初始化
函数参数：无
函数返回值：无
****************************/
void Xunxian_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	//时钟使能
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOB,ENABLE);
	
	//模式配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11;
	GPIO_Init(GPIOB,&GPIO_InitStructure);	
}

/****************************
函数名称：
函数作用：12路循迹获取状态
函数参数：无
函数返回值：那个灯检测到有线了
****************************/
uint16_t Xunxian_GetState(void)
{
	uint16_t State = 0X0000;
	State |= (TCRT_OUT1 << 11);
	State |= (TCRT_OUT2 << 10);
	State |= (TCRT_OUT3 << 9);
	State |= (TCRT_OUT4 << 8);
	State |= (TCRT_OUT5 << 7);
	State |= (TCRT_OUT6 << 6);
	State |= (TCRT_OUT7 << 5);
	State |= (TCRT_OUT8 << 4);
	State |= (TCRT_OUT9 << 3);
	State |= (TCRT_OUT10 << 2);
	State |= (TCRT_OUT11 << 1);
	State |= (TCRT_OUT12 << 0);
	return State;
}

/**读取灰度检测的结果，返回出来一个偏差***/
//左边返回出来的数为负，右边为正
void Read_Err(void)
{
//	if(Xunxian_GetState()==0x0fff)
//	{
//		CarSpeed_Mode = Car_STOP;
//		Car_ModeCtrl(0,0);
//	}
	if(TCRT_OUT6==have&&TCRT_OUT7==have)
	{
		Err = 0;
		a=0;
	}
	else if(TCRT_OUT5==no&&TCRT_OUT6==have&&TCRT_OUT7==no)
	{
		Err = -3.66;
		a=0;
	}
	else if(TCRT_OUT5==no&&TCRT_OUT6==no&&TCRT_OUT7==have)
	{
		Err = 3.66;
		a=0;
	}
	else if(TCRT_OUT5==have&&TCRT_OUT6==have&&TCRT_OUT7==no)
	{
		Err=-8.13;
		a=0;
	}
	else if(TCRT_OUT6==no&&TCRT_OUT7==have&&TCRT_OUT8==have)
	{
		Err = 8.13;
		a=0;
	}
	else if(TCRT_OUT4==have&&TCRT_OUT5==have&&TCRT_OUT6==no&&TCRT_OUT7==no)
	{
		Err = -12.51;
		a=0;
	}
	else if(TCRT_OUT6==no&&TCRT_OUT7==no&&TCRT_OUT8==have&&TCRT_OUT9==have)
	{
		Err = 12.51;
		a=0;
	}
	else if(TCRT_OUT3==have&&TCRT_OUT4==have&&TCRT_OUT5==no&&TCRT_OUT6==no)
	{
		Err = -16.74;
		a=0;
	}
	else if(TCRT_OUT7==no&&TCRT_OUT8==no&&TCRT_OUT9==have&&TCRT_OUT10==have)
	{
		Err = 16.74;
		a=0;
	}
	else if(TCRT_OUT2==have&&TCRT_OUT3==have&&TCRT_OUT4==no&&TCRT_OUT5==no)
	{
		Err = -20.79;
		a=0;
	}
	else if(TCRT_OUT8==no&&TCRT_OUT9==no&&TCRT_OUT10==have&&TCRT_OUT11==have)
	{
		Err = 20.79;
		a=0;
	}
	else if(TCRT_OUT1==have&&TCRT_OUT2==have&&TCRT_OUT3==no&&TCRT_OUT4==no)
	{
		Err = -28.26;
		a=-5;
	}
	else if(TCRT_OUT9==no&&TCRT_OUT10==no&&TCRT_OUT11==have&&TCRT_OUT12==have)
	{
		Err = 28.26;
		a=-5;
	}
	else 
	{
		Err = 0;
		a=0;
		way_flag=1;
//		CarSpeed_Mode = Car_STOP;
//		Car_ModeCtrl(0,0);
	}
//	return Err;
}
