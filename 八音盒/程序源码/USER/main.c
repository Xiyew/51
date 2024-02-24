#include <REGX52.H>

#include "Delay.h"
#include "Timer0.h"
#include "music.h"
#include "keyboard.h"
#include "LED.h"

//蜂鸣器端口定义
sbit Buzzer = P1^5;
sbit keypause = P3^2;
sbit keyspeed = P3^3;

uchar MusicSelect = 0,LEDMusicSelect = 0;//音乐表
uchar FreqSelect;//频率
uint location = 1;//数码管位选码

uchar changemusic = 0x00;//歌曲选择
uint SPEED = noteSPEED;//播放速度
uint speedflag = 1; 

bit flag = 0x00;//音乐暂停播放标志位

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
			if(Music[changemusic][MusicSelect][0] != 0xFF)	//如果不是停止标志位
			{
				FreqSelect=Music[changemusic][MusicSelect][0];	//选择音符对应的频率
				Delay(SPEED/4*Music[changemusic][MusicSelect][1]);	//选择音符对应的时值
				MusicSelect++;
				
				TR0 = 0;
				Delay(10);
				TR0 = 1;
				
				if(Music[changemusic][MusicSelect][0] == 0xFF)	//如果是停止标志位
				{
					TR0 = 0;
					TR1 = 0;
					MusicSelect = 0;
					flag = 0;
				}
			}	
		}

		else if(input() != 0)//判断是否有按键按下并返回FreqSelect
		{
			Delay(500);//等待 音符或频率模式 播放完毕
			TR0 = 0;
			FreqSelect = 0;
		}
	}
}

void pause() interrupt 0	//音乐暂停
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

void musicspeed() interrupt 2	//音乐速度
{
	if(keyspeed == 0)
	{
		Delay(10);
		if(keyspeed == 0)
		{
			speedflag++;
			SPEED = SPEED/speedflag;
			if(speedflag == 3)	//两档调速
			{
				speedflag = 1;
				SPEED = noteSPEED;
			}
	 		while(!keyspeed);
		}
	}
}

void Timer0_Routine() interrupt 1	//音乐播放中断
{
	if(FreqTable[FreqSelect])	//如果不是休止符
	{
		/*取对应频率值的重装载值到定时器*/
		TL0 = FreqTable[FreqSelect]%256;		//设置定时初值
		TH0 = FreqTable[FreqSelect]/256;		//设置定时初值
		Buzzer=!Buzzer;	//翻转蜂鸣器IO口
	}
}

void Timer1_Routine() interrupt 3	//LED显示中断
{
	P0 = 0x00;			//段码清0，消影
	TR1 = 0;			//定时器1停止计时
	TL1 = 0x66;			//设置定时初值1ms
	TH1 = 0xFC;		
	
	if(Music[changemusic][MusicSelect][0] != 0xFF)	//如果不是休止符
	{
		/*歌词显示*/
		if(LEDMusicSelect<=121)	//防止最后音符位数不足导致LED乱码
		{
			led_display(location,Music[changemusic][LEDMusicSelect][2]);//歌词动态显示
		}

		location ++;
		LEDMusicSelect++;
		
		/*歌词随动显示*/
		if(LEDMusicSelect > (MusicSelect + 1))	//数码管显示位数等于歌唱音符位数+1
		{
			P0=0x00;	//中断为给低电平调节占空比实现PWM效果
		}
		
		/*动态显示*/
		if(location == 9)	//LED八位显示完毕重置位选码
		{
			location = 1;
			LEDMusicSelect = LEDMusicSelect - 8;
			if(MusicSelect % 8 == 0)
			{
				LEDMusicSelect = MusicSelect;
			}
		}
	}
	TR1 = 1;		//定时器1开始计时		
}
