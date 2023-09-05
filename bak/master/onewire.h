#ifndef _ONEWIRE_BITOPS_H_
#define _ONEWIRE_BITOPS_H_
//#include <stdint.h>

#define 	DQ_PIN	GPIO_PIN_4
#define	  DQ_OUT	do { P1CON |= DQ_PIN; } while (0)
#define   DQ_IN		do { P1CON &= ~DQ_PIN; P1PH |= DQ_PIN;} while (0)
#define 	DQ_PIN_HIGH	(P1 |= DQ_PIN)
#define 	DQ_PIN_LOW	(P1 &= ~DQ_PIN)
#define		DQ_PIN_VALUE	(P1 & DQ_PIN)

#define tRSTL	480//-
#define tRSTH	480//-
#define tPDH	15//-60
#define tPDL	60//-240

#define tSLOT	60//60-120
#define tLOW1	1//1-15
#define tREC	1//1-
#define tLOW0	60//60-120<tSLOT

#define tRELEASE	0//0-45
#define tLOWR		1//1-15
#define tRDV	15
#define tSU		1//<1


#include "sc92f8003_gpio.h"

extern unsigned char ops_onewire_reset(void);
extern int ops_onewire_read(void *buff,int len);
extern int ops_onewire_write(void *buff,int len);
void delayus(uint16_t us);

extern void onewire_init(void);

#endif
