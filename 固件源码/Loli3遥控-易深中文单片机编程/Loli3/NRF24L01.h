#include "type.h"

#ifndef __NRF24L01_H_
#define __NRF24L01_H_

//无线通信
extern __bit cancel,connecting;
extern u8 TX_power;
extern __bit CD;
extern u8 __data RC_TX_state;
extern u8 t_check,t_hopping,NRF_error;

extern u8 CD_data[16];

extern u8 NRF_mapping[8];
extern u16 NRF_buff[8];
extern u8 timer2,timer3,receive;


extern u8 FUCK_sum;//坑爹的xdata，定义成rx不能用，定义成FUCK就能用了


extern u8 address[5];
extern u8 hopping[5];
extern u8 __code address_0[5];//使用LOVE作为对频暗语



extern u8 tx_buff[11],tx_data[11],rx[11];
extern u8 tx_order[11];


extern u8 TX_protocol;


extern u8 Power_V,Chip_V,TX_min,RX_min;
	
//NRF24L01+
extern u8 SPI(u8 byte);

void REG_write(u8 address,u8 command);

u8 REG_read(u8 address);

void FIFO_write(u8 DATA_OUT[],u8 lengh);

void FIFO_write2(u8 DATA_OUT[],u8 lengh);

//读取接收数据缓冲区
void FIFO_read(u8 DATA_IN[],u8 lengh);

//读取接收数据缓冲区
void FIFO_read2(u8 DATA_IN[],u8 lengh);

void TX_address(u8 DATA_IN[]);

void RX_address(u8 DATA_IN[]);

void RX_mode();   
	
void TX_mode();

//发射功率设置
void  NRF_power(u8 P);

void NRF_size(u8 l);

void NRF_channel(u8 c);


void NRF_init();

//无线模块终极测试
void NRF_test()	;

#endif