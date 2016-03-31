////******LCD12864*****///
/*
 *12864��ʾ����display(n,m,*str)  nΪ��mΪ��*strΪ�����׵�ַ
 */

#include <msp430f5529.h>
#include "LCD12864.h"
#define uchar unsigned char
#define uint unsigned int
extern unsigned char ADS1256_buf[9];


void int_port(void) //�ܽų�ʼ��
{
    P2SEL&=~BIT4; //P2.4ģ��SID������Ϊi/o���������
    P2DIR|=BIT4;
    P2OUT&=~BIT4;
    P2SEL&=~BIT5; //P2.5ģ��SCLK������Ϊi/o���������
    P2DIR|=BIT5;
    P2OUT&=~BIT5;
}
/********************************************************************
* ���� : delay()
*
* ���� : t
* ��� : ��
***********************************************************************/
void delay(uint t) //��ʱ����
{ //������ʱ������ʱ��Ҫ��
    uint i,j;
    for(i=0; i<t; i++)
    for(j=0; j<10; j++);
}
/********************************************************************
* ���� : sendbyte()
* ���� : ����Һ���Ĵ���ͨ��Э�飬��������
* ���� : zdata
* ��� : ��
***********************************************************************/
void sendbyte(uchar zdata) //���ݴ��ͺ���
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
            P2OUT|=BIT4; //SIDΪ1
        }
        else
        {
            P2OUT&=~BIT4; //SIDΪ0
        }
        P2OUT&=~BIT5; //����ʱ���ź�����
        P2OUT|=BIT5; //����ʱ���ź�����
        serial_shift=serial_shift>>1; //׼��������һλ����
    }
}
/********************************************************************
* ���� : write_com()
* ���� : д����ָ��
* ���� : cmdcode
* ��� : ��
***********************************************************************/
void write_com(unsigned char cmdcode) //д�����
{
    //���ڿ��Ƹ�ʽ��11111AB0��
    //A���ݷ�����ƣ�A=Hʱ����A=Lʱд
    //B��������ѡ��B=HʱΪ��ʾ���ݣ�B=LʱΪ����
    sendbyte(0xf8); //MCU��LCD������
    sendbyte(cmdcode & 0xf0); //������λ����(���ݸ�ʽD7D6D5D4_0000)
    sendbyte((cmdcode << 4) & 0xf0);//������λ����(���ݸ�ʽD3D2D1D0_0000)
    delay(2); //��ʱ�ȴ�
}

/********************************************************************
* ���� : write_data()
* ���� : д��������
* ���� : Dispdata
* ��� : ��
***********************************************************************/
void write_data(uchar Dispdata)//д���ݺ���
{
    sendbyte(0xfa); //MCU��LCD������
    sendbyte(Dispdata & 0xf0); //������λ����
    sendbyte((Dispdata << 4) & 0xf0);//������λ����
    delay(2);
}
/********************************************************************
* ���� : lcdinit()
* ���� : ��ʼ������
* ���� : cmdcode
* ��� : ��
***********************************************************************/
void lcdinit() //LCD��ʼ��
{
    delay(20000); //��λ�ȴ����ڲ��Դ��ϵ縴λ��·����ʱ��ϳ�
    write_com(0x30); //�����趨������ָ�����
    delay(50); //��ʱ�ȴ�
    write_com(0x0c); //������ʾ�����α�
    delay(50);
    write_com(0x01); //����
    delay(50);
    write_com(0x06); //�������
    delay(50);
}

/********************************************************************
* �������� : LCD_12864_address_set( uchar x, y )
* �������� : 12864��ַ��������
* �������� : X���꣬Y����
* ��������ֵ: ��
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



/**********��������Ϊ��ʾ����*********/

void display(uchar n,uchar m,char  *ptr) //��n�� ��m��  ������ʾ���� ��Ҫ������
{

     LCD_12864_address_set(m,n);
     while(*ptr !='\0') //C�������ַ���ĩβ�Զ��ӡ�\0��(ASCII��ֵΪ0)
     {
         write_data(*ptr++); //�����ַ���Ӧ��ASCII�룬12864ָ��������Զ�ָ����һ����ʾ��ַ
         delay(20);
     }
}

void display_tizhong() //��ʾ����
{
    uchar i;
    i=0;
    while(ADS1256_buf[i]!='\0') //C�������ַ���ĩβ�Զ��ӡ�\0��(ASCII��ֵΪ0)
    {
        write_data(ADS1256_buf[i++]); //�����ַ���Ӧ��ASCII�룬12864ָ��������Զ�ָ����һ����ʾ��ַ
        delay(50);
    }
}
