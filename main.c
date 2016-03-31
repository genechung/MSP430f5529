////*******������******///
#include <msp430f5529.h>
#include "stdio.h"
#include "math.h"
#include "6050.h"
#include "LCD12864.h"
void Delays( uchar i)
{

    unsigned int j;

    while(i--)
    {
            j=2000;
            while(j--);
    }
}


void main(void)
{
    WDTCTL = WDTPW + WDTHOLD;   // �رտ��Ź�

    char sum1[10],sum2[10],sum3[10]; // ���ڷ��ͻ���
    float a_x,a_y,a_z;
    int_port() ;//�ܽų�ʼ��
    lcdinit();
    InitMPU6050(); // ��ʼ��ģ��
    display(1,1,"�Ƕ�X:");
    display(2,1,"�Ƕ�Y:");
    display(3,1,"�Ƕ�Z:");
    while(1)
    {
       // Delays(2);
        a_x = mpu6050_Angle(2);
        a_y = mpu6050_Angle(1);
        a_z = mpu6050_Angle(0);

        sprintf(sum1,"%.2f",a_x); // ���������ֵת��Ϊ�ַ���
        sprintf(sum2,"%.2f",a_y);
        sprintf(sum3,"%.2f",a_z);

        display(1,4,sum1);
        display(2,4,sum2);
        display(3,4,sum3);

    }
}


