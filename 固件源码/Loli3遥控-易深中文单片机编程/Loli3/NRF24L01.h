#include "引脚定义.h"

#ifndef __NRF24L01_H_
#define __NRF24L01_H_

//无线通信
__bit cancel,connecting;
u8 TX_power=3;
__bit CD;
u8 __data RC_TX_state=0;
u8 t_check,t_hopping,NRF_error=0;

u8 CD_data[16];

u8 NRF_mapping[8];
u16 NRF_buff[8];
u8 timer2=0,timer3=0,receive=0;


u8 FUCK_sum=0;//坑爹的xdata，定义成rx不能用，定义成FUCK就能用了


u8 address[5];
u8 hopping[5];
u8 __code address_0[5]={'L','O','V','E','!'};//使用LOVE作为对频暗语



u8 tx_buff[11],tx_data[11],rx[11]={0,0,0};
u8 tx_order[11];


u8 TX_protocol=3;


u8 Power_V=70,Chip_V=40,TX_min=60,RX_min=60;
	
//NRF24L01+
u8 SPI(u8 byte)
{
	u8 i;
	for(i=0;i<8;i++)
	{
		MDI_P04_无线模块=(byte&0x80);
		SCK_P01_无线模块=1;
		byte<<=1;
		byte|=MDO_P02_无线模块;
		SCK_P01_无线模块=0;
	}
	return byte;
}

void REG_write(u8 address,u8 command)
{
	CSN_P05_无线模块=0;
	SPI(0x20+address);
	SPI(command);
	CSN_P05_无线模块=1;
}

u8 REG_read(u8 address)
{
	u8 m;
	CSN_P05_无线模块=0;
	SPI(address);
	m=SPI(0);
	CSN_P05_无线模块=1;
	return m;
}

void FIFO_write(u8 DATA_OUT[],u8 lengh)
{
	u8 i;
	CSN_P05_无线模块=0;
	SPI(0xa0);
	for(i=0;i<lengh;i++)
	SPI(DATA_OUT[i]);
	CSN_P05_无线模块=1;
}
void FIFO_write2(u8 DATA_OUT[],u8 lengh)
{
	u8 i;
	CSN_P05_无线模块=0;
	SPI(0xa0);
	for(i=0;i<lengh;i++)
	SPI(DATA_OUT[i]);
	CSN_P05_无线模块=1;
}
void FIFO_read(u8 DATA_IN[],u8 lengh)		//读取接收数据缓冲区
{
	u8 i;
	CSN_P05_无线模块=0;
	SPI(0x61);	//读取命令
	for(i=0;i<lengh;i++)
	DATA_IN[i]=SPI(0);	   
	CSN_P05_无线模块=1;
}
void FIFO_read2(u8 DATA_IN[],u8 lengh)		//读取接收数据缓冲区
{
	u8 i;
	CSN_P05_无线模块=0;
	SPI(0x61);	//读取命令
	for(i=0;i<lengh;i++)
	DATA_IN[i]=SPI(0);	   
	CSN_P05_无线模块=1;
}
void TX_address(u8 DATA_IN[])
{
	CSN_P05_无线模块=0;		 
	SPI(0x20+0x10);
	SPI(DATA_IN[0]);
	SPI(DATA_IN[1]);
	SPI(DATA_IN[2]);
	SPI(DATA_IN[3]);
	SPI(DATA_IN[4]);
	CSN_P05_无线模块=1;  
}  
void RX_address(u8 DATA_IN[])
{
	CSN_P05_无线模块=0;		 
	SPI(0x20+0x0a);
	SPI(DATA_IN[0]);
	SPI(DATA_IN[1]);
	SPI(DATA_IN[2]);
	SPI(DATA_IN[3]);
	SPI(DATA_IN[4]);
	CSN_P05_无线模块=1;  
}
void RX_mode()				 
{
	CE_P00_无线模块=0;
	REG_write(0x00,0x3b); //CRC,8 __bit,Power on,RX
	CE_P00_无线模块=1;
} 				   
	
void TX_mode()				 
{
	CE_P00_无线模块=0;
	REG_write(0x00,0x0a);
  CE_P00_无线模块=1;
}

void  NRF_power(u8 P)				//发射功率设置
{
	CE_P00_无线模块=0;
	if(P==3)REG_write(0x06,0x27);		  //0db 修正之前注释错误
	else if(P==2)REG_write(0x06,0x25);	  //-6db
	else if(P==1)REG_write(0x06,0x23);	  //-12db
	else if(P==0)REG_write(0x06,0x21);    //-18db
	CE_P00_无线模块=1;
}

void NRF_size(u8 l)
{
	CE_P00_无线模块=0;
	REG_write(0x11,l);  
	CE_P00_无线模块=1;
}

void NRF_channel(u8 c)
{
	CE_P00_无线模块=0;
	REG_write(0x05,c);  
	CE_P00_无线模块=1;
}


void NRF_init()
{	
	CE_P00_无线模块=0;
	SCK_P01_无线模块=0;
	REG_write(0x01,0x00); //禁止 自动应答
	REG_write(0x02,0x01); //允许 P0信道
	REG_write(0x04,0x00); //禁止 自动重发
	TX_mode(); //REG_write(0x1d,0x01);			 
	NRF_channel(66);
	NRF_power(TX_power);
	NRF_size(11);
	TX_address(address);
	RX_address(address);
}

void NRF_test()	//无线模块终极测试
{	
	u8 reset_err=0;
	
	if(MDO_P02_无线模块==0){NRF_error|=0x02;}//MOSI bad
	if(MDI_P04_无线模块==0)NRF_error|=0x10;		//MISO bad
	if(IRQ_P03_无线模块==0)NRF_error|=0x08;	//IRQ_P03_无线模块 bad
		if(CSN_P05_无线模块==0)NRF_error|=0x01;//CSN_P05_无线模块,CLK_P44_液晶屏 bad
	
	CE_P00_无线模块=0;
	SCK_P01_无线模块=0;
	CSN_P05_无线模块=0;
	
	if(SPI(0x20)!=0x0e){reset_err=1;}
	SPI(0x0a);

	CSN_P05_无线模块=1;

	CSN_P05_无线模块=0;
	SPI(0x00);

	if(SPI(0x00)!=0x0a){NRF_error|=0x02;}//MOSI bad
	CSN_P05_无线模块=1;

	REG_write(0x01,0x00);
	REG_write(0x04,0x00);
	REG_write(0x11,1);
	
	FIFO_write(tx_buff,1);
	CE_P00_无线模块=1;

	delay_ms(2);
	
	CSN_P05_无线模块=0;

	if(SPI(0x00)!=0x2e){NRF_error|=0x04;}//CE_P00_无线模块 bad
	CSN_P05_无线模块=1;
		
	if(IRQ_P03_无线模块)NRF_error|=0x18;	//IRQ_P03_无线模块 bad
	else 
	{
		if(NRF_error&0x04==0)NRF_error|=0x10;		//MISO bad
	}
	CE_P00_无线模块=1;
	
	if(reset_err&&NRF_error>1)NRF_error|=0x01;//CSN_P05_无线模块,CLK_P44_液晶屏 bad
	
	REG_write(0x07,0x20);	//清除TX中断信号
}

#endif