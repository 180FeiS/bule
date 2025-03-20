#include <main.h>
#include <iic.h>
#include <ds1302.h>
void tast_clock()
{
	
	if(++key_tt == 20) {key_tt = 0;key_scan();}
	if(++show_tt == 100) {show_tt = 0;show_ref = 1;}
	if(++AD_tt == 20) {AD_tt = 0;AD_ref = 1;}
	if(++time_tt == 500) {time_tt = 0;time_ref = 1;}
	if(++work_tt == 200) {work_tt =0;work_ref = 1;}
}
void main()
{
	all_init();
	Timer0_Init();
	
	//AT24C02_w(0x01,0);delay_ms(5);
	open = AT24C02_r(0x01);delay_ms(5);
	AT24C02_w(0x01,++open);delay_ms(5);
	time_init();time_get();
	while(1)
	{
		delay_ms(1);
		if(show_ref == 1)
		{
			show_ref = 0;
			tast_seg();
			
		}
		if(key_flag == 1)
		{
			key_flag = 0;
			tast_key();
		}
		if(AD_ref == 1)
		{
			AD_ref = 0;
			AD_value = AD_r(0x03)*0.39;
		}
		if(time_ref == 1)
		{
			time_ref = 0;
			time_get();
		}
		if(work_ref == 1)
		{
			work_ref = 0;
			work();
		}
	}
}
	
void tast_seg()
{
	switch(show_state)
	{
		case 1:
		{
			seg_buf[0] = time2[2]/10;
			seg_buf[1] = time2[2]%10;
			seg_buf[2] = 21;
			seg_buf[3] = time2[1]/10;
			seg_buf[4] = time2[1]%10;
			seg_buf[5] = 20;
			seg_buf[6] = AD_value/10;
			seg_buf[7] = AD_value%10;
		}
		break;
		case 2:
		{
			seg_buf[0] = 21;
			seg_buf[1] = 21;
			seg_buf[2] = 20;
			seg_buf[3] = 20;
			seg_buf[4] = 20;
			seg_buf[5] = 20;
			seg_buf[6] = humidity_flag/10;
			seg_buf[7] = humidity_flag%10;
		}
		break;
	}
}

void work()
{
	if(work_state == 0)
	{
		ledx_on(1);ledx_off(2);
		if(AD_value>=humidity_flag)
		{
			relay_off;
		}
		else
		{
			relay_on;
		}
	}
	else 
	{
		ledx_on(2);ledx_off(1);
	}
	if(buzzer_flag == 0)
		{
			if(AD_value<humidity_flag) {ledx_on(8);}
			else 
			{
				ledx_off(8);
			}
				
		}
		else 
		{
			ledx_off(8);
		}
}

void tast_key()
{
	if(key_value == 7)
	{
		if(work_state == 0) work_state = 1;
		else if(work_state == 1) work_state = 0;
		key_value = 0;
	}
	if(work_state == 0) // 自动
	{
		if(key_value == 6)
		{
			if(show_state == 2){show_state = 1;AT24C02_w(0x02,humidity_flag);}
			else
          {
						show_state = 2;
					}
			key_value = 0;
		}
		if((key_value == 5)&&(show_state == 2))
		{
			humidity_flag++;
			key_value = 0;
		}
		if((key_value == 4)&&(show_state == 2))
		{
			humidity_flag--;
			key_value = 0;
		}
	}
	else //手动
	{
		if(buzzer_flag == 0)
		{
			if(AD_value<humidity_flag) {ledx_on(8);}
			else 
			{
				ledx_off(8);
			}
		}
		else 
		{
			ledx_off(8);
		}
		if(key_value == 6)
		{
			if(buzzer_flag == 0) {buzzer_flag = 1;}
			else 
			{
				buzzer_flag = 0;
			}
			key_value = 0;
		}
		if(key_value == 5)
		{
			relay_on;
			key_value = 0;
		}
		if(key_value == 4)
		{
			relay_off;
			key_value = 0;
		}
	}
}

void Time0 () interrupt 1
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

void Timer0_Init(void)		//1毫秒@11.0592MHz
{
	AUXR |= 0x80;			//定时器时钟1T模式
	TMOD &= 0xF0;			//设置定时器模式
	TL0 = 0xCD;				//设置定时初始值
	TH0 = 0xD4;				//设置定时初始值
	TF0 = 0;				//清除TF0标志
	TR0 = 1;	//定时器0开始计时
	EA = 1;
	ET0 = 1;
}


void all_init()
{
	P0 = 0XFF;
	P2|= 0X80;P2&=0X9F;
	P2&=0X1F; //流水灯
	P0 = 0XFF;
	P2|=0XE0; //段选端
	P2&=0X1F;
  P0 = 0X00;
	P2|=0XC0;P2&=0XDF;
	P2&=0X1F; //位选端
	P0 = 0X00;
	P2|=0XA0;P2&=0XBF;
	P2&=0X1F; //蜂鸣器继电器
}
void key_scan()
{
	switch(key_state)
	{
		case key_check:
		{
			if((P30 ==0) || (P31 ==0) ||(P32 ==0) ||(P33 ==0))
				key_state = key_press;
		}
		break;
		case key_press:
		{
			if(P30 ==0) key_value = 7;
			if(P31 ==0) key_value = 6;
			if(P32 ==0) key_value = 5;
			if(P33 ==0) key_value = 4;
			key_state = key_release;
		}
		break;
		case key_release:
		{
			if((P30 ==0) || (P31 ==0) ||(P32 ==0) ||(P33 ==0))
			{
				key_time+=10;
			}
			else 
			{
				key_state = key_check;
				key_flag = 1;
			}
		}
		break;
	}
}
void delay_ms(uint ms)
{
	uint i,j;
	for(i=0;i<ms;i++)
		for(j=0;j<853;j++);
}
