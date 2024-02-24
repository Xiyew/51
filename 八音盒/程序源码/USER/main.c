#include <REGX52.H>

#include "Delay.h"
#include "Timer0.h"
#include "music.h"
#include "keyboard.h"
#include "LED.h"

//�������˿ڶ���
sbit Buzzer = P1^5;
sbit keypause = P3^2;
sbit keyspeed = P3^3;

uchar MusicSelect = 0,LEDMusicSelect = 0;//���ֱ�
uchar FreqSelect;//Ƶ��
uint location = 1;//�����λѡ��

uchar changemusic = 0x00;//����ѡ��
uint SPEED = noteSPEED;//�����ٶ�
uint speedflag = 1; 

bit flag = 0x00;//������ͣ���ű�־λ

void main() 
{
	Timer0Init();
	Timer1Init();
	Exti0Init();
	Exti1Init();
	TR0 = 0;
	TR1 = 0;
	while(1)
	{		
 		if(flag == 0x01)
		{
			if(Music[changemusic][MusicSelect][0] != 0xFF)	//�������ֹͣ��־λ
			{
				FreqSelect=Music[changemusic][MusicSelect][0];	//ѡ��������Ӧ��Ƶ��
				Delay(SPEED/4*Music[changemusic][MusicSelect][1]);	//ѡ��������Ӧ��ʱֵ
				MusicSelect++;
				
				TR0 = 0;
				Delay(10);
				TR0 = 1;
				
				if(Music[changemusic][MusicSelect][0] == 0xFF)	//�����ֹͣ��־λ
				{
					TR0 = 0;
					TR1 = 0;
					MusicSelect = 0;
					flag = 0;
				}
			}	
		}

		else if(input() != 0)//�ж��Ƿ��а������²�����FreqSelect
		{
			Delay(500);//�ȴ� ������Ƶ��ģʽ �������
			TR0 = 0;
			FreqSelect = 0;
		}
	}
}

void pause() interrupt 0	//������ͣ
{
	if(keypause == 0)
	{
		Delay(10);
		if(keypause == 0)
		{
			flag = !flag;
			TR1 = flag;
	 		while(!keypause);
		}
	}
}

void musicspeed() interrupt 2	//�����ٶ�
{
	if(keyspeed == 0)
	{
		Delay(10);
		if(keyspeed == 0)
		{
			speedflag++;
			SPEED = SPEED/speedflag;
			if(speedflag == 3)	//��������
			{
				speedflag = 1;
				SPEED = noteSPEED;
			}
	 		while(!keyspeed);
		}
	}
}

void Timer0_Routine() interrupt 1	//���ֲ����ж�
{
	if(FreqTable[FreqSelect])	//���������ֹ��
	{
		/*ȡ��ӦƵ��ֵ����װ��ֵ����ʱ��*/
		TL0 = FreqTable[FreqSelect]%256;		//���ö�ʱ��ֵ
		TH0 = FreqTable[FreqSelect]/256;		//���ö�ʱ��ֵ
		Buzzer=!Buzzer;	//��ת������IO��
	}
}

void Timer1_Routine() interrupt 3	//LED��ʾ�ж�
{
	P0 = 0x00;			//������0����Ӱ
	TR1 = 0;			//��ʱ��1ֹͣ��ʱ
	TL1 = 0x66;			//���ö�ʱ��ֵ1ms
	TH1 = 0xFC;		
	
	if(Music[changemusic][MusicSelect][0] != 0xFF)	//���������ֹ��
	{
		/*�����ʾ*/
		if(LEDMusicSelect<=121)	//��ֹ�������λ�����㵼��LED����
		{
			led_display(location,Music[changemusic][LEDMusicSelect][2]);//��ʶ�̬��ʾ
		}

		location ++;
		LEDMusicSelect++;
		
		/*����涯��ʾ*/
		if(LEDMusicSelect > (MusicSelect + 1))	//�������ʾλ�����ڸ質����λ��+1
		{
			P0=0x00;	//�ж�Ϊ���͵�ƽ����ռ�ձ�ʵ��PWMЧ��
		}
		
		/*��̬��ʾ*/
		if(location == 9)	//LED��λ��ʾ�������λѡ��
		{
			location = 1;
			LEDMusicSelect = LEDMusicSelect - 8;
			if(MusicSelect % 8 == 0)
			{
				LEDMusicSelect = MusicSelect;
			}
		}
	}
	TR1 = 1;		//��ʱ��1��ʼ��ʱ		
}
