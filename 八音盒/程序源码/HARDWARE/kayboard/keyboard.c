#include "keyboard.h"
#include "Delay.h"
#include "REGX52.h"
#include "LED.h"

uchar mode;     //Ƶ��ģʽ

/*�������*/
int input(void)
{

    uchar button;
    P1 = 0xFF;
    
    if(MusicSelect == 0)//�ж��Ƿ�����ʾ���
    {
        led_display(1,mode);//��ʾ��ʼƵ��ģʽ
    }

    P1 = 0xf0;                      //������
    if(0xf0 != (P1 & 0xf0))           //ʶ�����Ƿ��а�������
    {
        Delay(10);                
        if(0xf0 != (button = P1 & 0xf0))   //�ٴ�ʶ�������벢��������
        {
            P1 = 0x0F;              //������
            button |= (P1&0x0f);       //ȡ�������������
					
            while(P1 != 0x0F)   //�жϰ����Ƿ��ɿ�
            {
				P1 = 0x0F;
            }
            switch(button)             //�ж��Ǹ�����
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
					mode ++;    //Ƶ�ʵ���
					if(mode > 2)
					{
						mode = 0;
					}

                    TR1 = 0;
                    P0=0x00;//��Ӱ
                    led_display(1,mode);

                    Delay(500);
                    if(MusicSelect != 0)//�ж��Ƿ�����ʾ���
                    {
                        TR1 = 1;
                    }

					FreqSelect = 0;
                    break;   //S8
                case 0xD7:  
                    changemusic = !changemusic;
                    MusicSelect = 0;
                    TR1 = 0;
                    P0=0x00;//��Ӱ
                    led_display(2,changemusic + 1);

                    Delay(500);
                    if(MusicSelect != 0)//�ж��Ƿ�����ʾ���
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
