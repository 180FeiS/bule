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
uchar key_tt = 0;

//数码管
code uint seg_num[] = {0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,
											 0x40,0x79,0x24,0x30,0x19,0x12,0x02,0x78,0x00,0x10,
											 0xff};
uint seg_buf[] = {20,20,20,20,20,20,20,20};
uchar num = 0;

//显示
uchar show_state  = 1;
uchar show_tt = 0;
uchar show_ref =  0;

//超声波
sbit TX = P1^0 ;
sbit RX = P1^1 ;
uint lcm_time = 0;
uint lcm = 0;
uint ultra_tt = 0;
bit ultra_ref = 0;

//PWM out
sbit pwm_out = P3^4;
uint pwm_tt = 0;
bit pwm_ref =0;
uint pwm_freq = 0;
uint pwm_duty = 0;
uint pwm_freq_para = 1000;
uint pwm_duty_para = 50;
uint pwm_count = 0;

//PWM IN
sbit pwm_in = P3^4;
uint pwm_l_save = 0;
uint pwm_h_save = 0;
uint pwm_l = 0;
uint pwm_h = 0;
bit pwm_in_flag = 0;
uint pwm_in_freq = 0;
uint pwm_in_duty = 0;

//串口
uchar uart_receive[] = {"nihao\r\n"};
uint uart_send = 0;

//温度
uint temp_tt = 0;
bit temp_ref = 0;
unsigned long temp_val;

//AT
uchar open;

//AD
uint AD_tt = 0;
bit AD_ref = 0;
uint AD_val = 0;

//时间
extern unsigned int time_ds[3];
uint time_tt = 0;
bit time_ref = 0;

void delay_ms(uint ms);
void key_arrscan();
void all_init();

void key_w(uchar value);
unsigned char key_r();
void Timer2_Init();

void tast_key();
void tast_clock();
void tast_show();

void key_scan();
void send_wave();
void ultra();
//void Timer1_Init(void);
void Delay15us(void);

void Timer0_Init(void);

//void Uart1_Init(void);
void Uart1_Init(void);
void uart(uchar *p);

#endif