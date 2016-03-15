/*
 *MSP4305529 在定时器的作用下小灯交替闪烁LEDP1.0LEDP4.7
 */
  #include <msp430.h>
 int n=0;
 int m=0;

int main()
{
	WDTCTL=WDTPW+WDTHOLD;
	P1DIR|=BIT0;
	P1OUT=BIT0;
	P4DIR|=BIT7;
	P4OUT=BIT7;
	TA0CCTL0=CCIE;
	TA0CCR0=50000;
	TA0CTL=TASSEL_2+MC_1+TACLR+ID_3;
	_bis_SR_register(LPM0_bits+GIE);
	_no_operation();

}
#pragma vector=TIMER0_A0_VECTOR
__interrupt void TIMER0_A0_ISR(void )
{
	if(n==0)
	{
	P1OUT^=BIT0;
//	P4OUT^=BIT7;
	n=1;
	}
	else if(n==1)
	{
		P4OUT^=BIT7;
		n=0;
	}

}
