 
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
#if 1  /* ����ϵͳ��ʱʱ�����������*/
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
  * @brief  ��ʼ��������
  * @param  none
  * @retval none
*/
void onewire_init(void)
{
	GPIO_Init(GPIO1,DQ_PIN,GPIO_MODE_OUT_PP);	
}



/**
  * @brief  �����߸�λʱ��
  * @param  onewire ���߽ṹ��ָ��
  * @retval �ɹ�����0
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
  * @brief  �����߶�ȡһ�ֽ�����
  * @param  onewire ���߽ṹ��ָ��
  * @retval ���ض�ȡ������
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
  * @brief  ��ȡ���ֽ�
  * @param  onewire ���߽ṹ��ָ��
	* @param  buff ������ݻ���
	* @param  size ���ݴ�С
  * @retval ���ض�ȡ�������ݴ�С
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
  * @brief  ������дһ�ֽ�
  * @param  onewire ���߽ṹ��ָ��
	* @param  data ��д����
  * @retval ���ض�ȡ������
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
  * @brief  д���ֽ�
  * @param  onewire ���߽ṹ��ָ��
	* @param  buff ��д���ݵ�ַ
	* @param  size ���ݴ�С
  * @retval д�����ݴ�С
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
