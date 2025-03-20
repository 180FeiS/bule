#ifndef __main_H__
#define __main_H__
#include <stc15.h>
#include <intrins.h>

#define uint unsigned int
#define uchar unsigned char 
	
uchar led_bit = 0xff;
uchar act_bit = 0x00;
#define ledx_on(n) {led_bit&=_crol_(0xfe,n-1);P0 = led_bit;P2|=0X80;P2&=0X9F;P2&=0X1F;}
#define ledx_off(n) {led_bit|=_crol_(0x01,n-1);P0 = led_bit;P2|=0X80;P2&=0X9F;P2&=0X1F;}
#define buzzer_on {act_bit|=0x40;P0 = act_bit;P2|=0XA0;P2&=0XBF;P2&=0X1F;}
#define buzzer_off {act_bit&=0xbf;P0 = act_bit;P2|=0XA0;P2&=0XBF;P2&=0X1F;}
#define relay_on {act_bit|=0x10;P0 = act_bit;P2|=0XA0;P2&=0XBF;P2&=0X1F;}
#define relay_off {act_bit&=0xef;P0 = act_bit;P2|=0XA0;P2&=0XBF;P2&=0X1F;}

typedef enum
{
	key_check,
	key_press,
	key_release,
}keystate;
keystate key_state = key_check;
uint key_time = 20;
uchar key_value = 0;
uchar key_num = 0;
bit key_flag=0;
uchar key_tt=0;

code uint seg_num[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,
										 0x40,0x79,0x24,0x30,0x19,0x12,0x02,0x78,0x00,0x10,
										 0xff,0xbf,// - 21
										};
uint seg_buf[]={20,20,20,20,20,20,20,20};
uchar num=0;

uchar show_state=1;
uint show_tt=0;
bit show_ref=0;

uint temp_tt=0;
bit temp_ref=0;
unsigned long temp_value=0;
uchar temp_true=0;
uchar temp_section = 1;
uchar temp_max = 30;
uchar temp_min = 20;
uint temp_compare_tt=0;
bit temp_compare_ref = 0;
uint temp_maxmin[4] = {20,20,20,20};
uchar temp_num = 0;

uint led_tt=0;
bit led_ref=0;
bit led_flash=0;
uint led_time =400;

void delay_ms(uint ms);
void arrkey_scan();
void all_init();
void key_w(uchar value);
uchar key_r();
void Timer0_Init(void);
void tast_clock();
void tast_key();
void tast_show();
void temp_compare();
#endif