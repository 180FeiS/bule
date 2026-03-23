#include <main.h>
#include <iic.h>

void tast_clock()
{
	if(++key_tt == 10) {key_tt = 0;key_scan();}
	if(++show_tt == 10) {show_tt = 0;show_ref = 1;}
	if(++ultra_tt == 500) {ultra_tt = 0;ultra_ref = 1;}
	if(++AD_tt == 100) {AD_tt = 0;AD_ref = 1;}
	if(++led_tt == 100) {led_tt = 0;led_ref = 1;}
}
void main()
{
	all_init();
	Timer0_Init();delay_ms(1);
	Timer1_Init();		

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
		if((ultra_ref == 1)&&(up_para_real>AD_val)&&(down_para_real<AD_val))
		{
			ultra_ref = 0;
			ultra();
		}
		if(AD_ref == 1)
		{
			uint i = 0;
			AD_ref = 0;
			AD_val = AD_r(0x03);
			AD_val = AD_val*1.963;
			if((up_para_real>AD_val)&&(down_para_real<AD_val)){ultra_flag = 0;}
			else {ultra_flag = 1;AD_w(0);}
			if(ultra_flag == 0)
			{
				if(lcm>=80){AD_w(255);}
				else if(lcm<=20){AD_w(51);}
				else 
				{
					i = (uint)(lcm*0.0667-0.333)*51;
					AD_w(i); 
				}
			}
		}
		if(led_ref == 1)
		{
			led_ref = 0;
			tast_led();
		}
	}
}
//LED---------------------------------------------
void tast_led()
{
	if(showstate == 1){led_on(1);led_off(2);led_off(3);}
	else	if(showstate == 2){led_on(2);led_off(1);led_off(3);}
	else 	if(showstate == 3){led_on(3);led_off(2);led_off(1);}
	if(ultra_flag == 0)
	{
		if(led8_flag == 1){led8_flag = 0;led_on(8);}
		else {led8_flag = 1;led_off(8);}
	}
	else 
	{
		led_off(8);
	}

}
//---------------------------------------------

//按键任务----------------------------------------
void tast_key()
{
	if(key_val == 4)
	{
		key_val = 0;
		if(showstate == 1) {showstate = 2;}
		else if(showstate == 2) {showstate = 3;para_flag = 0;}
		else if(showstate == 3) {showstate = 1;up_para_real = up_para;down_para_real = down_para;}
	}
	if(key_val == 5)
	{
		key_val = 0;
		if(para_flag == 0) {para_flag = 1;}
		else if(para_flag == 1) {para_flag = 0;}
	}
	if((key_val == 6)&&(showstate == 3)) // 加
	{
		key_val = 0;
		if(para_flag == 0)
		{
			if(up_para>=500) {up_para = 50;}
			else {up_para+=50;}
		}
		else 
		{
			if(down_para>=500) {down_para = 50;}
			else {down_para+=50;}
		}
	}
	if((key_val == 7)&&(showstate == 3))// 减
	{
		key_val = 0;
		if(para_flag == 0)
		{
			if(up_para<=50) {up_para = 500;}
			else {up_para-=50;}
		}
		else 
		{
			if(down_para<=50) {down_para = 500;}
			else {down_para-=50;}
		}
	}
}
//-------------------------------------------------


//显示任务-----------------------------------------
void tast_show()
{
	switch(showstate)
	{
		case 1: // 电压
		{
			seg_buf[0] = 21;
			seg_buf[1] = 20;
			seg_buf[2] = 20;
			seg_buf[3] = 20;
			seg_buf[4] = 20;
			seg_buf[5] = AD_val/100+10;
			seg_buf[6] = AD_val%100/10;
			seg_buf[7] = AD_val%10;
		}
		break;
		case 3: // 参数
		{
			seg_buf[0] = 22;
			seg_buf[1] = 20;
			seg_buf[2] = 20;
			seg_buf[3] = up_para/100+10;
			seg_buf[4] = up_para%100/10;
			seg_buf[5] = 20;
			seg_buf[6] = down_para/100+10;
			seg_buf[7] = down_para%100/10;
		}
		break;
		case 2: // 测距
		{
			seg_buf[0] = 23;
			seg_buf[1] = 20;
			seg_buf[2] = 20;
			seg_buf[3] = 20;
			seg_buf[4] = 20;
			if(ultra_flag == 1){seg_buf[5] = 24;seg_buf[6] = 24;seg_buf[7] = 24;}
			else 
			{
				if(lcm>=100) 							 {seg_buf[5] = lcm/100;seg_buf[6] = lcm%100/10;seg_buf[7] = lcm%10;}
				else if((lcm<100)&&(lcm>=10)){seg_buf[5] = 20;seg_buf[6] = lcm%100/10;seg_buf[7] = lcm%10;}
			  else if((lcm<10)&&(lcm>=0)){seg_buf[5] = 20;seg_buf[6] = 20;seg_buf[7] = lcm%10;}
			}
		}
		break;
	}
}
//---------------------------------------------------

//超声波---------------------------------------------

void ultra()
{
	send_wave();
	TR1 = 1;
	while((RX == 1)&&(TF1 == 0));
	TR1 = 0;
	if(TF1 == 1)
	{           
		TF1 = 0;
		lcm = 999;
	}
	else if(RX == 0)
	{
		RX = 1;
		lcm_time = TH1<<8;
		lcm_time|= TL1;
		lcm = (uint)(lcm_time*0.017);
	}
	TL1 = 0x00;TH1 = 0x00;
}
void Timer1_Init(void)		//1微秒@12.000MHz
{
	AUXR &= 0xBF;			//定时器时钟12T模式
	TMOD &= 0x0F;			//设置定时器模式
	TL1 = 0x00;				//设置定时初始值
	TH1 = 0x00;				//设置定时初始值
	TF1 = 0;				//清除TF1标志
	//TR1 = 1;
				//定时器1开始计时
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
			else if(P31 == 0) key_val = 6;
			else if(P32 == 0) key_val = 5;
			else if(P33 == 0) key_val = 4;
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

void all_init()
{
	P0 = 0XFF;
	P2|=0X80;P2&=0X9F;
	P2&=0X1F; //流水灯
	
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