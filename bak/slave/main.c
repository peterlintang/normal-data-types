
#include "sc92f8003_gpio.h"
#include "sc92f8003_timer0.h"

#define   DQ_PIN  GPIO_PIN_4
#define   DQ_OUT  do { P1CON |= DQ_PIN; } while (0)
#define   DQ_IN   do { P1CON &= ~DQ_PIN; P1PH |= DQ_PIN;} while (0)
#define   DQ_PIN_HIGH     (P1 |= DQ_PIN)
#define   DQ_PIN_LOW      (P1 &= ~DQ_PIN)
#define   DQ_PIN_VALUE    (P1 & DQ_PIN)

#define 	CMD_REPORT_STATUS	0x05

#define 	CHARGE_STATUS_MASK	0xc0
#define		BATTERY_STATUS_MASK	0x30
#define 	MODE_STATUS_MASK		0x08
#define 	SPEED_STATUS_MASK		0x06
#define		FILTER_STATUS_MASK	0x01

#define		CHARGE_STATUS_SHIFT		6	
#define		BATTERY_STATUS_SHIFT	4
#define 	MODE_STATUS_SHIFT			3
#define 	SPEED_STATUS_SHIFT		1
#define		FILTER_STATUS_SHIFT		0

#define 	GET_CHARGE_STATUS(status)		(((status) & CHARGE_STATUS_MASK) >> CHARGE_STATUS_SHIFT)
#define 	GET_BATTERY_STATUS(status)	(((status) & BATTERY_STATUS_MASK) >> BATTERY_STATUS_SHIFT)
#define 	GET_MODE_STATUS(status)			(((status) & MODE_STATUS_MASK) >> MODE_STATUS_SHIFT)
#define 	GET_SPEED_STATUS(status)		(((status) & SPEED_STATUS_MASK) >> SPEED_STATUS_SHIFT)
#define		GET_FILTER_STATUS(status)		(((status) & FILTER_STATUS_MASK) >> FILTER_STATUS_SHIFT)

#define		CHARGE_STATUS_CHARGING			0x01
#define		CHARGE_STATUS_NOT_CHARGING	0x00
#define 	CHARGE_STATUS_CHARGING_FULL	0x02
#define		CHARGE_STATUS_DEFAULT				0xff

#define		BATTERY_STATUS_LOW		0x01		//  0% -- 20% 
#define		BATTERY_STATUS_MIDDLE	0x02		//  20% -- 50%
#define		BATTERY_STATUS_HIGH		0x03		//  50% -- 100%
#define		BATTERY_STATUS_DEFAULT	0xff

#define 	MODE_STATUS_HARD_SOLEPLATE	0x00
#define		MODE_STATUS_carpet					0x01
#define		MODE_STATUS_DEFAULT					0xff

#define		SPEED_STATUS_LOW			0x01
#define		SPEED_STATUS_MIDDLE		0x02
#define		SPEED_STATUS_HIGH			0x03
#define		SPEED_STATUS_CLOGGED	0x00
#define		SPEED_STATUS_DEFAULT	0xff

#define		FILTER_STATUS_DIRTY			0x01
#define		FILTER_STATUS_CLEAN			0x00
#define		FILTER_STATUS_DEFAULT		0xff

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
unsigned char charge_status = CHARGE_STATUS_DEFAULT;
unsigned char battery_status = BATTERY_STATUS_DEFAULT;
unsigned char mode_status = MODE_STATUS_DEFAULT;
unsigned char speed_status = SPEED_STATUS_DEFAULT;
unsigned char filter_status = FILTER_STATUS_DEFAULT;

void process_charge_status(void)
{
	switch (charge_status)
	{
		case CHARGE_STATUS_CHARGING:
			GPIO_WriteHigh(GPIO1, GPIO_PIN_3);
		break;
		
		case CHARGE_STATUS_NOT_CHARGING:
		break;
		
		case CHARGE_STATUS_CHARGING_FULL:
			GPIO_WriteLow(GPIO1, GPIO_PIN_3);
		break;
		
		default:
		break;
	}
}

void process_battery_status(void)
{
	switch (battery_status)
	{
		case BATTERY_STATUS_LOW:
		break;
		
		case BATTERY_STATUS_MIDDLE:
		break;
		
		case BATTERY_STATUS_HIGH:
		break;
		
		default:
		break;
	}
}

void process_mode_status(void)
{
	switch (mode_status)
	{
		case MODE_STATUS_HARD_SOLEPLATE:
			
		break;
		
		case MODE_STATUS_carpet:
			
		break;
		
		default:
		break;
	}
}

void process_speed_status(void)
{
	switch (speed_status)
	{
		case SPEED_STATUS_LOW:
		break;
		
		case SPEED_STATUS_MIDDLE:
		break;
		
		case SPEED_STATUS_HIGH:
		break;
		
		case SPEED_STATUS_CLOGGED:
		break;
		
		default:
		break;
	}
}

void process_filter_status(void)
{
	switch (filter_status)
	{
		case FILTER_STATUS_DIRTY:
		break;
		
		case FILTER_STATUS_CLEAN:
		break;
		
		default:
		break;
	}
}

void init_gpio(void)
{
}

void main(void)
{
	unsigned char recv_data = 0x00;
	unsigned char Count = 0;
  unsigned char value = 0x00;

  OPINX = 0xc1;
  value = OPREG;
  value &= 0xcf;
  OPREG = value;

	GPIO_Init(GPIO1,GPIO_PIN_3,GPIO_MODE_OUT_PP);	// for test
	
	init_gpio();
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
			recv_data = 0;
			ACT_INIT();
			continue;
		}

    Count++;
		recv_data >>= 1;
    if (lowTime < tLOW1)
		{
			recv_data |= 0x80 ;
    }
		else if (lowTime > tLOW0)
		{
		}
		
		if (Count == 8)
		{
			if (recv_data == CMD_REPORT_STATUS)
			{
			}
			else
			{
				charge_status = GET_CHARGE_STATUS(recv_data);
				battery_status = GET_BATTERY_STATUS(recv_data);
				mode_status = GET_MODE_STATUS(recv_data);
				speed_status = GET_SPEED_STATUS(recv_data);
				filter_status = GET_FILTER_STATUS(recv_data);		
				
				process_charge_status();
				process_battery_status();
				process_mode_status();
				process_speed_status();
				process_filter_status();
			}
			
			Count = 0;
		//	recv_data = 0x00;
			
		
			WRITE_BYTES(&recv_data, 1);
			recv_data = 0x00;
			
		}

	}
	
}