#ifndef __main_H__
#define __main_H__

#include <stc15.h>
#include <intrins.h>

#define uint unsigned int 
#define uchar unsigned char 
	
uchar led_bit = 0xff;
#define ledx_on(n) {led_bit&=_crol_(0xfe,n-1);P0 = led_bit;P2|=0X80;P2&=0X9F;P2&=0X1F;}
#define ledx_off(n) {led_bit|=_crol_(0x01,n-1);P0 = led_bit;P2|=0X80;P2&=0X9F;P2&=0X1F;}

	
code uint seg_num[] = {0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,
											 0x40,0x79,0x24,0x30,0x19,0x12,0x02,0x78,0x00,0x10,
											 0xff,0xbf,0xc6};
uint seg_bit[] = {20,20,20,20,20,20,20,20};
uchar num=0;
typedef enum
{
	key_check,
	key_press,
	key_release
}keystate;
keystate key_state = key_check;
uint key_time = 20;
uchar key_tt=0;
bit key_flag=0;
uchar key_val = 0;

uchar show_state = 1;
uchar show_tt=0;
bit show_ref=0;

uint temp_tt=0;
bit temp_ref=0;
unsigned long temp_val=0;

uint pwm_tt=0;
uchar pwm_duty = 2;

uchar work_mode = 1;
uint work_time = 0;
uint work_time_tt = 0;
uint work_time_remain = 0;
bit work_ref=0;

uint led_tt=0;
bit led_ref =0;

void delay_ms(uint ms);
void all_init();
void key_scan();
void Timer0_Init();
void Timer1_Init(void);
void tast_clock();
void tast_key();
void tast_show();



#endif