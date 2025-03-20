#ifndef __main_H__
#define __main_H__

#include <stc15.h>
#include <intrins.h>

#define uint unsigned int
#define uchar unsigned char
	
uchar led_bit = 0xff;

#define ledx_on(n) {led_bit &= _crol_(0xfe,n-1);P0 = led_bit;P2|=0X80;P2&=0X9F;P2&=0X1F;}
#define ledx_off(n) {led_bit |= _crol_(0x01,n-1);P0 = led_bit;P2|=0X80;P2&=0X9F;P2&=0X1F;}

code uint seg_num[] = {0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,
											 0x40,0x79,0x24,0x30,0x19,0x12,0x02,0x78,0x00,0x10,
											 0xff,0xbf // - 21
	};
uint seg_buf[] = {20,20,20,20,20,20,20,20};
uchar seg_count = 0;

typedef enum
{
	key_check,
	key_press,
	key_release
}key_state;
key_state keystate = key_check;
uchar key_val = 0;
uint key_time = 20;
bit key_flag = 0;
uchar key_tt=0;

uchar show_state = 1;
uchar show_tt=0;
bit show_ref = 0;

uint temp_tt=0;
bit temp_ref=0;
unsigned long temp_val=0;

extern unsigned int time1[3];
extern unsigned int time2[3];
uint time_tt=0;
uint time_ref=0;

uchar scan_time=1;

uint collect_store[10];
uchar collect_time = 0;
uint collect_tt = 0;
uint collect_ref = 0;
uchar collect_count = 0;

uchar indexes = 0;
uint indexes_tt=0;

uint led_tt=0;
bit led_ref=0;
bit led_flag = 0;
bit led = 0;


void delay_ms(uint ms);
void all_init();
void key_scan();
void Timer0_Init(void);
void tast_clock();
void tast_key();
void tast_show();

#endif