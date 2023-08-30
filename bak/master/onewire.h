#ifndef _ONEWIRE_BITOPS_H_
#define _ONEWIRE_BITOPS_H_
//#include <stdint.h>

#define 	DQ_PIN	GPIO_PIN_4
#define	  DQ_OUT	GPIO_Init(GPIO1,DQ_PIN,GPIO_MODE_OUT_PP)
#define   DQ_IN		GPIO_Init(GPIO1,DQ_PIN,GPIO_MODE_IN_PU)

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
extern int ops_onewire_read(void *buff,int size);
extern int ops_onewire_write(void *buff,int size);
void delayus(uint32_t us);

extern void onewire_init(void);

#endif
