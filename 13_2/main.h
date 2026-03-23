#ifndef __main_H__
#define __main_H__

#include <stc15.h>
#include <intrins.h>

#define uint unsigned int 
#define uchar unsigned char
	
//灯 蜂鸣器继电器-----------------
uchar led_bit = 0xff;
uchar act_bit = 0x00;
#define led_on(n) {led_bit&=_crol_(0xfe,n-1);P0 = led_bit;P2|=0X80;P2&=0X9F;P2&=0X1F; }
#define led_off(n) {led_bit|=_crol_(0x01,n-1);P0 = led_bit;P2|=0X80;P2&=0X9F;P2&=0X1F; }
#define buzzer_on {act_bit|=0x40;P0 = act_bit;P2|=0XA0;P2&=0XBF;P2&=0X1F;}
#define buzzer_off {act_bit&=0xbf;P0 = act_bit;P2|=0XA0;P2&=0XBF;P2&=0X1F;}
#define relay_on {act_bit|=0x10;P0 = act_bit;P2|=0XA0;P2&=0XBF;P2&=0X1F;}
#define relay_off {act_bit&=0xef;P0 = act_bit;P2|=0XA0;P2&=0XBF;P2&=0X1F;}

//按键-------------------------------
typedef enum 
{
	key_check,
	key_press,
	key_release
}key_state;
key_state keystate = key_check;
uint key_time = 20;
uchar key_tt = 0;
bit key_flag = 0;
uchar key_val = 0;

//数码管
code uint seg_num[] = {0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,
											 0x40,0x79,0x24,0x30,0x19,0x12,0x02,0x78,0x00,0x10,
											 0xff,0xc1,0x8c,0xc7,0x88};
uint seg_buf[] = {20,20,20,20,20,20,20,20};
uchar num = 0;

//显示
uchar showstate = 1;
uchar show_tt = 0;
bit show_ref = 0;

//超声波
sbit TX = P1^0;
sbit RX = P1^1;
uint ultra_tt = 0;
bit ultra_ref = 0;
uint lcm = 0;
uint lcm_time = 0;
bit ultra_flag = 0;

//AD
uint AD_tt = 0;
bit AD_ref = 0;
uint AD_val = 0;

//参数
uint up_para = 450;
uint down_para = 50;
uint up_para_real = 450;
uint down_para_real = 50;
bit para_flag = 0;//0:上 1:下

//LED
uchar led_tt = 0;
bit led_ref = 0;
bit led8_flag = 0;


void delay_ms(uint ms);
void all_init();
void key_scan();
void Timer0_Init(void);
void Timer1_Init(void);		
void tast_clock();
void tast_key();
void tast_show();

void send_wave();
void ultra();
void Delay15us(void);

void tast_led();

#endif