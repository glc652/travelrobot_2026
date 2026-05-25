#include "pid.h"

float Moto1,Moto2;//Moto1和Moto2分别是左右两个电机PWM所计算出的值
float PID_sd=0.0;//小车差速PID控制器的PWM输出值
float PID_jc=0.0;//小车基础速度PID控制器的PWM输出值

//定时器5中断服务程序
void TIM5_IRQHandler(void)   //TIM5中断
{
	if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)  //检查TIM5更新中断发生与否
	{
		TIM_ClearITPendingBit(TIM5, TIM_IT_Update);  //清除TIMx更新中断标志
		PID_sd=Position_PID(Err,0);//将差速PID控制器输出的PWM值赋值给PID_sd//将error和0传入
		PID_jc=BaseSpeed_PD(a,0,15);//将基础速度PID控制器输出的PWM值赋值给PID_jc  

		Moto1=PID_jc+PID_sd;//基础速度-速度偏差量 //基础速度也可以恒定为某个值，这样调差速和舵机就比较好调  
		Moto2=PID_jc-PID_sd;//基础速度+速度偏差量
		Moto1/=100;			//除以分辨率得到真实的速度
		Moto2/=100;
//		if(Xunxian_GetState()==0x0fff)Moto1=Moto2=0;
		Xianfu_Pwm();
//		if(Moto1>96)Moto1=96;		//限幅
//		if(Moto2>96)Moto2=96;
//		
	}
	
}

/*************************************************
函数功能：将计算出来的PWM值赋值给电机控制函数
*************************************************/
void PID_xunxian(void)
{
	if(Xunxian_GetState()==0x0fff)
	{
		Moto1=Moto2=0;
	}
	CarSpeed_Mode = Car_GO;
	Car_ModeCtrl(Moto1,Moto2);
}
/**************************************************************************
///函数名称：Position_PD(比例和微分)
///函数功能：基础速度PID控制器
///入口参数：Actual:实际值  Target:目标值  speed_base:基础速度
///返 回 值：Pwm
///使用注意：PID控制属于位置式PID
**************************************************************************/
//基础速度PID控制器
int BaseSpeed_PD(int Actual,int Target,int speed_base)
{
	float Position_KP=200,Position_KD=8; //参数整定，需要调到适合自己小车的数值大小  //因为没有用到KI(积分)，所以这里去除了和KI相关的式子
	static float Bias,Last_Bias,Pwm;//定义静态局部变量
	Bias=Actual-Target; //偏差=实际值-目标值
	Pwm=speed_base*100-(Position_KP*Bias+Position_KD*(Bias-Last_Bias));//位置式PID控制器//这里6000是小车电机PWM的最大值,
	Last_Bias=Bias;//保存上一次偏差
	return Pwm;//返回PWM值
}

/**************************************************************************
///函数名称：Position_PID
///函数功能：差速PID控制器
///入口参数：Actual:实际值  Target:目标值
///返 回 值：Pwm
///使用注意：PID控制属于位置式PID
**************************************************************************/
//差速PID控制器
//Position_KI=0.45
//Integral_bias
//Integral_bias+=Bias;//求出偏差的积分
//Pwm=Position_KP*Bias+Position_KI*Integral_bias+Position_KD*(Bias-Last_Bias);
//提高100倍的分辨率
float Position_PID(float Actual,int Target)
{
	float Position_KP=75,Position_KI=0.0,Position_KD=5;
	static float Bias,Last_Bias,Integral_bias,Pwm;
	Bias=Actual-Target;//计算偏差
	Integral_bias+=Bias;
	Pwm=Position_KP*Bias+Position_KI*Integral_bias+Position_KD*(Bias-Last_Bias);
//	Pwm=Position_KP*Bias+Position_KD*(Bias-Last_Bias); //位置式PID控制器
	Last_Bias=Bias;//保存上一次偏差
	return Pwm;//返回PWM值
}

