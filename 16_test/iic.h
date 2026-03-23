#ifndef __iic_H__
#define __iic_H__

unsigned char AT_R(unsigned char add);
void AT_W(unsigned char add,unsigned char dat);
void AD_W(unsigned char dat);
unsigned char AD_R(unsigned char add);
#endif