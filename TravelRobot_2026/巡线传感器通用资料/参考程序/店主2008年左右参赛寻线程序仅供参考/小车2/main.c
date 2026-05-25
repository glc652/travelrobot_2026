#include <iom16v.h>
#include <macros.h>

//主程序

void main() 
{
unsigned char i;
init_devices();
delay_1ms(3000);

/******************************主程序从此开始***************************/

door_open();

while((PINA & 0B00000010) != 0)  //当见到第一个黑线时，跳出循环
  {
  fwd(1);
  }
  fast_stop();  //快速停车
  stop(300);
  turn_ln(92);
  stop(500);
  fwd(90);   //向前走一段，让红外管不再照到黑线

  
while((PINA & 0B00010000) != 0)  //第二个右转，
    {
     fwd(1);
    } 
  fast_stop();  //快速停车
  stop(400);

  turn_rn(110); 
  stop(500); 
  fwd(100);   //向前走一段，让红外管不再照到黑线
 
  

while((PINA & 0B00010000) != 0)  //第三个右转，
    {
     fwd(1);
    } 
  fast_stop();  //快速停车
  stop(300);

  turn_rn(137); 
  stop(400); 
  fwd(100);   //向前走一段，让红外管不再照到黑线
  
  
while((PINA & 0B00010000) != 0)  //第四个右转，
    {
     fwd(1);
    } 
  fast_stop();  //快速停车
  stop(300);

  turn_rn(135); 
  stop(400); 
  fwd(90);   //向前走一段，让红外管不再照到黑线
  
    
  
while((PINA & 0B00001000) != 0)  //第五个左转，
    {
     fwd(1);
    } 
  fwd(50);
  door_close();	
  fast_stop();  //快速停车
  stop(300);

  turn_ln(99); 
  stop(400); 
  door_close();
  fwd(30);   //向前走一段，让红外管不再照到黑线
  
    
 /***********************************************************************/


while((PINA & 0B00000010) != 0)   //向黑线走，到了黑线停
  {
  fwd(1);
  }
fast_stop();
stop(300);
door_close();

turn_rn(150);
stop(400);

fwd(10);

while((PINA & 0B10000000)!= 0)   //寻迹，到蓝色区域
 {
  fwd1();
  find_track();
 }
 
for(i=0;i<50;i++)
  back();
  
stop(200);

door_open();

stop(250);

for(i = 0; i < 150;i++) 
   { 
     back();
	 delay_1ms(1);
	 back();
   }	
stop(200);   
turn_rn(145); //右拐90度，去寻五边形
stop(200);   
 
while((PINA & 0B00000001) != 0)
     fwd(1);
	 
fast_stop();
stop(500);

turn_rn(172);    //右拐108，进入五边形

stop(500);

for(i=0;i<30;i++)
 {
 fwd(1);
 find_track();
 }

  find_five();  //寻五角形程序
  stop(100);

while((PINA & 0B00010000) != 0)  //去搬最后一个红的
 {
 fwd(1);
 find_track();
 }   
 
 fast_stop();  
 stop(100); 
  
 door_close();   
 turn_rn(148);
 turn_rn(148);  // 关门，转180度
 
 stop(150);

while((PINA & 00000001) != 0)  //向回走，寻迹
   {
   fwd(1);
   find_track(); 
   }

while((PINA & 0B00010000) != 0)  //向黑线，走直线
  {
  fwd(1);
  }   
 fast_stop();
 stop(150);
 turn_ln(140);  
 stop(250);  

while((PINA & 10000000) != 0)  //向红色区域走，寻迹，直到撞到墙
   {
   fwd(1);
   find_track(); 
   } 
   
 fast_stop();
 stop(250);
 
 for(i=0;i<50;i++)  back(); 

  stop(200); 
  door_open();   //后退一点，开门
  stop(300);
  
  for(i=0;i<198;i++)  
   back();
   
stop(200);
turn_ln(98); 
stop(200);  //再后退一些，左拐

for(i=0;i<178;i++)
  {
  fwd(3); 
  }       //走向黄色区域

while(1)
  {
  NOP();  
  }      //停机

}
