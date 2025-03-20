#include <main.h>
#include <onewire.h>

void tast_clock()
{
	if(++key_tt == 20) {key_tt=0;key_scan();}
	if(++show_tt == 20) {show_tt=0;show_ref=1;}
	if(++temp_tt == 450) {temp_tt = 0;temp_ref= 1;}
	if(++work_time_tt == 1000) {work_time_tt = 0;work_ref =1;}
	if(++led_tt == 200) {led_tt = 0;led_ref = 1;}
}

void main()
{
	all_init();
	temp_get();delay_ms(850);
	temp_val= temp_get()*625;
	Timer0_Init();
	Timer1_Init();
	while(1)
	{
		if(work_mode == 1){pwm_duty = 2;}
		else if(work_mode == 2){pwm_duty = 3;}
		else if(work_mode == 3){pwm_duty = 7;}
		
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
		if(temp_ref == 1)
		{
			temp_ref=0;
			temp_val= temp_get()*625;
	  }
		if((work_ref ==1)&&(work_time_remain>0))
		{
			work_ref = 0;
			work_time_remain--;
		}
		
		if(led_ref == 1)
		{
			led_ref = 0;
			if(work_time_remain != 0)
			{
				if(work_mode == 1){ledx_on(1);ledx_off(2);ledx_off(3);}
				else if(work_mode == 2){ledx_on(2);ledx_off(1);ledx_off(3);}
				else if(work_mode == 3){ledx_on(3);ledx_off(2);ledx_off(1);}
			}
			else 
			{
				ledx_off(1);ledx_off(2);ledx_off(3);
			}
		}
	}
}

void tast_key()
{
	if(key_val == 7)
	{
		key_val=0;
		if(show_state ==1){show_state = 2;}
		else {show_state = 1;}
	}
	if(key_val == 6)
	{
		key_val=0;
		work_time_remain = 0;
		work_time = 0;
	}
	if(key_val == 5)
	{
		key_val=0;
		if(work_time == 0){work_time = 1;}
		else if(work_time == 1){work_time = 2;}
		else if(work_time == 2){work_time = 0;}
		work_time_remain = work_time*60;
	}
	if(key_val == 4)
	{
		key_val=0;
		if(work_mode == 1){work_mode = 2;}
		else if(work_mode == 2){work_mode = 3;}
		else if(work_mode == 3){work_mode = 1;}
		
	}
}

void tast_show()
{
	switch(show_state)
	{
		case 1:
		{
			seg_bit[0] = 21;
			seg_bit[1] = work_mode;
			seg_bit[2] = 21;
			seg_bit[3] = 20;
			seg_bit[4] = work_time_remain/1000;
			seg_bit[5] = work_time_remain%1000/100;
			seg_bit[6] = work_time_remain%100/10;
			seg_bit[7] = work_time_remain%10;
		}
		break;
		case 2:
		{
			seg_bit[0] = 21;
			seg_bit[1] = 4;
			seg_bit[2] = 21;
			seg_bit[3] = 20;
			seg_bit[4] = 20;
			seg_bit[5] = temp_val/100000;
			seg_bit[6] = temp_val%100000/10000;
			seg_bit[7] = 22;
		}
		break;
	}
}

void time1 () interrupt 3
{
	if(work_time_remain != 0)
	{
		if(++pwm_tt == pwm_duty) 
		{
			P34 = 0;
		}
		else if(pwm_tt == 10)
		{
			P34 = 1;
			pwm_tt = 0;
		}
	}
	else 
		{
			P34 = 0;
		}
	
}

void time0 () interrupt 1
{
	P0 = 0X00;
	P2|=0XC0;P2&=0XDF;
	P2&=0X1F;
	
	P0 = seg_num[seg_bit[num]];
	P2|= 0XE0;
	P2&=0X1F;
	
	P0 = _crol_(0x01,num);
	P2|=0XC0;P2&=0XDF;
	P2&=0X1F;
	
	if(++num == 8) num=0;
	
	tast_clock();
}

void key_scan()
{
	switch(key_state)
	{
		case key_check:
		{
			if((P30 == 0)||(P31 == 0)||(P32 == 0)||(P33 == 0))key_state = key_press;
		}
		break;
		case key_press:
		{
			if((P30 == 0)) key_val = 7;
			if((P31 == 0)) key_val = 6;
			if((P32 == 0)) key_val = 5;
			if((P33 == 0)) key_val = 4;
			key_state = key_release;
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
				key_state = key_check;
				key_flag=1;
			}
		}
		break;	
	}
}

void all_init()
{
	P0 = 0XFF;
	P2|=0X80;P2&=0X9F;
	P2&=0X1F; // 流水灯
	
	P0 = 0XFF;
	P2|= 0XE0;
	P2&=0X1F; // 段选端
	
	P0 = 0X00;
	P2|=0XC0;P2&=0XDF;
	P2&=0X1F; // 位选端
	
	P0 = 0X00;
	P2|=0XA0;P2&=0XBF;
	P2&=0X1F; // 蜂鸣器继电器
	
}

void Timer1_Init(void)		//100微秒@11.0592MHz
{
	AUXR |= 0x40;			//定时器时钟1T模式
	TMOD &= 0x0F;			//设置定时器模式
	TL1 = 0xAE;				//设置定时初始值
	TH1 = 0xFB;				//设置定时初始值
	TF1 = 0;				//清除TF1标志
	TR1 = 1;				//定时器1开始计时
	EA=1;
	ET1=1;
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


void delay_ms(uint ms)
{
	uint i,j;
	for(i=0;i<ms;i++)
		for(j=0;j<853;j++);
}