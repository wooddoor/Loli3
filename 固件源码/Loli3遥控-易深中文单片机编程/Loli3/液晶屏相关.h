#include "引脚定义.h"
#include "延时.h"

#ifndef __液晶屏相关_H_
#define __液晶屏相关_H_

//***********************************************
//数据、标志位，12864液晶操作函数中使用
//sdcc中没有bdata，bdata位于0x20-0x2F，bit也是位于该区域，因此bdata改为用data，然后用bit代替
__data u8 __at (0x20) DATA;
__bit __at (0x7) D_7;
__bit __at (0x6) D_6;
__bit __at (0x5) D_5;
__bit __at (0x4) D_4;
__bit __at (0x3) D_3;
__bit __at (0x2) D_2;
__bit __at (0x1) D_1;
__bit __at (0x0) D_0;

//显示
u8 word_hzk序号数组[10];
u8 WW_菜单字符串[11][6];//保存待显示字符串的字库序号
__bit LCD_big;
__bit LCD_light=1;
__bit LCD_reverse;
u8 LCD_vop=42;

u8 boot_screen=0;
__bit boot_screen_en=1;
	
//***********************************************
//12864液晶操作函数
void LCD(u8 D)
{
 	DATA=D;
	CLK_P44_液晶屏=0;SDA_P45_液晶屏=D_7;CLK_P44_液晶屏=1;
 	CLK_P44_液晶屏=0;SDA_P45_液晶屏=D_6;CLK_P44_液晶屏=1;
 	CLK_P44_液晶屏=0;SDA_P45_液晶屏=D_5;CLK_P44_液晶屏=1;
	CLK_P44_液晶屏=0;SDA_P45_液晶屏=D_4;CLK_P44_液晶屏=1;
	CLK_P44_液晶屏=0;SDA_P45_液晶屏=D_3;CLK_P44_液晶屏=1;
	CLK_P44_液晶屏=0;SDA_P45_液晶屏=D_2;CLK_P44_液晶屏=1;
	CLK_P44_液晶屏=0;SDA_P45_液晶屏=D_1;CLK_P44_液晶屏=1;
	CLK_P44_液晶屏=0;SDA_P45_液晶屏=D_0;CLK_P44_液晶屏=1;
	CLK_P44_液晶屏=0;
}


void location(u8 x,u8 y)
{
	RS_P46_液晶屏=0;
	if(LCD_reverse)
	{
		if(LCD_big)x+=3;
		else x+=4;
	}
	else
	{
		if(LCD_big)x++;	
	}
	
	LCD(0xb0+y);
	LCD(((x>>4)&0x0f)+0x10);
	LCD(x&0x0f);
	RS_P46_液晶屏=1;
}

void LCD_ON()
{CS_P06_液晶屏=0;}

void LCD_OFF()
{CS_P06_液晶屏=1;}

void LCD_clean(u8 x,u8 y,u8 length,u8 width)//清空指定区域
{
	u8 i,j;
  for(i=0;i<width;i++)
  {
		location(x,i+y);
		for(j=0;j<length;j++)
	  LCD(0);
  }
}

void LCD_init()
{
  LCD_ON();
	
	RST_P07_液晶屏=0;
  delay_ms(2);
  RST_P07_液晶屏=1;
  delay_ms(10);

  RS_P46_液晶屏=0;
  LCD(0xe2);//软复位
	Delay1ms();
  
  LCD(0x2c);
	LCD(0x2e);
	LCD(0x2f);//开启升压
  LCD(0x23);//粗调对比度
  LCD(0x81);//微调对比度
  LCD(LCD_vop);
	
	LCD(0xa0);//从左到右
  LCD(0xa2);//偏压1:9 
	LCD(0xa4);//正常显示
	LCD(0xa6);//正显 
  LCD(0xc8);//从上到下
  
	if(LCD_big)LCD(0x60);//起始行2
  else LCD(0x40);//起始行1
  LCD(0xaf);
  RS_P46_液晶屏=1;
	
  LCD_clean(0,0,128,8);
  LCD_OFF();
 
}

void send(u8 k)
{
	u8 j;
	for(j=0;j<6;j++)
   	LCD(font[k-32][j]);		
}

void send2_写大字母(u8 k,u8 x,u8 y)		 //写大字母
{
	u8 i;
	k-=32;
	location(x,y);
	for(i=0;i<8;i++)
	{
		LCD(font_big[k][i]);	
	}
	location(x,y+1);
	for(i=8;i<16;i++)
	{
		LCD(font_big[k][i]);	
	}
}

/**
写大字母，反显
*/
void send2_rev_写大字母_反显(u8 k,u8 x, u8 y)
{
	u8 i;
	k-=32;
	location(x,y);
	for(i=0;i<8;i++)
	{
		LCD(~font_big[k][i]);	
	}
	location(x,y+1);
	for(i=8;i<16;i++)
	{
		LCD(~font_big[k][i]);	
	}
}

void send2_hex(u8 k,u8 x,u8 y)
{
	u8 i;
	if(k>9)
	{
		k+=23;
		location(x,y);
		for(i=0;i<8;i++)
		{
			LCD(font_big[k][i]);	
		}
		location(x,y+1);
		for(i=8;i<16;i++)
		{
			LCD(font_big[k][i]);	
		}
	}
	else
	{
		k+=16;
		location(x,y);
		for(i=0;i<8;i++)
		{
			LCD(font_big[k][i]);	
		}
		location(x,y+1);
		for(i=8;i<16;i++)
		{
			LCD(font_big[k][i]);	
		}
	}
	
}

void send3_写汉字(u8 *p,u8 x,u8 y)		 //写汉字
{
	u8 i;
	location(x,y);
	for(i=0;i<12;i++)
	{
		LCD(*p),p++;	
	}
	location(x,y+1);
	for(i=0;i<12;i++)
	{
		LCD(*p),p++;	
	}
}

void write_显示汉字(u8 x,u8 y)			//显示汉字
{
	u8 *d;
	d=word_hzk序号数组;
	while(*d)
	{		
		send3_写汉字(hzk[*d],x,y);
		d++;x+=12;
	}

}
void write0_显示汉字串(u8 *d,u8 x,u8 y)			//显示汉字串
{

	while(*d)
	{		
		send3_写汉字(hzk[*d],x,y);
		d++;x+=12;
	}

}
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


void photo_显示指定大小的图片(u8 *p,u8 x,u8 y,u8 length,u8 width)//显示指定大小的图片
{
  u8 i,j;
  for(i=0;i<width;i++)
  {
		location(x,i+y);
		for(j=0;j<length;j++)
	  LCD(*p),p++;
  }  	
}

void display_trim1_显示竖微调(u8 v,u8 base)//显示竖微调
{
	u8 i;unsigned long L1,L2=0,L3,L4=0;
	L1=0x03e00000;v/=5;
	for(i=0;i<v;i++)
	{
		if(L1&1)
		{
			L1>>=1,L2>>=1;L2|=0x80000000;
		}
		else L1>>=1,L2>>=1;
	}
	L2|=0x40000000;

	if(v==0)L3=0x00100000,L4=0;
	else if(v==50)L3=0,L4=0x00000100;
	else
	{
		L3=0x02080000;L4=0;
		for(i=1;i<v;i++)
		{
			if(L3&1)
			{
				L3>>=1,L4>>=1;L4|=0x80000000;
			}
			else L3>>=1,L4>>=1;
		}
	}
	L4|=0x40000000;

	location(base,7);LCD(L1>>24);LCD(L3>>24);LCD(0x03);LCD(L3>>24);LCD(L1>>24);
	location(base,6);LCD(L1>>16);LCD(L3>>16);LCD(0xff);LCD(L3>>16);LCD(L1>>16);
	location(base,5);LCD(L1>>8); LCD(L3>>8); LCD(0xff);LCD(L3>>8); LCD(L1>>8);
	location(base,4);LCD(L1);    LCD(L3);	 LCD(0xff);LCD(L3);    LCD(L1);
	location(base,3);LCD(L2>>24);LCD(L4>>24);LCD(0xff);LCD(L4>>24);LCD(L2>>24);
	location(base,2);LCD(L2>>16);LCD(L4>>16);LCD(0xff);LCD(L4>>16);LCD(L2>>16);
	location(base,1);LCD(L2>>8); LCD(L4>>8); LCD(0xff);LCD(L4>>8); LCD(L2>>8);  
	location(base,0);LCD(L2);	 LCD(L4);	 LCD(0xf8);LCD(L4);    LCD(L2);	   	 
}

void display_trim2_显示横微调(u8 v)//显示横微调
{
	u8 i;char m;
	v=v/5+2;		
	for(i=0;i<55;i++)
	{		
		if(i==27)LCD(0x7c);
		else
		{
			m=v-i;
			if(m==3)LCD(0x38);
			else if(m==2)LCD(0x54);	
			else if(m==1)LCD(0x54);
			else if(m==0)LCD(0x54);
			else if(m==-1)LCD(0x54);
			else if(m==-2)LCD(0x54);
			else if(m==-3)LCD(0x38);
			else LCD(0x10);
		}
		
	}
}

void display_throttle_显示油门(u8 x)//显示油门
{
	u8 i;long L=0;
	x=x/8;
	for(i=0;i<x;i++)
	{
		L>>=1;
		L|=0x80000000;
	}
	L++;
	location(9,6);LCD(L>>24);LCD(L>>24);
	location(9,5);LCD(L>>16);LCD(L>>16);
	location(9,4);LCD(L>>8);LCD(L>>8);
	location(9,3);LCD(L);LCD(L);
}

void warning_显示警告图标(u8 x,u8 y,u8 W)//警告
{
	location(x,y);
	if(W)
	{
		LCD(0x40);LCD(0x60);LCD(0x50);LCD(0x48);LCD(0x44);LCD(0x42);LCD(0x6d);
		LCD(0x42);LCD(0x44);LCD(0x48);LCD(0x50);LCD(0x60);LCD(0x40);	
	}
	else
	{
		LCD(0x00);LCD(0x00);LCD(0x00);LCD(0x00);LCD(0x00);LCD(0x00);LCD(0x00);
		LCD(0x00);LCD(0x00);LCD(0x00);LCD(0x00);LCD(0x00);LCD(0x00);
	}
	
}

void write_num100(int num,u8 x,u8 y,u8 c)//写1000内数字，c为类型选择
{
	if(c=='-'||c=='/'||c=='#')
	{
		if(num>0)send2_写大字母('+',x-8,y);
		else if(num<0)num=-num,send2_写大字母('-',x-8,y);
		else send2_写大字母(' ',x-8,y);
	}
	if(num>99)send2_写大字母(num/100+48,x,y);
	else send2_写大字母(' ',x,y);
	send2_写大字母(num%100/10+48,x+8,y);
	if(c=='.'||c=='#')
	{
		send2_写大字母('.',x+16,y);
		send2_写大字母(num%10+48,x+24,y);
		if(c=='#')send2_写大字母('%',x+32,y);
		
	}
	else
	{
		send2_写大字母(num%10+48,x+16,y);
		if(c=='%'||c=='/')send2_写大字母('%',x+24,y);
	}
	
}

void write_num1000(int num,u8 c)//写10000内数字
{		
	
	if(num>999)
	{
		if(c==0)send(49);
		else num/=10;
		
		send(num%1000/100+48);							
		send(num%100/10+48);
			if(c=='.')LCD(0x00),LCD(0x40);			
		send(num%10+48);
	}
	else 
	{
		if(c==0)send(' ');
		else if(c=='-')
		{
			if(num>0)send('+');
			else if(num<0)num=-num,send('-');
			else send(' ');
		}
		send(num%1000/100+48);
			if(c=='.')LCD(0x00),LCD(0x40);				
		send(num%100/10+48);			
		send(num%10+48);
	}	
	
	
									
	
}

#endif