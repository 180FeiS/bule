#include <main.h>
#include <onewire.h>
void tast_clock()
{
	if(++key_tt == 20) {key_tt = 0;arrkey_scan();}
	if(++show_tt ==20) {show_tt = 0;show_ref=1;}
	if(++temp_tt == 400) {temp_tt = 0;temp_ref = 1;}
	if(++temp_compare_tt == 10) {temp_compare_tt = 0;temp_compare_ref = 1;}
	if(++led_tt == led_time) {led_tt = 0; led_ref = 1;}
}
void main()
{
	all_init();
	temp_get();delay_ms(850);
	temp_value = temp_get()*625;
	Timer0_Init();
	while(1)
	{
		delay_ms(1);
		if(key_flag == 1)
		{
			tast_key();
			key_flag = 0;
		}
		if(show_ref ==1)
		{
			show_ref = 0;
			tast_show();
		}
		if(temp_ref == 1)
		{
			temp_ref  = 0;
			temp_value = 200000;
			temp_value = temp_get()*625;
		}
		if(temp_compare_ref == 1)
		{
			temp_compare_ref = 0;
			temp_compare();
		}
		if(led_ref == 1)
		{
			led_ref = 0;
			if(led_flash == 0){led_flash = 1;ledx_on(1);}
			else if(led_flash == 1){led_flash = 0;ledx_off(1);}
		}
	}
}

void tast_key()
{
	if(key_value == 13)
	{
		key_value = 0;
		if(show_state == 1)
		{
			temp_maxmin[0] = 20;temp_maxmin[1] = 20;
			temp_maxmin[2] = 20;temp_maxmin[3] = 20;
			show_state  = 2;
		}
		else 
		{
			if(((temp_maxmin[0]*10+temp_maxmin[1])>=(temp_maxmin[2]*10+temp_maxmin[3]))&&(temp_maxmin[3]!=20))
			{
				temp_max = temp_maxmin[0]*10+temp_maxmin[1];
				temp_min = temp_maxmin[2]*10+temp_maxmin[3];
				ledx_off(2);
			}
			temp_num = 0;
			show_state = 1;
		}
	}
	else if(key_value == 14)
	{
		key_value = 0;
		temp_num =0;
		temp_maxmin[0] = 20;temp_maxmin[1] = 20;
		temp_maxmin[2] = 20;temp_maxmin[3] = 20;
	}
	else if((temp_num<4)&&(show_state == 2))
		{
			switch (key_num)
			{
				case 2:temp_maxmin[temp_num] = 0;break;
				case 5:temp_maxmin[temp_num] = 3;break;
				case 8:temp_maxmin[temp_num] = 6;break;
				case 11:temp_maxmin[temp_num] = 9;break;
				case 3:temp_maxmin[temp_num] = 1;break;
				case 6:temp_maxmin[temp_num] = 4;break;
				case 9:temp_maxmin[temp_num] = 7;break;
				case 4:temp_maxmin[temp_num] = 2;break;
				case 7:temp_maxmin[temp_num] = 5;break;
				case 10:temp_maxmin[temp_num] = 8;break;		
				default :break;
			}
			if(key_num != 0)
			{
				temp_num++;
			}
			key_num = 0;
		}
		if((temp_num==4)&&(show_state == 2))
		{
			if(((temp_maxmin[0]*10+temp_maxmin[1])>=(temp_maxmin[2]*10+temp_maxmin[3]))&&(temp_maxmin[3]!=20))
			{
				ledx_off(2);
			}
			if(((temp_maxmin[0]*10+temp_maxmin[1])<(temp_maxmin[2]*10+temp_maxmin[3]))&&(temp_maxmin[3]!=20))
			{
				ledx_on(2);
			}
		}
	}

void tast_show()
{
	switch(show_state)
	{
		case 2:
		{
			seg_buf[0] = 21;
			seg_buf[1] = temp_maxmin[0];
			seg_buf[2] = temp_maxmin[1];
			seg_buf[3] = 20;
			seg_buf[4] = 20;
			seg_buf[5] = 21;
			seg_buf[6] = temp_maxmin[2];
			seg_buf[7] = temp_maxmin[3];
		}
		break;
		case 1:
		{
			seg_buf[0] = 21;
			seg_buf[1] = temp_section;
			seg_buf[2] = 21;
			seg_buf[3] = 20;
			seg_buf[4] = 20;
			seg_buf[5] = 20;
			seg_buf[6] = temp_value/100000;seg_buf[7] = temp_value%100000/10000;
		}
		break;
	}
}

void temp_compare()
{
	temp_true = (temp_value/100000)*10+temp_value%100000/10000;
	if((temp_true<temp_min)&&(temp_section != 0))
	{
		led_time = 800;
		led_tt =  0;
		temp_section = 0;
		relay_off;
	}
	else if(((temp_true>=temp_min)&&(temp_true<=temp_max))&&(temp_section != 1))
	{
		led_time = 400;
		led_tt = 0;
		temp_section = 1;
		relay_off;
	}
	else if((temp_true>temp_max)&&(temp_section != 2))
	{
		led_time = 200;
		led_tt = 0;
		relay_on;
		temp_section = 2;
	}
}

void Time0() interrupt 1
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


void arrkey_scan()
{
	uchar key,key1,key2;
	switch(key_state)
	{
		case key_check:
		{
			key_w(0x0f);
			if((key_r()&0x0f)!=0x0f) key_state = key_press;
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
					case 0x7e:key_num = 2;break;
					case 0x7d:key_num = 5;break;
					case 0x7b:key_num = 8;break;
					case 0x77:key_num = 11;break;
					case 0xbe:key_num = 3;break;
					case 0xbd:key_num = 6;break;
					case 0xbb:key_num = 9;break;
					case 0xb7:key_value = 13;break; // 设置
					case 0xde:key_num = 4;break;
					case 0xdd:key_num = 7;break;
					case 0xdb:key_num = 10;break;
					case 0xd7:key_value = 14;break; // 清除
					default :break;
				}
				key_state = key_release;
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
				key_state = key_check;
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

uchar key_r()
{
	uchar val = 0xff,val_p3 = 0,val_p44 = 0,val_p42 = 0;
	val_p3 = P3;val_p44 = P44;val_p42 = P42;
	val = ((val_p3&0x3f) | (val_p44<<7) | (val_p42<<6));
	return val;
}

void all_init()
{
	P0 = 0XFF;
	P2|=0X80;P2&=0X9F;
	P2&=0X1F; //LED灯
	P0 = 0XFF;
	P2|=0XE0;
	P2&=0X1F; //段选端
	P0 = 0X00;
	P2|=0XC0;P2&=0XDF;
	P2&=0X1F; //位选端
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
