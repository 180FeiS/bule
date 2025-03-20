#include <main.H>
#include <onewire.H>
#include <ds1302.H>
void clock_tast()
{
	if(++key_tt == 10) {key_tt = 0;key_scan();}
	if(++show_tt == 20) {show_tt = 0;show_ref = 1;}
	if(++temp_tt == 450) {temp_tt = 0;temp_ref = 1;}
	if(++time_tt == 500) {time_tt = 0;time_ref = 1;}
	if(++led_tt%200 == 0) {led_ref = 1;}
	if(++pwm_tt == 500) {pwm_tt = 0;pwm_ref = 1;}
}
void main()
{
	all_init();
	delay_ms(1);
	temp_get();delay_ms(850);
	temp_val = temp_get()*625;
	Timer0_Init();
	 Timer1_Init();
	time_init();time_get();
	while(1)
	{
		if(show_ref == 1)
		{
			show_ref = 0;
			show_tast();
		}
		if(key_flag == 1)
		{
			key_flag = 0;
			key_tast();
		}
		if(temp_ref == 1)
		{
			temp_ref = 0;
			temp_val = temp_get()*625;
		}
		if(time_ref == 1)
		{
			time_ref = 0;
			time_get();
		}
		if(((time2[0]>=time3[0])&&(time2[0]<=time3[0]+5))&&(time2[1]==time3[1])&&(time2[2]==time3[2]))
		{
			if(led_ref == 1)
			{
				led_ref = 0;
				if(led_flag == 1) {led_flag = 0;ledx_on(1);}
				else if(led_flag == 0) {led_flag = 1;ledx_off(1);}
			}
			if(led_tt >=5001) {ledx_off(1);}
		}
		
		if(pwm_ref == 1)
		{
			pwm_ref = 0;
			buty = (zheng_now*100)/(zheng_now+fan_now);
			pwm_time = zheng_now+fan_now;
			freq = 100000/pwm_time;
			
			seg_bit[0] = buty/10;
			seg_bit[1] = buty%10;
			seg_bit[2] = 20;
			seg_bit[3] = 20;
			seg_bit[4] = freq/1000;
			seg_bit[5] = freq%1000/100;
			seg_bit[6] = freq%100/10;
			seg_bit[7] = freq%10;
		}
	}
}

void time5 () interrupt 3
{
	if(P34==1)
	{
		zheng++;
		if(buty_flag==1)
		{
			buty_flag=0;
			fan_now=fan;
			fan=0;
		}
	}
	else if(P34==0)
	{
		fan++;
		if(buty_flag==0)
		{
			buty_flag=1;
			zheng_now=zheng;
			zheng=0;
		}
	}
}


void show_tast()
{
	switch(show_state)
	{
		case 1:
		{
			seg_bit[0] = time2[2]/10;
			seg_bit[1] = time2[2]%10;
			seg_bit[2] = 21;
			seg_bit[3] = time2[1]/10;
			seg_bit[4] = time2[1]%10;
			seg_bit[5] = 21;
			seg_bit[6] = time2[0]/10;
			seg_bit[7] = time2[0]%10;
			if((time2[0]%2 == 0)&&flash_bit)
				{
					seg_bit[flash_pos_bit [flash_pos]/10] = 20;
					seg_bit[flash_pos_bit [flash_pos]%10] = 20;
				}
			else if((time2[0]%2 == 1)&&flash_bit)
				{
					seg_bit[flash_pos_bit [flash_pos]/10] = time2[flash_pos]/10;
					seg_bit[flash_pos_bit [flash_pos]%10] = time2[flash_pos]%10;
				}
		}
		break;
		case 2:
		{
			seg_bit[0] = 20;
			seg_bit[1] = 20;
			seg_bit[2] = 20;
			seg_bit[3] = 20;
			seg_bit[4] = 20;
			seg_bit[5] = temp_val/100000;
			seg_bit[6] = temp_val%100000/10000;
			seg_bit[7] = 22;
		}
		break;
		case 3:
		{
			seg_bit[0] = time3[2]/10;
			seg_bit[1] = time3[2]%10;
			seg_bit[2] = 21;
			seg_bit[3] = time3[1]/10;
			seg_bit[4] = time3[1]%10;
			seg_bit[5] = 21;
			seg_bit[6] = time3[0]/10;
			seg_bit[7] = time3[0]%10;
			if((time2[0]%2 == 0)&&flash_bit)
				{
					seg_bit[flash_pos_bit [flash_pos]/10] = 20;
					seg_bit[flash_pos_bit [flash_pos]%10] = 20;
				}
			else if((time2[0]%2 == 1)&&flash_bit)
				{
					seg_bit[flash_pos_bit [flash_pos]/10] = time3[flash_pos]/10;
					seg_bit[flash_pos_bit [flash_pos]%10] = time3[flash_pos]%10;
				}
		}
		break;
	}
}

//flash_pos 2-时 1-分 0-秒  flash_bit 1-设置 0-显示
void key_tast()
{
	if(key_val == 7)
	{
		key_val = 0;
		if(flash_pos == 2) 
		{
			flash_pos = 1;
			if(flash_bit == 0) {flash_bit = 1;flash_pos = 2;}
		} 
		else if (flash_pos == 1) {flash_pos = 0;}
		else if (flash_pos == 0) 
		{
			flash_pos = 2;
			if(flash_bit == 1){flash_bit = 0;}
			else flash_bit = 1;
		}
	}
	if(key_val == 6)
	{
		key_val = 0;
		flash_pos = 0;
		flash_bit = 0;
		if(show_state == 1) {show_state = 3;}
		else if (show_state == 3) {show_state = 1;}
	}
	if(key_val == 5) // 增加
	{
		key_val = 0;
		if((show_state == 1)&&(flash_bit == 1)) // 时钟
		{
			time2[flash_pos]++;
			if(time2[0] > 59) time2[0] = 0;
			if(time2[1] > 59) time2[1] = 0;
			if(time2[2] > 23) time2[2] = 0;
			time_init();
		}
		else if((show_state == 3)&&(flash_bit == 1)) // 闹钟
		{
			time3[flash_pos]++;
			if(time3[0] > 59) time3[0] = 0;
			if(time3[1] > 59) time3[1] = 0;
			if(time3[2] > 23) time3[2] = 0;
			
		}
	}
	if(key_val == 4) // 减少
	{
		key_val = 0;
		if((show_state == 1)&&(flash_bit == 1)) // 时钟
		{
			time2[flash_pos]--;
			if(time2[0] < 0) time2[0] = 59;
			if(time2[1] < 0) time2[1] = 59;
			if(time2[2] < 0) time2[2] = 23;
			time_init();
		}
		else if((show_state == 3)&&(flash_bit == 1)) // 闹钟
		{
			time3[flash_pos]--;
			if(time3[0] < 0) time3[0] = 59;
			if(time3[1] < 0) time3[1] = 59;
			if(time3[2] < 0) time3[2] = 23;
		}
	}
	
}

void key_scan()
{
	switch(keystate)
	{
		case key_check:
		{
			if((P30 == 0)||(P31 == 0)||(P32 == 0)||(P33 == 0)) keystate = key_press;
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
				if((show_state == 1)&&(key_val == 4)&&(flash_bit == 0))
				{
					show_state = 2;
				}
			}
			else 
			{
				keystate = key_check;
				key_flag = 1;
				if((show_state == 2)&&(key_val == 4)&&(flash_bit == 0))
				{
					show_state = 1;
				}
			}
		}
		break;
	}
}

void Timer1_Init(void)		//10微秒@11.0592MHz
{
	AUXR |= 0x40;			//定时器时钟1T模式
	TMOD &= 0x0F;			//设置定时器模式
	TL1 = 0x91;				//设置定时初始值
	TH1 = 0xFF;				//设置定时初始值
	TF1 = 0;				//清除TF1标志
	TR1 = 1;				//定时器1开始计时
	EA = 1;
	ET1 = 1;
}


void time0 () interrupt 1
{
	P0 = 0X00;
	P2|=0XC0;P2&=0XDF;
	P2&=0X1F;
	P0 = seg_num[seg_bit[num]];
	P2|=0XE0;
	P2&=0X1F;
	P0 = _crol_(0x01,num);
	P2|=0XC0;P2&=0XDF;
	P2&=0X1F;
	if(++num == 8) num=0;
	
	clock_tast();
}

void Timer0_Init(void)		//1毫秒@11.0592MHz
{
	AUXR |= 0x80;			//定时器时钟1T模式
	TMOD &= 0xF0;			//设置定时器模式
	TL0 = 0xCD;				//设置定时初始值
	TH0 = 0xD4;				//设置定时初始值
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
	P2&=0X1F; //段选端
	P0 = 0X00;
	P2|=0XC0;P2&=0XDF;
	P2&=0X1F; //位
	P0 = 0X00;
	P2|=0XA0;P2&=0XBF;
	P2&=0X1F; // 蜂鸣器继电器
}

void delay_ms(uint ms)
{
	uint i,j;
	for(i=0;i<ms;i++)
		for(j=0;j<853;j++);
}