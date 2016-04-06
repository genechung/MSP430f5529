/*
 *MSP430F5529 P1.2口输出75%的PWM波  P1.3口输出25%的PWM波
 */
 #include <msp430f5529.h>
void main()
{

	WDTCTL=WDTPW+WDTHOLD;
	P1DIR|=BIT2+BIT3;//P1.2和P1.3设为输出
	P1SEL|=BIT2+BIT3;//P1.2和P1.3引脚功能选为定时器输出
	TA0CCR0=512-1;//PWM周期定义
	TA0CCTL1=OUTMOD_7;//CCR1比较输出模式7:复位置位
	TA0CCR1=384;//CCR1  PWM占空比定义
	TA0CCTL2=OUTMOD_7;//CCR2比较输出模式7:复位置位
	TA0CCR2=128;//CCR2  PWM占空比定义
	TA0CTL=TASSEL_1+MC_1+TACLR;//ACLK增计数器清除TAR计数器
	__bis_SR_register(LPM3_bits);//进入LPM3
}
