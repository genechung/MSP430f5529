#include <msp430f5529.h>
unsigned char Key_Val;
void delay(void)
{
 unsigned int tmp;

 for(tmp = 12000;tmp > 0;tmp--);
}
void Check_Key(void)
{
	unsigned char row ,col,tmp1,tmp2;
	unsigned char keymap[] = {1,2,3,4,5,6};
	tmp1 = 0x08;
	for(row = 0;row < 3;row++)
	{
		P1OUT = 0x38;
		P1OUT -= tmp1;
		tmp1 <<=1;
		if((P1IN & 0xC0)<0xC0)
		{
			tmp2 = 0x80;
			for(col = 0;col < 2;col++)
					{
				if((P1IN & tmp2) == 0x00)
				{
					Key_Val = keymap[row*4 + col];
					return;
				}
				tmp2 >>= 1;
					}
		}


	}

}
void Init_Keypad(void)
{
	P1DIR = 0x38;
	P1OUT=0;
	P1IES =0xC0;
	P1IE=0xC0;
	P1IFG=0;
	Key_Val = 0;
}
void Key_Event(void)
{
	unsigned char tmp;
	P1OUT =0;
	tmp = P1IN;
	if((tmp & 0xf0) < 0xf0)
	{
		delay();
		Check_Key();
	}
}
void main()
{
	WDTCTL=WDTPW+WDTHOLD;
	Init_Keypad();
	_BIS_SR(LPM3_bits + GIE);
	while(1);
}
#pragma vector=PORT1_VECTOR
__interrupt void Port(void)
{
	delay();
	if((P1IN&0XC0)<0XC0)
	{
		Check_Key();
		switch(Key_Val)
		{
			case 1:
			{//自己接相应按键的功能
				break;
			}
			case 2:
			{//自己接相应按键的功能
				break;
			}
			case 3:
			{//自己接相应按键的功能
				break;
			}
			case 4:
			{//自己接相应按键的功能
				break;
			}
			case 5:
			{//自己接相应按键的功能
				break;
			}
			case 6:
			{//自己接相应按键的功能
				break;
			}
			default:break;
		}
	}
	P1IFG=0;
}

