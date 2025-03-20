#include <stc15.h>
#include <intrins.h>
#include <main.h>
#include <onewire.h>
#include <iic.h>

void Task_clock()
{
	if(++Key_tt==20)  {Key_tt=0;key_scan();}
	if(++show_tt==20) {show_tt=0;show_ref=1;}
	if(++temp_tt==100){temp_tt=0;temp_ref=1;}
	if(++AD_tt==500)  {AD_tt=0;AD_ref=1;}
  if(++LCM_tt==300){LCM_tt = 0 ; LCM_Ref = 1 ;}
}
void Task_key()
{
	switch(Key_Value)
	{
		case 7:
		{
			if(temp_choose == 1&&SEG_Show == 2)
			{
				temp_max--;
			}
			else if(temp_choose == 0&&SEG_Show == 2)
			{
				temp_min--;
			}
			if(temp_max <=0) {temp_max = 0;}
			if(temp_min <=0) {temp_min = 0;}
	  }
		break;
		case 6:
		{
			if(temp_choose == 1&&SEG_Show == 2)
				{
					temp_max++;
				}
				else if(temp_choose == 0&&SEG_Show == 2)
				{
					temp_min++;
				}
				if(temp_max >=99) {temp_max = 99;}
				if(temp_min >=99) {temp_min = 99;}
	  }
		break;
		case 5:
		{
			
			if(temp_choose == 0&&SEG_Show == 2)
			{
				temp_choose = 1;
			}
			else if(temp_choose == 1&&SEG_Show == 2)
			{
				temp_choose = 0;
			}
	  }
		break;
		case 4:
		{
			
			if(SEG_Show == 1)
			{
				temp_maxstore = temp_max;
		  	temp_minstore = temp_min;
				SEG_Show = 2;temp_choose = 0;
			}
			else
			{
				if(temp_max<temp_min)
				{
					SEG_Show = 1;
					temp_max = temp_maxstore;
					temp_min = temp_minstore;
					LEDx_ON(4);
				}
				else
				{
					SEG_Show = 1;
					LEDx_OFF(4);
				}
			}
	  }
		break;
	}
}
void main()
{
	All_init();
	Temp_get();delay_ms(900);
	temp_value = Temp_get()*625;
	Timer0_Init();
	Timer1_Init();
	while(1)
	{
		if(Key_Flag == 1)
		{
			Key_Flag = 0;
			Task_key();
		}
		if(show_ref==1)
		{
			show_ref=0;
			SEG_refresh();
		}
		if(temp_ref==1)
		{
			temp_ref=0;
			temp_value = Temp_get()*625;
			temp_test = temp_value/100000*10+temp_value%100000/10000;
			if(temp_test>temp_max) {AD_write(204);LEDx_ON(1);LEDx_OFF(2);LEDx_OFF(3);}
			else if((temp_test<=temp_max)&&(temp_value>=temp_min)) {AD_write(153);LEDx_ON(2);LEDx_OFF(1);LEDx_OFF(3);}
			else if(temp_test<temp_min){AD_write(102);LEDx_ON(3);LEDx_OFF(2);LEDx_OFF(1);}
		}
//		if(AD_ref==1)
//		{
//			if(temp_test>temp_max) {AD_write(204);LEDx_ON(1);LEDx_OFF(2);LEDx_OFF(3);}
//			else if((temp_test<=temp_max)&&(temp_value>=temp_min)) {AD_write(153);LEDx_ON(2);LEDx_OFF(1);LEDx_OFF(3);}
//			else if(temp_test<temp_min){AD_write(102);LEDx_ON(3);LEDx_OFF(2);LEDx_OFF(1);}
//		}
		if(LCM_Ref ==1)
		{
			LCM_Ref = 0;
			ultra_num();
		}
	}
}

void ultra_num()
{
	ultra_wave();
	TR1 = 1;
	while((RX == 1) && (TF1 == 0)) ;
	TR1 = 0;
	if(TF1 == 1)
	{
		TF1 = 0;
		LCM = 999;
	}
	else 
	{
		LCM_Time = TH1;
		LCM_Time<<=8;
		LCM_Time|=TL1;
		
		LCM = (uint)(LCM*0.017);
	}
	TL1 = 0x00;			
	TH1 = 0x00;	
	
	
}
void Timer1_Init(void)		//0微秒@12.000MHz
{
	AUXR &= 0xbf;			//定时器时钟12T模式
	TMOD &= 0x0F;			//设置定时器模式
	TL1 = 0x00;				//设置定时初始值
	TH1 = 0x00;				//设置定时初始值
	TF1 = 0;				//清除TF1标志
//	TR1 = 1;				//定时器1开始计时
}

void Delay14us()	//@12.000MHz
{
	unsigned char data i;

	_nop_();
	_nop_();
	i = 39;
	while (--i);
}

void ultra_wave()
{
	uchar i;
	for(i=0;i<8;i++)
	{
		TX = 1;Delay14us();
		TX = 0;Delay14us();
	}
}

void SEG_refresh()
{
	switch(SEG_Show)
	{
		case 0:
		{
			SEG_Buf[0] = 20;SEG_Buf[1] = 20;SEG_Buf[2] = 20;SEG_Buf[3] = 20;SEG_Buf[4] = 20;
			SEG_Buf[5] = LCM/100;
			SEG_Buf[6] = LCM%100/10;
			SEG_Buf[7] = LCM%10;
		}
		break;
		case 1:
		{
			SEG_Buf[0] = 21;SEG_Buf[1] = 20;SEG_Buf[2] = 20;
			SEG_Buf[3] = 20;SEG_Buf[4] = 20;SEG_Buf[5] = 20;
			SEG_Buf[6]=temp_value/100000;
			SEG_Buf[7]=temp_value%100000/10000;
			
		}
		break;
		case 2:
		{
			SEG_Buf[0] = 22;SEG_Buf[1] = 20;SEG_Buf[2] = 20;
			SEG_Buf[3] = temp_max/10;SEG_Buf[4] = temp_max%10;SEG_Buf[5] = 20;
			SEG_Buf[6]=temp_min/10;
			SEG_Buf[7]=temp_min%10;
		}
		break;
		case 3:
		{
			SEG_Buf[0]=21;SEG_Buf[1]=21;SEG_Buf[2]=21;SEG_Buf[3]=21;
			SEG_Buf[4]=21;SEG_Buf[5]=AD_value/100+10;SEG_Buf[6]=AD_value%100/10;SEG_Buf[7]=AD_value%10;
			
		}
		break;
		case 4:
		{
			SEG_Buf[3]=1;
			LEDx_ON(4);LEDx_OFF(1);LEDx_OFF(2);LEDx_OFF(3);Relay_OFF();
		}
		break;
	}
}
void Timer0() interrupt 1
{
	P0=0X00;
	P2|=0XC0;P2&=0XDF;
	P2&=0X1F; //位选端
	P0 =SEG_Num[SEG_Buf[num]];
	P2|=0XE0;
	P2&=0X1F; //段选端
	P0=_crol_(0X01,num);
	P2|=0XC0;P2&=0XDF;
	P2&=0X1F; //位选端
	if(++num==8) num=0;
	
	Task_clock();
}
/*
void Arrkey_scan()
{
	uchar key,key1,key2;
	switch (KeyState)
	{
		case Key_check:
		{
			key_w(0x0f);
			if((key_r()&0x0f)!=0x0f) KeyState = Key_press;
		}
		break;
		case Key_press:
		{
			key_w(0x0f);
			if((key_r()&0x0f)!=0x0f) key1 = key_r()&0x0f;
			key_w(0xf0);
			if((key_r()&0xf0)!=0xf0) key2 = key_r()&0xf0;
			key = key1|key2;
			if(key!=0xff)
			{
				switch (key)
				{
					case 0x7e:Key_Value = 7;break;
					case 0x7d:Key_Value = 6;break;
					case 0x7b:Key_Value = 5;break;
					case 0x77:Key_Value = 4;break;
					case 0xbe:Key_Value = 11;break;
					case 0xbd:Key_Value = 10;break;
					case 0xbb:Key_Value = 9;break;
					case 0xb7:Key_Value = 8;break;
					case 0xde:Key_Value = 15;break;
					case 0xdd:Key_Value = 14;break;
					case 0xdb:Key_Value = 13;break;
					case 0xd7:Key_Value = 12;break;
					case 0xee:Key_Value = 19;break;
					case 0xed:Key_Value = 18;break;
					case 0xeb:Key_Value = 17;break;
					case 0xe7:Key_Value = 16;break;
				}
				KeyState = Key_release;
			}
		}
		break;
		case Key_release:
		{
			key_w(0x0f);
			if((key_r()&0x0f)!=0x0f)
			{
				Key_Time+=10;
			}
			else 
			{
				KeyState = Key_check;
				Key_Flag=1;
			}
		}
	}
}

void key_w(uchar Value)
{
	uchar Val = Value;
	P3 = Val;P44 = Val>>7 ; P42 = (Val&0X40)>>6;
}
uchar key_r()
{
	uchar Val = 0xff,Val_P3=0,Val_P42=0,Val_P44=0;
	Val_P3 = P3&0x3f;Val_P42 = P42;Val_P44 = P44;
	Val = (Val_P3|(Val_P42<<6)|(Val_P44<<7));
	return Val;
}
*/

void key_scan()
{
	switch(KeyState)
	{
		case Key_check:
		{
			if((P30==0)||(P31==0)||(P32==0)||(P33==0)) KeyState = Key_press;
		}
		break;
		case Key_press:
		{
			if(P30==0) {Key_Value = 7;KeyState = Key_release;}
			if(P31==0) {Key_Value = 6;KeyState = Key_release;}
			if(P32==0) {Key_Value = 5;KeyState = Key_release;}
			if(P33==0) {Key_Value = 4;KeyState = Key_release;}
		}
		break;
		case Key_release:
		{
			if((P30==0)||(P31==0)||(P32==0)||(P33==0))
			{
				Key_Time+=10;
			}
			else 
			{
				KeyState = Key_check;
				Key_Flag = 1;
			}
		}
		break;
	}
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

void All_init()
{
	P0 = 0XFF;
	P2|=0X80;P2&=0X9F;
	P2&=0X1F; //流水灯
	P0 = 0XFF;
	P2|=0XE0;
	P2&=0X1F; //段选端
	P0=0X00;
	P2|=0XC0;P2&=0XDF;
	P2&=0X1F; //位选端
	P0=0X00;
	P2|=0XA0;P2&=0XBF;
	P2&=0X1F; //蜂鸣器·继电器
}
void delay_ms(uint ms)
{
	uint i,j;
	for(i=0;i<ms;i++)
		for(j=0;j<853;j++);
}
