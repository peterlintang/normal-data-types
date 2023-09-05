 
#include "onewire.h"
#include "sc92f8003_gpio.h"
#include "sc92f8003_timer0.h"

unsigned char delay_flag = 0;


void Delayus2(uint16_t us)
{
	uint16_t value = 0;
	
	value = 65536 - 16 * us;
	delay_flag = 1;
	TIM0_WorkMode1Config(value);
	TIM0_ITConfig(ENABLE, LOW);						//使能Timer0中断
	TIM0_Cmd(ENABLE);								//使能Timer0计数
	while (delay_flag == 1)
		;
	TIM0_Cmd(0);
	TIM0_ITConfig(0, LOW);
}


void delayus(uint16_t us)
{
#if 0  /* 不用系统延时时开启这个！！*/
    volatile uint16_t i;
	
    for (; us > 0; us--)
    {
        i = 50;  //mini 17
        while(i--);
    }
#else
		Delayus2(us);
#endif
}

/**
  * @brief  初始化单总线
  * @param  none
  * @retval none
*/
void onewire_init(void)
{
	DQ_PIN_HIGH;
	GPIO_Init(GPIO1,DQ_PIN,GPIO_MODE_OUT_PP);	
//	
}



/**
  * @brief  单总线复位时序
  * @param  onewire 总线结构体指针
  * @retval 成功返回0
*/
unsigned char ops_onewire_reset(void)
{
	unsigned char ret = 0;

	DQ_OUT;
	DQ_PIN_HIGH;
	delayus(50);
	DQ_PIN_LOW;
	delayus(500);
	DQ_PIN_HIGH;
	DQ_IN;
	delayus(40);
	ret = DQ_PIN_VALUE;
	DQ_OUT;
	DQ_PIN_HIGH;
	delayus(100);

	return ret;
}

/**
  * @brief  单总线读取一字节数据
  * @param  onewire 总线结构体指针
  * @retval 返回读取的数据
*/
static unsigned char ops_onewire_read_byte(void)
{
	unsigned char value = 0;
	unsigned char i = 0;
	unsigned char j = 0;
	unsigned char k = 0;

	for(i=8;i>0;i--)
	{
		value >>= 1;
		DQ_OUT;
		DQ_PIN_LOW;
		k = 3; while (k--) ;
		DQ_PIN_HIGH;
		DQ_IN;
		k = 3; while (k--);
		if(DQ_PIN_VALUE)
			value |= 0x80;
		else
			value &= 0x7f;
		delayus(40);
		DQ_OUT;
		DQ_PIN_HIGH;
	}
	return value;
}

/**
  * @brief  读取多字节
  * @param  onewire 总线结构体指针
	* @param  buff 存放数据缓存
	* @param  size 数据大小
  * @retval 返回读取到的数据大小
*/
int ops_onewire_read(void *buff,int len)
{
	int i;
	unsigned char *p = (unsigned char*)buff;
	
	for(i=0;i<len;i++)
		p[i++]=ops_onewire_read_byte();
	return i;
}

/**
  * @brief  单总线写一字节
  * @param  onewire 总线结构体指针
	* @param  data 待写数据
  * @retval 返回读取的数据
*/
static int ops_onewire_write_byte(char value)
{
	unsigned char i;
	unsigned char k;

	DQ_OUT;
	for(i=8;i>0;i--)
	{
		DQ_PIN_LOW;
		k = 2; while (k--);

		//		delayus(1);
		if(value&0x01)
			DQ_PIN_HIGH;

		delayus(40);

		DQ_PIN_HIGH;
		k = 15; while (k--);
		value >>= 1;
	}
	return 0;
}

/**
  * @brief  写多字节
  * @param  onewire 总线结构体指针
	* @param  buff 代写数据地址
	* @param  size 数据大小
  * @retval 写入数据大小
*/
int ops_onewire_write(void *buff,int len)
{
	int i;
	char *p = (char*)buff;

	for(i=0;i<len;i++)
	{
		if(ops_onewire_write_byte(p[i]) != 0)
			break;
	}
	return i;
}
