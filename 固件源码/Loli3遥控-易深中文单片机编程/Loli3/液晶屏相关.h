#include "type.h"


#ifndef __液晶屏相关_H_
#define __液晶屏相关_H_

//***********************************************
//数据、标志位，12864液晶操作函数中使用
//sdcc中没有bdata，bdata位于0x20-0x2F，bit也是位于该区域，因此bdata改为用data，然后用bit代替
//应该也可以用联合体来代替使用吧？
//union DataAndBit{
		//__data u8 __at (0x20) DATA;
		//__bit DX[8];
	//}
extern __data u8 __at (0x20) DATA;
extern __bit __at (0x7) D_7;
extern __bit __at (0x6) D_6;
extern __bit __at (0x5) D_5;
extern __bit __at (0x4) D_4;
extern __bit __at (0x3) D_3;
extern __bit __at (0x2) D_2;
extern __bit __at (0x1) D_1;
extern __bit __at (0x0) D_0;

//显示
extern u8 word_hzk序号数组[10];
extern u8 WW_菜单字符串[11][6];//保存待显示字符串的字库序号
extern __bit LCD_big;
extern __bit LCD_light;
extern __bit LCD_reverse;
extern u8 LCD_vop;

extern u8 boot_screen;
extern __bit boot_screen_en;
	
//***********************************************
//12864液晶操作函数
void LCD(u8 D);


void location(u8 x,u8 y);

void LCD_ON();

void LCD_OFF();

//清空指定区域
void LCD_clean(u8 x,u8 y,u8 length,u8 width);

void LCD_init();

void send(u8 k);

//写大字母
void send2_写大字母(u8 k,u8 x,u8 y);

/**
写大字母，反显
*/
void send2_rev_写大字母_反显(u8 k,u8 x, u8 y);

void send2_hex(u8 k,u8 x,u8 y);

//写汉字
void send3_写汉字(u8 *p,u8 x,u8 y);

//void write_显示汉字(u8 *d, u8 x, u8 y)			//显示汉字
//{
	////柴门改：原源码中是void write_显示汉字(u8 x, u8 y)，然后在函数内部使用了全局变量，既然这样干嘛不干脆改为传参呢？
	////改为传参后就与下一个write0_显示汉字串（）一样了，那这个就没必要存在了
	////u8 *d;
	////d=word_hzk序号数组;
	//while(*d)
	//{		
		//send3_写汉字(hzk[*d],x,y);
		//d++;x+=12;
	//}
//
//}

//显示汉字串
void write0_显示汉字串(u8 *d,u8 x,u8 y);			
/*
void write2(u8 x,u8 y)			//显示大字母
{
	u8 *d;
	d=word_hzk序号数组;
	while(*d)
	{		
		send2_写大字母(*d,x,y);
		d++;x+=8;
	}

}*/

//显示指定大小的图片
void photo_显示指定大小的图片(u8 *p,u8 x,u8 y,u8 length,u8 width);

//显示竖微调
void display_trim1_显示竖微调(u8 v,u8 base);

//显示横微调
void display_trim2_显示横微调(u8 v);

//显示油门
void display_throttle_显示油门(u8 x);

//警告
void warning_显示警告图标(u8 x,u8 y,u8 W);

//写1000内数字，c为类型选择
void write_num100(int num,u8 x,u8 y,u8 c);

//写10000内数字
void write_num1000(int num,u8 c);

#endif