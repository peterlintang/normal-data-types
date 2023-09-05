//************************************************************
//  Copyright (c) 深圳市赛元微电子有限公司
//	文件名称	: sc92f8003_it.c
//	作者		: 
//	模块功能	: sc92F8003_IT固件库函数C文件
//	局部函数列表:
//  最后更正日期: 2018/9/8
// 	版本		: V1.0   
//*************************************************************

#include "sc92f8003_gpio.h"
#include "sc92f8003_timer0.h"
#include "sc92f8003_timer1.h"
#include "sc92f8003_timer2.h"
#include "sc92f8003_adc.h"
#include "sc92f8003_btm.h"
#include "sc92f8003_pwm.h"
#include "sc92f8003_exti.h"
#include "sc92f8003_uart0.h"
#include "sc92f8003_ssi.h"

void EXTI0Interrupt()		interrupt 0
{

}

extern unsigned char delay_flag;

void Timer0Interrupt()		interrupt 1
{
	//TIM0_Mode0SetReloadCounter(0);
	delay_flag = 0;
	/*
	if(GPIO_ReadPin(GPIO1, GPIO_PIN_0))
	{
		GPIO_WriteLow(GPIO1,GPIO_PIN_0);	
	}
	else
	{
		GPIO_WriteHigh(GPIO1,GPIO_PIN_0);
	}
	*/
}

void EXTI1Interrupt()		interrupt 2
{

}

void Timer1Interrupt()		interrupt 3
{

}

void UART0Interrupt()		interrupt 4
{

}

void Timer2Interrupt()		interrupt 5
{

}

void ADCInterrupt()			interrupt 6
{

}

void SSIInterrupt()			interrupt 7
{
	
}

void PWMInterrupt()			interrupt 8
{

}

void BTMInterrupt()			interrupt 9
{

}

void EXTI2Interrupt()		interrupt 10
{

}