#include "keyboard.h"
#include "Delay.h"
#include "REGX52.h"
#include "LED.h"

uchar mode;     //频率模式

/*按键检测*/
int input(void)
{

    uchar button;
    P1 = 0xFF;
    
    if(MusicSelect == 0)//判断是否在显示歌词
    {
        led_display(1,mode);//显示初始频率模式
    }

    P1 = 0xf0;                      //列输入
    if(0xf0 != (P1 & 0xf0))           //识别列是否有按键按下
    {
        Delay(10);                
        if(0xf0 != (button = P1 & 0xf0))   //再次识别列输入并发送列码
        {
            P1 = 0x0F;              //行输入
            button |= (P1&0x0f);       //取行码与列码组合
					
            while(P1 != 0x0F)   //判断按键是否松开
            {
				P1 = 0x0F;
            }
            switch(button)             //判断那个按键
            {                        
                case 0x77:
                    TR0 = 1;
                    FreqSelect = 1 + (mode * 12); 
                    break;   //S1
                case 0x7B:
                    TR0 = 1;
                    FreqSelect = 3 + (mode * 12); 
                    break;   //S2
                case 0x7D:
                    TR0 = 1;
                    FreqSelect = 5 + (mode * 12); 
                    break;   //S3
                case 0x7E:
                    TR0 = 1;
                    FreqSelect = 6 + (mode * 12);
                    break;   //S4
                case 0xB7:
                    TR0 = 1;
                    FreqSelect = 8 + (mode * 12); 
                    break;   //S5
                case 0xBB:
                    TR0 = 1;
                    FreqSelect = 10 + (mode * 12); 
                    break;   //S6
                case 0xBD:
                    TR0 = 1;
                    FreqSelect = 12 + (mode * 12); 
                    break;   //S7
                case 0xBE:  
					mode ++;    //频率调节
					if(mode > 2)
					{
						mode = 0;
					}

                    TR1 = 0;
                    P0=0x00;//消影
                    led_display(1,mode);

                    Delay(500);
                    if(MusicSelect != 0)//判断是否在显示歌词
                    {
                        TR1 = 1;
                    }

					FreqSelect = 0;
                    break;   //S8
                case 0xD7:  
                    changemusic = !changemusic;
                    MusicSelect = 0;
                    TR1 = 0;
                    P0=0x00;//消影
                    led_display(2,changemusic + 1);

                    Delay(500);
                    if(MusicSelect != 0)//判断是否在显示歌词
                    {
                        TR1 = 1;
                    }

					FreqSelect = 0;
                    break;   //S9
                default: 
                    return 0;
                    break;
            }
        }
    } 
		P1 = 0xFF;
		return FreqSelect;
}
