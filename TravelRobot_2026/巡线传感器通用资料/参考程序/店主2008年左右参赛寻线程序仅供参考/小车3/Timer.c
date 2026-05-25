/* 
***电子钟程序***
***使用Atmega8,使用2.000MHz片内晶振***
***左按键用于功能选择***
***中按键用于加，跑秒开始，或暂停后开始***
***右按键用于减，跑秒暂停，再按一下归零，若暂停时退出则自动归零。闹铃响时，关闭闹铃***
  *****功能键被按功能顺序*****
                  调整小时
                  调整分钟
                  调整月
                  调整日
                 调闹铃时
		         调闹铃分
                  跑秒    
*/


#include<iom8v.h>
#include<macros.h>

unsigned char led[10]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};
char hour=12,minute=0,second=0,day=1,month=1;  //初始化时，分，秒，日，月
unsigned char ledchar[5];  //数管五位显示的字符
unsigned char maxday;
unsigned int  ms=0,cms;
char alarm_hh = 0,alarm_mm = 0,alarm_on = 0; // 闹铃时间，闹铃开关,0为关
unsigned char keya = 0; //记录按键A被按的次数

//以下为子函数声明

void display();    //数码管显示子函数
void key();     //键盘扫描子函数
void power();    //低电量判断
void start();   //初始化端口和计时器
void alarm();   //闹铃设置子函数
void delay(unsigned char);  //延时子程序
void month_select(); //每月最大天数选择
void timer_adjust(); //时间调整函数
void month_adjust(); //月份调整函数
void count(); //秒表函数


//以下为子程序

//1ms定时器0中断，8分频
#pragma interrupt_handler timer0_ovf_isr:iv_TIM0_OVF
void timer0_ovf_isr(void)
{
  TCNT0 = 0x06; 
  ms++;
  if ((ms % 10) == 0) cms++;
  if(ms == 1000)  
  {
   ms = 0;
   second++;
   if(second == 60)
     {
       second = 0;
       minute++;
       if(minute == 60)
         {
          minute = 0;
          hour++;
          if(hour == 24)
            {
             hour = 0;
             day++;
             if(day == maxday)
                {
                day = 1;
                month++;  
                if(month == 13) month = 1;
                else month_select();
                } 
            } 
         }
      }
   }    
}   
  
  
void timer_desplay()
{  
  if(hour<10)  ledchar[0] = 0x00;
  else  ledchar[0] = led[hour/10]; //若第一位为0 ，则不显示
  ledchar[1] = led[hour%10];
  if( ms < 300 )  ledchar[2] = 0x40;
   else  ledchar[2] = 0x00;
  ledchar[3] = led[minute/10];
  ledchar[4] = led[minute%10];  
}


void start()   //初始化端口和计时器
{
 DDRD=0XFF;
 PORTD=0XFF;
 DDRB=0XFF;
 PORTB=0XFF;
 DDRC=0XF0;  //设高四位为输出，低四位为输入
 PORTC=0X0F;
 
  //stop errant interrupts until set up
 CLI(); //disable all interrupts
 TCCR0 = 0x00; //stop
 TCNT0 = 0x06; //set count
 TCCR0 = 0x02; //start timer
 MCUCR = 0x00;
 GICR  = 0x00;
 TIMSK = 0x01; //timer interrupt sources
 SEI(); //re-enable interrupts
}


//低电量判断
void power()
 {
 if((PINC & 0X08) == 0X00) 
   PORTC = PORTC | 0X10;
  else PORTC = PORTC & 0XEF;
 }
 
  
//延时函数
 void delay(unsigned char j)
 {
 unsigned char i,k;
 for(i = 0;i < j; i++)
    for(k = 0;k < 255;k++);
 }
 
 
//数字显示函数 
 void display()
 {
 unsigned char j;
 for(j = 0;j < 5; j++)
   {
    PORTB = 0xFF;
    PORTD = ledchar[j];
    PORTB = ~BIT(j);
    delay(5);
   } 
 }
 

//每个月日期数计算
void month_select()
{
 switch(month)
  {
    case 1:
    case 3:
    case 5:
    case 7:
    case 8:
    case 10:
    case 12:  maxday = 32; break;
    case 2:   maxday = 29; break;
    default:  maxday = 31; break;
   } 
} 


//闹铃函数
void alarm()
{
if((alarm_hh == hour) && (alarm_mm == minute) && (alarm_on == 1))
 { 
   PORTC = ~BIT(6); 
   if((PINC & 0X04) == 0X00) alarm_on = 0; 
 }

}


//键盘扫描函数 
void key()
{
   if((PINC & 0X01) == 0x00) 
    {
     delay(255);
     if(keya > 6) keya = 0;
     else keya++;
    }
}


 //时间调整函数  ，只显示调整的
void timer_adjust()
{
    switch(keya)   // 时间调整
    {
     case 1 :
      if(hour<10)  ledchar[0] = 0x00;
      else  ledchar[0] = led[hour/10]; //若第一位为0 ，则不显示
      ledchar[1] = led[hour%10];
      ledchar[2] = 0x00;
      ledchar[3] = 0x00;
      ledchar[4] = 0x00;
      if((PINC & 0X02) == 0X00)
         {
          delay(255);
          hour++;
		  if(hour == 24) hour = 0;
         }
      if((PINC & 0X04) == 0X00)
        {
          delay(255);
          hour--;
		  if(hour == -1) hour = 23;
        }
      break;  
    case 2:
      if( ms < 300 )  ledchar[2] = 0x40;
      else  ledchar[2] = 0x00;
       ledchar[3] = led[minute/10];
       ledchar[4] = led[minute%10]; 
       ledchar[0] = 0x00;
       ledchar[1] = 0x00;
     if((PINC & 0X02) == 0X00)
       {
        delay(255);
        minute++;
		if(minute == 60) minute = 0;
       }
      if((PINC & 0X04) == 0X00)
       {
       delay(255);
       minute--;
	   if(minute == -1) minute = 59;
       }
     break; 
    case 3:    //日期调整
     if(month < 10)  ledchar[0] = 0x00;
      else  ledchar[0] = led[month/10]; //若第一位为0 ，则不显示
      ledchar[1] = led[month%10];
      ledchar[2] = 0x00;
      ledchar[3] = 0x00;
      ledchar[4] = 0x00;
      if((PINC & 0X02) == 0X00)
       {
        delay(255);
        month++;
		if(month == 13) month = 1;
       }
     if((PINC & 0X04) == 0X00)
       {
        delay(255);
        month--;
		if(month == 0) month = 12;
       }
      break; 
    case 4:
       ledchar[3] = led[day/10];
       ledchar[4] = led[day%10]; 
       ledchar[0] = 0x00;
       ledchar[1] = 0x00;
       ledchar[2] = 0x00;
       if((PINC & 0X02) == 0X00)
        {
         delay(255);
         day++;
		 if(day > maxday) day = 0;
        }
       if((PINC & 0X04) == 0X00)
       {
        delay(255);
        day--;
		if(day == 0) day = maxday;
       }
      break;  
    case 5 :  // 闹铃调整
     if(alarm_hh<10)  ledchar[0] = 0x00;
      else  ledchar[0] = led[alarm_hh/10]; //若第一位为0 ，则不显示
      ledchar[1] = led[alarm_hh%10];
      ledchar[2] = 0x00;
      ledchar[3] = 0x00;
      ledchar[4] = 0x00;
      if((PINC & 0X02) == 0X00)
       {
        delay(255);
        alarm_hh++;
		if(alarm_hh == 24) alarm_hh = 0;
       }
     if((PINC & 0X04) == 0X00)
       {
        delay(255);
        alarm_hh--;
		if(alarm_hh == -1) alarm_hh = 23;
       }
      break;
     case 6:  
       ledchar[3] = led[alarm_mm/10];
       ledchar[4] = led[alarm_mm%10]; 
       ledchar[0] = 0x00;
       ledchar[1] = 0x00;
       ledchar[2] = 0x00;
       if((PINC & 0X02) == 0X00)
        {
         delay(255);
         alarm_mm++;
		 if(alarm_mm == 60) alarm_mm = 0;
        }
       if((PINC & 0X04) == 0X00)
       {
        delay(255);
        alarm_mm--;
		if(alarm_mm == -1) alarm_mm = 59;
       }
       break; 
      case 7 :
        count();
       break;
   }   
}    

//秒表函数
void count()
{
 unsigned char k,count_on = 0;  
 
   for(k = 0;k < 5;k++)   ledchar[k]=led[0];
   
   if((PINC & 0X02) == 0X00)
    {
      delay(255);
      count_on = 1;
    }
   if((PINC & 0X04) == 0X00)
    {
      delay(255);
      if(count_on == 1)
       {
        for(k = 0;k < 5;k++)
        ledchar[k]=led[0];
       }
      else count_on = 0;
    }
 
 if (count_on == 1)
   {
   cms = 0;
   ledchar[4] = led[(cms%100)/10];
   ledchar[3] = led[(cms%10)/100];
   }
} 
   
void key_display()
{
 if((PINC & 0X02) == 0X00) //显示日期
    {
     if(month<10)  ledchar[0] = 0x00;
      else  ledchar[0] = led[month/10]; //若第一位为0 ，则不显示
      ledchar[1] = led[month%10];
	  ledchar[2] = 0x00;
	  ledchar[3] = led[day/10];
      ledchar[4] = led[day%10]; 
    }
if((PINC & 0X04) == 0X00)  //显示闹铃时间，按一次开或关闹铃
    {
     if(alarm_hh<10)  ledchar[0] = 0x00;
     else  ledchar[0] = led[alarm_hh/10]; //若第一位为0 ，则不显示
     ledchar[1] = led[alarm_hh%10]; 
	 ledchar[3] = led[alarm_mm/10];
     ledchar[4] = led[alarm_mm%10]; 
	 
    if((PINC & 0X02) == 0X00 && alarm_on == 0)
	   {
	    alarm_on = 1;
		ledchar[2] = 0x40;
	   }
	 else if((PINC & 0X02) == 0X00 && alarm_on == 1)
	   {
	    alarm_on = 0;
		ledchar[2] = 0x00;
	   }
    }	
}


//主函数
void main()
{
 start();
 month_select();
 while(1)
   { 
	power();
	key();
	alarm();
	if(keya == 0)
	 { 
	  timer_desplay();
	  key_display();
	  }
	else timer_adjust();
	display();
   }
}
