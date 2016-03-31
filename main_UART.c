/******************************************************
�����ܣ�PCͨ�����ڵ��Ծ�����MCU�������ݣ�MCU������1602
          Һ������ʾ
-------------------------------------------------------
ͨ�Ÿ�ʽ��N.8.1,  9600
------------------------------------------------------
����˵�����򿪴��ڵ������֣���ȷ����ͨ�Ÿ�ʽ�����PC����
          ��ѧϰ�巢�����ݣ��۲�Һ������ʾ���ַ���
******************************************************/
#include  <msp430.h>
#include  "12864.h"
#include"delay.h"

void InitUART(void);
void PutString(uchar *ptr);
uchar table1[]={"�����ʣ�9600"};
uchar table2[]={"���յ�������Ϊ��"};
uchar table[]={"0123456789"};
uchar buffer[100]={0};
uchar i=0,K=0;
/***************������************/
void main( void )
{
    /*�������г���ر����е�IO��*/

    uchar *tishi = "sennd data to MCU, and they will be displayed on 12864!";

    WDTCTL = WDTPW + WDTHOLD;     //�ع�

    InitUART();                   //��ʼ��UART
    init();                   //��ʼ��LCD

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
�������ƣ�PutSting
��    �ܣ���PC�������ַ���
��    ����ptr--ָ�����ַ�����ָ��
����ֵ  ����
********************************************/
void PutString(uchar *ptr)
{
      while(*ptr != '\0')
      {
            while (!(IFG1 & UTXIFG0));             // TX������У��������UTXIFG0��1��IFG1=0X80
            TXBUF0 = *ptr++;                       // ��������
      }
      while (!(IFG1 & UTXIFG0));
      TXBUF0 = '\n';
}
/*******************************************
�������ƣ�InitUART
��    �ܣ���ʼ��UART�˿�
��    ������
����ֵ  ����
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
    IE1 |= URXIE0;                            // ʹ��USART0�Ľ����ж�
}
/*******************************************
�������ƣ�UART0_RXISR
��    �ܣ�UART0�Ľ����жϷ������������﻽��
          CPU��ʹ���˳��͹���ģʽ
��    ������
����ֵ  ����
********************************************/
#pragma vector = UART0RX_VECTOR
__interrupt void UART0_RXISR(void)
{
    i++;
    if(i==32)
    {
      i=0;
      write_cmd(0x01);   //�����ʾ
      Disp_HZ(0x80,table1,6);
      Disp_HZ(0x90,table2,8);
    }               //�˳��͹���ģʽ
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
�������ƣ�PutChar
��    �ܣ���PC������һ���ַ���Ӧ��ASCII��
��    ����zifu--���͵��ַ�
����ֵ  ����
********************************************/
void PutChar(uchar zifu)
{
      while (!(IFG1 & UTXIFG0));
      if(zifu > 9)            //���ͼ�ֵ1~16��Ӧ��ASCII��
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
      TXBUF0 = '\n';          //���ͻس��ַ�
}





