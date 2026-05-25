#include<iom16v.h>
#include<macros.h>


unsigned int count_l=0,count_r=0;


#pragma interrupt_handler int0_isr:iv_INT0
void int0_isr(void)
{
 count_l++;
 delay_1ms(2); 
   
}


#pragma interrupt_handler int1_isr:iv_INT1
void int1_isr(void)
{
 count_r++;
 delay_1ms(2);  
}


/********************************************************/

void turn_ll(unsigned char oo) //两个轮子同量转，拐弯
{
unsigned char i;
 for(i=0;i<oo;i++)
 {
  PORTA = 0B00001001;
  delay_1ms(10);
  PORTA =0X00;
  delay_1ms(15);
  }
}

void turn_rr(unsigned char oo) //两个轮子同量转，拐弯
{
unsigned char i;
 for(i=0;i<oo;i++)
  {
    PORTA = 0B00000110;
	delay_1ms(10);
	PORTA =0X00;
    delay_1ms(15);
	}
}

/*********************************************************/

void turn_ln(unsigned  int cc)   //左拐 n 度
{
GICR = 0xC0;  //开外中断
count_l = 0;
count_r = 0;

while((count_l + count_r) < cc)
 {
   turn_ll(2);
 }
 GICR = 0x00;  //关外中断
}



void turn_rn(unsigned int cc)   //右拐 n 度
{
GICR = 0xC0;  //开外中断
count_l = 0;
count_r = 0;

while((count_l + count_r) < cc) 
 {
  turn_rr(2);
 }
 GICR = 0x00;  //关外中断
}


void back(unsigned int pp)
{
count_l = 0;
GICR = 0XC0;
while(pp > count_l)
  {
   PORTA = 0B00001000;
   delay_10us(30);
   PORTA = 0B00001010;
   delay_1ms(5);
   }
}


void fwd2(unsigned char uu)
{
count_l = 0;
GICR = 0XC0;
while(count_l < uu)  
  fwd(1);
}