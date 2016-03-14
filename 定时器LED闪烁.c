/*
 *MSP430f5529 在定时器作用下LED闪烁 P1.0 P4.7
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
	//在比较模式下用来设定计数终值
	TA0CTL=TASSEL_2+MC_1+TACLR+ID_3;
	//MC=00停止模式 MC=01增模式MC=10连续模式MC=11增减模式
	_bis_SR_register(LPM0_bits+GIE);
	_no_operation();

}
#pragma vector=TIMER0_A0_VECTOR
__interrupt void TIMER0_A0_ISR(void )
{
	P1OUT^=BIT0;
	P4OUT^=BIT7;
}
