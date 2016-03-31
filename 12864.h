#ifndef __CRY12864_H__
#define __CRY12864_H__
#include"delay.h"
extern const unsigned char shuzi_table[];
#define Busy 0x80
#define Lcd_dataIn    P6DIR=0x00    //数据口方向设置为输入
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
/*void ifbusy() //读忙状态
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
函数名称：Ini_Lcd
功    能：初始化液晶模块
参    数：无
返回值  ：无
********************************************/
/*void Ini_Lcd(void)
{
    LcD_cMDOut;    //液晶控制端口设置为输出

   // Delay_Nms(500);
    write_cmd(0x30);   //基本指令集
    Delay_1ms();
    write_cmd(0x02);   // 地址归位
    Delay_1ms();
    write_cmd(0x0c);   //整体显示打开,游标关闭
    Delay_1ms();
    write_cmd(0x01);   //清除显示
    Delay_1ms();
    write_cmd(0x06);   //游标右移
    Delay_1ms();
    write_cmd(0x80);   //设定显示的起始地址
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
void     initina2(void)           //LcD显示图片(扩展)初始化程序
          {
           write_cmd(0x36);  //Extended Function Set RE=1: extended instruction
           delayms(1);            //大于100uS的延时程序
           write_cmd(0x36);  //Extended Function Set:RE=1: extended instruction set
           delayms(1);             ////大于37uS的延时程序
           write_cmd(0x3E);  //EXFUNcTION(DL=8BITS,RE=1,G=1)
           delayms(1);            //大于100uS的延时程序
           write_cmd(0x01);  //cLEAR ScREEN
           delayms(1);            //大于100uS的延时程序
          }
/*******************************************
函数名称:clear_GDRAM
功    能:清除液晶GDRAM中的随机数据
参    数:无
返回值  :无
********************************************/
void clear_GDRAM(void)
{
    uchar i,j,k;

	write_cmd(0x34);        //打开扩展指令集
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
	write_cmd(0x30);        //回到基本指令集

}
/*******************************************
函数名称：Disp_HZ
功    能：控制液晶显示汉字
参    数：addr--显示位置的首地址
          pt--指向显示数据的指针
          num--显示字符个数
返回值  ：无
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
函数名称：Draw_TX
功    能：显示一个16*16大小的图形
参    数：Yaddr--Y地址
          Xaddr--X地址
          dp--指向图形数据存放地址的指针
返回值  ：无
********************************************/
void Draw_TX(uchar Yaddr,uchar Xaddr,const uchar * dp)
{
    uchar j;
    uchar k=0;

   // write_cmd(0x01);  //清屏,只能清除DDRAM
    write_cmd(0x34);  //使用扩展指令集，关闭绘图显示
    for(j=0;j<16;j++)
    {
        write_cmd(Yaddr++);       //Y地址
        write_cmd(Xaddr);    //X地址
  	write_data(dp[k++]);
  	write_data(dp[k++]);
    }
    write_cmd(0x36);  //打开绘图显示
  //  write_cmd(0x30);  //回到基本指令集模式
}
/*******************************************
函数名称:DisplayGraphic
功    能:在整个液晶屏幕上画图
参    数:无
返回值  :无
********************************************/

void DisplayGraphic(const uchar *adder)
{

   int i,j;
//*******显示上半屏内容设置
   for(i=0;i<32;i++)              //
    {
      write_cmd(0x80 + i); //SET  垂直地址 VERTIcAL ADD
      write_cmd(0x80);       //SET  水平地址 HORIZONTAL ADD
      for(j=0;j<16;j++)
       {
          write_data(*adder);
         adder++;
       }
    }
//*******显示下半屏内容设置
   for(i=0;i<32;i++)              //
    {
      write_cmd((0x80 + i)); //SET 垂直地址 VERTIcAL ADD
      write_cmd(0x88);       //SET 水平地址 HORIZONTAL ADD
      for(j=0;j<16;j++)
       {
          write_data(*adder);
         adder++;
       }
    }
       write_cmd(0x36);        //打开绘图显示
}


/*在显示时DDAM和GDRAM是同时显示的，也就是它们的显示结果是叠加在一起的
    //SM12864液晶基本指令集控制命令测试//
    //1.设定DDRAM地址命令
    write_cmd(0x90);            //设定DDRAM地址，因为此时DDRAM地址已经溢出
    //2.显示状态命令
    write_cmd(0x08);            //整体显示关，游标关，游标位置关
    write_cmd(0x0c);            //整体显示开，游标关，游标位置关
    write_cmd(0x0e);            //整体显示开，游标开，游标位置关
    write_cmd(0x0f);            //整体显示开，游标开，游标位置开
    //3.位址归位
    write_cmd(0x02);            //位址归位，游标回到原点

    write_cmd(0x84);            //将DDRAM地址设为0x88，游标在此闪烁
    //4.点设定指令
    //（以下四个命令是控制写入字符以后光标及整屏显示的移动）
    write_cmd(0x07);            //光标右移整体显示左移
    write_Data(0x20);           //写入两个空格
    write_Data(0x20);

    write_cmd(0x05);            //光标左移整体显示右移
    write_Data(0x20);           //写入两个空格
    write_Data(0x20);

    write_cmd(0x06);            //光标右移整体显示不移动
    write_Data(0x20);           //写入两个空格
    write_Data(0x20);

    write_cmd(0x04);            //光标左移整体显示不移动
    write_Data(0x20);           //写入两个空格
    write_Data(0x20);
    //5.游标和显示移位控制
    //（以下四个命令无需写入显示数据，直接控制光标和整屏显示的移动）
    write_cmd(0x10);            //光标左移
    write_cmd(0x14);            //光标右移
    write_cmd(0x18);            //整体显示左移，光标跟随
    write_cmd(0x1c);            //整体显示右移，光标跟随

    write_cmd(0x0c);            //关闭光标

    //6.进入扩展功能模式命令
    write_cmd(0x34);            //打开扩展功能模式，绘图显示关闭
    //7.反白命令
    write_cmd(0x04);            //同时反白1、3行
    write_cmd(0x04);            //再次反白1、3行，相当于关闭1、3行反白
    write_cmd(0x05);            //同时反白2、4行
    write_cmd(0x05);            //再次反白2、4行，相当于关闭2、4行反白
    //8.睡眠模式命令
    write_cmd(0x08);            //进入睡眠模式
    write_cmd(0x0c);            //退出睡眠模式
    //9.待命模式命令
    write_cmd(0x01);            //进入待命模式
    //10.打开GDRAM显示
    write_cmd(0x36);            //打开扩展功能模式，打开绘图显示
    Draw_TX(0x80,0x84,laba);    //设置16*16大小图形
    clear_GDRAM();              //清除上电复位后RAM中的随机数值
    Draw_TX(0x80,0x84,laba);    //重新显示设置16*16大小图形
   //11.关闭GDRAM显示
    write_cmd(0x34);            //打开扩展功能模式，关闭绘图显示
    //12.设定基本指令集
    write_cmd(0x30);              //回到基本指令集
    //13.清除显示命令*/



#endif
