////******LCD12864*****///
/*
 *12864显示函数display(n,m,*str)  n为行m为列*str为数组首地址
 */

#include <msp430f5529.h>
#include "LCD12864.h"
#define uchar unsigned char
#define uint unsigned int
extern unsigned char ADS1256_buf[9];


void int_port(void) //管脚初始化
{
    P2SEL&=~BIT4; //P2.4模拟SID，设置为i/o口输出方向
    P2DIR|=BIT4;
    P2OUT&=~BIT4;
    P2SEL&=~BIT5; //P2.5模拟SCLK，设置为i/o口输出方向
    P2DIR|=BIT5;
    P2OUT&=~BIT5;
}
/********************************************************************
* 名称 : delay()
*
* 输入 : t
* 输出 : 无
***********************************************************************/
void delay(uint t) //延时函数
{ //粗略延时，满足时序要求
    uint i,j;
    for(i=0; i<t; i++)
    for(j=0; j<10; j++);
}
/********************************************************************
* 名称 : sendbyte()
* 功能 : 按照液晶的串口通信协议，发送数据
* 输入 : zdata
* 输出 : 无
***********************************************************************/
void sendbyte(uchar zdata) //数据传送函数
{
    uchar code_seg7;
    uchar serial_clk;
    uchar serial_shift;
    code_seg7=zdata;
    serial_shift=0x80;
    for(serial_clk=0; serial_clk<8; serial_clk++)
    {
        if(code_seg7&serial_shift)
        {
            P2OUT|=BIT4; //SID为1
        }
        else
        {
            P2OUT&=~BIT4; //SID为0
        }
        P2OUT&=~BIT5; //产生时钟信号下沿
        P2OUT|=BIT5; //产生时钟信号上沿
        serial_shift=serial_shift>>1; //准备发送下一位数据
    }
}
/********************************************************************
* 名称 : write_com()
* 功能 : 写串口指令
* 输入 : cmdcode
* 输出 : 无
***********************************************************************/
void write_com(unsigned char cmdcode) //写命令函数
{
    //串口控制格式（11111AB0）
    //A数据方向控制，A=H时读，A=L时写
    //B数据类型选择，B=H时为显示数据，B=L时为命令
    sendbyte(0xf8); //MCU向LCD发命令
    sendbyte(cmdcode & 0xf0); //发高四位数据(数据格式D7D6D5D4_0000)
    sendbyte((cmdcode << 4) & 0xf0);//发低四位数据(数据格式D3D2D1D0_0000)
    delay(2); //延时等待
}

/********************************************************************
* 名称 : write_data()
* 功能 : 写串口数据
* 输入 : Dispdata
* 输出 : 无
***********************************************************************/
void write_data(uchar Dispdata)//写数据函数
{
    sendbyte(0xfa); //MCU向LCD发数据
    sendbyte(Dispdata & 0xf0); //发高四位数据
    sendbyte((Dispdata << 4) & 0xf0);//发低四位数据
    delay(2);
}
/********************************************************************
* 名称 : lcdinit()
* 功能 : 初始化函数
* 输入 : cmdcode
* 输出 : 无
***********************************************************************/
void lcdinit() //LCD初始化
{
    delay(20000); //复位等待（内部自带上电复位电路），时间较长
    write_com(0x30); //功能设定：基本指令集操作
    delay(50); //延时等待
    write_com(0x0c); //整体显示，关游标
    delay(50);
    write_com(0x01); //清屏
    delay(50);
    write_com(0x06); //入口设置
    delay(50);
}

/********************************************************************
* 函数名称 : LCD_12864_address_set( uchar x, y )
* 函数功能 : 12864地址坐标设置
* 函数参数 : X坐标，Y坐标
* 函数返回值: 无
*********************************************************************/
void LCD_12864_address_set( uchar x,uchar y )
{
    uchar Address ;
    switch( y )
    {
        case 1 : Address = 0x80 + (x - 1); break;
        case 2 : Address = 0x90 + (x - 1); break;
        case 3 : Address = 0x88 + (x - 1); break;
        case 4 : Address = 0x98 + (x - 1); break;
        default : break;
    }
    write_com(Address);
}



/**********以下所有为显示函数*********/

void display(uchar n,uchar m,char  *ptr) //第n行 第m列  整体显示函数 需要即调用
{

     LCD_12864_address_set(m,n);
     while(*ptr !='\0') //C语言里字符串末尾自动加“\0”(ASCII码值为0)
     {
         write_data(*ptr++); //发送字符对应的ASCII码，12864指针可设置自动指向下一个显示地址
         delay(20);
     }
}

void display_tizhong() //显示体重
{
    uchar i;
    i=0;
    while(ADS1256_buf[i]!='\0') //C语言里字符串末尾自动加“\0”(ASCII码值为0)
    {
        write_data(ADS1256_buf[i++]); //发送字符对应的ASCII码，12864指针可设置自动指向下一个显示地址
        delay(50);
    }
}
