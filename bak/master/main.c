
#include "sc92f8003_gpio.h"
#include "onewire.h"
#include "sc92f8003_timer0.h"
#include <intrins.h>

void Timer0Init()
{
	TIM0_TimeBaseInit(TIM0_PRESSEL_FSYS_D1,TIM0_MODE_TIMER);	//Timer0 12��Ƶ,����ʱ��
	TIM0_WorkMode1Config(192);						//����Timer0����ģʽ0����ֵΪ7192����ʱ1ms
//	TIM0_WorkMode1Config(65536-1600);
//	TIM0_ITConfig(ENABLE, LOW);						//ʹ��Timer0�ж�
//	TIM0_Cmd(ENABLE);								//ʹ��Timer0����
}


void main(void)
{
	unsigned char i;
	unsigned char value = 0x00;
	unsigned char cmd = 0xa5;
	
	OPINX = 0xc1;
	value = OPREG;
	value &= 0xcf;
	OPREG = value;
	
	GPIO_Init(GPIO1,GPIO_PIN_3,GPIO_MODE_OUT_PP);	// just for test led
	onewire_init();
	Timer0Init();
	enableInterrupts();
		
	while (1)
	{
		DQ_OUT;
		
		ops_onewire_reset();
		
		ops_onewire_write(&cmd, 1);
		ops_onewire_read(&value, 1);
		if (value != cmd)
		{
			unsigned char j, k;
		  k = 230; while (k--)
			{
				j = 230; while (j--) ;
			}
		  GPIO_WriteHigh(GPIO1,GPIO_PIN_3);
		  k = 230; while (k--)
			{
				j = 230; while (j--) ;
			}
		  GPIO_WriteLow(GPIO1,GPIO_PIN_3); 
		}
		cmd++;
		
	}	
}