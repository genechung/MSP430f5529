/*
 *MSP4305529 ºôÎüµÆLEDP1.0
 */
#include <msp430.h>
int n=0;
int m=0;
int Low=0;
int Cycle=300;
int main()
{

    WDTCTL=WDTPW+WDTHOLD;
    volatile unsigned int i;
    P1DIR|=BIT0;
    P1OUT=BIT0;
    while(1)
    {
        for( Low = 0; Low < Cycle; Low++ )
        {
            P1DIR^=BIT0;

            for(n=0; n<Low; n++);

            P1DIR^=BIT0;

            for(n=0; n< Cycle-Low; n++);

        }
        for(; Low>0; Low--)
        {
            P1DIR^=BIT0;
            for(n=Low; n>0; n--);
            P1DIR^=BIT0;
            for(n=0; n<Cycle-Low; n++);
        }
    }
}
