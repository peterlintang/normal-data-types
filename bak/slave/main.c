
#include "sc92f8003_gpio.h"


#define		DQ_PIN	GPIO_PIN_4
#define	  DQ_OUT	GPIO_Init(GPIO1,DQ_PIN,GPIO_MODE_OUT_PP)
#define   DQ_IN		GPIO_Init(GPIO1,DQ_PIN,GPIO_MODE_IN_PU)
#define 	DQ_DE		GPIO_DeInit()

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

static void delay_us(uint32_t us)
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

	
void ACT_INIT(void)
{
	GPIO_Init(GPIO1,DQ_PIN,GPIO_MODE_OUT_PP);
	delay_us(tPDH);
	GPIO_WriteLow(GPIO1,DQ_PIN);
	delay_us(tPDL);
	delay_us(tPDL);
	GPIO_WriteHigh(GPIO1,DQ_PIN);
}


void WRITE_BYTES(const unsigned char * src, unsigned char len){
	unsigned char i = 0;
	char value;
	
    DQ_IN;
    while(len>0){
        value=*src;
		for(i=0;i<8;i++){

			while( GPIO_ReadPin(GPIO1,DQ_PIN) == 1 );
			
			DQ_OUT;
			if (value & 0x1)
				GPIO_WriteHigh(GPIO1,DQ_PIN);
			else
				GPIO_WriteLow(GPIO1,DQ_PIN);
			
			value>>=1;
            
			delay_us(tRDV);//max tLOWR
			delay_us(tRDV);//
			
			GPIO_WriteHigh(GPIO1,DQ_PIN);
            
			DQ_IN;
			if( GPIO_ReadPin(GPIO1,DQ_PIN) == 0 ){
				while( GPIO_ReadPin(GPIO1,DQ_PIN) == 0 );
                ACT_INIT();
				return;
			}
		}
        src++;
        len--;
    }
}



void main(void)
{
	unsigned int i = 0;
	unsigned int lowTime;
	unsigned char cmd = 0, Count = 0;
	unsigned char datas[10] = { 0 };	
	BitStatus ret = 0;
	
	GPIO_Init(GPIO1,GPIO_PIN_3,GPIO_MODE_OUT_PP);
	
	while (1)
	{
		        DQ_IN;
		while (1)
		{
			ret = GPIO_ReadPin(GPIO1,DQ_PIN);
			if (ret != 1) 
			{
//				GPIO_WriteHigh(GPIO1,GPIO_PIN_3);
				break;
			}
		}
//    	while( GPIO_ReadPin(GPIO1,DQ_PIN) == 1 );
    	
		lowTime = 0;
		while (1)
		{
			ret = GPIO_ReadPin(GPIO1, DQ_PIN);
			if (ret != 0)
			{
//				GPIO_WriteLow(GPIO1,GPIO_PIN_3); 
				break;
			}
			lowTime++;
			delay_us(1);
		}
		/*
    	lowTime = 0;
    	while( GPIO_ReadPin(GPIO1,DQ_PIN) == 0 ){
			lowTime++;
			delay_us(1);
		}
		*/
		
    	if( lowTime > tRSTL )
			{
				Count = 0;
//				GPIO_WriteHigh(GPIO1,GPIO_PIN_3);
				ACT_INIT();
//				GPIO_WriteLow(GPIO1,GPIO_PIN_3); 
				continue;
			}
			
    	Count++;
			cmd>>=1;
    	if( lowTime < tLOW1 ){
				cmd |= 0x80 ;
				GPIO_WriteHigh(GPIO1,GPIO_PIN_3);
    	}
			else if (lowTime > tLOW0)
			{
				GPIO_WriteLow(GPIO1,GPIO_PIN_3); 
			}

		if( Count == 8 )
		{
			Count = 0;
			datas[i] = cmd;
			i++;
			if (i >= 10) i = 0;
			/*
			if (cmd == 0x10)
			{
				delay_us(100);
				GPIO_WriteHigh(GPIO1,GPIO_PIN_3);
				delay_us(100);
				GPIO_WriteLow(GPIO1,GPIO_PIN_3); 
			}
			*/
			cmd = 0x00;
			/*
			if( cmd == datas[0] ){
				WRITE_BYTES( datas + 1, 8 );
			}
			*/
		}

	}
	
	/*
	uint16_t i,j;
	
	GPIO_Init(GPIO1,DQ_PIN,GPIO_MODE_OUT_PP);	//将P14设置为强推挽输出

	while(1)
	{
		GPIO_WriteHigh(GPIO1,DQ_PIN);			//P14写高
		for(i=0;i<600 * 1;i++)							//延时一段时间
		{
			for(j=0;j<300;j++);
		}
		GPIO_WriteLow(GPIO1,DQ_PIN);			//P14写低
		for(i=0;i<600 * 1;i++)							//延时一段时间
		{
			for(j=0;j<300;j++);
		}
	}
	*/
}