/*****�ô������� ��ʲô  �ظ�ʲô****/



#include "msp430f5529.h"
// ACLK = REFO = 32768Hz, MCLK = SMCLK = default DCO/2 = 1048576Hz
// P3.4��5����USCI_A0 TXD/RXD��P9.4,5����USCI_A2 TXD/RXD��P10.4,5����USCI_A3 TXD/RXD��
void main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  P4SEL |=BIT4+BIT5 ;                             // P5.6,7 = USCI_A1 TXD/RXD
  UCA1CTL1 |= UCSWRST;                      // **Put state machine in reset**
  UCA1CTL1 |= UCSSEL_2;                     // SMCLK
  UCA1BR0 = 9;                              // 1MHz 115200 (see User's Guide)
  UCA1BR1 = 0;                              // 1MHz 115200
  UCA1MCTL |= UCBRS_1 + UCBRF_0;            // Modulation UCBRSx=1, UCBRFx=0
  UCA1CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
  UCA1IE |= UCRXIE;                         // Enable USCI_A1 RX interrupt
  __bis_SR_register(LPM0_bits + GIE);       // Enter LPM0, interrupts enabled
}
// Echo back RXed character, confirm TX buffer is ready first����������֮ǰȷ�����ͻ���׼����
#pragma vector=USCI_A1_VECTOR
__interrupt void USCI_A1_ISR(void)
{
  switch(__even_in_range(UCA1IV,4))
  {
  case 0:break;                             // Vector 0 - no interrupt
  case 2:                                   // Vector 2 - RXIFG
   while (!(UCA1IFG&UCTXIFG));    // USCI_A1 TX buffer ready?   UCTXIFG(USCI Transmit Interrupt Flag)
                                   //�ȴ����ݷ������ ���UCTXIFG��1 ����ѭ��
    UCA1TXBUF = UCA1RXBUF;                  // TX -> RXed character
    break;
  case 4:break;                             // Vector 4 - TXIFG
  default: break;
  }
}
// UCTXIFG=0x02��UCA1IFG&UCTXIFG����UCA1IFG��UCTXIFGλΪ1ʱ��˵��UCA1TXBUFΪ�գ�
//����whileѭ��ѭ������UCTXIFGλΪ0ʱUCA1TXBUF��Ϊ�գ�ͣ��ѭ����
