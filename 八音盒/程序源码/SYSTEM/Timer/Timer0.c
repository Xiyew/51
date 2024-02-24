#include <REGX52.H>
#include "Timer0.h"

/**
  * @brief  ��ʱ��0��ʼ����1����@12.000MHz
  * @param  ��
  * @retval ��
  */
void Timer0Init(void)
{
	//TMOD &= 0xF0;		//���ö�ʱ��ģʽ
	TMOD |= 0x01;		//���ö�ʱ��ģʽ
	
	TL0 = 0x18;		//���ö�ʱ��ֵ
	TH0 = 0xFC;		//���ö�ʱ��ֵ
	
	TF0 = 0;		//����ж�����TF0��־
	TR0 = 1;		//��ʱ��0��ʼ��ʱ
	
	ET0 = 1;
	
	EA = 1;
	
	PT0 = 1;
}

void Timer1Init(void)
{
	TMOD |= 0x10;		//���ö�ʱ��ģʽ

	TL0 = 0x18;		//���ö�ʱ��ֵ
	TH0 = 0xFC;		//���ö�ʱ��ֵ

	TF1 = 0;		//����ж�����TF1��־
	TR1 = 1;		//��ʱ��1��ʼ��ʱ

	ET1 = 1;

	EA = 1;

	PT1 = 0;
}

void Exti0Init(void)
{
	EA = 1;
	EX0 = 1;//�����ⲿ�ж�0
	IT0 = 1;//ѡ���ⲿ�ж�0Ϊ��ƽ������ʽ
}

void Exti1Init(void)
{
	EA = 1;
	EX1 = 1;//�����ⲿ�ж�0
	IT1 = 1;//ѡ���ⲿ�ж�0Ϊ��ƽ������ʽ
}


