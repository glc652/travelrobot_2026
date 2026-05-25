#include<iom16v.h>


void delay(unsigned char j)  //调舵机专用延时
  {
 unsigned char i,k;
 for(i = 0;i < j; i++)
    for(k = 0;k < 200;k++);
  }
  

  
void door_close()
{
unsigned char i;
for(i=0;i<10;i++)
{
  PORTB = PORTB | 0X10;
  delay(30);
  PORTB = PORTB & 0XEF;
  delay(200);
  delay(180);
}
}



void door_open()
{
unsigned char i;
for(i=0;i<10;i++)
{
  PORTB = PORTB | 0X10;
  delay(20);
  PORTB = PORTB & 0XEF;
  delay(200);
  delay(180); 
}
}