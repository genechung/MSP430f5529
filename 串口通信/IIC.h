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
// ����MPU6050�ڲ���ַ
//****************************************
#define	SMPLRT_DIV		0x19	//�����ǲ����ʣ�����ֵ��0x07(125Hz)
#define	CONFIG			0x1A	//��ͨ�˲�Ƶ�ʣ�����ֵ��0x06(5Hz)
#define	GYRO_CONFIG		0x1B	//�������Լ켰������Χ������ֵ��0x18(���Լ죬1000deg/s)
#define	ACCEL_CONFIG	0x1C	//���ټ��Լ졢������Χ����ͨ�˲�Ƶ�ʣ�����ֵ��0x01(���Լ죬2G��5Hz)
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
#define	PWR_MGMT_1		0x6B	//��Դ��������ֵ��0x00(��������)
#define	WHO_AM_I			0x75	//IIC��ַ�Ĵ���(Ĭ����ֵ0x68��ֻ��)
#define	SlaveAddress	0xD0	//IICд��ʱ�ĵ�ַ�ֽ����ݣ�+1Ϊ��ȡ


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
