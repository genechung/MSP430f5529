/*
 *MSP430f5529 ��������key ΪP1.1 LED��ΪP1.0
 */
 #include <msp430.h>

void delay()
{
	int i;
	for(i=100;i>0;i--);
}
int main()
{
	WDTCTL=WDTPW+WDTHOLD;
	P1DIR=0xfd;
	P1REN|=BIT1;
	//P1DIR|=BIT1;
	P1OUT=0xff;
	while(1)
	{
		if((P1IN&BIT1)==0)
		{
			delay();
			if(!(P1IN&BIT1))
			{
				while(!(P1IN&BIT1));
				P1OUT^=0x01;
			}
		}
	}
	return 0;
}

