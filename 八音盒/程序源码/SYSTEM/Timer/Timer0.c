#include <REGX52.H>
#include "Timer0.h"

/**
  * @brief  定时器0初始化，1毫秒@12.000MHz
  * @param  无
  * @retval 无
  */
void Timer0Init(void)
{
	//TMOD &= 0xF0;		//设置定时器模式
	TMOD |= 0x01;		//设置定时器模式
	
	TL0 = 0x18;		//设置定时初值
	TH0 = 0xFC;		//设置定时初值
	
	TF0 = 0;		//清除中断请求TF0标志
	TR0 = 1;		//定时器0开始计时
	
	ET0 = 1;
	
	EA = 1;
	
	PT0 = 1;
}

void Timer1Init(void)
{
	TMOD |= 0x10;		//设置定时器模式

	TL0 = 0x18;		//设置定时初值
	TH0 = 0xFC;		//设置定时初值

	TF1 = 0;		//清除中断请求TF1标志
	TR1 = 1;		//定时器1开始计时

	ET1 = 1;

	EA = 1;

	PT1 = 0;
}

void Exti0Init(void)
{
	EA = 1;
	EX0 = 1;//允许外部中断0
	IT0 = 1;//选择外部中断0为电平触发方式
}

void Exti1Init(void)
{
	EA = 1;
	EX1 = 1;//允许外部中断0
	IT1 = 1;//选择外部中断0为电平触发方式
}


