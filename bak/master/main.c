
#include "sc92f8003_gpio.h"
#include "onewire.h"

	



void main(void)
{
	unsigned int i;
	unsigned char datas[48] = {
		0x99,0x0,0xff,0x0,0xff,0x0,0xff,0x0,0xff,
	};	
	for (i = 0; i < 48; i++)
		datas[i] = 10;
	
	GPIO_Init(GPIO1,GPIO_PIN_3,GPIO_MODE_OUT_PP);	
	onewire_init();
	
	while (1)
	{
		DQ_OUT;
		delayus(1000);
		
		ops_onewire_reset();
		for (i = 0; i < 48; i++)
		{
			ops_onewire_write(&(datas[i]), 1);
			delayus(1000);
			GPIO_WriteHigh(GPIO1,GPIO_PIN_3);
			delayus(1000);
			GPIO_WriteLow(GPIO1,GPIO_PIN_3); 
		}
	}	
}