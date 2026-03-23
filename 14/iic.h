#ifndef __iic_H__
#define __iic_H__

void AT_W(unsigned char add,unsigned int dat);
unsigned char AT_R(unsigned char add);
unsigned char AD_R(unsigned char add);
void AD_W(unsigned char dat);


#endif