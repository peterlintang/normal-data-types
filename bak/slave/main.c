
#include "sc92f8003_gpio.h"
#include "sc92f8003_timer0.h"

#define   DQ_PIN  GPIO_PIN_4
#define   DQ_OUT  do { P1CON |= DQ_PIN; } while (0)
#define   DQ_IN   do { P1CON &= ~DQ_PIN; P1PH |= DQ_PIN;} while (0)
#define   DQ_PIN_HIGH     (P1 |= DQ_PIN)
#define   DQ_PIN_LOW      (P1 &= ~DQ_PIN)
#define   DQ_PIN_VALUE    (P1 & DQ_PIN)


#define tRSTL	480//-
#define tRSTH	480//-
#define tPDH	15//-60
#define tPDL	60//-240

#define tSLOT	60//60-120
#define tLOW1	15//1-15
#define tREC	1//1-
#define tLOW0	60//60-120<tSLOT

#define tRELEASE	0//0-45
#define tLOWR		1//1-15
#define tRDV	15
#define tSU		1//<1

static void delay_us(uint16_t us)
{
#if 1  /* 不用系统延时时开启这个！！*/
    uint16_t i;
	
    for (; us > 0; us--)
    {
        i = 50;  //mini 17
        while(i--);
    }
#else
		Delayus(us);
#endif
}

	
void ACT_INIT(void)
{
	unsigned char i;
	unsigned char j;
	GPIO_Init(GPIO1,DQ_PIN,GPIO_MODE_OUT_PP);
	j = 1; while (j--)
	{
		i = 9; while (i--) ;
	}
//	delay_us(tPDH);
	GPIO_WriteLow(GPIO1,DQ_PIN);
//	delay_us(tPDL);
	j = 2; while (j--)
	{
		i = 40; while (i--) ;
	}
	GPIO_WriteHigh(GPIO1,DQ_PIN);
}


void WRITE_BYTES(const unsigned char * src, unsigned char len)
{
		unsigned char i = 0;
		unsigned char k = 0;
		char value;

		DQ_IN;	
    while(len>0)
		{
      value=*src;
			for(i=0;i<8;i++)
			{
				while (DQ_PIN_VALUE != 0) ;
			
				DQ_OUT;
				if (value & 0x1)
					DQ_PIN_HIGH;
				else
					DQ_PIN_LOW;
			
				value>>=1;
       
				k = 20; while (k--);
				
				DQ_PIN_HIGH;
            
				DQ_IN;
				if( DQ_PIN_VALUE == 0 )
				{
					while( DQ_PIN_VALUE == 0 ) ;
                ACT_INIT();
					return;
				}
				
			}
      src++;
      len--;
    }
}

uint16_t time0_value = 0;
void Timer0Init(uint16_t us)
{
	
	time0_value = 8190; //- 16 * us;
	TIM0_TimeBaseInit(TIM0_PRESSEL_FSYS_D1,TIM0_MODE_TIMER);	//Timer0 12分频,做定时器
	TIM0_WorkMode1Config(time0_value);						//设置Timer0工作模式0，初值为7192，定时1ms
//	TIM0_WorkMode1Config(65536-1600);
//	TIM0_ITConfig(ENABLE, LOW);						//使能Timer0中断
//	TIM0_Cmd(ENABLE);								//使能Timer0计数
}

uint16_t lowTime = 0;
void main(void)
{
	unsigned char i = 0;
	unsigned char k = 0;
	
	unsigned char cmd = 0x00, Count = 0;
  unsigned char value = 0x00;

  OPINX = 0xc1;
  value = OPREG;
  value &= 0xcf;
  OPREG = value;

	GPIO_Init(GPIO1,GPIO_PIN_3,GPIO_MODE_OUT_PP);
	Timer0Init(1);
//	enableInterrupts();
	
	DQ_PIN_HIGH;
	DQ_IN;
		
	while (1)
	{
		DQ_IN;

		while (DQ_PIN_VALUE != 0)
		{
			;
		}
    
		TL0 = 1;	
		TH0 = 0;		
		TR0 = 1;	
				
		while (DQ_PIN_VALUE == 0)
		{
			;
		}
		
		TR0 = 0;
		
		lowTime = TH0;
		lowTime = lowTime << 8;
		lowTime |= TL0;
		lowTime = lowTime >> 4;
		
    if( lowTime > tRSTL )
		{
			Count = 0;
			cmd = 0;
			ACT_INIT();
			continue;
		}

    Count++;
		cmd>>=1;
    if (lowTime < tLOW1)
		{
			cmd |= 0x80 ;
    }
		else if (lowTime > tLOW0)
		{
		}

		if (Count == 1)
		{
			k = 1;
		}
		else if (Count == 2)
		{
			k = 2;
		}
		else if (Count == 3)
		{
			k = 3;
		}
		else if (Count == 4)
		{
			k = 4;
		}
		else if (Count == 5)
		{
			k = 5;
		}
		else if (Count == 6)
		{
			k = 6;
		}
		else if (Count == 7)
		{
			k = 7;
		}
		else if (Count == 8)
		{
			k = 8;
		}
		
		if (Count == 8)
		{
			Count = 0;
			WRITE_BYTES(&cmd, 1);
			cmd = 0x00;
		}

	}
	
}