#ifndef __IIC_H__
#define __IIC_H__

unsigned char AD_r(unsigned char add);
void AD_w(unsigned char dat);
unsigned char AT24C02_r(unsigned char add);
void AT24C02_w(unsigned char add,unsigned char dat);

#endif