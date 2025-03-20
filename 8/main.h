#ifndef __main_H__
#define __main_H__

#include <stc15.h>
#include <intrins.h>

#define uint unsigned int
#define uchar unsigned char 
	
uchar led_bit = 0xff;
#define ledx_on(n)   {led_bit&=_crol_(0xfe,n-1);P0 = led_bit;P2|=0X80;P2&=0X9F;P2&=0X1F;}
#define ledx_off(n)  {led_bit|=_crol_(0x01,n-1);P0 = led_bit;P2|=0X80;P2&=0X9F;P2&=0X1F;}

typedef enum
{
	key_check,
	key_press,
	key_release
}key_state;
key_state keystate = key_check;
uchar key_tt=0;
bit key_flag=0;
uint key_time=20;
uchar key_val = 0;

code uint seg_num[] = {0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,
											 0x40,0x79,0x24,0x30,0x19,0x12,0x02,0x78,0x00,0x10,
											 0xff,0xbf,0xc6
};
uint seg_bit[] ={20,20,20,20,20,20,20,20};
uint num = 0;

uchar show_state = 0;
uchar show_tt = 0;
bit show_ref = 0;

uint temp_tt=0;
bit temp_ref=0;
unsigned long temp_val = 0;

uint time_tt=0;
bit time_ref=0;

uchar flash_pos = 0;
uchar flash_pos_bit [3] = {67,34,1};
bit flash_bit = 0;

extern unsigned int time1[3];
extern signed int time2[3];
signed int time3[3] = {0,0,0};

uchar led_tt = 0;
bit led_ref = 0;
bit led_flag=0;
bit led_flag2 = 0;

uint zheng = 0,fan = 0,zheng_now = 0,fan_now = 0;
uint buty = 0;
bit buty_flag = 0;
uint pwm_time = 0;
uint freq = 0;

uint pwm_tt  = 0;
bit pwm_ref = 0;


void key_scan();
void delay_ms(uint ms);
void all_init();
void Timer0_Init(void);
void key_tast();
void show_tast();
void clock_tast();
void Timer1_Init(void);

#endif