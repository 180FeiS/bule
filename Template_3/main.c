#include <main.h>

void tast_clock()
{
	if(++key_tt == 20) {key_tt = 0;key_scan();}
	if(++show_tt == 20) {show_tt = 0;show_ref = 1;}
	if(++lcm_tt == 300) {lcm_tt = 0;lcm_ref = 1;}
}

void main()
{
	
	all_init();
	delay_ms(1);
	Timer0_Init();
	Timer1_Init();
	while(1)
	{
		if(key_flag == 1)
		{
			key_flag = 0;
			tast_key();
		}
		if(show_ref == 1)
		{
			show_ref = 0;
			tast_show();
		}
		if(lcm_ref == 1)
		{
			lcm_ref = 0;
			ultra();
		}
			
	}
}

void tast_key()
{
	if(key_val == 7)
	{
		key_val = 0;
	}
}

void tast_show()
{
	switch(show_state)
	{
		case 1:
		{
			seg_buf[0] = key_val/10;
			seg_buf[1] = key_val%10;
			seg_buf[2] = 20;
			seg_buf[3] = 20;
			seg_buf[4] = 20;
			seg_buf[5] = lcm/100;
			seg_buf[6] = lcm%100/10;
			seg_buf[7] = lcm%10;
		}
		break;
	}
}

void ultra()
{
	send();
	TR1 = 1;
	while((RX == 1)&&(TF1 == 0));
	TR1 = 0;
	if(TF1 == 1)
	{
		TF1 = 0;
		lcm = 999;
	}
	else 
	{
		lcm_time = TH1;
		lcm_time<<=8;
		lcm_time|=TL1;
		lcm = (uint)(lcm_time*0.017);
	}
	TL1 = 0x00;
	TH1 = 0x00;
}

void send()
{
	uint i;
	for(i=0;i<8;i++)
	{
		TX = 1;Delay14us();
		TX = 0;Delay14us();
	}
}

void Delay14us(void)	//@12.000MHz
{
	unsigned char data i;

	_nop_();
	_nop_();
	i = 47;
	while (--i);
}

void Timer1_Init(void)		//1微秒@12.000MHz
{
	AUXR &= 0xBF;			//定时器时钟12T模式
	TMOD &= 0x0F;			//设置定时器模式
	TL1 = 0x00;				//设置定时初始值
	TH1 = 0x00;				//设置定时初始值
	TF1 = 0;				//清除TF1标志
	//TR1 = 1;				//定时器1开始计时
}


void time0() interrupt 1
{
	P0 = 0X00;
	P2|=0XC0;P2&=0XDF;
	P2&=0X1F;
	
	P0 = seg_num[seg_buf[num]];
	P2|=0XE0;
	P2&=0X1F;
	
	P0 = _crol_(0x01,num);
	P2|=0XC0;P2&=0XDF;
	P2&=0X1F;
	if(++num == 8) num=0;
	
	tast_clock();
}



void key_scan()
{
	uchar key,key1,key2;
	switch(keystate)
	{
		case key_check:
		{
			key_w(0x0f);
			if((key_r()&0x0f)!=0x0f) keystate = key_press;
		}
		break;
		case key_press:
		{
			key_w(0x0f);
			if((key_r()&0x0f)!=0x0f) key1 = key_r()&0x0f;
			key_w(0xf0);
			if((key_r()&0xf0)!=0xf0) key2 = key_r()&0xf0;
			key = key1|key2;
			if(key!=0xff)
			{
				switch(key)
				{
					case 0x7e:key_val = 7;break;
					case 0x7d:key_val = 6;break;
					case 0x7b:key_val = 5;break;
					case 0x77:key_val = 4;break;
					case 0xbe:key_val = 11;break;
					case 0xbd:key_val = 10;break;
					case 0xbb:key_val = 9;break;
					case 0xb7:key_val = 8;break;
					case 0xde:key_val = 15;break;
					case 0xdd:key_val = 14;break;
					case 0xdb:key_val = 13;break;
					case 0xd7:key_val = 12;break;
					case 0xee:key_val = 19;break;
					case 0xed:key_val = 18;break;
					case 0xeb:key_val = 17;break;
					case 0xe7:key_val = 16;break;
				}
				keystate = key_release;
			}
		}
		break;
		case key_release:
		{
			key_w(0x0f);
			if((key_r()&0x0f)!=0x0f)
			{
				key_time+=10;
			}
			else 
			{
				keystate = key_check;
				key_flag = 1;
			}
		}
		break;
	}
}

void key_w(uchar value)
{
	uchar val = value;
	P3 = val&0X3F;P44 = val>>7;P42 = (val&0x40)>>6;
}
uchar key_r()
{
	uchar val = 0XFF,val_P3 = 0,val_P42 = 0,val_P44 = 0;
	val_P3 = P3;val_P42 = P42;val_P44 = P44;
	val = ((val_P3&0x3f)|(val_P42<<6)|(val_P44<<7));
	return val;
}

void Timer0_Init(void)		//1毫秒@12.000MHz
{
	AUXR |= 0x80;			//定时器时钟1T模式
	TMOD &= 0xF0;			//设置定时器模式
	TL0 = 0x20;				//设置定时初始值
	TH0 = 0xD1;				//设置定时初始值
	TF0 = 0;				//清除TF0标志
	TR0 = 1;				//定时器0开始计时
	EA = 1;
	ET0 = 1;
}


void all_init()
{
	P0 = 0XFF;
	P2|=0X80;P2&=0X9F;
	P2&=0X1F; //流水灯
	
	P0 = 0XFF;
	P2|=0XE0;
	P2&=0X1F; //段
	 
	P0 = 0X00;
	P2|=0XC0;P2&=0XDF;
	P2&=0X1F; //位
	
	P0 = 0X00;
	P2|=0XA0;P2&=0XBF;
	P2&=0X1F; //蜂鸣器继电器
}

void delay_ms(uint ms)
{
	uint i,j;
	for(i=0;i<ms;i++)
		for(j=0;j<853;j++);
}