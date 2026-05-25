#include<iom16v.h>


void door_close()
{
unsigned char i;
for(i=0;i<10;i++)
{
  PORTA = PORTA | 0X10;
  
  delay_10us(150);
  PORTA = PORTA & 0B11101111;
  delay_1ms(20);
}
}


void door_open()
{
unsigned char i;
for(i=0;i<10;i++)
{
  PORTA = PORTA | 0X10;
  delay_10us(100);
  PORTA = PORTA & 0B11101111;
  delay_1ms(20); 
}
}