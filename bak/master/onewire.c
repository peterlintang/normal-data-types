 
#include "onewire.h"
#include "sc92f8003_gpio.h"

static void gpio_set_sdo(char state)
{
	if (state)
		GPIO_WriteHigh(GPIO1,DQ_PIN);
	else
		GPIO_WriteLow(GPIO1,DQ_PIN); 
}

static uint8_t gpio_get_sdo(void)
{
	return (GPIO_ReadPin(GPIO1,DQ_PIN));
}

void delayus(uint32_t us)
{
#if 1  /* 不用系统延时时开启这个！！*/
    volatile int32_t i;
	
    for (; us > 0; us--)
    {
        i = 100;  //mini 17
        while(i--);
    }
#else
		Delayus(us);
#endif
}

/**
  * @brief  初始化单总线
  * @param  none
  * @retval none
*/
void onewire_init(void)
{
	GPIO_Init(GPIO1,DQ_PIN,GPIO_MODE_OUT_PP);	
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
	gpio_set_sdo(1);
	delayus(50);
	gpio_set_sdo(0);
	delayus(500);
	gpio_set_sdo(1);
	delayus(40);
	DQ_IN;
	ret = gpio_get_sdo();
	DQ_OUT;
	delayus(500);
	gpio_set_sdo(1);

	return ret;
}

/**
  * @brief  单总线读取一字节数据
  * @param  onewire 总线结构体指针
  * @retval 返回读取的数据
*/
static char ops_onewire_read_byte(void)
{
	char value = 0;
	unsigned char i = 0;

	for(i=8;i>0;i--)
	{
		value >>= 1;
		DQ_OUT;
		gpio_set_sdo(0);
		delayus(5);
		gpio_set_sdo(1);
		DQ_IN;
		delayus(5);
		if(gpio_get_sdo())
			value |= 0x80;
		else
			value &= 0x7f;
		delayus(65);
		DQ_OUT;
		gpio_set_sdo(1);
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
	char *p = (char*)buff;
	
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

	DQ_OUT;
	for(i=8;i>0;i--)
	{
		gpio_set_sdo(0);
		delayus(5);
		if(value&0x01)
			gpio_set_sdo(1);
		else
			gpio_set_sdo(0);
		delayus(65);
		gpio_set_sdo(1);
		delayus(2);
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
