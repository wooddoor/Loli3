#引入 "STC12C5Axx.h"
#include"intrins.h"

#ifndef __EEPROM_H_
#define __EEPROM_H_

//ROM操作函数、数据读写控制
u8 EEPROM_read(u8 address_H,u16 address_L)	//读数据
{
	IAP_CMD=0x01;
	if(address_L>255)
	{
		IAP_ADDRH=address_H+1;
		IAP_ADDRL=address_L-255;
	}
	else 
	{
		IAP_ADDRH=address_H;
		IAP_ADDRL=address_L;
	}
	
	IAP_TRIG=0x5a;
	IAP_TRIG=0xa5; _nop_();
	return IAP_DATA;
}

void EEPROM_write(u8 address_H,u16 address_L,u8 byte)	//写入数据
{
	IAP_CMD=0x02;
	IAP_DATA=byte;
	if(address_L>255)
	{
		IAP_ADDRH=address_H+1;
		IAP_ADDRL=address_L-255;
	}
	else 
	{
		IAP_ADDRH=address_H;
		IAP_ADDRL=address_L;	
	}

	IAP_TRIG=0x5a;
	IAP_TRIG=0xa5; _nop_();
	
}

void EEPROM_clean(u8 address_H)	//擦除数据
{
	IAP_CMD=0x03;
	IAP_ADDRH=address_H;
	IAP_ADDRL=0;
	IAP_TRIG=0x5a;
	IAP_TRIG=0xa5;_nop_();
}

#endif