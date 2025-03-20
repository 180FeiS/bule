#ifndef __main_H__
#define __main_H__


#include <stc15.h>
#include <intrins.h>

#define uint unsigned int 
#define uchar unsigned char

uchar led_bit=0xff;
uchar act_bit = 0x00;
#define ledx_on(n) {led_bit&=_crol_(0xfe,n-1);P0 = led_bit;P2|= 0X80;P2&=0X9F;P2&=0X1F;}
#define ledx_off(n) {led_bit|=_crol_(0x01,n-1);P0 = led_bit;P2|= 0X80;P2&=0X9F;P2&=0X1F;}
#define buzzer_on {act_bit|=0x40;P0 = act_bit;P2|=0XA0;P2&=0XBF;P2&=0X1F;}
#define buzzer_off {act_bit&=0xbf;P0 = act_bit;P2|=0XA0;P2&=0XBF;P2&=0X1F;}
#define relay_on {act_bit|=0x10;P0 = act_bit;P2|=0XA0;P2&=0XBF;P2&=0X1F;}
#define relay_off {act_bit&=0xef;P0 = act_bit;P2|=0XA0;P2&=0XBF;P2&=0X1F;}

typedef enum 
{
	key_check,
	key_press,
	key_release
}keystate;
keystate key_state = key_check;
uchar key_value=0;
uint key_time=20;
bit key_flag=0;
uint key_tt = 0;

uchar num=0;
code uint seg_num[] = {0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,
											 0x40,0x79,0x24,0x30,0x19,0x12,0x02,0x78,0x00,0x10,
											 0xff,0xbf // - 21
											};
uint seg_buf[] = {20,20,20,20,20,20,20,20};

uchar show_state = 1;
uint show_tt=0;
bit show_ref = 0;

uint AD_tt=0;
bit AD_ref=0;
uint AD_value=0;

uint open=0;

uchar humidity_flag=50;
bit work_state=0;
bit work_ref=0;
uint work_tt=0;


extern unsigned int time1[3];
extern unsigned int time2[3];
uint time_tt=0;
bit time_ref=0;

bit buzzer_flag=0;

void Timer0_Init(void);
void delay_ms(uint ms);
void key_scan();
void all_init();
void tast_clock();
void tast_key();
void tast_seg();
void work();

#endif