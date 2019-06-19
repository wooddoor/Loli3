#include "type.h"

#ifndef __延时_H_
#define __延时_H_

//延时函数

void Delay1ms()		//@12.000MHz
{
	unsigned char i, j;
	i = 12;
	j = 168;
	do
	{
		while (--j);
	} while (--i);
}

void delay_ms(u16 i)	//延时毫秒
{
	while(i--)
	Delay1ms();
}

#endif