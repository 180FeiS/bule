#ifndef __main_H__
#define __main_H__

#include <stc15.h>
#include <intrins.h>

#define uint unsigned int 
#define uchar unsigned char
	
uchar led_bit = 0xff;
uchar act_bit = 0x00;
#define led_on(n) {led_bit&=_crol_(0xfe,n-1);P0 = led_bit;P2|=0X80;P2&=0X9F;P2&=0X1F;}
#define led_off(n) {led_bit|=_crol_(0x01,n-1);P0 = led_bit;P2|=0X80;P2&=0X9F;P2&=0X1F;}
#define buzzer_on {act_bit|=0x40;P0 = act_bit;P2|=0XA0;P2&=0XBF;P2&=0X1F;}
#define buzzer_off {act_bit&=0xbf;P0 = act_bit;P2|=0XA0;P2&=0XBF;P2&=0X1F;}
#define relay_on {act_bit|=0x10;P0 = act_bit;P2|=0XA0;P2&=0XBF;P2&=0X1F;}
#define relay_off {act_bit&=0xef;P0 = act_bit;P2|=0XA0;P2&=0XBF;P2&=0X1F;}

code uint seg_num[] = {0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,
											 0x40,0x79,0x24,0x30,0x19,0x12,0x02,0x78,0x00,0x10,
											 0xff,0xC1,0xBF};

uint seg_buf[] = {20,20,20,20,20,20,20,20};

uchar num=0;
//按键
typedef enum
{
	key_check,
	key_press,
	key_release
}key_state;
key_state keystate = key_check;
uint key_time = 20;
uchar key_val = 0;
bit key_flag = 0;
uchar key_tt=0;
bit key17_flag = 0;

//显示
uchar showstate = 1;
uchar show_tt=0;
bit show_ref = 0;

//超声波
uint ultra_tt = 0;
bit ultra_ref = 0;
sbit TX = P1^0;
sbit RX = P1^1;
uint lcm_time = 0;
uint lcm = 0;

//pwm输出
sbit pwm_out = P3^4;
uint pwm_out_tt = 0;
bit pwm_out_ref = 0;
uint pwm_freq = 0;
uint pwm_duty = 0;
uint pwm_freq_change = 1000;
uint pwm_duty_change = 50;
uint pwm_tt = 0;
//pwm捕获
sbit pwm_in = P3^4;
uint pwm_in_tt = 0;
bit pwm_in_ref = 0;
uint pwm_in_duty = 0;
uint pwm_in_freq = 0;
uint pwm_l = 0;
uint pwm_l_save = 0;
uint pwm_h = 0;
uint pwm_h_save = 0;
bit pwm_in_flag = 0;

//温度
uint temp_tt=0;
bit temp_ref = 0;
unsigned long temp_val=0;
uchar temp_para = 23;
//时间
extern uint time_real[3];
uint time_tt = 0;
bit time_ref = 0;
bit time_para = 0;

//控制
uchar control_state = 1;
uchar control_temp = 0;
bit control_time = 0;
bit control_time2 = 0;
uint control_tt = 0;
bit control_ref = 0;
bit control_temp2 = 0;

//LED
uint led_tt = 0;
bit led_ref = 0;
bit led_flash = 0;

void delay_ms(uint ms);
void all_init ();
void key_scan();

void key_w(uchar value);
unsigned char key_r();
void Timer0_Init(void);

void tast_clock();
void tast_key();
void tast_show();
void tast_control();
void tast_led();

void ultra();
void send_wave();
void Delay15us(void);
void Timer1_Init(void);
void Timer2_Init(void);

#endif