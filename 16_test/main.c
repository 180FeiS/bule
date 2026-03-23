#include <main.h>
#include <onewire.h>
#include <iic.h>
#include <ds1302.h>


void tast_clock()
{
	if(++key_tt == 10) {key_tt = 0;key_scan();}
	if(++show_tt == 10) {show_tt = 0;show_ref = 1;}
	//if(++ultra_tt == 500) {ultra_tt = 0;ultra_ref = 1;}
	if(++pwm_tt == 500) {pwm_tt = 0;pwm_ref = 1;}
	if(++temp_tt == 500) {temp_tt = 0;temp_ref = 1;}
	if(++AD_tt == 500) {AD_tt = 0;AD_ref = 1;}
	if(++time_tt == 950) {time_tt = 0;time_ref = 1;}
}

void main()
{
	all_init();
	temp_get();delay_ms(900);
	temp_val =  temp_get()*625;
	Timer2_Init();
	//Timer1_Init();
	Timer0_Init();
	Uart1_Init();
	//AT_W(0x00,0);
	open = AT_R(0x00);
	AT_W(0x00,++open);
	time_init();time_get();
	while(1)
	{
		if(key_flag)
		{
			key_flag = 0;
			tast_key();
		}
		if(show_ref)
		{
			show_ref = 0;
			tast_show();
		}
		
//		if(ultra_ref)
//		{
//			ultra_ref = 0;
//			ultra();
//		}
		
		if(pwm_ref)
		{
			pwm_ref = 0;
//			pwm_freq = 100000/pwm_freq_para-1;
//			pwm_duty = 100000/pwm_freq_para*pwm_duty_para/100;
			pwm_in_duty = pwm_h_save*100/(pwm_l_save+pwm_h_save);
			pwm_in_freq = 100000/(pwm_l_save+pwm_h_save);
		}
		
		if(temp_ref)
		{
			temp_ref = 0;
			temp_val =  temp_get()*625;
		}
		if(AD_ref)
		{
			AD_ref = 0;
			AD_val = AD_R(0x03);
			AD_W(AD_val);
			AD_val = AD_val *1.963;
		}
		
		if(time_ref)
		{
			time_ref = 0;
			time_get();
		}
	}
}

//按键
void tast_key()
{
	if(key_val == 7)
	{
		key_val = 0;
	}
	if(key_val == 6)
	{
		key_val = 0;
		uart(uart_receive);
	}
}

//显示
void tast_show()
{
	switch(show_state)
	{
		case 1:
		{
			seg_buf[0] = time_ds[2]/10;
			seg_buf[1] = time_ds[2]%10;
			seg_buf[2] = 20;
			seg_buf[3] = time_ds[1]/10;
			seg_buf[4] = time_ds[1]%10;
			seg_buf[5] = 20;
			seg_buf[6] = time_ds[0]/10;
			seg_buf[7] = time_ds[0]%10;
		}
		break;
	}
}
//串口

void uart(uchar *p)
{
	uchar index = 0;
	do
	{
		SBUF = p[index++];
		while(TI == 0);
		TI = 0;
	}
	while(p[index]!=0);
}

void Uart1_Isr(void) interrupt 4
{
	if (RI)				//检测串口1接收中断
	{
		RI = 0;			//清除串口1接收中断请求位
		uart_send = SBUF;
		if(uart_send == '1') {led_on(1);}
		else if(uart_send == '2') {led_off(1);}
	}
}

void Uart1_Init(void)	//1200bps@12.000MHz
{
	SCON = 0x50;		//8位数据,可变波特率
	AUXR |= 0x40;		//定时器时钟1T模式
	AUXR &= 0xFE;		//串口1选择定时器1为波特率发生器
	TMOD &= 0x0F;		//设置定时器模式
	TL1 = 0x3C;			//设置定时初始值
	TH1 = 0xF6;			//设置定时初始值
	ET1 = 0;			//禁止定时器中断
	TR1 = 1;			//定时器1开始计时
	ES = 1;				//使能串口1中断
	EA = 1;
}


//串口


//pwm

void Timer0() interrupt 1
{
	//led_on(1);
//	pwm_count++;
//	if(pwm_count == pwm_duty){pwm_out = 0;}
//	else if(pwm_count == pwm_freq){pwm_out = 1;pwm_count = 0;}
	
	if(pwm_in == 1)
	{
		pwm_h++;
		if(pwm_in_flag == 1)
		{
			pwm_l_save = pwm_l;
			pwm_l = 0;
			pwm_in_flag = 0;
		}
	}
	if(pwm_in == 0)
	{
		pwm_l++;
		if(pwm_in_flag == 0)
		{
			pwm_h_save = pwm_h;
			pwm_h = 0;
			pwm_in_flag = 1;
		}
	}
}

void Timer0_Init(void)		//10微秒@12.000MHz
{
	AUXR |= 0x80;			//定时器时钟1T模式
	TMOD &= 0xF0;			//设置定时器模式
	TL0 = 0x88;				//设置定时初始值
	TH0 = 0xFF;				//设置定时初始值
	TF0 = 0;				//清除TF0标志
	TR0 = 1;				//定时器0开始计时
	EA = 1;
	ET0 = 1;
	PT0 = 1;
}


//超声波
/*
void ultra()
{
	send_wave();
	TR1 = 1;
	while((RX == 1)&&(TF1 == 0));
	TR1 = 0;
	if(TF1 == 1)
	{
		TF1 = 0;
		lcm = 99;
	}
	else if(RX == 0)
	{
		RX = 1;
		lcm_time = TH1<<8;
		lcm_time|=TL1;
		lcm = (uint)(lcm_time*0.017);
	}
	TH1 = 0x00;	TL1 = 0x00;
}

void send_wave()
{
	uchar i = 0;
	for(i=0;i<8;i++)
	{
		TX = 1;Delay15us();
		TX = 0;Delay15us();
	}
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


void Delay15us(void)	//@12.000MHz
{
	unsigned char data i;

	_nop_();
	_nop_();
	i = 42;
	while (--i);
}
*/
void Timer2() interrupt 12
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
	if(++num == 8) num = 0;
	
	tast_clock();
}

void Timer2_Init(void)		//1毫秒@12.000MHz
{
	AUXR |= 0x04;			//定时器时钟1T模式
	T2L = 0x20;				//设置定时初始值
	T2H = 0xD1;				//设置定时初始值
	AUXR |= 0x10;			//定时器2开始计时
	EA = 1;
	IE2|=0X04;
}
//按键 独立
void key_scan()
{
	switch(keystate)
	{
		case key_check:
		{
			if((P30 == 0)||(P31 == 0)||(P32 == 0)||(P33 == 0))
			keystate = key_press;
		}
		break;
		case key_press:
		{
			if(P30 == 0) key_val = 7;
			if(P31 == 0) key_val = 6;
			if(P32 == 0) key_val = 5;
			if(P33 == 0) key_val = 4;
			keystate = key_release;
		}
		break;
		case key_release:
		{
			if((P30 == 0)||(P31 == 0)||(P32 == 0)||(P33 == 0))
			{
				key_time+=10;
			}
			else 
			{
				key_flag = 1;
				keystate = key_check;
			}
		}
		break;
	}
}

//按键 矩阵
void key_arrscan()
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
			key|=0x10;
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
				key_flag = 1;
				keystate = key_check;
			}
		}
		break;
	}
}

void key_w(uchar value)
{
	uchar val = value;
	P30 = (val&0X01);P31 = ((val&0x02)>>1);P32 = ((val&0x04)>>2);
	P33 = ((val&0x08)>>3);P35 = ((val&0x20)>>5);
	P44 = (val>>7);P42 = ((val&0X40)>>6);
}

unsigned char key_r()
{
	uchar val = 0xff,val_P3 = 0,val_P44 = 0,val_P42 = 0;
	val_P3 = P3;val_P44 = P44;val_P42 = P42;
	val = ((val_P3&0x2f)|(val_P44<<7)|(val_P42<<6));
	return val;
}

void delay_ms(uint ms)
{
	uint i,j;
	for(i = 0;i<ms;i++)
		for(j = 0;j<853;j++);
}
void all_init()
{
	P0 = 0XFF;
	P2|=0X80;P2&=0X9F;
	P2&=0X1F; // 灯
	
	P0 = 0XFF;
	P2|=0XE0;
	P2&=0X1F; // 段
	
	P0 = 0X00;
	P2|=0XC0;P2&=0XDF;
	P2&=0X1F; //位
	
	P0 = 0X00;
	P2|=0XA0;P2&=0XBF;
	P2&=0X1F; //蜂鸣器继电器
}
