#ifndef __CRY12864_H__
#define __CRY12864_H__
#include"delay.h"
extern const unsigned char shuzi_table[];
#define Busy 0x80
#define Lcd_dataIn    P6DIR=0x00    //���ݿڷ�������Ϊ����
#define lcd_dataout    P6DIR=0XFF
#define mcu2lcd_data   P6OUT
#define lcd2mcu_data   P6IN
#define lcd_cmdout     P3DIR|=0X07
#define rs_h           P3OUT|=BIT0
#define rs_l           P3OUT&=~BIT0
#define rw_h           P3OUT|=BIT1
#define rw_l           P3OUT&=~BIT1
#define en_h           P3OUT|=BIT2
#define en_l           P3OUT&=~BIT2
/*void ifbusy() //��æ״̬
{uchar lCdtemp = 0;
      while(1)
    {  LCD_DataIn;
    rs_l;
    rw_h;
    en_h;
    _NOP();
    lcdtemp = LCD2MCU_Data;
    en_l;
    if((lCdtemp&Busy)==0)
    break;
  }
}*/
void write_cmd(uchar cmd)
{//ifbusy();
  rs_l;
  rw_l;
  //lcd_dataout;
  mcu2lcd_data=cmd;
  delayms(5);
  en_h;
  delayms(5);
  en_l;
}

void write_data(uchar dat)
{
  //ifbusy();
  rs_h;


  mcu2lcd_data=dat;
  delayms(5);
  en_h;
  delayms(5);
  en_l;
}

/*******************************************
�������ƣ�Ini_Lcd
��    �ܣ���ʼ��Һ��ģ��
��    ������
����ֵ  ����
********************************************/
/*void Ini_Lcd(void)
{
    LcD_cMDOut;    //Һ�����ƶ˿�����Ϊ���

   // Delay_Nms(500);
    write_cmd(0x30);   //����ָ�
    Delay_1ms();
    write_cmd(0x02);   // ��ַ��λ
    Delay_1ms();
    write_cmd(0x0c);   //������ʾ��,�α�ر�
    Delay_1ms();
    write_cmd(0x01);   //�����ʾ
    Delay_1ms();
    write_cmd(0x06);   //�α�����
    Delay_1ms();
    write_cmd(0x80);   //�趨��ʾ����ʼ��ַ
}*/
void init()
{
lcd_dataout;
lcd_cmdout;
  en_l;
  write_cmd(0x30);
  write_cmd(0x0c);
  write_cmd(0x01);

}
void     initina2(void)           //LcD��ʾͼƬ(��չ)��ʼ������
          {
           write_cmd(0x36);  //Extended Function Set RE=1: extended instruction
           delayms(1);            //����100uS����ʱ����
           write_cmd(0x36);  //Extended Function Set:RE=1: extended instruction set
           delayms(1);             ////����37uS����ʱ����
           write_cmd(0x3E);  //EXFUNcTION(DL=8BITS,RE=1,G=1)
           delayms(1);            //����100uS����ʱ����
           write_cmd(0x01);  //cLEAR ScREEN
           delayms(1);            //����100uS����ʱ����
          }
/*******************************************
��������:clear_GDRAM
��    ��:���Һ��GDRAM�е��������
��    ��:��
����ֵ  :��
********************************************/
void clear_GDRAM(void)
{
    uchar i,j,k;

	write_cmd(0x34);        //����չָ�
	i = 0x80;
	for(j = 0;j < 32;j++)
	{
        write_cmd(i++);
        write_cmd(0x80);
  		for(k = 0;k < 16;k++)
  		{
  		    write_data(0x00);
  		}
	}
	i = 0x80;
 	for(j = 0;j < 32;j++)
	{
 	    write_cmd(i++);
        write_cmd(0x88);
  		for(k = 0;k < 16;k++)
  		{
   		    write_data(0x00);
   		}
	}
	write_cmd(0x30);        //�ص�����ָ�

}
/*******************************************
�������ƣ�Disp_HZ
��    �ܣ�����Һ����ʾ����
��    ����addr--��ʾλ�õ��׵�ַ
          pt--ָ����ʾ���ݵ�ָ��
          num--��ʾ�ַ�����
����ֵ  ����
********************************************/
void Disp_HZ(uchar addr,const uchar * pt,uchar num)
{
    uchar i;

    write_cmd(addr);
      for(i = 0;i < (num*2);i++)
      {
      write_data(*(pt++));
      delayms(1);
      }
}

/*******************************************
�������ƣ�Draw_TX
��    �ܣ���ʾһ��16*16��С��ͼ��
��    ����Yaddr--Y��ַ
          Xaddr--X��ַ
          dp--ָ��ͼ�����ݴ�ŵ�ַ��ָ��
����ֵ  ����
********************************************/
void Draw_TX(uchar Yaddr,uchar Xaddr,const uchar * dp)
{
    uchar j;
    uchar k=0;

   // write_cmd(0x01);  //����,ֻ�����DDRAM
    write_cmd(0x34);  //ʹ����չָ����رջ�ͼ��ʾ
    for(j=0;j<16;j++)
    {
        write_cmd(Yaddr++);       //Y��ַ
        write_cmd(Xaddr);    //X��ַ
  	write_data(dp[k++]);
  	write_data(dp[k++]);
    }
    write_cmd(0x36);  //�򿪻�ͼ��ʾ
  //  write_cmd(0x30);  //�ص�����ָ�ģʽ
}
/*******************************************
��������:DisplayGraphic
��    ��:������Һ����Ļ�ϻ�ͼ
��    ��:��
����ֵ  :��
********************************************/

void DisplayGraphic(const uchar *adder)
{

   int i,j;
//*******��ʾ�ϰ�����������
   for(i=0;i<32;i++)              //
    {
      write_cmd(0x80 + i); //SET  ��ֱ��ַ VERTIcAL ADD
      write_cmd(0x80);       //SET  ˮƽ��ַ HORIZONTAL ADD
      for(j=0;j<16;j++)
       {
          write_data(*adder);
         adder++;
       }
    }
//*******��ʾ�°�����������
   for(i=0;i<32;i++)              //
    {
      write_cmd((0x80 + i)); //SET ��ֱ��ַ VERTIcAL ADD
      write_cmd(0x88);       //SET ˮƽ��ַ HORIZONTAL ADD
      for(j=0;j<16;j++)
       {
          write_data(*adder);
         adder++;
       }
    }
       write_cmd(0x36);        //�򿪻�ͼ��ʾ
}


/*����ʾʱDDAM��GDRAM��ͬʱ��ʾ�ģ�Ҳ�������ǵ���ʾ����ǵ�����һ���
    //SM12864Һ������ָ������������//
    //1.�趨DDRAM��ַ����
    write_cmd(0x90);            //�趨DDRAM��ַ����Ϊ��ʱDDRAM��ַ�Ѿ����
    //2.��ʾ״̬����
    write_cmd(0x08);            //������ʾ�أ��α�أ��α�λ�ù�
    write_cmd(0x0c);            //������ʾ�����α�أ��α�λ�ù�
    write_cmd(0x0e);            //������ʾ�����α꿪���α�λ�ù�
    write_cmd(0x0f);            //������ʾ�����α꿪���α�λ�ÿ�
    //3.λַ��λ
    write_cmd(0x02);            //λַ��λ���α�ص�ԭ��

    write_cmd(0x84);            //��DDRAM��ַ��Ϊ0x88���α��ڴ���˸
    //4.���趨ָ��
    //�������ĸ������ǿ���д���ַ��Ժ��꼰������ʾ���ƶ���
    write_cmd(0x07);            //�������������ʾ����
    write_Data(0x20);           //д�������ո�
    write_Data(0x20);

    write_cmd(0x05);            //�������������ʾ����
    write_Data(0x20);           //д�������ո�
    write_Data(0x20);

    write_cmd(0x06);            //�������������ʾ���ƶ�
    write_Data(0x20);           //д�������ո�
    write_Data(0x20);

    write_cmd(0x04);            //�������������ʾ���ƶ�
    write_Data(0x20);           //д�������ո�
    write_Data(0x20);
    //5.�α����ʾ��λ����
    //�������ĸ���������д����ʾ���ݣ�ֱ�ӿ��ƹ���������ʾ���ƶ���
    write_cmd(0x10);            //�������
    write_cmd(0x14);            //�������
    write_cmd(0x18);            //������ʾ���ƣ�������
    write_cmd(0x1c);            //������ʾ���ƣ�������

    write_cmd(0x0c);            //�رչ��

    //6.������չ����ģʽ����
    write_cmd(0x34);            //����չ����ģʽ����ͼ��ʾ�ر�
    //7.��������
    write_cmd(0x04);            //ͬʱ����1��3��
    write_cmd(0x04);            //�ٴη���1��3�У��൱�ڹر�1��3�з���
    write_cmd(0x05);            //ͬʱ����2��4��
    write_cmd(0x05);            //�ٴη���2��4�У��൱�ڹر�2��4�з���
    //8.˯��ģʽ����
    write_cmd(0x08);            //����˯��ģʽ
    write_cmd(0x0c);            //�˳�˯��ģʽ
    //9.����ģʽ����
    write_cmd(0x01);            //�������ģʽ
    //10.��GDRAM��ʾ
    write_cmd(0x36);            //����չ����ģʽ���򿪻�ͼ��ʾ
    Draw_TX(0x80,0x84,laba);    //����16*16��Сͼ��
    clear_GDRAM();              //����ϵ縴λ��RAM�е������ֵ
    Draw_TX(0x80,0x84,laba);    //������ʾ����16*16��Сͼ��
   //11.�ر�GDRAM��ʾ
    write_cmd(0x34);            //����չ����ģʽ���رջ�ͼ��ʾ
    //12.�趨����ָ�
    write_cmd(0x30);              //�ص�����ָ�
    //13.�����ʾ����*/



#endif
