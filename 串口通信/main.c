/*****用串口助手 发什么  回复什么****/



#include "msp430f5529.h"
//#include"stdio.h"
#include"main.h"
// ACLK = REFO = 32768Hz, MCLK = SMCLK = default DCO/2 = 1048576Hz
// P3.4，5——USCI_A0 TXD/RXD；P9.4,5——USCI_A2 TXD/RXD；P10.4,5——USCI_A3 TXD/RXD；
#define item 1//车体平衡时的倾角
#define KP 12
#define KD 1

#define dt 0.01
#define kp 2.1
#define ki 0.011//0.006
// ACLK = REFO = 32768Hz, MCLK = SMCLK = default DCO/2 = 1048576Hz
// P3.4，5——USCI_A0 TXD/RXD；P9.4,5——USCI_A2 TXD/RXD；P10.4,5——USCI_A3 TXD/RXD；
unsigned int table[12]={1,2,3,4,5,6,7,8,9,10,11,12};
unsigned int j,ii;
char str[50];
int test=5200;
int acc_y=0;
int acc_z=0;
int gyr_x=0;
float A_X;
int pwm1;
int Output=0;
int Output2=0;
int gyro_data=0;
float y1=0;
void Delay(unsigned int time)
{
  unsigned int i,k;
  for(i=0;i<255;i++)
    for(k=0;k<time;k++)
      _NOP();
}
float count_acc_angle(int y,int z)//计算三轴的倾角值
{
  float acc_angle=0;
  if(y>=0)
  {
    acc_angle=atan2((float)y,(float)z)*(180/3.14159265);  //得到倾角
  }
 else
  {
    acc_angle=atan2((float)z,(float)y)*(180/3.14159265)-90;
    acc_angle=-acc_angle;
  }
  return acc_angle;
}


float read_gyro(int gry_x)//得到陀螺仪的数据
{
  float gyr_angle=0;
   gyro_data=gry_x;
  gyr_angle=(float)(gyro_data+19)/16.4; //30.4后面积分是可以调节这个参数*0.030517
  return gyr_angle;
}

float blance_filter(float angle,float x,float y)  //互补的平衡滤波
{
  angle=(0.98)*(angle+y*dt)+(0.02)*x;//angle为真实的倾角
  return angle;
}

int PID_control(float erro0,float erro1)//角度环 PD
{

 Output=(int)(KP*(erro0-item)+erro1*KD);

return (Output);
}

int PID_control_2(float error0,float error1)//速度环PI
{
  Output2=(int)(kp*(error0)+ki*error1);
  return(Output2);
}

void send_buf( char *ptr)
{
    while(*ptr !='\0') //C语言里字符串末尾自动加“\0”(ASCII码值为0)
    {
          while (!(UCA1IFG&UCTXIFG));
          UCA1TXBUF=*ptr; //发送字符对应的ASCII码，12864指针可设置自动指向下一个显示地址
          *ptr++;
          Delay(50);
    }
    UCA1TXBUF=(char)520;
}
void send_shujv()
{
   // acc_y=GetData(ACCEL_YOUT_H);               //读取三轴Y的 值
  //  acc_z=GetData(ACCEL_ZOUT_H);               //读取三轴Z的 值
  //  gyr_x=GetData(GYRO_XOUT_H);                //读取陀螺仪X的值
  //  A_X=count_acc_angle(acc_y,acc_z);          //计算X角度a

	//UCA1TXBUF=F2S(3.14,str);
	Delay(50);
}
void F2S(float d, char* str)
{
  char str1[40];
  int j=0,k,i;
  i = (int)d;  //浮点数的整数部分
  //d = d-(int)d;
  while(i>0)
  {
    str1[j++] = i%10+'0';
    i /= 10;
  }
  for(k=0;k<j;k++)
    str[k] = str1[j-1-k]; //
  str[j++] = '.';
  d -= (int)d;
  for(i=0;i<10;i++)
  {
    d *= 10;
    str[j++] = (int)d+'0';
    d -= (int)d;
  }
  while(str[--j]=='0');
    str[++j] = '\0';
   // UAC1TXBUF=*str;
    while(*str !='\0') //C语言里字符串末尾自动加“\0”(ASCII码值为0)
      {
      //      while (!(UCA1IFG&UCTXIFG));
            UCA1TXBUF=*str; //发送字符对应的ASCII码，12864指针可设置自动指向下一个显示地址
            *str++;
            Delay(50);
      }
}
void main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  P1DIR|=BIT0;
  P1OUT|=BIT0;
  P2IE  |= BIT1; //设置p2.1可以中断
  P2IES |= BIT1; //设置p2.1为下降沿中断
  P2IFG &= ~BIT1;//设置p2.1为0 无中断请求
  P2REN |= BIT1; //设置p2.1为上下拉电阻使能
  P2OUT |= BIT1;
  P4SEL |=BIT4+BIT5 ;                             // P5.6,7 = USCI_A1 TXD/RXD
  UCA1CTL1 |= UCSWRST;                      // **Put state machine in reset**
  UCA1CTL1 |= UCSSEL_2;                     // SMCLK
  UCA1BR0 = 9;                              // 1MHz 115200 (see User's Guide)
  UCA1BR1 = 0;                              // 1MHz 115200
  UCA1MCTL |= UCBRS_1 + UCBRF_0;            // Modulation UCBRSx=1, UCBRFx=0
  UCA1CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
  UCA1IE |= UCRXIE;
  // Enable USCI_A1 RX interrupt
  ii=0;
//  __bis_SR_register(LPM0_bits + GIE);  // Enter LPM0, interrupts enabled
  while(1)
  {
	   acc_y=GetData(ACCEL_YOUT_H);               //读取三轴Y的 值
	   acc_z=GetData(ACCEL_ZOUT_H);               //读取三轴Z的 值
	   gyr_x=GetData(GYRO_XOUT_H);                //读取陀螺仪X的值
	   A_X=count_acc_angle(acc_y,acc_z);          //计算X角度a
	//send_buf(*F2S(3.1415926, char* str));
	  F2S(A_X, str);
	  
	
  }

}

#pragma vector=USCI_A1_VECTOR
__interrupt void USCI_A1_ISR(void)
{
  switch(__even_in_range(UCA1IV,4))
  {
  case 0:break;                             // Vector 0 - no interrupt
  case 2:                                   // Vector 2 - RXIFG
      while (!(UCA1IFG&UCTXIFG));    // USCI_A1 TX buffer ready?   UCTXIFG(USCI Transmit Interrupt Flag)
      switch(UCA1RXBUF)
     {
        case 0x00:{P1OUT^=BIT0;
                    UCA1TXBUF=UCA1RXBUF;}
          break;

        case 0x01:{UCA1TXBUF=table[j];
                    j++;
                    if(j==12) j=0;}

         break;
      }                 // TX -> RXed character
      break;
  case 4:break;                             // Vector 4 - TXIFG
  default: break;
  }
}
// UCTXIFG=0x02，UCA1IFG&UCTXIFG，当UCA1IFG的UCTXIFG位为1时，说明UCA1TXBUF为空，
//跳出while循环循环；当UCTXIFG位为0时UCA1TXBUF不为空，停在循环。

/*#pragma vector=PORT2_VECTOR
__interrupt void Key_interrput()
{
    if(P2IFG&BIT1)
    {
      Delay(50);
      if(P2IFG&BIT1)
      {
          while((P2IN&BIT1)==0);//松手检测
          P2IFG &= ~BIT1;  //清除中断标志位

          //send_buf("2015HelloWorld\r\n");
          //send_shujv();
      }
    }

}*/





