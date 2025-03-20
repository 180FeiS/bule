#include <main.h>
#include <iic.h>
#include <onewire.h>
#include <ds1302.h>
void tast_seg()
{
	if(++key_tt == 20)  {key_tt = 0;arrkey_scan();}
	if(++show_tt == 50) {show_tt = 0;show_ref = 1;}
	if(++AD_tt == 100)  {AD_tt=0;AD_ref = 1;}
	if(++temp_tt == 400) {temp_tt=0;temp_ref = 1;}
	if(++time_tt == 1) {time_tt=0;time_ref = 1;}
	if(water_flow_flag == 1)
	{
		if(++water_time ==100) 
			{
				water_time = 0;water_flow++;
				if(water_flow>=99) 
				{
					water_flow=0;
					relay_off;
					water_flow_flag = 0;
					show_state = 2;
				}
				if((water_flow%2) == 0)
				{
					water_price+=1;
				}
			}
	}
	
}
void main()
{
	all_init();
	temp_get();delay_ms(900);
	temp_value = temp_get()*625;
	Timer0_Init();
	time_init();
	
	//AT24C02_write(0x01,0);delay_ms(5);
	open = AT24C02_read(0x01);delay_ms(5);
	AT24C02_write(0x01,++open);delay_ms(5);
	
	
	while(1)
	{
		//delay_ms(1);
		if(show_ref == 1)
		{
			show_ref = 0;
			tast_show();
		}
		if(key_flag == 1)
		{
			key_flag = 0;
			tast_key();
		}
		if(AD_ref == 1)
		{
			AD_ref = 0;
			AD_value = AD_read(0x01)*1.963;
			if(AD_value<=125)ledx_on(1);
			if(AD_value>125) ledx_off(1);
		}
		if(temp_ref ==1)
		{
			temp_ref = 0;
			temp_value = temp_get()*625;
		}
		if(time_ref == 1)
		{
			time_ref = 0;
			time_get();
		}
	}
}

void tast_key()
{
	if((key_value == 7)&&(water_flow_flag == 0))
	{
		water_price = 0;
		key_value = 0;
		relay_on;
		water_flow_flag = 1;
		show_state = 1;
	}
	if(key_value == 6)
	{
		water_flow=0;
		key_value = 0;
		relay_off;
		water_flow_flag = 0;
		show_state = 2;
	}
}
void tast_show()
{
	switch(show_state)
	{
		case 1:
		{
			seg_buf[0] = 20;
			seg_buf[1] = water_rate/100+10;
			seg_buf[2] = water_rate%100/10;
			seg_buf[3] = water_rate%10;
			
			seg_buf[4] = water_flow/1000;
			seg_buf[5] = water_flow%1000/100+10;
			seg_buf[6] = water_flow%100/10;
			seg_buf[7] = water_flow%10;
		}
		break;
		case 2:
		{
			seg_buf[0] = 20;
			seg_buf[1] = water_rate/100+10;
			seg_buf[2] = water_rate%100/10;
			seg_buf[3] = water_rate%10;
			
			seg_buf[4] = water_price/1000;
			seg_buf[5] = water_price%1000/100+10;
			seg_buf[6] = water_price%100/10;
			seg_buf[7] = water_price%10;
		}
		break;
	}
}

void key_scan()
{
	switch(key_state)
	{
		case key_check:
		{
			if((P30 == 0) || (P31 == 0) || (P32 == 0) || (P33 == 0)) key_state = key_press;
		}
		break;
		case key_press:
		{
			
				if(P30 == 0) key_value = 7;
				if(P31 == 0) key_value = 6;
				if(P32 == 0) key_value = 5;
				if(P33 == 0) key_value = 4;
				key_state = key_release;
			
		}
		break;
		case key_release:
		{
			if((P30 == 0) || (P31 == 0) || (P32 == 0) || (P33 == 0))
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


void arrkey_scan()
{
	uchar key,key1,key2;
	switch(key_state)
	{
		case key_check:
		{
			key_write(0x0f);
			if((key_read()&0x0f)!=0x0f) key_state = key_press;
		}
		break;
		case key_press:
		{
			key_write(0x0f);
			if((key_read()&0x0f)!=0x0f) 
				key1 = key_read()&0x0f;
			key_write(0xf0);
			if((key_read()&0xf0)!=0xf0) 
				key2 = key_read()&0xf0;
			key = key1|key2;
			if(key!=0xff)
			{
				switch(key)
				{
					case 0x7e:key_value = 7;break;
					case 0x7d:key_value = 6;break;
					case 0x7b:key_value = 5;break;
					case 0x77:key_value = 4;break;
					case 0xbe:key_value = 11;break;
					case 0xbd:key_value = 10;break;
					case 0xbb:key_value = 9;break;
					case 0xb7:key_value = 8;break;
					case 0xde:key_value = 15;break;
					case 0xdd:key_value = 14;break;
					case 0xdb:key_value = 13;break;
					case 0xd7:key_value = 12;break;
					case 0xee:key_value = 19;break;
					case 0xed:key_value = 18;break;
					case 0xeb:key_value = 17;break;
					case 0xe7:key_value = 16;break;
				}
					key_state = key_release;
			}
		}
		break;
		case key_release:
		{
			key_write(0x0f);
			if((key_read()&0x0f)!=0x0f)
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

void key_write(uchar value)
{
	uchar val = value;
	P3 = val; P44 = val>>7; P42 = (val&0x40)>>6;
}
unsigned char key_read()
{
	uchar val = 0xff,val_p3 = 0,val_p44 = 0,val_p42 = 0;
	val_p3 = P3;val_p44 = P44;val_p42 = P42;
	val = ((val_p3&0X3F)|(val_p44<<7)|(val_p42<<6));
	return val;
}

void Time0() interrupt 1
{
	P0 = 0X00;
	P2|=0XC0;P2&=0XDF;
	P2&=0X1F; //位选端
	P0 = seg_num[seg_buf[num]];
	P2|=0XE0;
	P2&=0X1F; //段选端
	P0 =_crol_(0x01,num);
	P2|=0XC0;P2&=0XDF;
	P2&=0X1F; //位选端
	if(++num ==8) num=0;
	
	tast_seg();

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
	P2&=0X1F; //位选端
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
	TR0 = 1;			//定时器0开始计时
	EA = 1;
	ET0 = 1;
}


void delay_ms(uint ms)
{
	uint i,j;
	for(i=0;i<ms;i++)
		for(j=0;j<853;j++);
}