#include "main.h"
uint8_t ack;
/*******************************************
函数名称：void iic_Start()
功    能：IIC启动
参    数： 无
返回值  ：无
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
函数名称：void iic_Stop() 
功    能：IIC停止
参    数： 无
返回值  ：无
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
函数名称：void iic_SendACK(uchar a) 
功    能：IIC应答
参    数：  a
返回值  ：无
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
函数名称：void iic_SendByte(uchar c) 
功    能：发送一个字节
参    数：  c
返回值  ：无
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
函数名称：uchar iic_RecvByte()
功    能：接受一个字节
参    数：  无
返回值  ：retc
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
函数名称：void Single_Write_iic(uchar REG_Address,uchar REG_data)
功    能：单个字节写入内部寄存器
参    数： REG_Address, REG_data
返回值  ：无
********************************************/
void Single_Write_iic(uint8_t REG_Address,uint8_t REG_data)
{
    iic_Start();                  //起始信号
    iic_SendByte(SlaveAddress);   //发送设备地址+写信号
    iic_SendByte(REG_Address);    //内部寄存器地址
    iic_SendByte(REG_data);       //内部寄存器数据
    iic_Stop();                   //发送停止信号
}
/*******************************************
函数名称：uchar Single_Read_iic(uchar REG_Address)
功    能：单个字节读取寄存器
参    数： REG_Address
返回值  ： REG_data
********************************************/
uint8_t Single_Read_iic(uint8_t REG_Address)
{   
    uint8_t REG_data;
    iic_Start();                          //起始信号
    iic_SendByte(SlaveAddress);           //发送设备地址+写信号
    iic_SendByte(REG_Address);                   //发送存储单元地址，从0开始	
    iic_Start();                          //起始信号
    iic_SendByte(SlaveAddress+1);         //发送设备地址+读信号
    REG_data=iic_RecvByte();              //读出寄存器数据
	iic_SendACK(1);   
	iic_Stop();                           //停止信号
    return REG_data; 
}
//**************************************
//初始化MPU6050
//**************************************
void InitMPU6050()
{
	Single_Write_iic(PWR_MGMT_1, 0x00);	//解除休眠状态
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
	return (H<<8)+L;   //合成数据
}
