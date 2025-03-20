#include <main.h>
#include <onewire.h>
#include <ds1302.h>
void tast_clock()
{
	if(++key_tt == 20) {key_tt = 0;key_scan();}
	if(++show_tt == 20) {show_tt = 0;show_ref = 1;}
	if(++temp_tt == 900) {temp_tt = 0;temp_ref = 1;}
	if(++time_tt == 500) {time_tt = 0;time_ref = 1;}
	if(++led_tt == 500) {led_tt = 0;led_ref = 1;}
	if(show_state == 2)
	{
		if(++collect_tt == scan_time*1000) {collect_tt = 0;collect_ref = 1;}
	}
}


void main()
{
	all_init();
	temp_get();delay_ms(850);
	temp_val = temp_get()*625;
	Timer0_Init();
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
		if(collect_ref == 1)
		{
			collect_ref = 0;
			collect_store[collect_count] = (temp_val/100000)*10+temp_val%100000/10000;
			if(++collect_count == 10) {show_state = 3;led_flag = 1;}
		}
		if((led_ref == 1)&&(led_flag == 1))
		{
			led_ref = 0;
			if(led == 0) {led = 1;ledx_on(1);}
			else {led = 0;ledx_off(1);}
		}
	}
}

void tast_key()
{
	if(key_val == 7)
	{
		key_val = 0;
		if(show_state == 3) {show_state = 1;indexes = 0;}
	}
	else if(key_val == 6)
	{
		key_val = 0;
		ledx_off(1);led_flag = 0;
		if(show_state == 3)
		{
			if(++indexes == 10) {indexes = 0;}
		}
	}
	else if(key_val == 5)
	{
		key_val = 0;
		if(show_state  == 1){show_state = 2;collect_count = 0;}
	}
	else if(key_val == 4)
	{
		key_val = 0;
		if(show_state == 1)
		{
			if(scan_time == 1) {scan_time = 5;}
		else if(scan_time == 5) {scan_time = 30;}
		else if(scan_time == 30) {scan_time = 60;}
		else if(scan_time == 60) {scan_time = 1;}
		}
		
	}
}

void tast_show()
{
	switch(show_state)
	{
		case 1:
		{
			seg_buf[0] = 20;
			seg_buf[1] = 20;
			seg_buf[2] = 20;
			seg_buf[3] = 20;
			seg_buf[4] = 20;
			seg_buf[5] = 21;
			seg_buf[6] = scan_time/10;
			seg_buf[7] = scan_time%10;
		}
		break;
		case 2:
		{
			seg_buf[0] = time2[2]/10;
			seg_buf[1] = time2[2]%10;
			
			seg_buf[3] = time2[1]/10;
			seg_buf[4] = time2[1]%10;
			
			seg_buf[6] = time2[0]/10;
			seg_buf[7] = time2[0]%10;
			if(time2[0]%2 == 0) {seg_buf[2] = 21;seg_buf[5] = 21;}
			else if(time2[0]%2 == 1) {seg_buf[2] = 20;seg_buf[5] = 20;}
		}
		break;
		case 3:
		{
			seg_buf[0] = 21;
			seg_buf[1] = 0;
			seg_buf[2] = indexes%10;
			seg_buf[3] = 20;
			seg_buf[4] = 20;
			seg_buf[5] = 21;
			seg_buf[6] = collect_store[indexes]/10;
			seg_buf[7] = collect_store[indexes]%10;
		}
		break;
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

void time () interrupt 1
{
	P0 = 0X00;
	P2|=0XC0;P2&=0XDF;
	P2&=0X1F;
	P0 = seg_num[seg_buf[seg_count]];
	P2|=0XE0;
	P2&=0X1F;
	P0 = _crol_(0x01,seg_count);
	P2|=0XC0;P2&=0XDF;
	P2&=0X1F;
	if(++seg_count == 8) seg_count = 0;
	
	tast_clock();
}

void all_init()
{
	P0 = 0XFF;
	P2|=0X80;P2&=0X9F;
	P2&=0X1F; // 流水灯
	P0 = 0XFF;
	P2|=0XE0;
	P2&=0X1F; // 段
	P0 = 0X00;
	P2|=0XC0;P2&=0XDF;
	P2&=0X1F; // 位
	P0 = 0X00;
	P2|=0XA0;P2&=0XBF;
	P2&=0X1F; //蜂鸣器继电器
}

void Timer0_Init(void)		//1毫秒@11.0592MHz
{
	AUXR |= 0x80;			//定时器时钟1T模式
	TMOD &= 0xF0;			//设置定时器模式
	TL0 = 0xCD;				//设置定时初始值
	TH0 = 0xD4;				//设置定时初始值
	TF0 = 0;				//清除TF0标志
	TR0 = 1;				//定时器0开始计时
	EA =1;
	ET0 = 1;
}



void delay_ms(uint ms)
{
	uint i,j;
	for(i=0;i<ms;i++)
		for(j=0;j<853;j++);
}