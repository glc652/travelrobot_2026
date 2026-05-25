#include<iom16v.h>
#include<macros.h>


void delay_1ms(unsigned char i)           //1ms延时
{
unsigned char j;
unsigned int k;
for(j=i;j>0;j--)
	for(k=2286;k>0;k--);
}


void delay_10us(unsigned char ii)             //10us延时
 {
 unsigned char p,vv;
 for(vv=0; vv<ii; vv++)
    for(p = 0; p<38;p++);
 }

void stop(unsigned int oo)
{unsigned int p;
 for(p=0;p<oo;p++)
  {
  PORTA = 0X00;
  delay_1ms(10);
  }
}

void port_init()  // 端口初始化
{
DDRA  = 0XFF;
PORTA = 0X00;
DDRB  = 0X00;
PORTB = 0XFF;
DDRD  = 0X00;
PORTD = 0XFF;
}


void init_devices(void)
{
 //stop errant interrupts until set up
 CLI(); //disable all interrupts
 port_init();
 MCUCR = 0x05;  //关外中断不打开，用的时候再开
 GICR  = 0x00;
 TIMSK = 0x04; //timer interrupt sources
 SEI(); //re-enable interrupts
 //all peripherals are now initialized
}



void fwd(unsigned char n)
{
 unsigned char i;
  for(i=0;i<n;i++)
    PORTA = 0B00000001;
	delay_10us(30);
    PORTA = 0B00000101;
	delay_1ms(5);
}



void find_track()  //寻迹,用前要先关中断
{
   if((PINB & 0X08) == 0 )      turn_rr(2);
   else if((PINB & 0X02) == 0 ) turn_ll(2);
   else if((PINB & 0X10) == 0 ) turn_rr(7);
   else if((PINB & 0X01) == 0 ) turn_ll(7);
}




void find_five() //寻五角形,用前要先关中断
{
unsigned char mm=0;
while(mm < 3)
 {
   fwd(1);
   if((PINB & 0x08) == 0) 
     turn_rr(2);
   else if((PINB & 0X02) == 0) 
     turn_ll(2);
   else if((PINB & 0X01) == 0)
    { 
     turn_ln(125);
     mm++;
    }
  }
}

