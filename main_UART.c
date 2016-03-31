/******************************************************
程序功能：PC通过串口调试精灵向MCU发送数据，MCU将其在1602
          液晶上显示
-------------------------------------------------------
通信格式：N.8.1,  9600
------------------------------------------------------
测试说明：打开串口调试助手，正确设置通信格式，向从PC机上
          向学习板发送数据，观察液晶上显示的字符。
******************************************************/
#include  <msp430.h>
#include  "12864.h"
#include"delay.h"

void InitUART(void);
void PutString(uchar *ptr);
uchar table1[]={"波特率：9600"};
uchar table2[]={"接收到的数据为："};
uchar table[]={"0123456789"};
uchar buffer[100]={0};
uchar i=0,K=0;
/***************主函数************/
void main( void )
{
    /*下面六行程序关闭所有的IO口*/

    uchar *tishi = "sennd data to MCU, and they will be displayed on 12864!";

    WDTCTL = WDTPW + WDTHOLD;     //关狗

    InitUART();                   //初始化UART
    init();                   //初始化LCD

    PutString(tishi);
    _EINT();

      Disp_HZ(0x80,table1,6);
      Disp_HZ(0x90,table2,8);
   /* while(1)
    {
      // LPM1;
      Disp_HZ(0x80,table1,6);
      Disp_HZ(0x90,table2,8);

    }*/

}
/*******************************************
函数名称：PutSting
功    能：向PC机发送字符串
参    数：ptr--指向发送字符串的指针
返回值  ：无
********************************************/
void PutString(uchar *ptr)
{
      while(*ptr != '\0')
      {
            while (!(IFG1 & UTXIFG0));             // TX缓存空闲？发送完毕UTXIFG0置1，IFG1=0X80
            TXBUF0 = *ptr++;                       // 发送数据
      }
      while (!(IFG1 & UTXIFG0));
      TXBUF0 = '\n';
}
/*******************************************
函数名称：InitUART
功    能：初始化UART端口
参    数：无
返回值  ：无
********************************************/
void InitUART(void)
{
    UCTL0 |= SWRST;
    P3SEL |= 0x30;                            // P3.4,5 = USART0 TXD/RXD
    P3DIR|=BIT4;
    ME1 |= URXE0 + UTXE0;                             // Enable USART0 T/RXD
    UCTL0 |= CHAR;                            // 8-bit character
    UTCTL0 |= SSEL0;                          // UCLK = ACLK
    U0BR0 = 0x03;                             // 32k/9600 - 3.41
    U0BR1 = 0x00;                             //
    UMCTL0 = 0x4A;                            // Modulation
    UCTL0 &= ~SWRST;                          // Initialize USART state machine
    IE1 |= URXIE0;                            // 使能USART0的接收中断
}
/*******************************************
函数名称：UART0_RXISR
功    能：UART0的接收中断服务函数，在这里唤醒
          CPU，使它退出低功耗模式
参    数：无
返回值  ：无
********************************************/
#pragma vector = UART0RX_VECTOR
__interrupt void UART0_RXISR(void)
{
    i++;
    if(i==32)
    {
      i=0;
      write_cmd(0x01);   //清除显示
      Disp_HZ(0x80,table1,6);
      Disp_HZ(0x90,table2,8);
    }               //退出低功耗模式
  buffer[i]=RXBUF0;
   if(i<=15)
   {
   write_cmd(0x88);
   for(K=0;K<=i;K++)
     write_data(buffer[K]);
   }
   else
   {
   write_cmd(0x88);
   for(K=0;K<=15;K++)
     write_data(buffer[K]);
   write_cmd(0x98);
   for(K=16;K<=i;K++)
     write_data(buffer[K]);
   }
}


/*******************************************
函数名称：PutChar
功    能：向PC机发送一个字符对应的ASCII码
参    数：zifu--发送的字符
返回值  ：无
********************************************/
void PutChar(uchar zifu)
{
      while (!(IFG1 & UTXIFG0));
      if(zifu > 9)            //发送键值1~16对应的ASCII码
      {
          TXBUF0 = 0x30 + zifu/10;
          while (!(IFG1 & UTXIFG0));
          TXBUF0 = 0x30 + zifu%10;
      }
      else
      {
          TXBUF0 = 0x30 + zifu;
      }
      while (!(IFG1 & UTXIFG0));
      TXBUF0 = '\n';          //发送回车字符
}





