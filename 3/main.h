#ifndef __MAIN_H__
#define __MAIN_H__

#include <stc15.h>
#include <intrins.h>

#define uint unsigned int
#define uchar unsigned char
	
uchar led_bit = 0xff;
uchar act_bit = 0x00;

#define ledx_on(n) {led_bit&=_crol_(0xfe,n-1);P0 = led_bit;P2|=0X80;P2&=0X9F;P2&=0X1F;}
#define ledx_off(n) {led_bit|=_crol_(0x01,n-1);P0 = led_bit;P2|=0X80;P2&=0X9F;P2&=0X1F;}
#define buzzer_on {act_bit|=0x40;P0 = act_bit;P2|=0XA0;P2&=0XBF;P2&=0X1F; }
#define buzzer_off {act_bit&=0xbf;P0 = act_bit;P2|=0XA0;P2&=0XBF;P2&=0X1F; }
#define relay_on {act_bit|=0x10;P0 = act_bit;P2|=0XA0;P2&=0XBF;P2&=0X1F; }
#define relay_off {act_bit&=0xef;P0 = act_bit;P2|=0XA0;P2&=0XBF;P2&=0X1F; }

code uint seg_num[] = {0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,
											 0x40,0x79,0x24,0x30,0x19,0x12,0x02,0x78,0x00,0x10,
											 0xff}; 
uint seg_buf[] = {20,20,20,20,20,20,20,20};
uchar num=0;

typedef enum
{
	key_check,
	key_press,
	key_release,
}keystate;
keystate key_state = key_check;
uchar key_value=0;
uint key_time=20;
bit key_flag=0;
uint key_tt=0;


uchar show_state=1;
uint show_tt=0;
bit show_ref = 0;

uint AD_tt=0;
bit AD_ref=0;
unsigned long AD_value=0;
uint open=0;

uint temp_tt=0;
bit temp_ref=0;
unsigned long temp_value=0;

extern unsigned int time1[3];
extern unsigned int time2[3];
uint time_tt=0;
bit time_ref=0;

uint water_rate = 50;
uint water_flow = 0;
uint water_price = 0;
uint water_time=0;
bit water_flow_flag=0;

void delay_ms(uint ms);
void all_init();
void arrkey_scan();
void Timer0_Init(void);
unsigned char key_read();
void key_write(uchar value);
void tast_seg();
void tast_show();
void tast_key();
void key_scan();

#endif