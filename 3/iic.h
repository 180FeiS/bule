#ifndef __IIC_H__
#define __IIC_H__

unsigned char AD_read(unsigned char add);
void AD_write(unsigned char dat);
void AT24C02_write(unsigned char add,unsigned char dat);
unsigned char AT24C02_read(unsigned char add);

#endif