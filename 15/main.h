#ifndef __main_H__
#define __main_H__

#include <stc15.h>
#include <intrins.h>

#define uint unsigned int 
#define uchar unsigned char 
	
//LED buzzer relay
uchar led_bit = 0xff;
uchar act_bit = 0x00;
#define led_on(n) {led_bit&=_crol_(0xfe,n-1);P0 = led_bit;P2|=0X80;P2&=0X9F;P2&=0X1F;}
#define led_off(n) {led_bit|=_crol_(0x01,n-1);P0 = led_bit;P2|=0X80;P2&=0X9F;P2&=0X1F;}

//按键
typedef enum
{
	key_check,
	key_press,
	key_release
}key_state;
key_state keystate = key_check;
uchar key_tt = 0;
uint key_time = 20;
uchar key_val = 0;
bit key_flag = 0;

//数码管
code uint seg_num[] = {0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,
											 0x40,0x79,0x24,0x30,0x19,0x12,0x02,0x78,0x00,0x10,
											 0xff,
	0x8E,//21 F
	0x8C,//22 P
	0x89,//23 H
	0x88,//24 A
	0xC7,//25 L
	0xBF,//26 -
};
uchar seg_buf[] = {20,20,20,20,20,20,20,20};
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

//PWM输出
sbit pwm_out = P3^4;
uint pwm_tt = 0;
bit pwm_ref = 0;
uchar pwm_duty = 0;
uint pwm_freq = 0;
uchar pwm_duty_buf = 50;
uint pwm_freq_buf = 1000;
uint pwm_count = 0;
//PWM捕获
sbit pwm_in = P3^4;
uint pwm_h = 0;
uint pwm_l = 0;
uint pwm_h_save = 0;
uint pwm_l_save = 0;
bit pwm_flag = 0;
int pwm_in_freq = 0;
uint pwm_in_duty = 0;

uint pwm_in_duty_last = 0;
uint pwm_para = 2000;
int pwm_adjust = 0;
uint pwm_max = 0;
//AD
uint AD_tt = 0;
bit AD_ref = 0;
uint AD_val = 0;
float k = 0;

//时间
uint time_tt = 0;
bit time_ref = 0;
extern uint time_ds[3];
uint time_save[3];

//led
uint led_tt = 0;
bit led_ref = 0;
bit led1_flag = 0;
bit led2_flag = 0;

void delay_ms(uint ms);
void key_scan();
void all_init();
void key_w(uchar val);
unsigned char key_r();
void Timer0_Init(void);

void tast_clock();
void tast_show();
void tast_key();
void tast_led();

void ultra();
void send_wave();
void Delay15us(void);
void Timer1_Init(void);
void Timer2_Init(void);
#endif