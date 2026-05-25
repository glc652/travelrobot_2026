#include<iom16v.h>
#include<macros.h>

void main()
{
unsigned char i;

delay_1ms(250);
delay_1ms(250);
delay_1ms(250);   //开机后，停一秒

init_devices();   //初始化

door_open();

/*****************************************************************/


while((PINB & 0B00000100) != 0 )   //到第一个黑线，搬第一个蓝色
      fwd(1);

stop(100);
 
turn_ln(77);    //左拐54度
 
stop(20);

for(i=0;i<100;i++)  fwd();
 
 
while((PINB & 0B00000100) != 0 )   //搬到第二个色块
   fwd(1);
   
  stop(20);
      
 turn_rn(108);
 
 stop(20);
 
while((PINB & 0X04) != 0 )    //搬到第三个色块
   fwd(1);
   
 stop(20);
  
 turn_rn(108);
 
 stop(20);
   
while((PINB & 0X04) != 0 )    //搬到第四个色块
   fwd(1);
   
stop(20); 
   
turn_rn(108);

stop(20);
 
while((PINB & 0X04) != 0 )   // 搬到第五个色块
   fwd(1);  
    
turn_ln(108);
 
/*************************************************************/ 


for(i=0;i<50;i++)  fwd(1);

while((PINB & 0X04) != 0 )  fwd(1);  //到直黑线，

delay_1ms(50);

door_close();

turn_rn(156);

delay_1ms(50);

while((PIND & 0X04) !=0)
 {                                  //寻迹到蓝色区域
 fwd(1); 
 find_track();
 }
 
 delay_1ms(100);
 
 door_open();
 
 delay_1ms(100);
 
 back(300);
 
 delay_1ms(100);
 
 turn_rn(156);
 
 delay_1ms(100);
 
 while((PINB & 0X10) !=0 ) fwd(1);  //到五边形
  
 delay_1ms(100); 
 
 turn_rn(187);
 
 find_five(); 
 
 delay_1ms(50);//搬完最后一个红色
 
 door_close();
 
 turn_rn(187);  //右转去找黑线
 
 delay_1ms(50);
 
 while((PINB & 0X04) != 0 )
   fwd(1);
 
 delay_1ms(100);
 
 turn_ln(156);
 
 delay_1ms(50);
 
while((PIND & 0X04) !=0)
 {                                  //寻迹到红色区域
 fwd(1); 
 find_track();
 }
 
 stop(50);
 
 back(40);
 
 door_open();
 
 back(150);  //后退
 
 turn_ln(140);
 
 fwd(255);  //直行到黄色区域
 fwd(255);
 fwd(255);
 
 
while(1)
  {
   NOP();
  }
}