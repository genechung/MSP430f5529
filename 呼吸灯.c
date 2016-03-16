#include <msp430f5529.h>
//PWM
void delay_nms(unsigned int n)// 延时函数

  {

    unsigned int j;

    for (j=0;j<(n);j++)

    {

      __delay_cycles(400);   //太短会使LED显得好像在常亮，太长就要等较长时间来观察了

    }

  }

void main(void)

{

  unsigned const PWMPeriod = 1500; //设置PWM周期参数，const声明此值不允许改变.该数值太大，会导致LED闪烁

  volatile unsigned int i;        //声明变量i是随时可变的，系统不要去优化这个值

  WDTCTL = WDTPW + WDTHOLD;   // 关闭看门狗

  P1DIR |=BIT3;              // 设置 P1.3为输出

  P1SEL |=BIT3;              // 设置 P1.3为TA0.2输出

  TA0CCR0 = PWMPeriod;           // 设置PWM 周期

  TA0CCTL2 = OUTMOD_7;           // 设置PWM 输出模式为：7 - PWM复位/置位模式，

                              // 即输出电平在TAR的值等于CCR2时复位为0，当TAR的值等于CCR0时置位为1，改变CCR2，从而产生PWM。其实模式2也可以

  TA0CTL= TASSEL_2 +MC_1;    // 设置TIMERA的时钟源为SMCLK, 计数模式为up,到CCR0再自动从0开始计数

  while(1)

  {

   TA0CCR2=0;//确保最开始是暗的

    //渐亮过程：不断设置TA0CCR2的值，使翻转的时间变长，改变PWM的占空比

    for(i=0;i<PWMPeriod;i+=1)

    {

      TA0CCR2=i;

      delay_nms(4-(i/500));  //占空比变化的延时，调整延迟时间可改变呼吸灯变暗的速度

                             //在暗的时候延长delay时间，可增强效果

     }

    //渐暗过程：不断设置TA0CCR2的值，使翻转的时间变短，改变PWM的占空比

    for(i=PWMPeriod;i>0;i-=1)

    {

      TA0CCR2=i;

      delay_nms(4-(i/500));           //占空比变化的延时，调整延迟时间可改变呼吸灯变暗的速度

                                    //在暗的时候延长delay时间，可增强效果

    }

     TA0CCR2=0;  //确保灯暗

     delay_nms(250); //时间长一点，增强视觉效果

  }

}
