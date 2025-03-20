#ifndef __MAIN_H__
#define __MAIN_H__

#include <stc15.h>
#include <intrins.h>

#define uint unsigned int 
#define uchar unsigned char 
	
uchar led_bit = 0xff;
#define ledx_on(n) {led_bit&=_crol_(0xfe,n-1);P0 = led_bit;P2|=0X80;P2&=0X9F;P2&=0X1F;}

sbit TX = P1^0;
sbit RX = P1^1;

code uint seg_num[] = {0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,
											 0x40,0x79,0x24,0x30,0x19,0x12,0x02,0x78,0x00,0x10,
											 0xff};
uint seg_buf[]  = {20,20,20,20,20,20,20,20};
uchar num=0;
typedef enum 
{
	key_check,
	key_press,
	key_release
}key_state;
key_state keystate = key_check;
uchar key_val=0;
uint key_tt=0;
uint key_time=20;
bit key_flag=0;

uchar show_state = 1;
uchar show_tt=0;
bit show_ref = 0;

uint lcm_tt=0;
bit lcm_ref=0;
uint lcm=0;
uint lcm_time=0;

void delay_ms(uint ms);
void all_init();
void key_scan();
void key_w(uchar value);
uchar key_r();
void Timer0_Init(void);
void tast_clock();
void tast_show();
void tast_key();

void send();
void Delay14us(void);
void ultra();
void Timer1_Init(void);

#endif