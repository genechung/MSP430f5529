/*
 *MSP430F5529 P1.2�����75%��PWM��  P1.3�����25%��PWM��
 */
 #include <msp430f5529.h>
void main()
{

	WDTCTL=WDTPW+WDTHOLD;
	P1DIR|=BIT2+BIT3;//P1.2��P1.3��Ϊ���
	P1SEL|=BIT2+BIT3;//P1.2��P1.3���Ź���ѡΪ��ʱ�����
	TA0CCR0=512-1;//PWM���ڶ���
	TA0CCTL1=OUTMOD_7;//CCR1�Ƚ����ģʽ7:��λ��λ
	TA0CCR1=384;//CCR1  PWMռ�ձȶ���
	TA0CCTL2=OUTMOD_7;//CCR2�Ƚ����ģʽ7:��λ��λ
	TA0CCR2=128;//CCR2  PWMռ�ձȶ���
	TA0CTL=TASSEL_1+MC_1+TACLR;//ACLK�����������TAR������
	__bis_SR_register(LPM3_bits);//����LPM3
}
