#include "main.h"
uint8_t ack;
/*******************************************
�������ƣ�void iic_Start()
��    �ܣ�IIC����
��    ���� ��
����ֵ  ����
********************************************/
void iic_Start()
{     
      S_SDA;
      _NOP();
      S_SCL;
      delay_us(5);
      C_SDA;
      delay_us(5);
      C_SCL;
      _NOP();
      _NOP();
}
/*******************************************
�������ƣ�void iic_Stop() 
��    �ܣ�IICֹͣ
��    ���� ��
����ֵ  ����
********************************************/
void iic_Stop() 
{   
    C_SCL;
    _NOP();
    C_SDA;
    delay_us(5);
    S_SCL;
    delay_us(5);
    S_SDA;   
    _NOP();
    _NOP();
    _NOP();
    _NOP();     
}
/*******************************************
�������ƣ�void iic_SendACK(uchar a) 
��    �ܣ�IICӦ��
��    ����  a
����ֵ  ����
********************************************/
void iic_SendACK(uint8_t a)
{     
      if(a==0)       
       { 
       C_SDA;
       delay_us(5);
       S_SCL;
       delay_us(5);
       C_SCL;
       delay_us(5);
       S_SDA;
      }
      else
       { 
        S_SDA;          
        delay_us(5);
        S_SCL;
        delay_us(5);
        C_SCL;
        delay_us(5);
      }      
}
/*******************************************
�������ƣ�void iic_SendByte(uchar c) 
��    �ܣ�����һ���ֽ�
��    ����  c
����ֵ  ����
********************************************/
void iic_SendByte(uint8_t c)
{
      uint8_t i;    
      for(i=0;i<8;i++)
          {
            if((c<<i)&0x80)
                S_SDA; 
            else
                C_SDA;
            delay_us(5);
            S_SCL;
            delay_us(5);
            C_SCL;
            delay_us(5);
          }
        S_SCL;
        delay_us(5);
        SDA_IN;    
        _NOP();
        _NOP();
        _NOP();   
      if(DIN&SDA==SDA)
          ack=0;
      else
          ack=1;
       delay_us(5);
       C_SCL;
     
      _NOP();
      _NOP();
}
/*******************************************
�������ƣ�uchar iic_RecvByte()
��    �ܣ�����һ���ֽ�
��    ����  ��
����ֵ  ��retc
********************************************/
uint8_t iic_RecvByte()
{
      uint8_t retc;
      uint8_t i;
      retc=0;   
      SDA_IN;
      for(i=0;i<8;i++)
        {       
          delay_us(5);
          S_SCL;
          delay_us(5);       
          retc=retc<<1;
          SDA_IN;
          if(READ_SDA==SDA)//read p4in 
              retc+=1;                         
         delay_us(5);
         C_SCL;
         delay_us(5);
         }     
      return(retc);        
}
/*******************************************
�������ƣ�void Single_Write_iic(uchar REG_Address,uchar REG_data)
��    �ܣ������ֽ�д���ڲ��Ĵ���
��    ���� REG_Address, REG_data
����ֵ  ����
********************************************/
void Single_Write_iic(uint8_t REG_Address,uint8_t REG_data)
{
    iic_Start();                  //��ʼ�ź�
    iic_SendByte(SlaveAddress);   //�����豸��ַ+д�ź�
    iic_SendByte(REG_Address);    //�ڲ��Ĵ�����ַ
    iic_SendByte(REG_data);       //�ڲ��Ĵ�������
    iic_Stop();                   //����ֹͣ�ź�
}
/*******************************************
�������ƣ�uchar Single_Read_iic(uchar REG_Address)
��    �ܣ������ֽڶ�ȡ�Ĵ���
��    ���� REG_Address
����ֵ  �� REG_data
********************************************/
uint8_t Single_Read_iic(uint8_t REG_Address)
{   
    uint8_t REG_data;
    iic_Start();                          //��ʼ�ź�
    iic_SendByte(SlaveAddress);           //�����豸��ַ+д�ź�
    iic_SendByte(REG_Address);                   //���ʹ洢��Ԫ��ַ����0��ʼ	
    iic_Start();                          //��ʼ�ź�
    iic_SendByte(SlaveAddress+1);         //�����豸��ַ+���ź�
    REG_data=iic_RecvByte();              //�����Ĵ�������
	iic_SendACK(1);   
	iic_Stop();                           //ֹͣ�ź�
    return REG_data; 
}
//**************************************
//��ʼ��MPU6050
//**************************************
void InitMPU6050()
{
	Single_Write_iic(PWR_MGMT_1, 0x00);	//�������״̬
        _NOP();_NOP();_NOP();   
	Single_Write_iic(SMPLRT_DIV, 0x07);
        _NOP();_NOP();_NOP(); 
	Single_Write_iic(CONFIG, 0x06);
        _NOP();_NOP();_NOP(); 
	Single_Write_iic(GYRO_CONFIG, 0x18);
        _NOP();_NOP();_NOP(); 
	Single_Write_iic(ACCEL_CONFIG, 0x01);
        _NOP();_NOP();_NOP(); 
}

int GetData(uint8_t REG_Address)
{
	char H,L;
	H=Single_Read_iic(REG_Address);
	L=Single_Read_iic(REG_Address+1);
	return (H<<8)+L;   //�ϳ�����
}
