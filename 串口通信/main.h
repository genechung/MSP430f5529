#ifndef MAIN
#define MAIN
 #include <msp430f5529.h>
//#include <msp430x14x.h>
#include "IIC.h"
#include <math.h>
#include "init.h"
#define uint unsigned int
#define uchar unsigned char
#define CPU_F ((double)8000000)
#define delay_us(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0)) //ÑÓÊ±º¯Êý
#define delay_ms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0))

#define uint8_t unsigned char
#define uint16_t unsigned int

#endif
