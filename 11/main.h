#ifndef __MAIN_H__
#define __MAIN_H__

#define uint unsigned int 
#define uchar unsigned char 
uchar LED_buf=0xff;
uchar ACT_buf=0x00;

#define LEDx_ON(n) {LED_buf&=_crol_(0xfe,n-1);P0=LED_buf;P2|=0X80;P2&=0X9F;P2&=0X1F;}
#define LEDx_OFF(n) {LED_buf|=_crol_(0x01,n-1);P0=LED_buf;P2|=0X80;P2&=0X9F;P2&=0X1F;}
#define Buzzer_ON() {ACT_buf|=0X40;P0=ACT_buf;P2&=0XA0;P2|=0XBF;P2&=0X1F;}
#define Buzzer_OFF() {ACT_buf&=0XBF;P0=ACT_buf;P2&=0XA0;P2|=0XBF;P2&=0X1F;}
#define Relay_ON() {ACT_buf|=0X10;P0=ACT_buf;P2&=0XA0;P2|=0XBF;P2&=0X1F;}
#define Relay_OFF() {ACT_buf&=0XEF;P0=ACT_buf;P2&=0XA0;P2|=0XBF;P2&=0X1F;}

code uint SEG_Num[] = {0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,
											 0x40,0x79,0x24,0x30,0x19,0x12,0x02,0x78,0x00,0x10,
											0xff,0xc6,//C 21
													 0x8c //P 22
	};
uint SEG_Buf[]={20,20,20,20,20,20,20,20};
uint num=0;

typedef enum 
{
	Key_check,
	Key_press,
	Key_release
}Key_state;
Key_state KeyState =  Key_check;
uint Key_Time=20;
uchar Key_Value=0;
bit Key_Flag = 0;
uint Key_tt=0;

uchar SEG_Show=0;
uint show_tt=0;
bit show_ref=0;

uint temp_tt=0;
bit temp_ref=0;
unsigned long temp_value=0;
uchar temp_test=0;

uint AD_tt=0;
bit AD_ref=0;
uint AD_value=0;

char temp_max=30;
char temp_min=20;
char temp_maxstore=0;
char temp_minstore=0;

bit temp_choose=0;

sbit TX = P1^0;
sbit RX = P1^1;
unsigned int LCM_tt = 0 ;
bit LCM_Ref = 0 ;
unsigned int LCM = 0 ;
unsigned int LCM_Time = 0 ;

void delay_ms(uint ms);
void All_init();
void Timer0_Init(void);
void Arrkey_scan();
void key_scan();
void key_w(uchar Value);
uchar key_r();
void Task_show();
void Task_clock();
void SEG_refresh();
void ultra_wave();
void Delay14us();
void ultra_num();
void Timer1_Init(void);
#endif