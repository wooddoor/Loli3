#ifndef __LOLI3PICTURE_H_
#define __LOLI3PICTURE_H_

//取码规则:	低位在前, 列行扫描, 阴码(1-亮, 0-灭)
//查表方法: 要显示的ASCII码-32就可以得到字库码的指针
extern unsigned char __code font[][6];

//大写字母表
extern unsigned char __code font_big[][16];

//汉字表
extern unsigned char __code hzk[][24];

//遥控界面
extern unsigned char __code screen1[];

//NRF TEST
extern unsigned char __code screen2[];

//开机画面Loli3
extern unsigned char __code screen3[];

//开机画面WIN10
extern unsigned char __code screen4[];

//版本 120*24
extern unsigned char __code bmp0[];

//遥控界面
extern unsigned char __code bmp1[];
	
//框框
extern unsigned char __code bmp3[];
	
//360 46*4
extern unsigned char __code bmp4[];
	
//360 50*4
extern unsigned char __code bmp5[];

//Angle 32*4
extern unsigned char __code bmp6[];

//SA3 6*2
extern unsigned char __code bmp7[];

//SB1 6*2
extern unsigned char __code bmp8[];

//SB2 6*2
extern unsigned char __code bmp9[];

//SB3 6*2
extern unsigned char __code bmp10[];

//遥控界面
extern unsigned char __code bmp2[];

//好小子 36*16
extern unsigned char __code name1[72];

//飘飘
extern unsigned char __code name2[72];
	
//F22
extern unsigned char __code name5[72];
	
//SU27
extern unsigned char __code name6[72];

#endif