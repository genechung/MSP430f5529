/*
 *MSP430f5529°´¼üÖÐ¶Ï keyP1.1 LED P1.0
 */
 #include <msp430.h>
int num=0;
int s=0;

int main()
{
	WDTCTL=WDTPW+WDTHOLD;
	P1DIR=0xfd;

	P1REN=BIT1;//配置按键上拉电阻
	P1IFG=0x00;//清除中断标志
	P1IE|=BIT1;//开启中断
	P1IES=BIT1;//下降沿触发
	P1IN=BIT1;
	_enable_interrupt();
	while(1)
	{

	}

}
#pragma vector=PORT1_VECTOR
__interrupt void Port1_ISR(void )
{
	P1OUT^=BIT0;
	P1IFG=0x00;
	volatile unsigned int i;
			i = 1000;
			do i--;
					while(i != 0);

}
