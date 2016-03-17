/*
 *MSP430F5529 P2.0���25%PWM
 */
 #include <msp430f5529.h>

void main()
{
	WDTCTL=WDTPW+WDTHOLD;
	P2DIR|=BIT0;
	P2SEL|=BIT0;
	TA1CCTL1=OUTMOD_3;
	TA1CCR0=16484;
	TA1CCR1=100;
	TA1CTL=TASSEL_1+MC_2+TACLR;
	__bis_SR_register(LPM3_bits);
}