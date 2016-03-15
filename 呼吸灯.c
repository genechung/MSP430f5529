#include <msp430f5529.h>

void delay_nms(unsigned int n)// ��ʱ����

  {

    unsigned int j;

    for (j=0;j<(n);j++)

    {

      __delay_cycles(400);   //̫�̻�ʹLED�Եú����ڳ�����̫����Ҫ�Ƚϳ�ʱ�����۲���

    }

  }

void main(void)

{

  unsigned const PWMPeriod = 1500; //����PWM���ڲ�����const������ֵ������ı�.����ֵ̫�󣬻ᵼ��LED��˸

  volatile unsigned int i;        //��������i����ʱ�ɱ�ģ�ϵͳ��Ҫȥ�Ż����ֵ

  WDTCTL = WDTPW + WDTHOLD;   // �رտ��Ź�

  P1DIR |=BIT3;              // ���� P1.3Ϊ���

  P1SEL |=BIT3;              // ���� P1.3ΪTA0.2���

  TA0CCR0 = PWMPeriod;           // ����PWM ����

  TA0CCTL2 = OUTMOD_7;           // ����PWM ���ģʽΪ��7 - PWM��λ/��λģʽ��

                              // �������ƽ��TAR��ֵ����CCR2ʱ��λΪ0����TAR��ֵ����CCR0ʱ��λΪ1���ı�CCR2���Ӷ�����PWM����ʵģʽ2Ҳ����

  TA0CTL= TASSEL_2 +MC_1;    // ����TIMERA��ʱ��ԴΪSMCLK, ����ģʽΪup,��CCR0���Զ���0��ʼ����

  while(1)

  {

   TA0CCR2=0;//ȷ���ʼ�ǰ���

    //�������̣���������TA0CCR2��ֵ��ʹ��ת��ʱ��䳤���ı�PWM��ռ�ձ�

    for(i=0;i<PWMPeriod;i+=1)

    {

      TA0CCR2=i;

      delay_nms(4-(i/500));  //ռ�ձȱ仯����ʱ�������ӳ�ʱ��ɸı�����Ʊ䰵���ٶ�

                             //�ڰ���ʱ���ӳ�delayʱ�䣬����ǿЧ��

     }

    //�������̣���������TA0CCR2��ֵ��ʹ��ת��ʱ���̣��ı�PWM��ռ�ձ�

    for(i=PWMPeriod;i>0;i-=1)

    {

      TA0CCR2=i;

      delay_nms(4-(i/500));           //ռ�ձȱ仯����ʱ�������ӳ�ʱ��ɸı�����Ʊ䰵���ٶ�

                                    //�ڰ���ʱ���ӳ�delayʱ�䣬����ǿЧ��

    }

     TA0CCR2=0;  //ȷ���ư�

     delay_nms(250); //ʱ�䳤һ�㣬��ǿ�Ӿ�Ч��

  }

}
