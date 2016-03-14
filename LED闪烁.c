/*
 *MSP430f5529 LEDµ∆…¡À∏LED P1.0
 */
 #include <msp430.h>

int main()
{
	WDTCTL=WDTPW+WDTHOLD;
	P1DIR|=BIT0;
	for(;;)
	{
		volatile unsigned int i;
		P1OUT^=BIT0;
		//void delay();
		i = 10000;					// SW Delay
				do i--;
				while(i != 0);
	}
}

