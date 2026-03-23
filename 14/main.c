#include <main.h>
#include <onewire.h>
#include <iic.h>
#include <ds1302.h>

void tast_clock()
{
	if(++key_tt == 10) {key_tt = 0;key_scan();}
	if(++show_tt == 10) {show_tt = 0;show_ref = 1;}
	//if(++ultra_tt == 500) {ultra_tt = 0;ultra_ref = 1;}
	if(++pwm_tt == 100) {pwm_tt = 0;pwm_ref = 1;}
	if(++temp_tt == 500) {temp_tt = 0;temp_ref = 1;}
	if(++AD_tt == 400) {AD_tt = 0;AD_ref = 1;}
	if(++time_tt == 800) {time_tt = 0;time_ref = 1;}
	if(++led_tt == 100) {led_tt = 0;led_ref = 1;}
	if(collect_flag == 1)
	{
		if(++collect_tt == 3000) {collect_tt = 0;collect_flag = 0;showstate = show_save;}
	}
}

void main()
{
	all_init ();
	temp_get();delay_ms(1000);
	temp_val = temp_get()*625;
	Timer0_Init();
	
	Timer2_Init();
	time_init();time_get();
	//AT_W(0x00,0);
	open = AT_R(0x00);
	AT_W(0x00,++open);
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
		if(ultra_ref)
		{
			ultra_ref = 0;
			ultra();
		}
		
		if(pwm_ref)
		{
			pwm_ref = 0;
//			pwm_freq = 100000/pwm_freq_tt-1;
//			pwm_duty = 100000/pwm_freq_tt*pwm_duty_tt/100;
			pwm_in_duty = pwm_h_save*100/(pwm_h_save+pwm_l_save);
			pwm_in_freq = 100000/(pwm_h_save+pwm_l_save);
//			if((pwm_in_freq<=2000)&&(pwm_in_freq>=200)) {humidity_para = 1;humidity = 0.044*pwm_in_freq-0.889;}//有效
//			else {humidity_para = 0;} // 无效
		}
		
		if(temp_ref)
		{
			temp_ref = 0;
			temp_val = temp_get()*625;
			if((temp_val/10000)>=99) {temp_val = 990000;}
			else if((temp_val/10000)<=0) {temp_val = 0;}
		}
		
		if(AD_ref)
		{
			uint i;
			AD_ref = 0;
			AD_val = AD_R(0x01)*1.96;
			if((AD_val<250)&&(AD_last>=250)&&(showstate!=6))
			{			if((pwm_in_freq<=2000)&&(pwm_in_freq>=200)) {humidity_para = 1;humidity = 0.044*pwm_in_freq-0.889;}//有效
			else {humidity_para = 0;} // 无效
				collect_flag = 1;
				show_save = showstate;
				showstate = 6;
				if(humidity_para == 1)
				{
					touch_time++;
					collect_temp[touch_time] = temp_val/10000;
					collect_hum[touch_time] = humidity;
					time_collect[0] = time_ds[1];
					time_collect[1] = time_ds[2];
					if(collect_temp[touch_time]>temp_max){temp_max = collect_temp[touch_time];}
					if(collect_hum[touch_time]>humidity_max){humidity_max = collect_hum[touch_time];}
					for(i=0;i<=touch_time;i++){humidity_save+=collect_hum[i];temp_save+=collect_temp[i];}
					humidity_ave = humidity_save*10/i;humidity_save = 0;
					temp_ave = temp_save*10/i;temp_save = 0;
				}
			}
			AD_last = AD_val;
		}
		
		if(time_ref)
		{
			time_ref = 1;
			time_get();
		}
		
		if(led_ref)
		{
			led_ref = 0;
			tast_led();
		}
	}
}
//LED----------------------------
void tast_led()
{
	if(showstate == 1){led_on(1);led_off(2);led_off(3);}
	else if((showstate == 2)||(showstate == 3)||(showstate == 4)) {led_on(2);led_off(1);led_off(3);}
	else if(showstate == 6){led_on(3);led_off(2);led_off(1);}
	
	if((collect_temp[touch_time]>temp_para)&&(touch_time>=0))
	{
		if(led4_flag == 0){led4_flag = 1;led_on(4);}
		else if(led4_flag == 1){led4_flag = 0;led_off(4);}
	}
	
	if(humidity_para == 0)
	{
		led_on(5);
		if(collect_tt>1)
		{
			if((collect_temp[touch_time]>collect_temp[touch_time-1])&&(collect_hum[touch_time]>collect_hum[touch_time-1]))
			{
				led_on(6);
			}
			else 
			{
				led_off(6);
			}
		}
		else
		{
			led_off(6);
		}
	}
	else 
	{
		led_off(5);
	}
	
	
}

//按键----------------------------
void tast_key()
{
	uint j;
	if(showstate!=6)
	{
		if(key_val == 4)
		{
			key_val = 0;
			if(showstate == 1){showstate = 2;}
			else if((showstate == 2)||(showstate == 3)||(showstate == 4)){showstate = 5;}
			else if(showstate == 5) {showstate = 1;}
		}
		if((key_val == 5)&&(showstate!=1)&&(showstate!=5))
		{
			key_val = 0;
			if(showstate == 2) {showstate = 3;}
			else if(showstate == 3) {showstate = 4;}
			else if(showstate == 4) {showstate = 2;}
			
		}
		if((key_val == 8)&&(showstate == 5))
		{
			key_val = 0;
			if(temp_para >=99) {temp_para = 99;}
			else {temp_para++;}
		}
		if(key_val == 9)
		{
			if(showstate == 5)
			{
				key_val = 0;
				if(temp_para <=0) {temp_para = 0;}
				else {temp_para--;}
			}
			else if((showstate == 4)&&(key9_flag))
			{
				key9_flag = 0;
				
				temp_max = 0;
				temp_ave = 0;
				humidity_max = 0;
				humidity_ave = 0;
				for(j=0;j<=touch_time;j++)
				{
					collect_temp[j] = 0; collect_hum[j] = 0;
				}
				touch_time = -1;
			}
		}
	}

}

//显示----------------------------
void tast_show()
{
	switch(showstate)
	{
		case 1: // 时间
		{
			seg_buf[0] = time_ds[2]/10;
			seg_buf[1] = time_ds[2]%10;
			seg_buf[2] = 21;
			seg_buf[3] = time_ds[1]/10;
			seg_buf[4] = time_ds[1]%10;
			seg_buf[5] = 21;
			seg_buf[6] = time_ds[0]/10;
			seg_buf[7] = time_ds[0]%10;
		}
		break;
		case 2: // 回显--温度
		{
			seg_buf[0] = 22;
			seg_buf[1] = 20;
			if((touch_time+1) == 0)
			{
				seg_buf[2] = 20;seg_buf[3] = 20;seg_buf[4] = 20;seg_buf[5] = 20;seg_buf[6] = 20;seg_buf[7] = 20;
			}
			else 
			{
				seg_buf[2] = temp_max/10;
				seg_buf[3] = temp_max%10;
				seg_buf[4] = 21;
				seg_buf[5] = temp_ave/100;
				seg_buf[6] = temp_ave%100/10+10;
				seg_buf[7] = temp_ave%10;
			}
		}
		break;
		case 3://回显--湿度
		{
			seg_buf[0] = 23;seg_buf[1] = 20;
			if((touch_time+1) == 0)
			{
				seg_buf[2] = 20;seg_buf[3] = 20;seg_buf[4] = 20;seg_buf[5] = 20;seg_buf[6] = 20;seg_buf[7] = 20;
			}
			else 
			{
				seg_buf[2] = humidity_max/10;
				seg_buf[3] = humidity_max%10;
				seg_buf[4] = 21;
				seg_buf[5] = humidity_ave/100;
				seg_buf[6] = humidity_ave%100/10+10;
				seg_buf[7] = humidity_ave%10;
			}
		}
		break;
		case 4://回显--时间
		{
			seg_buf[0] = 24;
			seg_buf[1] = (touch_time+1)/10;
			seg_buf[2] = (touch_time+1)%10;
			if((touch_time+1) == 0)
			{
				seg_buf[3] = 20;seg_buf[4] = 20;seg_buf[5] = 20;seg_buf[6] = 20;seg_buf[7] = 20;
			}
			else 
			{
				seg_buf[3] = time_collect[1]/10;
				seg_buf[4] = time_collect[1]%10;
				seg_buf[5] = 21;
				seg_buf[6] = time_collect[0]/10;
				seg_buf[7] = time_collect[0]%10;
			}
		}
		break;
		case 5: // 参数
		{
			seg_buf[0] = 25;
			seg_buf[1] = 20;
			seg_buf[2] = 20;
			seg_buf[3] = 20;
			seg_buf[4] = 20;
			seg_buf[5] = 20;
			seg_buf[6] = temp_para/10;
			seg_buf[7] = temp_para%10;
		}
		break;
		case 6: // 温湿度界面
		{
			seg_buf[0] = 26;
			seg_buf[1] = 20;
			seg_buf[2] = 20;
			seg_buf[5] = 21;
			
			if(humidity_para == 1)
			{seg_buf[3] = collect_temp[touch_time]/10;
			 seg_buf[4] = collect_temp[touch_time]%10;
				seg_buf[6] = collect_hum[touch_time]/10;
				seg_buf[7] = collect_hum[touch_time]%10;
	
			}
			else
			{seg_buf[3] = temp_val/100000;
				seg_buf[4] = temp_val%100000/10000;
				seg_buf[6] = 27;
				seg_buf[7] = 27;
			}
		}
		break;
	}
}

//PWM输出捕获
void Timer2() interrupt 12
{
//	pwm_count++;
//	if(pwm_count == pwm_duty){pwm_out = 0;}
//	else if(pwm_count == pwm_freq){pwm_out = 1;pwm_count = 0;}
	
	if(pwm_in == 1)
	{
		pwm_in_h++;
		if(pwm_flag == 1)
		{
			pwm_l_save = pwm_in_l;
			pwm_in_l = 0;
			pwm_flag = 0;
		}
	}
	if(pwm_in == 0)
	{
		pwm_in_l++;
		if(pwm_flag == 0)
		{
			pwm_h_save = pwm_in_h;
			pwm_in_h = 0;
			pwm_flag = 1;
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
		lcm_time|=TL1;
		lcm = (uint)(lcm_time*0.017);
	}
	TL1 = 0x00;TH1 = 0x00;	
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

//数码管
void Timer0 () interrupt 1
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
			if((key_r()&0x0f)!=0x0f) keystate = key_press;
		}
		break;
		case key_press:
		{
			key_w(0x0f);
			if((key_r()&0x0f)!=0x0f) key1 = key_r()&0x0f;
			key_w(0xf0);
			if((key_r()&0xf0)!=0xf0) key2 = key_r()&0xf0;
			key = key1|key2;key|=0x30;
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
				if(key_val == 9)
				{
					key_time+=10;
				}
			}
			else 
			{
				if(key_time>=2000)
				{
					key_time = 20;
					key9_flag = 1;
				}
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
	P30 = value&0X01;P31 = (value&0X02)>>1;P32 = (value&0X04)>>2;P33 = (value&0X08)>>3;
	P44 = value>>7;P42 = (value&0x40)>>6;
}
unsigned char key_r()
{
	uchar val = 0xff,VAL_P3 = 0,VAL_P44 = 0,VAL_P42 = 0;
	VAL_P3 = P3;VAL_P44 = P44;VAL_P42 = P42;
	val = (VAL_P3&0x0f)|(VAL_P44<<7)|(VAL_P42<<6);
	return val;
}

void all_init ()
{
	P0 = 0XFF;
	P2|=0X80;P2&=0X9F;
	P2&=0X1F; // 流水灯
	
	P0 = 0XFF;
	P2|=0XE0;
	P2&=0X1F; // 段选
	
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
