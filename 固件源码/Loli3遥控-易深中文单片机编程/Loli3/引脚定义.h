#引入 "STC12C5Axx.h"

#ifndef __引脚定义_H_
#define __引脚定义_H_

//*************************************************
//LCD引脚定义
#define CS_P06_液晶屏 P06
#define RST_P07_液晶屏 P07
#define RS_P46_液晶屏 P46								  
#define SDA_P45_液晶屏 P45							   
#define CLK_P44_液晶屏 P44
#define LEDA_P25_液晶屏 P25

//**************************************************
//NRF24L01 引脚定义

#define CE_P00_无线模块 P00
#define SCK_P01_无线模块 P01
#define MDO_P02_无线模块 P02
#define IRQ_P03_无线模块 P03
#define MDI_P04_无线模块 P04
#define CSN_P05_无线模块 P05

//*************************************************
//I/O定义

#define buzzer_P34_蜂鸣器 P34
#define PPM_P33_模拟器接口 P33

#define KEY_P47_菜单键 P47
#define K1_P30_ch4右微调_菜单右 P30
#define K2_P31_ch4左微调_菜单左 P31
#define K3_P37_ch1左微调_菜单返回 P37
#define K4_P36_ch1右微调_菜单确认 P36
#define K5_P32_ch2下微调_菜单下 P32
#define K6_P35_ch2上微调_菜单上 P35

#define SA_P20_2档开关A P20
#define SB1_P21_3档开关B_1 P21
#define SB2_P22_3档开关B_2 P22
#define SC1_P23_3档开关C_1 P23
#define SC2_P24_3档开关C_2 P24
#define SD_P26_2档开关D P26
#define SE_P27_2档开关E P27

#endif