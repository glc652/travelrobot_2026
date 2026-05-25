#include <iom16v.h>
#include <macros.h>

unsigned char count_l=0,count_r=0;


void delay_1ms(unsigned int i)                  //延时程序
{
unsigned int j,k;
for(j=1;j<=i;j++)
	for(k=1;k<500;k++);
}


void delay_m()  //调直线专用延时
 {
 unsigned char p;
 for(p = 0; p<255;p++)
   NOP();
 }

#pragma interrupt_handler int0_isr:iv_INT0
void int0_isr(void)
{
 count_r++;
 delay_1ms(5);//external interupt on INT0
}

#pragma interrupt_handler int1_isr:iv_INT1
void int1_isr(void)
{
 count_l++;
 delay_1ms(5);//external interupt on INT1
}


//call this routine to initialize all peripherals
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

/**********************************************************/

void fwd1()  // 直线前行,调速用
{
PORTB = 0B00001010;
delay_1ms(5);
PORTB = 0X00;
delay_1ms(13);
}

 
void fwd(unsigned char n)   //直前行
{
unsigned char i;
//GICR = 0x00;  //关外中断
for(i=0;i<n;i++)
  {
  PORTB = 0B00000010;
  delay_m();
  fwd1();
  }     
}

void back()  //直后退
 {
 PORTB = 0B00000101;
 delay_1ms(5);
 PORTB = 0B00000100;
 delay(1);
 PORTB = 0X00;
 delay_1ms(13);
 }

/****************************************************************/ 
 
void port_init()   //端口初使化
{
DDRA  = 0X00;
PORTA = 0XFF;
DDRB  = 0XFF;
PORTB = 0X00;
DDRC  = 0X00;
PORTC = 0XFF;
DDRD  = 0X00;
PORTD = 0XFF;
}


/**************************************************************/
/*  
    左拐72度参数为 99 ， 左拐90度参数为130
    右拐72度参数为 111， 右拐90度参数为135
*/

void turn_ln(unsigned  char cc)   //左拐 n 度
{
GICR = 0xC0;  //开外中断
count_l = 0;
count_r = 0;
while((count_l + count_r) < cc)
 {
  turn_left(2);
  delay_1ms(1);
 }
 GICR = 0x00;  //关外中断
 PORTB = 0B00000110;
 delay_1ms(30);  // 反转加快停车
 PORTB = 0x00;
}


void turn_rn(unsigned char cc)   //右拐 n 度
{
GICR = 0xC0;  //开外中断
count_l = 0;
count_r = 0;
while((count_l + count_r) < cc)
 {
  turn_right(2);
  delay_1ms(1);
 }
 GICR = 0x00;  //关外中断
 PORTB = 0B00001001;
 delay_1ms(30);  // 反转加快停车
 PORTB = 0X00;
}



/*************************************************************/

void turn_left(unsigned char ii)  //左拐，要重复调用
{
unsigned char jj;
for(jj=0;jj<ii;jj++)
 {
 unsigned char dd;
 dd = PORTB;
 PORTB = 0B00001001;
 delay_1ms(3);
 stop(10);
 PORTB = dd;
 }
}


void turn_right(unsigned char ii)   //右拐，要重复调用
{
unsigned char jj;
for(jj=0;jj<ii;jj++)
 {
 unsigned char dd;
 dd = PORTB;
 PORTB = 0B000000110;
 delay_1ms(3);
 stop(10);
 PORTB = dd;
 }
}


/***************************************************************/

void stop(unsigned int oo) // 停止
{
PORTB = 0X00;
delay_1ms(oo);
}


void fast_stop() //快速停止
{
PORTB = 0B00000101;
delay_1ms(15);
PORTB = 0X00;
}

/***************************************************************/

void find_track()  //寻迹,用前要先关中断
{
if((PINA & 0x02) == 0)      turn_right(2);
else if((PINA & 0X08) == 0) turn_left(2);
else if((PINA & 0X01) == 0) turn_right(6);
else if((PINA & 0X10) == 0) turn_left(6);
}


void find_five() //寻五角形,用前要先关中断
{
unsigned char mm=0;
while(mm < 4)
 {
   fwd(1);
   if((PINA & 0x02) == 0) 
     turn_right(2);
   else if((PINA & 0X08) == 0) 
     turn_left(2);
/*
else if((PINA & 0X01) == 0)
  { 
  fast_stop();
  stop(250);
  turn_rn(111);
  stop(250);
 }
*/

   else if((PINA & 0X10) == 0)
    { 
     fast_stop();
     stop(250);
     turn_ln(99);
     stop(250);
     mm++;
    }
  }
}

/**********************^^^^***************************************/

