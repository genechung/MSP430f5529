/*
 *MSP430f5529 �ڶ�ʱ��������LED��˸ P1.0 P4.7
 */
 #include <msp430.h>
unsigned int n=0;

int main()
{
	WDTCTL=WDTPW+WDTHOLD;
	P1DIR|=BIT0;
	P1OUT^=BIT0;
	P4DIR|=BIT7;
	P4OUT^=BIT7;

	TA0CCTL0=CCIE;
	TA0CCR0=50000;
	//�ڱȽ�ģʽ�������趨������ֵ
	TA0CTL=TASSEL_2+MC_1+TACLR+ID_3;
	//MC=00ֹͣģʽ MC=01��ģʽMC=10����ģʽMC=11����ģʽ
	_bis_SR_register(LPM0_bits+GIE);
	_no_operation();

}
#pragma vector=TIMER0_A0_VECTOR
__interrupt void TIMER0_A0_ISR(void )
{
	P1OUT^=BIT0;
	P4OUT^=BIT7;
}
