#include <main.h>
#include <onewire.h>
#include <ds1302.h>

void tast_clock()
{
	if(++key_tt == 10) {key_tt = 0;key_scan();}
	if(++show_tt == 10) {show_tt = 0;show_ref = 1;}
	//if(++ultra_tt == 500) {ultra_tt = 0;ultra_ref = 1;}
	if(++pwm_out_tt == 500) {pwm_out_tt = 0;pwm_out_ref = 1;}
	if(++pwm_in_tt == 500) {pwm_in_tt = 0;pwm_in_ref = 1;}
	if(++temp_tt == 500) {temp_tt = 0;temp_ref = 1;}
	if(++time_tt == 900) {time_tt = 0;time_ref = 1;}
	if(++control_tt == 100) {control_tt = 0;control_ref = 1;}
	if(++led_tt == 100) {led_tt = 0;led_ref = 1;}

}
void main()
{
	all_init ();
	
	temp_get();delay_ms(1000);
	temp_val = temp_get()*625;
  Timer0_Init();
	Timer1_Init();
  Timer2_Init();
	time_init();time_get();

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
		
		if(ultra_ref == 1)
		{
			ultra_ref = 0;
			ultra();
		}
		
		if(pwm_out_ref == 1)
		{
			pwm_out_ref = 0;
			pwm_freq = 100000/pwm_freq_change-1;
			pwm_duty = 100000/pwm_freq_change*pwm_duty_change/100;
		}
		
		if(pwm_in_ref == 1)
		{
			pwm_in_ref = 0;
			pwm_in_duty = (pwm_h_save*100)/(pwm_l_save+pwm_h_save);
			pwm_in_freq = 100000/(pwm_l_save+pwm_h_save);
		}
		
		if(temp_ref == 1)
		{
			temp_ref=0;
			temp_val = temp_get()*625;
			control_temp = temp_val/10000;
		}
		
		if(time_ref == 1)
		{
			time_ref = 0;
			time_get();
		}
		
		if(control_ref == 1)
		{
			control_ref = 0;
			tast_control();
		}
		if(led_ref == 1)
		{
			led_ref = 0;
			tast_led();
		}
	}
}
void tast_key()
{
	if(key_val == 12) // 1 温度 2 时间 3 参数
	{
		key_val = 0;
		if(showstate == 1){showstate = 2;}
		else if(showstate == 2){showstate = 3;}
		else if(showstate == 3){showstate = 1;}
	}
	if(key_val == 13)
	{
		key_val = 0;
		if(control_state == 1){control_state = 2;relay_off;}
		else if(control_state == 2){control_state = 1;relay_off;}
	}
	if((key_val == 16)&&(showstate == 3))
	{
		key_val = 0;
		
		if(temp_para == 99) {temp_para = 99;}
		else {temp_para++;}
	}
	if(key_val == 17) // 1 分秒  0 时 分
	{
		key_val = 0;
		if((showstate == 3)&&(temp_para>10)){temp_para--;}
		else 	if((showstate == 3)&&(temp_para==10)){temp_para=10;}
	}
}

void tast_show()
{
	switch(showstate)
	{
		case 1:
		{
			seg_buf[0] = 21;
			seg_buf[1] = 1;
			seg_buf[2] = 20;
			seg_buf[3] = 20;
			seg_buf[4] = 20;
			seg_buf[5] = temp_val/100000;
			seg_buf[6] = temp_val%100000/10000+10;
			seg_buf[7] = temp_val%10000/1000;
		}
		break;
		case 2:
		{
			seg_buf[0] = 21;
			seg_buf[1] = 2;
			seg_buf[2] = 20;
			if(key17_flag == 0){seg_buf[3] = time_real[2]/10;
			seg_buf[4] = time_real[2]%10;seg_buf[6] = time_real[1]/10;
			seg_buf[7] = time_real[1]%10;}
			else if(key17_flag == 1){seg_buf[3] = time_real[1]/10;
			seg_buf[4] = time_real[1]%10;seg_buf[6] = time_real[0]/10;
			seg_buf[7] = time_real[0]%10;}
			seg_buf[5] = 22;
		}
		break;
		case 3:
		{
			seg_buf[0] = 21;
			seg_buf[1] = 3;
			seg_buf[2] = 20;
			seg_buf[3] = 20;
			seg_buf[4] = 20;
			seg_buf[5] = 20;
			seg_buf[6] = temp_para/10;
			seg_buf[7] = temp_para%10;
		}
		break;
	}
}
void tast_led()
{
	if((time_real[0] == 0)&&(time_real[1] == 0))
	{
		led_on(1);
		control_time2 = 1;
	}
	else if((control_time2 == 1)&&((time_real[0]-0) >= 5))
	{
		led_off(1);
		control_time2 = 0;
	}
	
	if(control_state == 1){led_on(2);}
	else if(control_state == 2){led_off(2);}
	
	if((control_time2 == 1)||(control_time == 1)||(control_temp2 == 1))
	{
		if(led_flash == 1){led_off(3);led_flash = 0;}
		else 	if(led_flash == 0){led_on(3);led_flash = 1;}

	}
	else
	{
		led_off(3);
	}
}

// 控制
void tast_control()
{
	if(control_state == 1)
	{
		if(control_temp>temp_para){relay_on;control_temp2 = 1;}
		else {relay_off;control_temp2 = 0;}
	}
	else 
	{
		if((time_real[0] == 0)&&(time_real[1] == 0))
		{
			relay_on;
			control_time = 1;
			
		}
		else if((control_time == 1)&&((time_real[0]-0) >= 5))
		{
			relay_off;
			control_time = 0;
		}
	}
}


//pwm输出和捕获
void Timer2() interrupt 12
{
//	pwm_tt++;
//	if(pwm_tt == pwm_duty) {pwm_out = 0;}
//	else if(pwm_tt == pwm_freq) {pwm_out = 1;pwm_tt = 0;}
	if(pwm_in == 1)
	{
		pwm_h++;
		if(pwm_in_flag == 1)
		{
			pwm_l_save = pwm_l;
			pwm_in_flag = 0;
			pwm_l = 0;
		}
	}
	else if(pwm_in == 0)
	{
		pwm_l++;
		if(pwm_in_flag == 0)
		{
			pwm_h_save = pwm_h;
			pwm_in_flag = 1;
			pwm_h = 0;
		}
	}
	
}

void Timer2_Init(void)		//10微秒@12.000MHz
{
	AUXR |= 0x04;			//定时器时钟1T模式
	T2L = 0x88;				//设置定时初始值
	T2H = 0xFF;				//设置定时初始值
	AUXR |= 0x10;			//定时器2开始计时
	EA = 1;
	IE2|=0X04;
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
		lcm_time|= TL1;
		lcm = (uint)(lcm_time*0.017);
	}
		TL1 = 0x00;		TH1 = 0x00;	
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

void send_wave()
{
	uint i;
	for(i=0;i<8;i++)
	{
		TX = 1;Delay15us();
		TX = 0;Delay15us();
	}
}


void Delay15us(void)	//@12.000MHz
{
	unsigned char data i;

	_nop_();
	_nop_();
	i = 42;
	while (--i);
}


void Timer0() interrupt 1
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


//按键
void key_scan()
{
	uchar key,key1,key2;
	switch(keystate)
	{
		case key_check:
		{
			key_w(0x0f);
			if((key_r()&0x0f)!=0x0f)  keystate = key_press;
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
				key_w(0x0f);
				if((key_r()&0x0f)!=0x0f) key1 = key_r()&0x0f;
				key_w(0xf0);
				if((key_r()&0xf0)!=0xf0) key2 = key_r()&0xf0;
				key = key1|key2;
				if(key==0xeb)
				{
					key17_flag = 1;
				}
			}
			else 
			{
				key17_flag = 0;
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
	P3 = val&0x3f;P44 = val>>7;P42 = (val&0x40)>>6;
}
unsigned char key_r()
{
	uchar val = 0xff ,val_P3 = 0,val_P44 = 0,val_P42 = 0;
	val_P3 = P3;val_P44 = P44;val_P42 = P42;
	val = ((val_P3&0x3f)|(val_P44<<7)|(val_P42<<6));
	return val;
}

// 初始化
void all_init ()
{
	P0 = 0XFF;
	P2|=0X80;P2&=0X9F;
	P2&=0X1F;// 流水灯
	
	P0 = 0XFF;
	P2|=0XE0;
	P2&=0X1F; //段选
	
	P0 = 0X00;
	P2|=0XC0;P2&=0XDF;
	P2&=0X1F; // 位
	
	P0 = 0X00;
	P2|=0XA0;P2&=0XBF;
	P2&=0X1F; // 蜂鸣器继电器
}

// 延时
void delay_ms(uint ms)
{
	uint i,j;
	for(i=0;i<ms;i++)
		for(j=0;j<853;j++);
}