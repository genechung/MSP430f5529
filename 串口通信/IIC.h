#ifndef IIC
#define IIC

#define uint8_t unsigned char
#define uint16_t unsigned int
extern void iic_Start();
extern void iic_Stop();
extern void iic_SendACK(uint8_t a);
extern void iic_SendByte(uint8_t c);
extern uint8_t iic_RecvByte();
extern void Single_Write_iic(uint8_t REG_Address,uint8_t REG_data);
extern uint8_t Single_Read_iic(uint8_t REG_Address);
extern void InitMPU6050();
extern int GetData(uint8_t REG_Address);
//****************************************
// 定义MPU6050内部地址
//****************************************
#define	SMPLRT_DIV		0x19	//陀螺仪采样率，典型值：0x07(125Hz)
#define	CONFIG			0x1A	//低通滤波频率，典型值：0x06(5Hz)
#define	GYRO_CONFIG		0x1B	//陀螺仪自检及测量范围，典型值：0x18(不自检，1000deg/s)
#define	ACCEL_CONFIG	0x1C	//加速计自检、测量范围及高通滤波频率，典型值：0x01(不自检，2G，5Hz)
#define	ACCEL_XOUT_H	0x3B
#define	ACCEL_XOUT_L	0x3C
#define	ACCEL_YOUT_H	0x3D
#define	ACEL_YOUT_L	0x3E
#define	ACCEL_ZOUT_H	0x3F
#define	ACCEL_ZOUT_L	0x40
#define	TEMP_OUT_H		0x41
#define	TEMP_OUT_L		0x42
#define	GYRO_XOUT_H		0x43
#define	GYRO_XOUT_L		0x44	
#define	GYRO_YOUT_H		0x45
#define	GYRO_YOUT_L		0x46
#define	GYRO_ZOUT_H		0x47
#define	GYRO_ZOUT_L		0x48
#define	PWR_MGMT_1		0x6B	//电源管理，典型值：0x00(正常启用)
#define	WHO_AM_I			0x75	//IIC地址寄存器(默认数值0x68，只读)
#define	SlaveAddress	0xD0	//IIC写入时的地址字节数据，+1为读取


#define   SDA   BIT0  //SDA
#define   SCL   BIT1
#define   DIN   P6IN
#define   DOUT  P6OUT
#define   DDIR  P6DIR
#define   S_SDA DDIR|=SDA,DOUT|=SDA
#define   C_SDA DDIR|=SDA,DOUT&=~SDA
#define   S_SCL DDIR|=SCL,DOUT|=SCL
#define   C_SCL DDIR|=SCL,DOUT&=~SCL
#define   SDA_IN DDIR&=~SDA
#define   READ_SDA  (DIN&SDA)


#endif
