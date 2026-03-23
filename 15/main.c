#include <main.h>
#include <iic.h>
#include <ds1302.h>

void tast_clock()
{
	if(++key_tt == 10) {key_tt = 0;key_scan();}
	if(++show_tt == 10) {show_tt = 0;show_ref = 1;}
	if(++ultra_tt == 500) {ultra_tt = 0;ultra_ref = 1;}
	if(++pwm_tt == 500) {pwm_tt = 0;pwm_ref = 1;}
	if(++AD_tt == 500) {AD_tt = 0;AD_ref = 1;}
	if(++time_tt == 900) {time_tt = 0;time_ref = 1;}
	if(++led_tt == 200) {led_tt = 0;led_ref = 1;}

}

void main()
{
	all_init();
	Timer0_Init();delay_ms(1);
	Timer1_Init();
	Timer2_Init();
	time_init();time_get();
	PT0 = 1;
	while(1)
	{
		if(show_ref)
		{
			show_ref = 0;
			tast_show();
		}
		
		if(key_flag)
		{
			key_flag = 0;
			tast_key();
		}
		if(ultra_ref)
		{
			ultra_ref = 0;
			ultra();
		}
		if(pwm_ref)
		{
			pwm_ref = 0;
//			pwm_freq = 100000/pwm_freq_buf-1;
//			pwm_duty = 100000/pwm_freq_buf*pwm_duty_buf/100;
			pwm_in_duty = (pwm_h_save*100)/(pwm_l_save+pwm_h_save);
			pwm_in_duty_last = 100000/(pwm_l_save+pwm_h_save);
			pwm_in_freq = pwm_in_duty_last+pwm_adjust;
			if((pwm_in_freq>pwm_max)&&(pwm_in_freq>=0) )
      {pwm_max = pwm_in_freq;time_save[0] = time_ds[0];time_save[1] = time_ds[1];time_save[2] = time_ds[2];}
			
		}
		
		if(AD_ref)
		{
			AD_ref = 0;
			if(pwm_in_freq>=pwm_para){AD_W(255);}
			else if(pwm_in_freq<=500){AD_W(51);}
			else 
			{
				k = 4/(float)(pwm_para-500);
			  AD_val = (k*pwm_in_freq+(5-k*pwm_para))*51;
				AD_W(AD_val);
			}
		}
		
		if(time_ref)
		{
			time_ref = 0;
			time_get();
		}
		
		if(led_ref)
		{
			led_ref = 0;
			tast_led();
		}
	}
}
//led
void tast_led()
{
	led_off(3);led_off(4);led_off(5);led_off(6);led_off(7);led_off(8);
	if(showstate == 1)
	{
		if(led1_flag == 0){led_on(1);led1_flag = 1;}
		else {led_off(1);led1_flag = 0;}
	}
	else 
	{
		led_off(1);
	}
	if(pwm_in_freq>pwm_para)
	{
		if(led2_flag == 0){led_on(2);led2_flag = 1;}
		else {led_off(2);led2_flag = 0;}
	}
	else if(pwm_in_freq<0)
	{
		led_on(2);
	}
	else 
	{
		led_off(2);
	}
	

	
}
//按键
void tast_key()
{
	if(key_val == 4)
	{
		key_val = 0;
		if(showstate == 1){showstate = 2;}
		else if((showstate == 2)||(showstate == 3)){showstate = 4;}
		else if(showstate == 4) {showstate = 5;}
		else if((showstate == 5)||(showstate == 6)){showstate = 1;}
	}
	else if(key_val == 5)
	{
		key_val = 0;
		if(showstate == 2){showstate = 3;}
		else if(showstate == 3){showstate = 2;}
		else if(showstate == 5){showstate = 6;}
		else if(showstate == 6){showstate = 5;}
	}
	else if(key_val == 8)// 加
	{
		key_val = 0;
		if(showstate == 2)
		{
			if(pwm_para>=9000){pwm_para = 9000;}
			else {pwm_para+=1000;}
		}
		else if(showstate == 3)
		{
			if(pwm_adjust>=900){pwm_adjust = 900;}
			else {pwm_adjust+=100;}
		}
	}
	else if(key_val == 9)// 减
	{
		key_val = 0;
		if(showstate == 2)
		{
			if(pwm_para<=1000){pwm_para = 1000;}
			else {pwm_para-=1000;}
		}
		else if(showstate == 3)
		{
			if(pwm_adjust<=-900){pwm_adjust = -900;}
			else {pwm_adjust-=100;}
		}
	}
}
//显示
void tast_show() //showstate 1 频率界面 2 超限参数界面 3 校准参数界面 4 时间界面 
{								 //          5 频率回显 6 时间回显
	switch(showstate)
	{
		case 1:
		{
			if(pwm_in_freq<0)
			{
				seg_buf[0] = 21;
				seg_buf[1] = 20;
				seg_buf[2] = 20;
				seg_buf[3] = 20;
			  seg_buf[4] = 20;
			  seg_buf[5] = 20;
				seg_buf[6] = 25;
			  seg_buf[7] = 25;
			}
			else 
			{
				seg_buf[0] = 21;
				seg_buf[1] = 20;
				seg_buf[2] = 20;
				if(pwm_in_freq>=10000) 
				{
					seg_buf[3] = pwm_in_freq/10000;
					seg_buf[4] = pwm_in_freq%10000/1000;
					seg_buf[5] = pwm_in_freq%1000/100;
					seg_buf[6] = pwm_in_freq%100/10;
					seg_buf[7] = pwm_in_freq%10;
				}
				else if((pwm_in_freq<10000)&&(pwm_in_freq>=1000))
				{
					seg_buf[3] = 20;
					seg_buf[4] = pwm_in_freq%10000/1000;
					seg_buf[5] = pwm_in_freq%1000/100;
					seg_buf[6] = pwm_in_freq%100/10;
					seg_buf[7] = pwm_in_freq%10;
				}
				else if((pwm_in_freq<1000)&&(pwm_in_freq>=100))
				{
					seg_buf[3] = 20;
					seg_buf[4] = 20;
					seg_buf[5] = pwm_in_freq%1000/100;
					seg_buf[6] = pwm_in_freq%100/10;
					seg_buf[7] = pwm_in_freq%10;
				}
				else if((pwm_in_freq<100)&&(pwm_in_freq>=10))
				{
					seg_buf[3] = 20;
					seg_buf[4] = 20;
					seg_buf[5] = 20;
					seg_buf[6] = pwm_in_freq%100/10;
					seg_buf[7] = pwm_in_freq%10;
				}
			}
		}
		break;
		case 2:
		{
			seg_buf[0] = 22;
			seg_buf[1] = 1;
			seg_buf[2] = 20;
			seg_buf[3] = 20;
			seg_buf[4] = pwm_para/1000;
			seg_buf[5] = pwm_para%1000/100;
			seg_buf[6] = pwm_para%100/10;
			seg_buf[7] = pwm_para%10;
		}
		break;
		case 3:
		{
			seg_buf[0] = 22;
			seg_buf[1] = 2;
			seg_buf[2] = 20;
			seg_buf[3] = 20;
			if(pwm_adjust>0)
			{
					seg_buf[4] = 20;
					seg_buf[5] = pwm_adjust/100;
					seg_buf[6] = pwm_adjust%100/10;
					seg_buf[7] = pwm_adjust%10;
			}
			else if(pwm_adjust<0)
			{
			  seg_buf[4] = 26;
				seg_buf[5] = (-pwm_adjust)/100;
				seg_buf[6] = pwm_adjust%100/10;
				seg_buf[7] = pwm_adjust%10;
			}
			else 
			{
				seg_buf[4] = 20;
				seg_buf[5] = 20;
				seg_buf[6] = 20;
				seg_buf[7] = 0;
			}
		}
		break;
		case 4:
		{
			seg_buf[0] = time_ds[2]/10;
			seg_buf[1] = time_ds[2]%10;
			seg_buf[2] = 26;
			seg_buf[3] = time_ds[1]/10;
			seg_buf[4] = time_ds[1]%10;
			seg_buf[5] = 26;
			seg_buf[6] = time_ds[0]/10;
			seg_buf[7] = time_ds[0]%10;
		}
		break;
		case 5:
		{
			seg_buf[0] = 23;
			seg_buf[1] = 21;
			seg_buf[2] = 20;
			if(pwm_max>=10000) 
			{
				seg_buf[3] = pwm_max/10000;
				seg_buf[4] = pwm_max%10000/1000;
				seg_buf[5] = pwm_max%1000/100;
				seg_buf[6] = pwm_max%100/10;
				seg_buf[7] = pwm_max%10;
			}
			else if((pwm_max<10000)&&(pwm_max>=1000))
			{
				seg_buf[3] = 20;
				seg_buf[4] = pwm_max%10000/1000;
				seg_buf[5] = pwm_max%1000/100;
				seg_buf[6] = pwm_max%100/10;
				seg_buf[7] = pwm_max%10;
			}
			else if((pwm_max<1000)&&(pwm_max>=100))
			{
				seg_buf[3] = 20;
				seg_buf[4] = 20;
				seg_buf[5] = pwm_max%1000/100;
				seg_buf[6] = pwm_max%100/10;
				seg_buf[7] = pwm_max%10;
			}
			else if((pwm_max<100)&&(pwm_max>=10))
			{
				seg_buf[3] = 20;
				seg_buf[4] = 20;
				seg_buf[5] = 20;
				seg_buf[6] = pwm_max%100/10;
				seg_buf[7] = pwm_max%10;
			}
		}
		break;
		
		case 6:
		{
			seg_buf[0] = 23;
			seg_buf[1] = 24;
			seg_buf[2] = time_save[2]/10;
			seg_buf[3] = time_save[2]%10;
			seg_buf[4] = time_save[1]/10;
			seg_buf[5] = time_save[1]%10;
			seg_buf[6] = time_save[0]/10;
			seg_buf[7] = time_save[0]%10;
		}
		break;
	}
}
//pwm
void Timer2() interrupt 1
{
//	pwm_count++;
//	if(pwm_count == pwm_duty){pwm_out = 0;}
//	else if(pwm_count == pwm_freq) {pwm_out = 1;pwm_count = 0;}
	
	if(pwm_in == 1)
	{
		pwm_h++;
		if(pwm_flag == 1)
		{
			pwm_l_save = pwm_l;
			pwm_l = 0;
			pwm_flag = 0;
		}
	}
	else if(pwm_in == 0)
	{
		pwm_l++;
		if(pwm_flag == 0)
		{
			pwm_h_save = pwm_h;
			pwm_h = 0;
			pwm_flag = 1;
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
	EA  =1;
	ET0 = 1;
}

//超声波
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
	TL1 = 0x00;TH1 = 0x00;
}

void send_wave()
{
	uint i;
	for(i = 0;i<8;i++)
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


//数码管
void Timer0() interrupt 12
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
	IE2|=0X04;
}



//按键
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
			if((key_r()&0x0f)!=0x0f)key1 = key_r()&0x0f;
			key_w(0xf0);
			if((key_r()&0xf0)!=0xf0)key2 = key_r()&0xf0;
			key =key1|key2;
			key|=0x30;
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

void key_w(uchar val)
{
	uchar value = val;
	P30 = (value&0x01);P31 = (value&0x02)>>1;P32 = (value&0x04)>>2;P33 = (value&0x08)>>3;
	P44 = (value>>7);P42 = (value&0x40)>>6;
}
unsigned char key_r()
{
	uchar val = 0xff,VAL_P3 = 0,VAL_P44 = 0,VAL_P42 = 0;
	VAL_P3 = P3;VAL_P44 = P44;VAL_P42 = P42;
	val = ((VAL_P3&0x0f)|(VAL_P44<<7)|(VAL_P42<<6));
	return val;
}

void all_init()
{
	P0 = 0XFF;
	P2|=0X80;P2&=0X9F;
	P2&=0X1F; // 流水灯
	
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