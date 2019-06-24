//#include<reg51.h>			//Large编译，否则xdata异常
#include<math.h>
#include "LOLI_3.h"  	 //显示需要的图形数据
#define u8 unsigned char
#define u16 unsigned int
#include "intrins.h"
#include "STC12C5Axx.h"
/*****************************************************/
//STC12 系列新增寄存器
/*__sfr IPH=0xb7;
__sfr P4=0xc0;
__sfr P4SW=0xbb;
__sfr P1ASF=0x9d;
__sfr P4M0=0xb4;
__sfr P3M0=0xb2;
__sfr P3M1=0xb1;
__sfr P2M0=0x96;
__sfr P0M0=0x94;
__sfr ADC_CONTR=0xbc;
__sfr ADC_RES=0xbd;
__sfr ADC_RESL=0xbe;

__sfr BRT=0x9c;
__sfr AUXR=0x8e;
__sfr WAKE_CLKO=0x8f;

__sfr IAP_DATA=0xc2;
__sfr IAP_ADDRH=0xc3;
__sfr IAP_ADDRL=0xc4;
__sfr IAP_CMD=0xc5;
__sfr IAP_TRIG=0xc6;
__sfr IAP_CONTR=0xc7;

__sfr CCON        =   0xD8;           //PCA control register
__sbit CCF0       =   CCON^0;         //PCA module-0 __interrupt flag
__sbit CCF1       =   CCON^1;         //PCA module-1 __interrupt flag
__sbit CR         =   CCON^6;         //PCA timer run control __bit
__sfr CMOD        =   0xD9;           //PCA mode register
__sfr CL          =   0xE9;           //PCA base timer LOW
__sfr CH          =   0xF9;           //PCA base timer HIGH
__sfr CCAPM0      =   0xDA;           //PCA module-0 mode register
__sfr CCAP0L      =   0xEA;           //PCA module-0 capture register LOW
__sfr CCAP0H      =   0xFA;           //PCA module-0 capture register HIGH
__sfr CCAPM1      =   0xDB;           //PCA module-1 mode registers
__sfr CCAP1L      =   0xEB;           //PCA module-1 capture register LOW
__sfr CCAP1H      =   0xFB;           //PCA module-1 capture register HIGH
*/

/*************************************************/
//LCD引脚定义
#define  CS P06
#define  RST P07
#define  RS P46								  
#define  SDA P45						   
#define  CLK P44
#define  LEDA P25

/**************************************************/
//NRF24L01 引脚定义

#define  CE P00
#define  SCK P01
#define  MDO P02
#define  IRQ P03
#define  MDI P04
#define  CSN P05

/*************************************************/
//I/O定义

#define buzzer P34
#define PPM P33

#define KEY P47
#define K1 P30
#define K2 P31
#define K3 P37
#define K4 P36
#define K5 P32
#define K6 P35

#define SA P20
#define SB1 P21
#define SB2 P22
#define SC1 P23
#define SC2 P24
#define SD P26
#define SE P27


/***********************************************/
//数据、标志位
__data u8 __at (0x20) DATA;
__bit __at (0x7) D_7;
__bit __at (0x6) D_6;
__bit __at (0x5) D_5;
__bit __at (0x4) D_4;
__bit __at (0x3) D_3;
__bit __at (0x2) D_2;
__bit __at (0x1) D_1;
__bit __at (0x0) D_0;



//ROM存储
#define Model_length 125   //模型数据 长度 宏定义
#define Version  0xAC 		//系统版本 A13

u8 Model_number=0,Model_current=0,Model_addrH=2;
__bit Model_change,Model_reset,reset_action;
/*******+军刀破+******************/
//模型命名
u8 Name_idx=0; //名字字母索引，最大7

u8 Model_name[7]={0,0,0,0,0,0,0};
//原来萝丽的定义是351，加上命名内容，定义为375
u8 TEMP_DATA[375];
/*******+军刀破+******************/
__bit ROM_enable;
__bit Data_system,Data_model;
__bit save_action;



__bit calibration_action,calibration_ok;


//显示
u8 word[10];
u8 WW[11][6];
__bit LCD_big;
__bit LCD_light=1;
__bit LCD_reverse;
u8 LCD_vop=42;

u8 boot_screen=0;
__bit boot_screen_en=1;





//时钟
__bit event1,event2,event3;
u8 t1=0,t2=0,t3=0;

__bit flag_20ms;


u8 Master_clock,second=0,minute=0,hour=0;

u8 clock2,second2=0,minute2=0;
u8 clock_set=5,clock_switch=0;
__bit clock_go;


u16 throttle_idle=0;
u8 delay_time=0;
u8 flash_time=0;



//蜂鸣器
__bit Message_AlarmClock,Message_LowPower;
u8 Message_KeyTone=0,Message_DataReturn_bad=0;
u8 beep=0,sound=0;  
__bit Sound_enable=1;	
	
	
	
	

//菜单模式
__bit PPM_OUT,PPM_IN,RC_TX,IR_TX,RC_check;
__bit config_change,star=1,RC_check_star;
u8 menu=0,menu_1=0,menu_2=0,menu_3=0;
u8 line=0,line_1=0,line_2=0,line_3=0;
u8 last_line=0;


//按键
__bit KS,KS_long_press;
u8 KS_press_time;
__bit key1,key2,key3,key4,key5,key6;
__bit s1,s2,s3,s4,s5,s6;
__bit s1_long_press,s2_long_press,s3_long_press,s4_long_press,s5_long_press,s6_long_press;
u8 s1_press_time,s2_press_time,s3_press_time,s4_press_time,s5_press_time,s6_press_time;
__bit key_time;
__bit increase,decrease,up,down,left,right;


//ADC
u16 __data adc[8],adc_buff[8];
u8 t_adc=0;
u8 Battery_1s=1,VLD1=35,VLD2=70;
u16 battery_voltage;
char v_trim1=0,v_trim2=0;
__bit USB_5V;






//硬件配置
//u8 Right_throttle;

__bit throttle_lock,Throttle_check=1,Auto_lock,Trim_step;
u8 tt=0;
u8 middle_change_last;

u16 value0,value1;



u8 IN_mapping[8];

__bit recover;

__bit NRF_config_change,config_change;
__bit first;

u16 CH1_max=600,CH2_max=600,CH3_max=600,CH4_max=600;
u16 CH1_min=400,CH2_min=400,CH3_min=400,CH4_min=400;
u8 CH1_offset=51,CH2_offset=51,CH3_offset=51,CH4_offset=51;
u16 CH_value[4];



u8 ratio_switch=0;
__bit use_new_ratio;



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


u8 rx_sum,FUCK_sum=0;//坑爹的xdata，定义成rx不能用，定义成FUCK就能用了


u8 address[5];
u8 hopping[5];
u8 __code address_0[5]={'L','O','V','E','!'};//使用LOVE作为对频暗语



u8 tx_buff[11],tx_data[11],rx[11]={0,0,0};
u8 tx_order[11];


u8 TX_protocol=3;


u8 Power_V=70,Chip_V=40,TX_min=60,RX_min=60;




//接收机设置
u8 receiver_mode[8];
__bit Receiver_change;
__bit NRF_change;

char out_control[8];
u16 out_control_data[8];


//PPM

u8 T_l,T_h;
u8 t_ppm=0,t_PPMout=0;
int PPM_DATA[8]={0,0,0,0,0,0,0,0};
int __data PPM_buff[8];
u16 __data PPMout[8]={500,600,800,200,1000,0,100,300};
u8 PPM_mapping[8]={0,1,2,3,4,5,6,7};

u8 train_switch=0;
__bit train_output;
u8 PPM_updata=0;
char PPM_offset=0;

//自动控制
u8 auto_switch=0,auto_mode=2;
char auto_middle=-100,auto_star=-50,auto_end=50;
u8 auto_speed=10;
int auto_value=0;
u16 auto_max,auto_min;
__bit auto_h,auto_l=0;




//开关
u8 Switch_A,testljsf234099asfljl,Switch_C,Switch_D,Switch_E;
u8 Combination_Switch=0,Com_Swt_IN1=0,Com_Swt_IN2=0;
u8 Com_Swt_out[6]={16,29,42,55,68,81};
u8 Com_Swt_mapping=4;
u8 Return_alarm_switch=0;


//通道运算
int __data operation[8];



char CH_middle[8];
__bit ratio_mode;
u8 CH_ratio[2][8];
u8 New_ratio[2][4]={60,60,60,60,60,60,60,60};
u8 CH_inverted[8];
u8 CH_middle_change=0;


u8 mixer_name;
__bit mixer1,mixer2,mixer3,mixer4;
u8 mixer1_same,mixer1_contrary;
u8 mixer2_same,mixer2_contrary;
u8 mixer3_same,mixer3_contrary;
u8 mixer4_same,mixer4_contrary;






//曲线运算
u8 curve_buff[51];
u8 index_number=0;
__bit index_change;
char last_index;
char CH_index[3];

u8 point_num=3,curve_num=0;
u8 turnning_point[5][7];
u16 throttle;



/***********************************************************/
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

/**********************************************************/
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

void Data_change(u8 m)//数据改变
{
	if(m==1)Data_system=1;
	if(m==2)Data_model=1;
	
	event3=1,t3=0;//刷新事件3定时器
	
}

void DATA_read1()	//读取系统参数
{								
	u8 i;u16 sum=0,sum_read;
	
	IAP_CONTR=0x83;
	/**
	+军刀破+
	将原来的74改为78，增加回传报警内容
	**/
	for(i=0;i<78;i++)
	{
		sum+=EEPROM_read(0,i);
	}
	
	sum_read=EEPROM_read(0,78);//读校验和
	sum_read<<=8;
	sum_read+=EEPROM_read(0,79);
	
	if(EEPROM_read(0,0)!=Version||sum!=sum_read)	 
	{
		first=1;
		
		
		RC_TX=1;PPM_OUT=1;

	}
	else
	{
//		system=EEPROM_read(0,1);
		Battery_1s=EEPROM_read(0,2);
		LCD_vop=EEPROM_read(0,3);
		LCD_light=EEPROM_read(0,4);
		CH1_offset=EEPROM_read(0,5);
		CH2_offset=EEPROM_read(0,6);
		CH3_offset=EEPROM_read(0,7);
		CH4_offset=EEPROM_read(0,8);
		
		address[0]=EEPROM_read(0,9);
		address[1]=EEPROM_read(0,10);
		address[2]=EEPROM_read(0,11);
		address[3]=EEPROM_read(0,12);
		address[4]=EEPROM_read(0,13);
		hopping[0]=EEPROM_read(0,14);
		hopping[1]=EEPROM_read(0,15);
		hopping[2]=EEPROM_read(0,16);
		hopping[3]=EEPROM_read(0,17);
		hopping[4]=EEPROM_read(0,18);
//		
		VLD1=EEPROM_read(0,19);
		VLD2=EEPROM_read(0,20);
		Sound_enable=EEPROM_read(0,21);	
		LCD_big=EEPROM_read(0,22);
		TX_power=EEPROM_read(0,23);
		v_trim1=EEPROM_read(0,24);
		v_trim2=EEPROM_read(0,25);
		clock_set=EEPROM_read(0,26);
		
		boot_screen=EEPROM_read(0,27);
//		Right_throttle=EEPROM_read(0,28);
	
		Throttle_check=EEPROM_read(0,29);

		Auto_lock=EEPROM_read(0,30);
		clock_switch=EEPROM_read(0,31);
		ratio_switch=EEPROM_read(0,32);
		PPM_offset=EEPROM_read(0,33);
		boot_screen_en=EEPROM_read(0,34);
		LCD_reverse=EEPROM_read(0,35);
		Return_alarm_switch=EEPROM_read(0,36);
/*		NRF_mapping[7]=EEPROM_read(0,37);*/
		
		PPM_mapping[0]=EEPROM_read(0,38);
		PPM_mapping[1]=EEPROM_read(0,39);
		PPM_mapping[2]=EEPROM_read(0,40);
		PPM_mapping[3]=EEPROM_read(0,41);
		PPM_mapping[4]=EEPROM_read(0,42);
		PPM_mapping[5]=EEPROM_read(0,43);
		PPM_mapping[6]=EEPROM_read(0,44);
		PPM_mapping[7]=EEPROM_read(0,45);

		train_switch=EEPROM_read(0,46);
		RC_TX=EEPROM_read(0,47);
		Model_number=EEPROM_read(0,48);Model_current=Model_number;
		PPM_OUT=EEPROM_read(0,49);
		PPM_IN=EEPROM_read(0,50);
		
		auto_switch=EEPROM_read(0,51);
		auto_star=EEPROM_read(0,52);
		auto_end=EEPROM_read(0,53);
		auto_middle=EEPROM_read(0,54);
//		auto_mapping=EEPROM_read(0,55);
		auto_speed=EEPROM_read(0,56);
		
		New_ratio[0][0]=EEPROM_read(0,57);
		New_ratio[0][1]=EEPROM_read(0,58);
		New_ratio[0][2]=EEPROM_read(0,59);
		New_ratio[0][3]=EEPROM_read(0,60);
		New_ratio[1][0]=EEPROM_read(0,61);
		New_ratio[1][1]=EEPROM_read(0,62);
		New_ratio[1][2]=EEPROM_read(0,63);
		New_ratio[1][3]=EEPROM_read(0,64);
		
		Com_Swt_IN1=EEPROM_read(0,65);
		Com_Swt_IN2=EEPROM_read(0,66);
		Com_Swt_mapping=EEPROM_read(0,67);
		Com_Swt_out[0]=EEPROM_read(0,68);
		Com_Swt_out[1]=EEPROM_read(0,69);
		Com_Swt_out[2]=EEPROM_read(0,70);
		Com_Swt_out[3]=EEPROM_read(0,71);
		Com_Swt_out[4]=EEPROM_read(0,72);
		Com_Swt_out[5]=EEPROM_read(0,73);
		/***************+军刀破+****************/
		//读取回传报警值
		Power_V=EEPROM_read(0,74); //报警动力电压
		Chip_V=EEPROM_read(0,75);	//IC电压
		TX_min=EEPROM_read(0,76);	//最小发射信号
		RX_min=EEPROM_read(0,77);	//最小接收信号
		/***************+军刀破+****************/		
		
	}
	

	IAP_CONTR=0;
}


void DATA_save1()	//保存系统参数
{
	u8 i;u16 sum=0;
	
	IAP_CONTR=0x83;

	
	
		EEPROM_clean(0);
		EEPROM_write(0,0,Version);

//		EEPROM_write(0,1,system);
		EEPROM_write(0,2,Battery_1s);
		EEPROM_write(0,3,LCD_vop);
		EEPROM_write(0,4,LCD_light);
		EEPROM_write(0,5,CH1_offset);
		EEPROM_write(0,6,CH2_offset);
		EEPROM_write(0,7,CH3_offset);
		EEPROM_write(0,8,CH4_offset);
	
		EEPROM_write(0,9,address[0]);
		EEPROM_write(0,10,address[1]);
		EEPROM_write(0,11,address[2]);
		EEPROM_write(0,12,address[3]);
		EEPROM_write(0,13,address[4]);
		EEPROM_write(0,14,hopping[0]);
		EEPROM_write(0,15,hopping[1]);
		EEPROM_write(0,16,hopping[2]);
		EEPROM_write(0,17,hopping[3]);
		EEPROM_write(0,18,hopping[4]);
		
		EEPROM_write(0,19,VLD1);
		EEPROM_write(0,20,VLD2);
		EEPROM_write(0,21,Sound_enable);

		

		EEPROM_write(0,22,LCD_big);
		EEPROM_write(0,23,TX_power);
		EEPROM_write(0,24,v_trim1);
		EEPROM_write(0,25,v_trim2);

		EEPROM_write(0,26,clock_set);
		EEPROM_write(0,27,boot_screen);
//		EEPROM_write(0,28,Right_throttle);
		EEPROM_write(0,29,Throttle_check);

		EEPROM_write(0,30,Auto_lock);
		EEPROM_write(0,31,clock_switch);
		EEPROM_write(0,32,ratio_switch);
		EEPROM_write(0,33,PPM_offset);
		EEPROM_write(0,34,boot_screen_en);
		EEPROM_write(0,35,LCD_reverse);
		EEPROM_write(0,36,Return_alarm_switch);
/*		EEPROM_write(0,37,NRF_mapping[7]);*/
		
		EEPROM_write(0,38,PPM_mapping[0]);
		EEPROM_write(0,39,PPM_mapping[1]);
		EEPROM_write(0,40,PPM_mapping[2]);
		EEPROM_write(0,41,PPM_mapping[3]);
		EEPROM_write(0,42,PPM_mapping[4]);
		EEPROM_write(0,43,PPM_mapping[5]);
		EEPROM_write(0,44,PPM_mapping[6]);
		EEPROM_write(0,45,PPM_mapping[7]);
		
		EEPROM_write(0,46,train_switch);

		EEPROM_write(0,47,RC_TX);
		EEPROM_write(0,48,Model_number);
		EEPROM_write(0,49,PPM_OUT);
		EEPROM_write(0,50,PPM_IN);
		
		EEPROM_write(0,51,auto_switch);
		EEPROM_write(0,52,auto_star);
		EEPROM_write(0,53,auto_end);
		EEPROM_write(0,54,auto_middle);
//		EEPROM_write(0,55,auto_mapping);
		EEPROM_write(0,56,auto_speed);
		
		EEPROM_write(0,57,New_ratio[0][0]);
		EEPROM_write(0,58,New_ratio[0][1]);
		EEPROM_write(0,59,New_ratio[0][2]);
		EEPROM_write(0,60,New_ratio[0][3]);
		EEPROM_write(0,61,New_ratio[1][0]);
		EEPROM_write(0,62,New_ratio[1][1]);
		EEPROM_write(0,63,New_ratio[1][2]);
		EEPROM_write(0,64,New_ratio[1][3]);
		
		EEPROM_write(0,65,Com_Swt_IN1);
		EEPROM_write(0,66,Com_Swt_IN2);
		EEPROM_write(0,67,Com_Swt_mapping);
		EEPROM_write(0,68,Com_Swt_out[0]);
		EEPROM_write(0,69,Com_Swt_out[1]);
		EEPROM_write(0,70,Com_Swt_out[2]);
		EEPROM_write(0,71,Com_Swt_out[3]);
		EEPROM_write(0,72,Com_Swt_out[4]);
		EEPROM_write(0,73,Com_Swt_out[5]);
		/****+军刀破+****/
		EEPROM_write(0,74,Power_V); //报警动力电压
		EEPROM_write(0,75,Chip_V);	//IC电压
		EEPROM_write(0,76,TX_min);	//最小发射信号
		EEPROM_write(0,77,RX_min);	//最小接收信号
		/***************+军刀破+****************/
	//将原来的74改为78，增加保存回传报警内容	
	for(i=0;i<78;i++)
	{
		sum+=EEPROM_read(0,i);
	}
	EEPROM_write(0,78,sum>>8);
	EEPROM_write(0,79,sum);	
		
	IAP_CONTR=0;
}


void Model_data_reset()//模型数据复位
{
	u8 i;
	for(i=0;i<8;i++)
		CH_middle[i]=0;
	
	for(i=0;i<8;i++)		
		CH_ratio[0][i]=100;
	
	for(i=0;i<8;i++)		
		CH_ratio[1][i]=100;
	
	for(i=0;i<8;i++)
		CH_inverted[i]=0;
		
		mixer1=0;
		mixer2=0;
		mixer3=0;
		mixer4=0;
		mixer1_same=1;
		mixer1_contrary=0;
		mixer2_same=2;
		mixer2_contrary=3;
		mixer3_same=3;
		mixer3_contrary=0;
		mixer4_same=5;
		mixer4_contrary=0;
	
	for(i=0;i<6;i++)
		IN_mapping[i]=i;

		IN_mapping[6]=7;
		IN_mapping[7]=8;
		
	for(i=0;i<8;i++)	
		NRF_mapping[i]=i;
		
	for(i=0;i<8;i++)
		receiver_mode[i]=0;
		
	for(i=0;i<8;i++)
		out_control[i]=0;
		
	out_control[2]=-100;

	CH_index[0]=0;
	CH_index[1]=0;
	CH_index[2]=0;
	for(i=0;i<5;i++)
	{
		turnning_point[i][0]=0;
		turnning_point[i][1]=16;
		turnning_point[i][2]=32;
		turnning_point[i][3]=50;
		turnning_point[i][4]=68;
		turnning_point[i][5]=84;
		turnning_point[i][6]=100;
	}
	/***************+军刀破+****************/
	//置空模型名
	for(i=0;i<7;i++)
	{
		Model_name[i]=0;
	}
	/**************************************/

}

u16 Model_adress(u16 num)//模型数据基地址
{
	if(num>2)num-=3,Model_addrH=4;
	else Model_addrH=2;

	if(num==0)num=0;
	else if(num==1)num=Model_length;
	else num=Model_length*2;
	
	return num;
}

void DATA_read2()	//读取模型数据
{
	u16 i,base;
	u16 sum,sum_read;
	
	IAP_CONTR=0x83;
	base=Model_adress(Model_current); 	//读出所有数据
	for(i=0;i<Model_length*3;i++)
	{
		TEMP_DATA[i]=EEPROM_read(Model_addrH,i);	
	}
	
	sum=0;
	for(i=base;i<base+Model_length-2;i++)//计算校验和
	{
		sum+=TEMP_DATA[i];
	}
	sum_read=TEMP_DATA[base+Model_length-2];//读校验和
	sum_read<<=8;
	sum_read+=TEMP_DATA[base+Model_length-1];
	
	if(TEMP_DATA[base]!=Version||sum!=sum_read)//比较校验和
	{
		Model_data_reset();
	}
	else
	{
		
			
			CH_middle[0]=TEMP_DATA[1+base];				//读出目标模型数据
			CH_middle[1]=TEMP_DATA[2+base];
			CH_middle[2]=TEMP_DATA[3+base];
			CH_middle[3]=TEMP_DATA[4+base];
			CH_middle[4]=TEMP_DATA[5+base];
			CH_middle[5]=TEMP_DATA[6+base];
			CH_middle[6]=TEMP_DATA[7+base];
			CH_middle[7]=TEMP_DATA[8+base];
			CH_ratio[0][0]=TEMP_DATA[9+base];
			CH_ratio[0][1]=TEMP_DATA[10+base];
			CH_ratio[0][2]=TEMP_DATA[11+base];
			CH_ratio[0][3]=TEMP_DATA[12+base];
			CH_ratio[0][4]=TEMP_DATA[13+base];
			CH_ratio[0][5]=TEMP_DATA[14+base];
			CH_ratio[0][6]=TEMP_DATA[15+base];
			CH_ratio[0][7]=TEMP_DATA[16+base];
			CH_ratio[1][0]=TEMP_DATA[17+base];
			CH_ratio[1][1]=TEMP_DATA[18+base];
			CH_ratio[1][2]=TEMP_DATA[19+base];
			CH_ratio[1][3]=TEMP_DATA[20+base];
			CH_ratio[1][4]=TEMP_DATA[21+base];
			CH_ratio[1][5]=TEMP_DATA[22+base];
			CH_ratio[1][6]=TEMP_DATA[23+base];
			CH_ratio[1][7]=TEMP_DATA[24+base];			
			CH_inverted[0]=TEMP_DATA[25+base];
			CH_inverted[1]=TEMP_DATA[26+base];
			CH_inverted[2]=TEMP_DATA[27+base];
			CH_inverted[3]=TEMP_DATA[28+base];
			CH_inverted[4]=TEMP_DATA[29+base];
			CH_inverted[5]=TEMP_DATA[30+base];
			CH_inverted[6]=TEMP_DATA[31+base];
			CH_inverted[7]=TEMP_DATA[32+base];
			mixer1=TEMP_DATA[33+base];
			mixer2=TEMP_DATA[34+base];
			mixer3=TEMP_DATA[35+base];
			mixer4=TEMP_DATA[36+base];
			mixer1_same=TEMP_DATA[37+base];
			mixer1_contrary=TEMP_DATA[38+base];
			mixer2_same=TEMP_DATA[39+base];
			mixer2_contrary=TEMP_DATA[40+base];
			mixer3_same=TEMP_DATA[41+base];
			mixer3_contrary=TEMP_DATA[42+base];
			mixer4_same=TEMP_DATA[43+base];
			mixer4_contrary=TEMP_DATA[44+base];
			
			IN_mapping[0]=TEMP_DATA[45+base];
			IN_mapping[1]=TEMP_DATA[46+base];
			IN_mapping[2]=TEMP_DATA[47+base];
			IN_mapping[3]=TEMP_DATA[48+base];
			IN_mapping[4]=TEMP_DATA[49+base];
			IN_mapping[5]=TEMP_DATA[50+base];
			IN_mapping[6]=TEMP_DATA[51+base];
			IN_mapping[7]=TEMP_DATA[52+base];
			NRF_mapping[0]=TEMP_DATA[53+base];
			NRF_mapping[1]=TEMP_DATA[54+base];
			NRF_mapping[2]=TEMP_DATA[55+base];
			NRF_mapping[3]=TEMP_DATA[56+base];
			NRF_mapping[4]=TEMP_DATA[57+base];
			NRF_mapping[5]=TEMP_DATA[58+base];
			NRF_mapping[6]=TEMP_DATA[59+base];
			NRF_mapping[7]=TEMP_DATA[60+base];
			receiver_mode[0]=TEMP_DATA[61+base];
			receiver_mode[1]=TEMP_DATA[62+base];
			receiver_mode[2]=TEMP_DATA[63+base];
			receiver_mode[3]=TEMP_DATA[64+base];
			receiver_mode[4]=TEMP_DATA[65+base];
			receiver_mode[5]=TEMP_DATA[66+base];
			receiver_mode[6]=TEMP_DATA[67+base];
			receiver_mode[7]=TEMP_DATA[68+base];
			out_control[0]=TEMP_DATA[69+base];
			out_control[1]=TEMP_DATA[70+base];
			out_control[2]=TEMP_DATA[71+base];
			out_control[3]=TEMP_DATA[72+base];
			out_control[4]=TEMP_DATA[73+base];
			out_control[5]=TEMP_DATA[74+base];
			out_control[6]=TEMP_DATA[75+base];
			out_control[7]=TEMP_DATA[76+base];
			
			CH_index[0]=TEMP_DATA[77+base];
			CH_index[1]=TEMP_DATA[78+base];
			CH_index[2]=TEMP_DATA[79+base];
			turnning_point[0][0]=TEMP_DATA[80+base];
			turnning_point[0][1]=TEMP_DATA[81+base];
			turnning_point[0][2]=TEMP_DATA[82+base];
			turnning_point[0][3]=TEMP_DATA[83+base];
			turnning_point[0][4]=TEMP_DATA[84+base];
			turnning_point[0][5]=TEMP_DATA[85+base];
			turnning_point[0][6]=TEMP_DATA[86+base];
			
			turnning_point[1][0]=TEMP_DATA[87+base];
			turnning_point[1][1]=TEMP_DATA[88+base];
			turnning_point[1][2]=TEMP_DATA[89+base];
			turnning_point[1][3]=TEMP_DATA[90+base];
			turnning_point[1][4]=TEMP_DATA[91+base];
			turnning_point[1][5]=TEMP_DATA[92+base];
			turnning_point[1][6]=TEMP_DATA[93+base];
			
			turnning_point[2][0]=TEMP_DATA[94+base];
			turnning_point[2][1]=TEMP_DATA[95+base];
			turnning_point[2][2]=TEMP_DATA[96+base];
			turnning_point[2][3]=TEMP_DATA[97+base];
			turnning_point[2][4]=TEMP_DATA[98+base];
			turnning_point[2][5]=TEMP_DATA[99+base];
			turnning_point[2][6]=TEMP_DATA[100+base];
			
			turnning_point[3][0]=TEMP_DATA[101+base];
			turnning_point[3][1]=TEMP_DATA[102+base];
			turnning_point[3][2]=TEMP_DATA[103+base];
			turnning_point[3][3]=TEMP_DATA[104+base];
			turnning_point[3][4]=TEMP_DATA[105+base];
			turnning_point[3][5]=TEMP_DATA[106+base];
			turnning_point[3][6]=TEMP_DATA[107+base];
			
			turnning_point[4][0]=TEMP_DATA[108+base];
			turnning_point[4][1]=TEMP_DATA[109+base];
			turnning_point[4][2]=TEMP_DATA[110+base];
			turnning_point[4][3]=TEMP_DATA[111+base];
			turnning_point[4][4]=TEMP_DATA[112+base];
			turnning_point[4][5]=TEMP_DATA[113+base];
			turnning_point[4][6]=TEMP_DATA[114+base];
			/*************+军刀破+************/
			//读模型名
			Model_name[0]=TEMP_DATA[115+base];
			Model_name[1]=TEMP_DATA[116+base];
			Model_name[2]=TEMP_DATA[117+base];
			Model_name[3]=TEMP_DATA[118+base];
			Model_name[4]=TEMP_DATA[119+base];
			Model_name[5]=TEMP_DATA[120+base];
			Model_name[6]=TEMP_DATA[121+base];
			/*************+军刀破+************/
	}

	IAP_CONTR=0;
}

void DATA_save2()	//保存模型数据
{
	u16 i,base;
	u16 sum;
	IAP_CONTR=0x83;
	base=Model_adress(Model_current);
		EEPROM_clean(Model_addrH);
		
			TEMP_DATA[base]=Version;

			TEMP_DATA[1+base]=CH_middle[0];	//修改目标模型数据
			TEMP_DATA[2+base]=CH_middle[1];
			TEMP_DATA[3+base]=CH_middle[2];
			TEMP_DATA[4+base]=CH_middle[3];
			TEMP_DATA[5+base]=CH_middle[4];
			TEMP_DATA[6+base]=CH_middle[5];
			TEMP_DATA[7+base]=CH_middle[6];
			TEMP_DATA[8+base]=CH_middle[7];
			TEMP_DATA[9+base]=CH_ratio[0][0];
			TEMP_DATA[10+base]=CH_ratio[0][1];
			TEMP_DATA[11+base]=CH_ratio[0][2];
			TEMP_DATA[12+base]=CH_ratio[0][3];
			TEMP_DATA[13+base]=CH_ratio[0][4];
			TEMP_DATA[14+base]=CH_ratio[0][5];
			TEMP_DATA[15+base]=CH_ratio[0][6];
			TEMP_DATA[16+base]=CH_ratio[0][7];
			TEMP_DATA[17+base]=CH_ratio[1][0];
			TEMP_DATA[18+base]=CH_ratio[1][1];
			TEMP_DATA[19+base]=CH_ratio[1][2];
			TEMP_DATA[20+base]=CH_ratio[1][3];
			TEMP_DATA[21+base]=CH_ratio[1][4];
			TEMP_DATA[22+base]=CH_ratio[1][5];
			TEMP_DATA[23+base]=CH_ratio[1][6];
			TEMP_DATA[24+base]=CH_ratio[1][7];			
			TEMP_DATA[25+base]=CH_inverted[0];
			TEMP_DATA[26+base]=CH_inverted[1];
			TEMP_DATA[27+base]=CH_inverted[2];
			TEMP_DATA[28+base]=CH_inverted[3];
			TEMP_DATA[29+base]=CH_inverted[4];
			TEMP_DATA[30+base]=CH_inverted[5];
			TEMP_DATA[31+base]=CH_inverted[6];
			TEMP_DATA[32+base]=CH_inverted[7];
			TEMP_DATA[33+base]=mixer1;
			TEMP_DATA[34+base]=mixer2;
			TEMP_DATA[35+base]=mixer3;
			TEMP_DATA[36+base]=mixer4;
			TEMP_DATA[37+base]=mixer1_same;
			TEMP_DATA[38+base]=mixer1_contrary;
			TEMP_DATA[39+base]=mixer2_same;
			TEMP_DATA[40+base]=mixer2_contrary;
			TEMP_DATA[41+base]=mixer3_same;
			TEMP_DATA[42+base]=mixer3_contrary;
			TEMP_DATA[43+base]=mixer4_same;
			TEMP_DATA[44+base]=mixer4_contrary;
			
			TEMP_DATA[45+base]=IN_mapping[0];
			TEMP_DATA[46+base]=IN_mapping[1];
			TEMP_DATA[47+base]=IN_mapping[2];
			TEMP_DATA[48+base]=IN_mapping[3];
			TEMP_DATA[49+base]=IN_mapping[4];
			TEMP_DATA[50+base]=IN_mapping[5];
			TEMP_DATA[51+base]=IN_mapping[6];
			TEMP_DATA[52+base]=IN_mapping[7];
			TEMP_DATA[53+base]=NRF_mapping[0];
			TEMP_DATA[54+base]=NRF_mapping[1];
			TEMP_DATA[55+base]=NRF_mapping[2];
			TEMP_DATA[56+base]=NRF_mapping[3];
			TEMP_DATA[57+base]=NRF_mapping[4];
			TEMP_DATA[58+base]=NRF_mapping[5];
			TEMP_DATA[59+base]=NRF_mapping[6];
			TEMP_DATA[60+base]=NRF_mapping[7];
			TEMP_DATA[61+base]=receiver_mode[0];
			TEMP_DATA[62+base]=receiver_mode[1];
			TEMP_DATA[63+base]=receiver_mode[2];
			TEMP_DATA[64+base]=receiver_mode[3];
			TEMP_DATA[65+base]=receiver_mode[4];
			TEMP_DATA[66+base]=receiver_mode[5];
			TEMP_DATA[67+base]=receiver_mode[6];
			TEMP_DATA[68+base]=receiver_mode[7];
			TEMP_DATA[69+base]=out_control[0];
			TEMP_DATA[70+base]=out_control[1];
			TEMP_DATA[71+base]=out_control[2];
			TEMP_DATA[72+base]=out_control[3];
			TEMP_DATA[73+base]=out_control[4];
			TEMP_DATA[74+base]=out_control[5];
			TEMP_DATA[75+base]=out_control[6];
			TEMP_DATA[76+base]=out_control[7];
			
			TEMP_DATA[77+base]=CH_index[0];
			TEMP_DATA[78+base]=CH_index[1];
			TEMP_DATA[79+base]=CH_index[2];
			TEMP_DATA[80+base]=turnning_point[0][0];
			TEMP_DATA[81+base]=turnning_point[0][1];
			TEMP_DATA[82+base]=turnning_point[0][2];
			TEMP_DATA[83+base]=turnning_point[0][3];
			TEMP_DATA[84+base]=turnning_point[0][4];
			TEMP_DATA[85+base]=turnning_point[0][5];
			TEMP_DATA[86+base]=turnning_point[0][6];
			
			TEMP_DATA[87+base]=turnning_point[1][0];
			TEMP_DATA[88+base]=turnning_point[1][1];
			TEMP_DATA[89+base]=turnning_point[1][2];
			TEMP_DATA[90+base]=turnning_point[1][3];
			TEMP_DATA[91+base]=turnning_point[1][4];
			TEMP_DATA[92+base]=turnning_point[1][5];
			TEMP_DATA[93+base]=turnning_point[1][6];
			
			TEMP_DATA[94+base]=turnning_point[2][0];
			TEMP_DATA[95+base]=turnning_point[2][1];
			TEMP_DATA[96+base]=turnning_point[2][2];
			TEMP_DATA[97+base]=turnning_point[2][3];
			TEMP_DATA[98+base]=turnning_point[2][4];
			TEMP_DATA[99+base]=turnning_point[2][5];
			TEMP_DATA[100+base]=turnning_point[2][6];
			
			TEMP_DATA[101+base]=turnning_point[3][0];
			TEMP_DATA[102+base]=turnning_point[3][1];
			TEMP_DATA[103+base]=turnning_point[3][2];
			TEMP_DATA[104+base]=turnning_point[3][3];
			TEMP_DATA[105+base]=turnning_point[3][4];
			TEMP_DATA[106+base]=turnning_point[3][5];
			TEMP_DATA[107+base]=turnning_point[3][6];
			
			TEMP_DATA[108+base]=turnning_point[4][0];
			TEMP_DATA[109+base]=turnning_point[4][1];
			TEMP_DATA[110+base]=turnning_point[4][2];
			TEMP_DATA[111+base]=turnning_point[4][3];
			TEMP_DATA[112+base]=turnning_point[4][4];
			TEMP_DATA[113+base]=turnning_point[4][5];
			TEMP_DATA[114+base]=turnning_point[4][6];
			/*************+军刀破+************/
			//模型名
			TEMP_DATA[115+base]=Model_name[0];
			TEMP_DATA[116+base]=Model_name[1];
			TEMP_DATA[117+base]=Model_name[2];
			TEMP_DATA[118+base]=Model_name[3];
			TEMP_DATA[119+base]=Model_name[4];
			TEMP_DATA[120+base]=Model_name[5];
			TEMP_DATA[121+base]=Model_name[6];
			/*************+军刀破+************/
			
						

	sum=0;
	for(i=base;i<base+Model_length-2;i++)//计算校验和
	{
		sum+=TEMP_DATA[i];
	}
	TEMP_DATA[Model_length-2+base]=sum>>8;//写入校验和
	TEMP_DATA[Model_length-1+base]=sum;
	

	for(i=0;i<Model_length*3;i++)
	{
		EEPROM_write(Model_addrH,i,TEMP_DATA[i]);	//写入所有数据
	}
	IAP_CONTR=0;
}

/**************************************************************************/
//NRF24L01+
u8 SPI(u8 byte)
{
	u8 i;
	for(i=0;i<8;i++)
	{
		MDI=(byte&0x80);
		SCK=1;
		byte<<=1;
		byte|=MDO;
		SCK=0;
	}
	return byte;
}

void REG_write(u8 address,u8 command)
{
	CSN=0;
	SPI(0x20+address);
	SPI(command);
	CSN=1;
}

u8 REG_read(u8 address)
{
	u8 m;
	CSN=0;
	SPI(address);
	m=SPI(0);
	CSN=1;
	return m;
}

void FIFO_write(u8 DATA_OUT[],u8 lengh)
{
	u8 i;
	CSN=0;
	SPI(0xa0);
	for(i=0;i<lengh;i++)
	SPI(DATA_OUT[i]);
	CSN=1;
}
void FIFO_write2(u8 DATA_OUT[],u8 lengh)
{
	u8 i;
	CSN=0;
	SPI(0xa0);
	for(i=0;i<lengh;i++)
	SPI(DATA_OUT[i]);
	CSN=1;
}
void FIFO_read(u8 DATA_IN[],u8 lengh)		//读取接收数据缓冲区
{
	u8 i;
	CSN=0;
	SPI(0x61);	//读取命令
	for(i=0;i<lengh;i++)
	DATA_IN[i]=SPI(0);	   
	CSN=1;
}
void FIFO_read2(u8 DATA_IN[],u8 lengh)		//读取接收数据缓冲区
{
	u8 i;
	CSN=0;
	SPI(0x61);	//读取命令
	for(i=0;i<lengh;i++)
	DATA_IN[i]=SPI(0);	   
	CSN=1;
}
void TX_address(u8 DATA_IN[])
{
	CSN=0;		 
	SPI(0x20+0x10);
	SPI(DATA_IN[0]);
	SPI(DATA_IN[1]);
	SPI(DATA_IN[2]);
	SPI(DATA_IN[3]);
	SPI(DATA_IN[4]);
	CSN=1;  
}  
void RX_address(u8 DATA_IN[])
{
	CSN=0;		 
	SPI(0x20+0x0a);
	SPI(DATA_IN[0]);
	SPI(DATA_IN[1]);
	SPI(DATA_IN[2]);
	SPI(DATA_IN[3]);
	SPI(DATA_IN[4]);
	CSN=1;  
}
void RX_mode()				 
{
	CE=0;
	REG_write(0x00,0x3b); //CRC,8 __bit,Power on,RX
	CE=1;
} 				   
	
void TX_mode()				 
{
	CE=0;
	REG_write(0x00,0x0a);
  CE=1;
}

void  NRF_power(u8 P)				//发射功率设置
{
	CE=0;
	if(P==3)REG_write(0x06,0x27);		  //0db 修正之前注释错误
	else if(P==2)REG_write(0x06,0x25);	  //-6db
	else if(P==1)REG_write(0x06,0x23);	  //-12db
	else if(P==0)REG_write(0x06,0x21);    //-18db
	CE=1;
}

void NRF_size(u8 l)
{
	CE=0;
	REG_write(0x11,l);  
	CE=1;
}

void NRF_channel(u8 c)
{
	CE=0;
	REG_write(0x05,c);  
	CE=1;
}


void NRF_init()
{	
	CE=0;
	SCK=0;
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
	
	if(MDO==0){NRF_error|=0x02;}//MOSI bad
	if(MDI==0)NRF_error|=0x10;		//MISO bad
	if(IRQ==0)NRF_error|=0x08;	//IRQ bad
		if(CSN==0)NRF_error|=0x01;//CSN,CLK bad
	
	CE=0;
	SCK=0;
	CSN=0;
	
	if(SPI(0x20)!=0x0e){reset_err=1;}
	SPI(0x0a);

	CSN=1;

	CSN=0;
	SPI(0x00);

	if(SPI(0x00)!=0x0a){NRF_error|=0x02;}//MOSI bad
	CSN=1;

	REG_write(0x01,0x00);
	REG_write(0x04,0x00);
	REG_write(0x11,1);
	
	FIFO_write(tx_buff,1);
	CE=1;

	delay_ms(2);
	
	CSN=0;

	if(SPI(0x00)!=0x2e){NRF_error|=0x04;}//CE bad
	CSN=1;
		
	if(IRQ)NRF_error|=0x18;	//IRQ bad
	else 
	{
		if(NRF_error&0x04==0)NRF_error|=0x10;		//MISO bad
	}
	CE=1;
	
	if(reset_err&&NRF_error>1)NRF_error|=0x01;//CSN,CLK bad
	
	REG_write(0x07,0x20);	//清除TX中断信号
}

void device_connect()
{
	u8 n;
	
	RC_TX=0;
	 
	tx_order[0]=0xa0;
	tx_order[1]=hopping[0];
	tx_order[2]=hopping[1];
	tx_order[3]=hopping[2];
	tx_order[4]=hopping[3];
	tx_order[5]=hopping[4];
	tx_order[6]=address[0];
	tx_order[7]=address[1];
	tx_order[8]=address[2];
	tx_order[9]=address[3];
	tx_order[10]=address[4];	
		 
	NRF_channel(33);
	NRF_power(0);
	NRF_size(11);
	TX_address(address_0);
	RX_address(address_0);
	
	cancel=0;connecting=1;
	while(!cancel&connecting)					 //把对频信息发给接收机，若收到回复表面通信成功，
	{								 //收不到继续发送
		TX_mode();
		FIFO_write(tx_order,11);
		Delay1ms();
		RX_mode();
		
		while(1)
		{		
			Delay1ms();
			if(IRQ==0)
			{
				FIFO_read(rx,11);		//读取接收数据
				CE=0;
				REG_write(0x07,0x40);	//清除RX中断信号
				CE=1;	
				if(rx[0]=='O'&&rx[1]=='K')
				{
					cancel=0;connecting=0;
				}
			}
				
			n++;if(n>50){n=0;break;}
		}
	}


	RC_TX=1;
	NRF_power(TX_power);//恢复发射状态
	NRF_size(11);
	TX_address(address);
	RX_address(address); 
	 		 
}






/***********************************************/
//12864液晶操作函数
void LCD(u8 D)
{
 	DATA=D;
	CLK=0;SDA=D_7;CLK=1;
 	CLK=0;SDA=D_6;CLK=1;
 	CLK=0;SDA=D_5;CLK=1;
	CLK=0;SDA=D_4;CLK=1;
	CLK=0;SDA=D_3;CLK=1;
	CLK=0;SDA=D_2;CLK=1;
	CLK=0;SDA=D_1;CLK=1;
	CLK=0;SDA=D_0;CLK=1;
	CLK=0;
}


void location(u8 x,u8 y)
{
	RS=0;
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
	RS=1;
}

void LCD_ON()
{CS=0;}

void LCD_OFF()
{CS=1;}

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
	
	RST=0;
  delay_ms(2);
  RST=1;
  delay_ms(10);

  RS=0;
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
  RS=1;
	
  LCD_clean(0,0,128,8);
  LCD_OFF();
 
}

void send(u8 k)
{
	u8 j;
	for(j=0;j<6;j++)
   	LCD(font[k-32][j]);		
}

void send2(u8 k,u8 x,u8 y)		 //写大字母
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
void send2_rev(u8 k,u8 x, u8 y)
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

void send3(u8 *p,u8 x,u8 y)		 //写汉字
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

void write(u8 x,u8 y)			//显示汉字
{
	u8 *d;
	d=word;
	while(*d)
	{		
		send3(hzk[*d],x,y);
		d++;x+=12;
	}

}
void write0(u8 *d,u8 x,u8 y)			//显示汉字串
{

	while(*d)
	{		
		send3(hzk[*d],x,y);
		d++;x+=12;
	}

}
/*
void write2(u8 x,u8 y)			//显示大字母
{
	u8 *d;
	d=word;
	while(*d)
	{		
		send2(*d,x,y);
		d++;x+=8;
	}

}*/


void photo(u8 *p,u8 x,u8 y,u8 length,u8 width)//显示指定大小的图片
{
  u8 i,j;
  for(i=0;i<width;i++)
  {
		location(x,i+y);
		for(j=0;j<length;j++)
	  LCD(*p),p++;
  }  	
}

void display_trim1(u8 v,u8 base)//显示竖微调
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

void display_trim2(u8 v)//显示横微调
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

void display_throttle(u8 x)//显示油门
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

void warning(u8 x,u8 y,u8 W)//警告
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
		if(num>0)send2('+',x-8,y);
		else if(num<0)num=-num,send2('-',x-8,y);
		else send2(' ',x-8,y);
	}
	if(num>99)send2(num/100+48,x,y);
	else send2(' ',x,y);
	send2(num%100/10+48,x+8,y);
	if(c=='.'||c=='#')
	{
		send2('.',x+16,y);
		send2(num%10+48,x+24,y);
		if(c=='#')send2('%',x+32,y);
		
	}
	else
	{
		send2(num%10+48,x+16,y);
		if(c=='%'||c=='/')send2('%',x+24,y);
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

u8 move_cursor(u8 line,u8 line_max)//光标移动
{
	if(last_line!=line)LCD_clean(0,0,128,8);
	last_line=line;
	
	if(up)
	{
		up=0;
		if(line>0)line--;
		else line=line_max;
	}
	if(down)
	{
		down=0;
		if(line<line_max)line++;
		else line=0;
	}
	
	return line;
}

/*********+军刀破+******************
显示当前模型名称，当模型未有命名时，显示第几号模型，即当时系统所有的模型号数
*/
void displayModel()
{
			u8 i=0;
		__bit have=0;
		for(i=0;i<7;i++)
		{
			send2(Model_name[i]+32,70+i*8,3);
			if(Model_name[i]>0 && Model_name[i]<=90) have=1;
		}
	//没有命名的模型显示序号 
	if(!have)
	{
		int idx=70;
		if(Model_current<10)
		{
		send3(hzk[Model_current+81],idx,3);
		idx+=12;
		send3(hzk[68],idx,3);  //号
		idx+=12;
		send3(hzk[8],idx,3);   //模
		idx+=12;
		send3(hzk[159],idx,3); //型
		}
		else if(Model_current<19)
		{
			idx=64;
			send3(hzk[90],idx,3);
			idx+=12;
			send3(hzk[Model_current+71],idx,3);  
			idx+=12;
			send3(hzk[68],idx,3);  //号
			idx+=12;
			send3(hzk[8],idx,3);   //模
			idx+=12;
			send3(hzk[159],idx,3); //型
		}
		else
		{
			idx=64;
			send3(hzk[(Model_current+1)/10+80],idx,3);
			idx+=12;
			send3(hzk[90],idx,3);  
			idx+=12;
			send3(hzk[68],idx,3);  //号
			idx+=12;
			send3(hzk[8],idx,3);   //模
			idx+=12;
			send3(hzk[159],idx,3); //型
		}
	}
}
/*******+军刀破+******************/
	
void display_menu(u8 line,u8 line_max)//显示菜单
{
	
	if(line_max<4)send3(hzk[1],0,line+line);
	else 
	{
		if(line<3)send3(hzk[1],0,line+line);
		else if(line==line_max)send3(hzk[1],0,6);
		else send3(hzk[1],0,4);
	}
	
	if(line<3)
	{
		if(line_max==2)
		{
			write0(WW[0],12,0);
			write0(WW[1],12,2);
			write0(WW[2],12,4);
		}
		else
		{
			write0(WW[0],12,0);
			write0(WW[1],12,2);
			write0(WW[2],12,4);
			write0(WW[3],12,6);	
		}
		
	}
	else if(line==line_max)
	{
		write0(WW[line-3],12,0);
		write0(WW[line-2],12,2);
		write0(WW[line-1],12,4);
		write0(WW[line],12,6);
	}
	else if(line<line_max)
	{
		write0(WW[line-2],12,0);
		write0(WW[line-1],12,2);
		write0(WW[line],12,4);
		write0(WW[line+1],12,6);
	}
	
	
	if(line_max>=9)
	{
		location(97,7);
		if(line>=9)send('1'),send(line+49-10);
		else send(' '),send(line+49);
		
		send('/');send('1');send(line_max+49-10);
	}
	else
	{
		location(109,7);send(line+49);send('/');send(line_max+49);
	}	
		
}

void display_mapping(u8 mode,u8 x,u8 y)//显示映射对象
{
	switch(mode)
	{
		case 0:send3(hzk[96],x,y);send3(hzk[97],x+12,y);send2('1',x+24,y);break;
		case 1:send3(hzk[96],x,y);send3(hzk[97],x+12,y);send2('2',x+24,y);break;
		case 2:send3(hzk[96],x,y);send3(hzk[97],x+12,y);send2('3',x+24,y);break;
		case 3:send3(hzk[96],x,y);send3(hzk[97],x+12,y);send2('4',x+24,y);break;
		case 4:send3(hzk[100],x,y);send3(hzk[101],x+12,y);send2('1',x+24,y);break;
		case 5:send3(hzk[100],x,y);send3(hzk[101],x+12,y);send2('2',x+24,y);break;
		case 6:send3(hzk[100],x,y);send3(hzk[101],x+12,y);send2('3',x+24,y);break;
		case 7:send3(hzk[58],x,y);send3(hzk[59],x+12,y);send2('A',x+24,y);break;
		case 8:send3(hzk[58],x,y);send3(hzk[59],x+12,y);send2('B',x+24,y);break;
		case 9:send3(hzk[58],x,y);send3(hzk[59],x+12,y);send2('C',x+24,y);break;
		case 10:send3(hzk[58],x,y);send3(hzk[59],x+12,y);send2('D',x+24,y);break;
		case 11:send3(hzk[58],x,y);send3(hzk[59],x+12,y);send2('E',x+24,y);break;
		
		case 12:send3(hzk[104],x,y);send3(hzk[105],x+12,y);send2('1',x+24,y);break;
/*		case 14:send3(hzk[104],x,y);send3(hzk[105],x+12,y);send2('2',x+24,y);break;*/
		case 13:send2('P',x,y);send2('P',x+8,y);send2('M',x+16,y);send2('1',x+24,y);break;
		case 14:send2('P',x,y);send2('P',x+8,y);send2('M',x+16,y);send2('2',x+24,y);break;
		case 15:send2('P',x,y);send2('P',x+8,y);send2('M',x+16,y);send2('3',x+24,y);break;
		case 16:send2('P',x,y);send2('P',x+8,y);send2('M',x+16,y);send2('4',x+24,y);break;
		case 17:send2('P',x,y);send2('P',x+8,y);send2('M',x+16,y);send2('5',x+24,y);break;
		case 18:send2('P',x,y);send2('P',x+8,y);send2('M',x+16,y);send2('6',x+24,y);break;
		case 19:send2('P',x,y);send2('P',x+8,y);send2('M',x+16,y);send2('7',x+24,y);break;
		case 20:send2('P',x,y);send2('P',x+8,y);send2('M',x+16,y);send2('8',x+24,y);break;
	}
	
}

void display_receiver_mode(u8 mode,u8 x,u8 y)//显示接收机模式
{
	switch(mode)
	{
		case 0:send3(hzk[36],x,y);send3(hzk[37],x+12,y);break;
		case 1:send3(hzk[175],x,y);send3(hzk[180],x+12,y);break;
		case 2:send2('P',x,y);send2('W',x+8,y);send2('M',x+16,y);break;
		case 3:send2('P',x,y);send2('P',x+8,y);send2('M',x+16,y);break;
		case 4:send2('S',x,y);send2('.',x+8,y);send2('B',x+16,y);break;
	}	
}

void display_switch(u8 sw,u8 x,u8 y)//显示开关
{
	switch(sw)
	{
		case 0:send2(' ',x,y);send2('-',x+4,y);send3(hzk[10],x+12,y);send2('-',x+24,y);break;
		case 1:send3(hzk[58],x,y);send3(hzk[59],x+12,y);send2('A',x+24,y);break;
		case 2:send3(hzk[58],x,y);send3(hzk[59],x+12,y);send2('B',x+24,y);break;
		case 3:send3(hzk[58],x,y);send3(hzk[59],x+12,y);send2('C',x+24,y);break;
		case 4:send3(hzk[58],x,y);send3(hzk[59],x+12,y);send2('D',x+24,y);break;
		case 5:send3(hzk[58],x,y);send3(hzk[59],x+12,y);send2('E',x+24,y);break;
	}
}

void display_bar(int *p)//显示条形图
{
	u8 i,j;
	for(i=0;i<8;i++)
	{
		location(20,i);LCD(0x3e);LCD(0x41);
		for(j=0;j<103;j++)
		{
			
			if(j==8)LCD(0x7f);
			else if(j==51)LCD(0x7f);
			else if(j==94)LCD(0x7f);
			else if(j>51)
			{
				if(j>*p/10)LCD(0x41);
				else LCD(0x5d);
			}
			else if(j<51)
			{
				if(j>*p/10)LCD(0x5d);
				else LCD(0x41);
			}	
			
		}
		LCD(0x41);LCD(0x3e);
		p++;
	}

}

void point(u8 x,u8 z,u8 line)			  //画点
{
	u8 d0=0,d1=0,d2=0,d3=0,d4=0,d5=0,d6=0,d7=0;
	
	if(z<8)
	{
		d0=0x80;d0>>=z;			
	}
	else if(z<16)
	{
		d1=0x80;d1>>=z-8;	
	}
	else if(z<24)
	{
		d2=0x80;d2>>=z-16;	
	}
	else if(z<32)
	{
		d3=0x80;d3>>=z-24;	
	}
	else if(z<40)
	{
		d4=0x80;d4>>=z-32;	
	}
	else if(z<48)
	{
		d5=0x80;d5>>=z-40;	
	}
	else if(z<56)
	{
		d6=0x80;d6>>=z-48;	
	}
	else if(z<64)
	{
		d7=0x80;d7>>=z-56;	
	}
	
	if(x==line)
	{
		d0=0x3f,d1=0xff,d2=0xff,d3=0xff,d4=0xff,d5=0xff,d6=0xff,d7=0xf8;
	}
	if(x<3)
	{
		d0|=0x80;d7|=0x02;
	}
	else if(x>47)
	{
		d0|=0x80;d7|=0x02;
	} 
	
	
	d3|=0x01;
	
	if(x==0)
	{
		location(73,0);LCD(0x1e);LCD(0x02);
		location(73,7);LCD(0xf0);LCD(0x80);
		location(126,0);LCD(0x02);LCD(0x1e);
		location(126,7);LCD(0x80);LCD(0xf0);	
	}
	
	location(75+x,0);LCD(d7);
	location(75+x,1);LCD(d6);
	location(75+x,2);LCD(d5);
	location(75+x,3);LCD(d4);
	location(75+x,4);LCD(d3);
	location(75+x,5);LCD(d2);
	location(75+x,6);LCD(d1);
	location(75+x,7);LCD(d0);
}

void display_curve() //显示指数曲线
{
	u8 i;
		

	point(25,31,25);
	for(i=0;i<25;i++)
	{

		point(i+26,curve_buff[i]+31,25);
		point(24-i,31-curve_buff[i],25);		

	}
	
}



void display_curve2() //显示折线曲线
{
	u8 i,n;
	

	if(point_num==0)n=0;
	if(point_num==1)n=8;
	if(point_num==2)n=16;
	if(point_num==3)n=25;
	if(point_num==4)n=34;
	if(point_num==5)n=42;
	if(point_num==6)n=50;

	for(i=0;i<51;i++)
	{

		point(i,curve_buff[i]+6,n);
		
	}
	
}



/************************************************************************/
//数据运算函数
/*
void data_check(int x,int max,int min)
{
	
	if(x>max)x=max;
	if(x<min)x=min;

}
*/


void Xdata_check(int *p)//坑爹的STC Xdata，只能用指针来调用，否则会出错
{
	u8 i;
	for(i=0;i<8;i++)
	{
		if(*p>1023)*p=1023;
		if(*p<0)*p=0; 
		p++;
	}
	
}

u8 rounding(float x)//四舍五入
{
	u8 y;
	y=x;
	if((x-y)>0.5)y++;
	return y;
}

void get_curve(float index)//计算指数曲线
{
	u8 i;
	
	if(index>=0)
	{
		index=index/100+1;
		
		for(i=1;i<26;i++)
		{

			
			curve_buff[i-1]=rounding(powf(i*0.04,index)*25);

		}
	}
	else if(index<0)
	{
		index=-index/100+1;
		for(i=1;i<26;i++)
		{

			
			curve_buff[i-1]=rounding(25-powf(1-i*0.04,index)*25);

		}
	}
	
	
}

void get_curve2()//油门曲线数据
{
	
	int i;
	u8 b[7];
	for(i=0;i<7;i++)
	{
		b[i]=turnning_point[curve_num][i]/2;
	}
	
	
	for(i=0;i<51;i++)
	{
		if(i<8)
		{

			curve_buff[i]=i*(b[1]-b[0])/8+b[0];
		}
		else if(i<16)
		{

			curve_buff[i]=(i-8)*(b[2]-b[1])/8+b[1];
		}
		
		else if(i<25)
		{

			curve_buff[i]=(i-16)*(b[3]-b[2])/9+b[2];
		}
		else if(i<34)
		{

			curve_buff[i]=(i-25)*(b[4]-b[3])/9+b[3];
		}
		else if(i<42)
		{

			curve_buff[i]=(i-34)*(b[5]-b[4])/8+b[4];
		}
		else
		{

			curve_buff[i]=(i-42)*(b[6]-b[5])/8+b[5];
		}			
	}
}

void function_size(u8 num,u8 mode)//舵量函数
{
	u8 r0,r1;
	if(mode)
	{
		r0=New_ratio[0][num];
		r1=New_ratio[1][num];
	}
	else
	{
		r0=CH_ratio[0][num];
		r1=CH_ratio[1][num];
	}
	
	if(operation[num]>0)
	{
		operation[num]=(long)operation[num]*r1/100;
	}
	else
	{
		operation[num]=(long)operation[num]*r0/100;
	}
	
}

void function_inverted(u8 num)//反转函数
{
	
	if(CH_inverted[num]==1)operation[num]=-operation[num]+CH_middle[num];
	else operation[num]+=CH_middle[num];
}


void function_mix(u8 same,u8 contrary)//混控函数 
{
	int y1,y2;
	
	y1=operation[same]+operation[contrary];
	y2=operation[same]-operation[contrary];
	
	operation[same]=y1;
	operation[contrary]=y2;
			
}

void function_curve(u8 num)//指数曲线
{
	float Index;int temp;
	
	if(num==2)num++;
	if(CH_index[num]>0)
	{
		Index=(float)CH_index[num]/100+1;
	
		temp=operation[num];
		if(temp<0)temp=-temp;

		temp=powf(0.0023474*temp,Index)*426;
		
		
		if(operation[num]<0)temp=-temp;
		
		operation[num]=temp;	
		
	}
	else if(CH_index[num]<0)
	{
		Index=(float)-CH_index[num]/100+1;
		
		temp=operation[num];
		if(temp<0)temp=-temp;

		temp=426-powf(1-0.0023474*temp,Index)*426;
		
		
		if(operation[num]<0)temp=-temp;
		
		operation[num]=temp;
	}

	
	
}

void function_curve2(u8 num)//多点曲线
{
	u16 temp;
	int b[7];
	u8 i;
	for(i=0;i<7;i++)
	{
		b[i]=turnning_point[num][i]*17/2;
	}
	

	
	if(num==0)temp=operation[2]+426;
	else temp=operation[num+3]+426;

	

		if(temp<136)
		{

			temp=(long)temp*(b[1]-b[0])/136+b[0];
		}
		else if(temp<272)
		{

			temp=(long)(temp-136)*(b[2]-b[1])/136+b[1];
		}
		
		else if(temp<426)
		{

			temp=(long)(temp-272)*(b[3]-b[2])/154+b[2];
		}
		else if(temp<580)
		{

			temp=(long)(temp-426)*(b[4]-b[3])/154+b[3];
		}
		else if(temp<716)
		{

			temp=(long)(temp-580)*(b[5]-b[4])/136+b[4];
		}
		else
		{

			temp=(long)(temp-716)*(b[6]-b[5])/136+b[5];
		}			
	
	
		
		
	if(num==0)throttle=temp,operation[2]=temp-426;
	else operation[num+3]=temp-426;
		
}





void function_mapping(u8 num)//输入映射
{

	switch(IN_mapping[num])
	{
		case 0:operation[num]=CH_value[0]*5/6-426;break;
		case 1:operation[num]=CH_value[1]*5/6-426;break;
		case 2:operation[num]=CH_value[2]*5/6-426;break;
		case 3:operation[num]=CH_value[3]*5/6-426;break;
		case 4:operation[num]=adc[1]*5/6-426;break;
		case 5:operation[num]=adc[2]*5/6-426;break;
		case 6:operation[num]=adc[3]*5/6-426;break;
		
		case 7:if(Switch_A==1)operation[num]=-426;
						else if(Switch_A==2)operation[num]=0;
						else operation[num]=426;break;
		case 8:if(testljsf234099asfljl==1)operation[num]=-426;
						else if(testljsf234099asfljl==2)operation[num]=0;
						else operation[num]=426;break;
		case 9:if(Switch_C==1)operation[num]=-426;
						else if(Switch_C==2)operation[num]=0;
						else operation[num]=426;break;
		case 10:if(Switch_D==1)operation[num]=-426;
						else if(Switch_D==2)operation[num]=0;
						else operation[num]=426;break;
		case 11:if(Switch_E==1)operation[num]=-426;
						else if(Switch_E==2)operation[num]=0;
						else operation[num]=426;break;
						
		case 12:operation[num]=auto_value-426;break;				
		case 13:operation[num]=PPM_DATA[0]-426;break;
		case 14:operation[num]=PPM_DATA[1]-426;break;
		case 15:operation[num]=PPM_DATA[2]-426;break;
		case 16:operation[num]=PPM_DATA[3]-426;break;
		case 17:operation[num]=PPM_DATA[4]-426;break;
		case 18:operation[num]=PPM_DATA[5]-426;break;
		case 19:operation[num]=PPM_DATA[6]-426;break;
		case 20:operation[num]=PPM_DATA[7]-426;break;
	
	}
}

void change_trim(u8 num,u8 direction)//按键微调
{
	if(direction)
	{
		if(CH_inverted[num]==0)
		{
			if(CH_middle[num]<125)
			{
				if(Trim_step)CH_middle[num]++;
				else if(CH_middle[num]%5)CH_middle[num]+=(5-CH_middle[num]%5);
				else CH_middle[num]+=5;	
				
		//		Data_change(2);
			}
		}
		else
		{
			if(CH_middle[num]>-125)
			{
				if(Trim_step)CH_middle[num]--;
				else if(CH_middle[num]%5)CH_middle[num]-=(CH_middle[num]%5);
				else CH_middle[num]-=5;
				
		//		Data_change(2);
			}
				
			
		}	
	}
	else
	{
		if(CH_inverted[num]==0)
		{
			if(CH_middle[num]>-125)
			{
				if(Trim_step)CH_middle[num]--;
				else if(CH_middle[num]%5)CH_middle[num]-=(CH_middle[num]%5);
				else CH_middle[num]-=5;
				
		//		Data_change(2);
			}
				
			
		}
		else
		{
			if(CH_middle[num]<125)
			{
				if(Trim_step)CH_middle[num]++;
				else if(CH_middle[num]%5)CH_middle[num]+=(5-CH_middle[num]%5);
				else CH_middle[num]+=5;
				
		//		Data_change(2);
			}
				
			
		}	
	}

	
	if(CH_middle[num]==0)Message_KeyTone=2;
	CH_middle_change=num,event1=1,t1=0;
}



int CH_calib(int x,u8 offset)				//修正摇杆行程
{

	x=512+(x-512)*51/offset;

	if(x>1023)x=1023;
	if(x<0)x=0;
	return x;		
}


void function_filter(u8 num)  //ADC滤波
{
	if(num==0)
	{
		adc[0]=(adc_buff[0]+(long)adc[0]*9/10);
	}
//	else adc[num]=(adc_buff[num]*5+adc[num]*5)/10;
	else adc[num]=adc_buff[num];
}

u8 Switch_Check(u8 num)//获取开关状态
{
	switch(num)
	{
		case 1:num=Switch_A;break;
		case 2:num=testljsf234099asfljl;break;
		case 3:num=Switch_C;break;
		case 4:num=Switch_D;break;
		case 5:num=Switch_E;break;
	}
	return num;
}


/**********************************************************/

void UartInit(void)		//57600bps@12.000MHz
{
	PCON |= 0x80;		//使能波特率倍速位SMOD
	SCON = 0x50;		//8位数据,可变波特率
	AUXR |= 0x04;		//独立波特率发生器时钟为Fosc,即1T
	BRT = 0xF3;		//设定独立波特率发生器重装值
	AUXR |= 0x01;		//串口1选择独立波特率发生器为波特率发生器
	AUXR |= 0x10;		//启动独立波特率发生器
}

void initial()			 //硬件初始化
{
	ADC_CONTR=0x80;		 //ADC配置
	P1ASF=0xff;
	
	LEDA=0;
	P4SW=0x70;			 //允许P4 IO
	P2M0=0x20;			 //推挽点亮背光
	
	


 	TMOD=0x11;
	TCON=0x04;           
                
  CCAPM0=0x49;		   //PCA定时器初始化
	
	value0 += 3000;	   //3ms
	CCAP0L = value0;
    CCAP0H = value0>> 8;
	value0 += 3000;	   //3ms
	
	value1 += 20000;	   //20ms
	CCAP1L = value1;
    CCAP1H = value1>> 8;  
		value1 += 3000;	   //3ms
	
	IP=0x80;
	IPH=0x0c;			 //优先级非常重要
	
	CCON=0x40;//打开PCA定时器
	IE=0x82;
  
}	
	

main()
{
	int temp;u8 i;

	delay_ms(200);
	initial();
/*
	while(PCON&0x20)//低压判断
	{
		PCON=0;
		Delay1ms();
	}
*/	
	DATA_read1();//系统数据
	DATA_read2();//模型数据
	LCD_init();
	LEDA=LCD_light;
	
	UartInit();

	LCD_ON();
	
	if(KEY==0)//强制恢复出厂
	{
		delay_ms(3000);
		if(KEY==0)
		{
			IAP_CONTR=0x82;		//擦除所有数据
			EEPROM_clean(0);
		 	EEPROM_clean(2);
			EEPROM_clean(4);
//			EEPROM_clean(6);
			IAP_CONTR=0;
			word[0]=106,word[1]=107,word[2]=32,word[3]=136,word[4]=0,write(40,2);
			word[0]=138,word[1]=139,word[2]=140,word[3]=57,word[4]=0,write(40,4);
			while(1);
		}	
	}
	
	if(first)//首次开机
	{
		while(1)//设置
		{
			word[0]=130,word[1]=81,word[2]=131,word[3]=58,word[4]=132,word[5]=0,write(30,0);
			word[0]=141,word[1]=142,word[2]=18,word[3]=13,word[4]=0,write(0,2);
			word[0]=149,word[1]=150,word[2]=137,word[3]=109,word[4]=0,write(64,2);
			if(adc_buff[4]>510)
			{
				send3(hzk[1],0,4);send2('1',12,4);send2('2',20,4);send2('L',28,4);send2('E',36,4);
				send3(hzk[2],0,6);send2('1',12,6);send2('2',20,6);send2('C',28,6);send2(' ',36,6);
				if(KEY==0)
				{
					Battery_1s=1;
					while(!KEY);break;
				}
				
			}
			else 
			{
				send3(hzk[2],0,4);send2('1',12,4);send2('2',20,4);send2('L',28,4);send2('E',36,4);
				send3(hzk[1],0,6);send2('1',12,6);send2('2',20,6);send2('C',28,6);send2(' ',36,6);
				if(KEY==0)
				{
					Battery_1s=0;
					while(!KEY);break;
				}
			}
			delay_ms(100);

		}
		

		if(hopping[0]>125)hopping[0]=(hopping[0]-5)/2;
		if(hopping[1]>125)hopping[1]=(hopping[1]-5)/2;
		if(hopping[2]>125)hopping[2]=(hopping[2]-5)/2;
		if(hopping[3]>125)hopping[3]=(hopping[3]-5)/2;
		if(hopping[4]>125)hopping[4]=(hopping[4]-5)/2;
		
		DATA_save1();
		
		word[0]=157,word[1]=132,word[2]=12,word[3]=13,word[4]=52,word[5]=53;word[6]=0,write(0,4);
		delay_ms(500);
		send3(hzk[1],0,6);delay_ms(200);
		send3(hzk[1],12,6);delay_ms(200);
		send3(hzk[1],24,6);delay_ms(200);
		send3(hzk[1],36,6);delay_ms(200);
		send3(hzk[1],48,6);delay_ms(200);
		send3(hzk[1],60,6);delay_ms(200);

		
	}
	else if(Throttle_check)//开机安全油门
	{
		delay_ms(100);
		CH_value[2]=CH_calib(adc_buff[4],CH3_offset);
		if(CH_value[2]>80)	
			{
				while(1)
				{
					if(boot_screen==0)photo(bmp5,40,2,50,4);
					else
					{
						word[0]=141,word[1]=142,word[2]=185,word[3]=186,word[4]=0,write(38,4);
					}
				
					word[0]=147,word[1]=148,word[2]=149,word[3]=150,word[4]=151,
					word[5]=52,word[6]=153,word[7]=0,write(20,6); 
			
					delay_ms(100);
					if(KEY==0)break;
					CH_value[2]=CH_calib(adc_buff[4],CH3_offset);
					if(CH_value[2]<80)break;
				}  
			}	
		
	}
	
	
	
	
	NRF_test();
	NRF_init();
	if(NRF_error)
	{
		photo(screen2,0,0,128,8);
		if(NRF_error&0x01)location(39,5),send(133),location(106,5),send(133);
		if(NRF_error&0x02)location(83,5),send(133);
		if(NRF_error&0x04)location(29,5),send(133);
		if(NRF_error&0x08)location(64,5),send(133);
		if(NRF_error&0x10)location(49,5),send(133);
		delay_ms(2000);
	}
	else if(boot_screen_en)
	{

		if(boot_screen==0)//开机画面
		{
			photo(screen3,0,0,128,8);
			if(i<100)
			{
				delay_ms(500);
				photo(bmp4,80,4,46,4);
				location(107,6);LCD(0);LCD(0x20);LCD(0xf0);LCD(0);
				location(107,7);LCD(0xc0);LCD(0xc8);LCD(0xcf);LCD(0xc8);
				delay_ms(500);
			}
			else if(i<200)
			{
				delay_ms(1000);
				photo(bmp4,80,4,46,4);
				location(107,6);LCD(0x20);LCD(0x10);LCD(0x10);LCD(0xe0);
				location(107,7);LCD(0xcc);LCD(0xca);LCD(0xc9);LCD(0xc8);
				delay_ms(1000);
			}
			else 
			{
				delay_ms(2000);
				photo(bmp4,80,4,46,4);
				delay_ms(1500);
			}	
		}
		else if(boot_screen==1)
		{
			photo(screen4,0,0,128,8);
			location(37,7);
			for(i=0;i<50;i++)
			{
				LCD(0xf7);
				delay_ms(40);
			}		
			delay_ms(500);	
		}

		

	}



	if(PPM_OUT)
	{
		TMOD=0x11;
		EX1=0;ET1=1;P3M0=0x08;
	}
	if(PPM_IN)
	{
		TMOD=0x91;
		EX1=1;ET1=0;
	}

	auto_max=auto_end*4.25+511;
	auto_min=auto_star*4.25+511;			 
		
	
	TR1=1;
	TR0=1;
	delay_ms(50);
	CCAPM1=0x49;


	adc[0]=adc_buff[0]*10;
	
//menu=1;menu_1=1;menu_2=30;
	
	
	
	while(1)	//主函数做刷新显示、菜单处理
	{
		
		if(flag_20ms)
		{
			flag_20ms=0,tt++;
/*************************************************************/
//按键检测			
			
			if(K1)					
			{
				s1_press_time=0;
				s1_long_press=0;
				if(s1)				   
				{					   
					s1=0;			  
					key1=1;
				} 
			}
			else 
			{
				s1=1;
				s1_press_time++;
				if(s1_press_time>50)
				{
					s1_press_time=0;s1=0;
					s1_long_press=1;
				}
			}				  
	
			if(K2)					
			{
				s2_press_time=0;
				s2_long_press=0;
				if(s2)				   
				{					   
					s2=0;			  
					key2=1;
				} 
			}
			else 
			{
				s2=1;
				s2_press_time++;
				if(s2_press_time>50)
				{
					s2_press_time=0;s2=0;
					s2_long_press=1;
				}
			}
			
			if(K3)					
			{
				s3_press_time=0;
				s3_long_press=0;
				if(s3)				   
				{					   
					s3=0;			  
					key3=1;
				} 
			}
			else 
			{
				s3=1;
				s3_press_time++;
				if(s3_press_time>50)
				{
					s3_press_time=0;s3=0;
					s3_long_press=1;
				}
			}
			
			if(K4)					
			{
				s4_press_time=0;
				s4_long_press=0;
				if(s4)				   
				{					   
					s4=0;			  
					key4=1;
				} 
			}
			else 
			{
				s4=1;
				s4_press_time++;
				if(s4_press_time>50)
				{
					s4_press_time=0;s4=0;
					s4_long_press=1;
				}
			}
			
			if(K5)					
			{
				s5_press_time=0;
				s5_long_press=0;
				if(s5)				   
				{					   
					s5=0;			  
					key5=1;
				} 
			}
			else 
			{
				s5=1;
				s5_press_time++;
				if(s5_press_time>50)
				{
					s5_press_time=0;s5=0;
					s5_long_press=1;
				}
			}
			
			if(K6)					
			{
				s6_press_time=0;
				s6_long_press=0;
				if(s6)				   
				{					   
					s6=0;			  
					key6=1;
				} 
			}
			else 
			{
				s6=1;
				s6_press_time++;
				if(s6_press_time>50)
				{
					s6_press_time=0;s6=0;
					s6_long_press=1;
				}
			}
				
		
			
		}
		else
		{
			while(!flag_20ms);
		}
		
			
		if(tt==1)//按键处理
		{
			
			if(s1_long_press)
			{
				key_time=!key_time;
				if(key_time)key1=1;
			}
			if(s2_long_press)
			{
				key_time=!key_time;
				if(key_time)key2=1;
			}
						
			if(menu==0)
			{
				if(s3_long_press)
				{
					key_time=!key_time;
					if(key_time)key3=1;
				}
				if(s4_long_press)
				{
					key_time=!key_time;
					if(key_time)key4=1;
				}		
			}
			
			if(s5_long_press)
			{
				key_time=!key_time;
				if(key_time)key5=1;
			}
			if(s6_long_press)
			{
				key_time=!key_time;
				if(key_time)key6=1;
			}

			if(key1)
			{
				key1=0;Message_KeyTone=1;
				if(menu)decrease=1;
				else 
				{
					change_trim(3,0);
				}
			}
			if(key2)
			{
				key2=0;Message_KeyTone=1;
				if(menu)increase=1;
				else 
				{
					change_trim(3,1);
				}
			}
			if(key3)
			{
				key3=0;Message_KeyTone=1;
				if(menu)
					left=1;
				else 
				{
					change_trim(0,0);
					
				}
			}
			if(key4)
			{
				key4=0;Message_KeyTone=1;
				if(menu)right=1;
				else 
				{
					change_trim(0,1);
					
				}
			}
			if(key5)
			{
				key5=0;Message_KeyTone=1;
				if(menu)down=1;
				else 
				{
					change_trim(1,0);
				}
			}
			if(key6)
			{
				key6=0;Message_KeyTone=1;
				if(menu)up=1;
				else 
				{
					change_trim(1,1);
				}
			}

		}
		else if(tt==2)
		{
			LCD_ON();
			if(menu)	//主菜单			 
			{			
				if(star)
				{
					star=0;
					up=0,down=0,left=0,right=0,increase=0,decrease=0;
					LCD_clean(0,0,128,8);
				}
				
				
			
				if(menu_1)//一级菜单
				{
								
					
					if(menu_2)//二级菜单
					{
					
						
						if(menu_3)//三级菜单
						{
							
							
							
							if(menu_3==1)//新舵量
							{
								line_3=move_cursor(line_3,3);
								
								WW[0][0]=94,WW[0][1]=95,WW[0][2]=81,WW[0][3]=0;
								WW[1][0]=94,WW[1][1]=95,WW[1][2]=82,WW[1][3]=0;
								WW[2][0]=94,WW[2][1]=95,WW[2][2]=83,WW[2][3]=0;
								WW[3][0]=94,WW[3][1]=95,WW[3][2]=84,WW[3][3]=0;
								
								display_menu(line_3,3);
								
								if(increase)
								{
									increase=0;
									if(New_ratio[ratio_mode][line_3]<120)
									{
										New_ratio[ratio_mode][line_3]++;Data_change(1);
									}
									
								}
								if(decrease)
								{
									decrease=0;
									if(New_ratio[ratio_mode][line_3]>0)
									{
										New_ratio[ratio_mode][line_3]--;Data_change(1);
									}
									
								}
								
								
									write_num100(New_ratio[ratio_mode][0],70,0,'%');
									write_num100(New_ratio[ratio_mode][1],70,2,'%');
									write_num100(New_ratio[ratio_mode][2],70,4,'%');
									write_num100(New_ratio[ratio_mode][3],70,6,'%');
								
								if(ratio_mode)
								{
									send2('+',62,0);
									send2('+',62,2);
									send2('+',62,4);
									send2('+',62,6);
								}
								else
								{
									send2('-',62,0);
									send2('-',62,2);
									send2('-',62,4);
									send2('-',62,6);
								}
							
							}
							else if(menu_3==2)//组合舵量
							{
								line_3=move_cursor(line_3,5);
								
								WW[0][0]=8,WW[0][1]=9,WW[0][2]=81,WW[0][3]=0;
								WW[1][0]=8,WW[1][1]=9,WW[1][2]=82,WW[1][3]=0;
								WW[2][0]=8,WW[2][1]=9,WW[2][2]=83,WW[2][3]=0;
								WW[3][0]=8,WW[3][1]=9,WW[3][2]=84,WW[3][3]=0;
								WW[4][0]=8,WW[4][1]=9,WW[4][2]=85,WW[4][3]=0;
								WW[5][0]=8,WW[5][1]=9,WW[5][2]=86,WW[5][3]=0;
								
								display_menu(line_3,5);
								
								if(increase)
								{
									increase=0;
									if(Com_Swt_out[line_3]<100)
									{
										Com_Swt_out[line_3]++;Data_change(1);
									}
									
								}
								if(decrease)
								{
									decrease=0;
									if(Com_Swt_out[line_3]>0)
									{
										Com_Swt_out[line_3]--;Data_change(1);
									}
									
								}
								
								if(line_3<3)
								{
									write_num100(Com_Swt_out[0],70,0,'%');
									write_num100(Com_Swt_out[1],70,2,'%');
									write_num100(Com_Swt_out[2],70,4,'%');
									write_num100(Com_Swt_out[3],70,6,'%');
								}
								else if(line_3<4)
								{
									write_num100(Com_Swt_out[1],70,0,'%');
									write_num100(Com_Swt_out[2],70,2,'%');
									write_num100(Com_Swt_out[3],70,4,'%');
									write_num100(Com_Swt_out[4],70,6,'%');
								}
								else
								{
									write_num100(Com_Swt_out[2],70,0,'%');
									write_num100(Com_Swt_out[3],70,2,'%');
									write_num100(Com_Swt_out[4],70,4,'%');
									write_num100(Com_Swt_out[5],70,6,'%');
								}
									
								
							}
							
							
							
							
							if(left)left=0,menu_3=0;//,star=1;
							increase=0;decrease=0;right=0;
							
						}
						else
						{
							if(menu_2==1)//正反
							{
								line_2=move_cursor(line_2,7);
								
								WW[0][0]=94,WW[0][1]=95,WW[0][2]=81,WW[0][3]=0;
								WW[1][0]=94,WW[1][1]=95,WW[1][2]=82,WW[1][3]=0;
								WW[2][0]=94,WW[2][1]=95,WW[2][2]=83,WW[2][3]=0;
								WW[3][0]=94,WW[3][1]=95,WW[3][2]=84,WW[3][3]=0;
								WW[4][0]=94,WW[4][1]=95,WW[4][2]=85,WW[4][3]=0;
								WW[5][0]=94,WW[5][1]=95,WW[5][2]=86,WW[5][3]=0;
								WW[6][0]=94,WW[6][1]=95,WW[6][2]=87,WW[6][3]=0;
								WW[7][0]=94,WW[7][1]=95,WW[7][2]=88,WW[7][3]=0;
								display_menu(line_2,7);
								
								if(line_2<3)
								{
									if(CH_inverted[0])send3(hzk[35],60,0);
									else send3(hzk[34],60,0);
									if(CH_inverted[1])send3(hzk[35],60,2);
									else send3(hzk[34],60,2);
									if(CH_inverted[2])send3(hzk[35],60,4);
									else send3(hzk[34],60,4);
									if(CH_inverted[3])send3(hzk[35],60,6);
									else send3(hzk[34],60,6);
								}
								else if(line_2==7)
								{
									if(CH_inverted[4])send3(hzk[35],60,0);
									else send3(hzk[34],60,0);
									if(CH_inverted[5])send3(hzk[35],60,2);
									else send3(hzk[34],60,2);
									if(CH_inverted[6])send3(hzk[35],60,4);
									else send3(hzk[34],60,4);
									if(CH_inverted[7])send3(hzk[35],60,6);
									else send3(hzk[34],60,6);
								}
								else
								{
									if(CH_inverted[line_2-2])send3(hzk[35],60,0);
									else send3(hzk[34],60,0);
									if(CH_inverted[line_2-1])send3(hzk[35],60,2);
									else send3(hzk[34],60,2);
									if(CH_inverted[line_2])send3(hzk[35],60,4);
									else send3(hzk[34],60,4);
									if(CH_inverted[line_2+1])send3(hzk[35],60,6);
									else send3(hzk[34],60,6);
								}
									
									
								if(increase|decrease)
								{
									increase=0;decrease=0;
									if(CH_inverted[line_2])
									{
										CH_inverted[line_2]=0;Data_change(2);
									}
									else
									{
										CH_inverted[line_2]=1;Data_change(2);
									}
									
								}						
								
								
							}
							else if(menu_2==2)//舵量
							{
								line_2=move_cursor(line_2,7);
								
								WW[0][0]=94,WW[0][1]=95,WW[0][2]=81,WW[0][3]=0;
								WW[1][0]=94,WW[1][1]=95,WW[1][2]=82,WW[1][3]=0;
								WW[2][0]=94,WW[2][1]=95,WW[2][2]=83,WW[2][3]=0;
								WW[3][0]=94,WW[3][1]=95,WW[3][2]=84,WW[3][3]=0;
								WW[4][0]=94,WW[4][1]=95,WW[4][2]=85,WW[4][3]=0;
								WW[5][0]=94,WW[5][1]=95,WW[5][2]=86,WW[5][3]=0;
								WW[6][0]=94,WW[6][1]=95,WW[6][2]=87,WW[6][3]=0;
								WW[7][0]=94,WW[7][1]=95,WW[7][2]=88,WW[7][3]=0;
								display_menu(line_2,7);
								
								if(increase)
								{
									increase=0;
									if(CH_ratio[ratio_mode][line_2]<120)
									{
										CH_ratio[ratio_mode][line_2]++;Data_change(2);
									}
									
								}
								if(decrease)
								{
									decrease=0;
									if(CH_ratio[ratio_mode][line_2]>0)
									{
										CH_ratio[ratio_mode][line_2]--;Data_change(2);
									}
									
								}
								
								if(line_2<3)
								{
									write_num100(CH_ratio[ratio_mode][0],70,0,'%');
									write_num100(CH_ratio[ratio_mode][1],70,2,'%');
									write_num100(CH_ratio[ratio_mode][2],70,4,'%');
									write_num100(CH_ratio[ratio_mode][3],70,6,'%');
								}
								else if(line_2==7)
								{
									write_num100(CH_ratio[ratio_mode][4],70,0,'%');
									write_num100(CH_ratio[ratio_mode][5],70,2,'%');
									write_num100(CH_ratio[ratio_mode][6],70,4,'%');
									write_num100(CH_ratio[ratio_mode][7],70,6,'%');
								}
								else
								{
									write_num100(CH_ratio[ratio_mode][line_2-2],70,0,'%');
									write_num100(CH_ratio[ratio_mode][line_2-1],70,2,'%');
									write_num100(CH_ratio[ratio_mode][line_2],70,4,'%');
									write_num100(CH_ratio[ratio_mode][line_2+1],70,6,'%');
								}
								
								if(ratio_mode)
								{
									send2('+',62,0);
									send2('+',62,2);
									send2('+',62,4);
									send2('+',62,6);
								}
								else
								{
									send2('-',62,0);
									send2('-',62,2);
									send2('-',62,4);
									send2('-',62,6);
								}
								
								
							}
							else if(menu_2==3)//输入映射
							{
								line_2=move_cursor(line_2,7);
								
								WW[0][0]=94,WW[0][1]=95,WW[0][2]=81,WW[0][3]=0;
								WW[1][0]=94,WW[1][1]=95,WW[1][2]=82,WW[1][3]=0;
								WW[2][0]=94,WW[2][1]=95,WW[2][2]=83,WW[2][3]=0;
								WW[3][0]=94,WW[3][1]=95,WW[3][2]=84,WW[3][3]=0;
								WW[4][0]=94,WW[4][1]=95,WW[4][2]=85,WW[4][3]=0;
								WW[5][0]=94,WW[5][1]=95,WW[5][2]=86,WW[5][3]=0;
								WW[6][0]=94,WW[6][1]=95,WW[6][2]=87,WW[6][3]=0;
								WW[7][0]=94,WW[7][1]=95,WW[7][2]=88,WW[7][3]=0;
								display_menu(line_2,7);
								
								send3(hzk[4],60,0);
								send3(hzk[4],60,2);
								send3(hzk[4],60,4);
								send3(hzk[4],60,6);
								
								if(increase)
								{
									increase=0;
									if(IN_mapping[line_2]<20)IN_mapping[line_2]++,Data_change(2);
								}
								if(decrease)
								{
									decrease=0;
									if(IN_mapping[line_2]>0)IN_mapping[line_2]--,Data_change(2);
								}
								
								if(line_2<3)
								{
									display_mapping(IN_mapping[0],72,0);
									display_mapping(IN_mapping[1],72,2);
									display_mapping(IN_mapping[2],72,4);
									display_mapping(IN_mapping[3],72,6);
								}
								else if(line_2==7)
								{
									display_mapping(IN_mapping[4],72,0);
									display_mapping(IN_mapping[5],72,2);
									display_mapping(IN_mapping[6],72,4);
									display_mapping(IN_mapping[7],72,6);
								}
								else 
								{
									display_mapping(IN_mapping[line_2-2],72,0);
									display_mapping(IN_mapping[line_2-1],72,2);
									display_mapping(IN_mapping[line_2],72,4);
									display_mapping(IN_mapping[line_2+1],72,6);
								}
								
								
								
								
							}
							else if(menu_2==4)//微调
							{
								line_2=move_cursor(line_2,7);
								
								WW[0][0]=94,WW[0][1]=95,WW[0][2]=81,WW[0][3]=0;
								WW[1][0]=94,WW[1][1]=95,WW[1][2]=82,WW[1][3]=0;
								WW[2][0]=94,WW[2][1]=95,WW[2][2]=83,WW[2][3]=0;
								WW[3][0]=94,WW[3][1]=95,WW[3][2]=84,WW[3][3]=0;
								WW[4][0]=94,WW[4][1]=95,WW[4][2]=85,WW[4][3]=0;
								WW[5][0]=94,WW[5][1]=95,WW[5][2]=86,WW[5][3]=0;
								WW[6][0]=94,WW[6][1]=95,WW[6][2]=87,WW[6][3]=0;
								WW[7][0]=94,WW[7][1]=95,WW[7][2]=88,WW[7][3]=0;
								display_menu(line_2,7);
								
								if(increase)
								{
									increase=0;
									if(CH_middle[line_2]<100)
									{
										CH_middle[line_2]++;Data_change(2);
									}
									
								}
								if(decrease)
								{
									decrease=0;
									if(CH_middle[line_2]>-100)
									{
										CH_middle[line_2]--;Data_change(2);
									}
									
								}
								
								if(line_2<3)
								{
									write_num100(CH_middle[0],70,0,'-');
									write_num100(CH_middle[1],70,2,'-');
									write_num100(CH_middle[2],70,4,'-');
									write_num100(CH_middle[3],70,6,'-');
								}
								else if(line_2==7)
								{
									write_num100(CH_middle[4],70,0,'-');
									write_num100(CH_middle[5],70,2,'-');
									write_num100(CH_middle[6],70,4,'-');
									write_num100(CH_middle[7],70,6,'-');
								}
								else
								{
									write_num100(CH_middle[line_2-2],70,0,'-');
									write_num100(CH_middle[line_2-1],70,2,'-');
									write_num100(CH_middle[line_2],70,4,'-');
									write_num100(CH_middle[line_2+1],70,6,'-');
								}
			
							}
							else if(menu_2==5)//混控
							{
								line_2=move_cursor(line_2,2);
								
								WW[0][0]=91,WW[0][1]=79,WW[0][2]=92,WW[0][3]=0;
								WW[1][0]=93,WW[1][1]=79,WW[1][2]=92,WW[1][3]=0;
								WW[2][0]=106,WW[2][1]=107,WW[2][2]=108,WW[2][3]=109;WW[2][4]=0;
								display_menu(line_2,2);
								
								if(mixer_name==1)
								{
									word[0]=110;word[1]=111;word[2]=112;word[3]=0;write(64,4);
								}
								else if(mixer_name==2)
								{
									word[0]=113;word[1]=114;word[2]=0;write(64,4);
								}
								else if(mixer_name==3)
								{
									send2('V',64,4);send3(hzk[115],72,4);
								}
								else if(mixer_name==4)
								{
									word[0]=116;word[1]=117;word[2]=112;word[3]=0;write(64,4);
								}
								
								
								
								if(line_2<1)
								{
									
									if(increase)
									{
										increase=0;
										if(mixer_name==1)
										{
											if(mixer1_same<7)mixer1_same++,Data_change(2);
										}
										else if(mixer_name==2)
										{
											if(mixer2_same<7)mixer2_same++,Data_change(2);
										}
										else if(mixer_name==3)
										{
											if(mixer3_same<7)mixer3_same++,Data_change(2);
										}
										else if(mixer_name==4)
										{
											if(mixer4_same<7)mixer4_same++,Data_change(2);
										}
									}
									if(decrease)
									{
										decrease=0;
										if(mixer_name==1)
										{
											if(mixer1_same>0)mixer1_same--,Data_change(2);
										}
										else if(mixer_name==2)
										{
											if(mixer2_same>0)mixer2_same--,Data_change(2);
										}
										else if(mixer_name==3)
										{
											if(mixer3_same>0)mixer3_same--,Data_change(2);
										}
										else if(mixer_name==4)
										{
											if(mixer4_same>0)mixer4_same--,Data_change(2);
										}
									}
									
								}
								else if(line_2<2)
								{
								if(increase)
									{
										increase=0;
										if(mixer_name==1)
										{
											if(mixer1_contrary<7)mixer1_contrary++,Data_change(2);
										}
										else if(mixer_name==2)
										{
											if(mixer2_contrary<7)mixer2_contrary++,Data_change(2);
										}
										else if(mixer_name==3)
										{
											if(mixer3_contrary<7)mixer3_contrary++,Data_change(2);
										}
										else if(mixer_name==4)
										{
											if(mixer4_contrary<7)mixer4_contrary++,Data_change(2);
										}
									}
									if(decrease)
									{
										decrease=0;
										if(mixer_name==1)
										{
											if(mixer1_contrary>0)mixer1_contrary--,Data_change(2);
										}
										else if(mixer_name==2)
										{
											if(mixer2_contrary>0)mixer2_contrary--,Data_change(2);
										}
										else if(mixer_name==3)
										{
											if(mixer3_contrary>0)mixer3_contrary--,Data_change(2);
										}
										else if(mixer_name==4)
										{
											if(mixer4_contrary>0)mixer4_contrary--,Data_change(2);
										}
									}													
									
								}
								else if(line_2<3)
								{
								if(right)
									{
										right=0;
										if(mixer_name==1)
										{
											mixer1_same=1;mixer1_contrary=0;Data_change(2);
										}
										else if(mixer_name==2)
										{
											mixer2_same=2;mixer2_contrary=3;Data_change(2);
										}
										else if(mixer_name==3)
										{
											mixer3_same=0;mixer3_contrary=3;Data_change(2);
										}
										else if(mixer_name==4)
										{
											mixer4_same=5;mixer4_contrary=0;Data_change(2);
										}
									}
								}	
									
										
								
								if(mixer_name==1)
								{
									send2(mixer1_same+49,60,0);
									send2(mixer1_contrary+49,60,2);
								}
								else if(mixer_name==2)
								{
									send2(mixer2_same+49,60,0);
									send2(mixer2_contrary+49,60,2);
								}
								else if(mixer_name==3)
								{
									send2(mixer3_same+49,60,0);
									send2(mixer3_contrary+49,60,2);
								}
								else if(mixer_name==4)
								{
									send2(mixer4_same+49,60,0);
									send2(mixer4_contrary+49,60,2);
								}
								
							}
							else if(menu_2==6)//失控舵量
							{
								line_2=move_cursor(line_2,7);
								
								WW[0][0]=29,WW[0][1]=30,WW[0][2]=81,WW[0][3]=0;
								WW[1][0]=29,WW[1][1]=30,WW[1][2]=82,WW[1][3]=0;
								WW[2][0]=29,WW[2][1]=30,WW[2][2]=83,WW[2][3]=0;
								WW[3][0]=29,WW[3][1]=30,WW[3][2]=84,WW[3][3]=0;
								WW[4][0]=29,WW[4][1]=30,WW[4][2]=85,WW[4][3]=0;
								WW[5][0]=29,WW[5][1]=30,WW[5][2]=86,WW[5][3]=0;
								WW[6][0]=29,WW[6][1]=30,WW[6][2]=87,WW[6][3]=0;
								WW[7][0]=29,WW[7][1]=30,WW[7][2]=88,WW[7][3]=0;
								display_menu(line_2,7);
								
								if(increase)
								{
									increase=0;
									if(out_control[line_2]<120)
									{
										out_control[line_2]++;Data_change(2);config_change=1;
										
									}
									
								}
								if(decrease)
								{
									decrease=0;
									if(out_control[line_2]>-120)
									{
										out_control[line_2]--;Data_change(2);config_change=1;
										
									}
									
								}
								
								if(config_change)
								{
									config_change=0;
									out_control_data[0]=(int)out_control[0]*4.25+511;
									out_control_data[1]=(int)out_control[1]*4.25+511;
									out_control_data[2]=(int)out_control[2]*4.25+511;
									out_control_data[3]=(int)out_control[3]*4.25+511;
									out_control_data[4]=(int)out_control[4]*4.25+511;
									out_control_data[5]=(int)out_control[5]*4.25+511;
									out_control_data[6]=(int)out_control[6]*4.25+511;
									out_control_data[7]=(int)out_control[7]*4.25+511;
									tx_order[1]=out_control_data[0]>>2;
									tx_order[2]=out_control_data[0]<<6;
									tx_order[2]+=out_control_data[1]>>4;
									tx_order[3]=out_control_data[1]<<4;
									tx_order[3]+=out_control_data[2]>>6;
									tx_order[4]=out_control_data[2]<<2;
									tx_order[4]+=out_control_data[3]>>8;
									tx_order[5]=out_control_data[3];

									tx_order[6]=out_control_data[4]>>2;
									tx_order[7]=out_control_data[4]<<6;
									tx_order[7]+=out_control_data[5]>>4;
									tx_order[8]=out_control_data[5]<<4;
									tx_order[8]+=out_control_data[6]>>6;
									tx_order[9]=out_control_data[6]<<2;
									tx_order[9]+=out_control_data[7]>>8;
									tx_order[10]=out_control_data[7];
									
									
									NRF_config_change=1;
								}
								
								if(line_2<3)
								{
									write_num100(out_control[0],70,0,'/');
									write_num100(out_control[1],70,2,'/');
									write_num100(out_control[2],70,4,'/');
									write_num100(out_control[3],70,6,'/');
								}
								else if(line_2==7)
								{
									write_num100(out_control[4],70,0,'/');
									write_num100(out_control[5],70,2,'/');
									write_num100(out_control[6],70,4,'/');
									write_num100(out_control[7],70,6,'/');
								}
								else
								{
									write_num100(out_control[line_2-2],70,0,'/');
									write_num100(out_control[line_2-1],70,2,'/');
									write_num100(out_control[line_2],70,4,'/');
									write_num100(out_control[line_2+1],70,6,'/');
								}
								
							}
							else if(menu_2==7)//通信密码
							{
								line_2=move_cursor(line_2,4);
								
								WW[0][0]=48,WW[0][1]=49,WW[0][2]=81,WW[0][3]=0;
								WW[1][0]=48,WW[1][1]=49,WW[1][2]=82,WW[1][3]=0;
								WW[2][0]=48,WW[2][1]=49,WW[2][2]=83,WW[2][3]=0;
								WW[3][0]=48,WW[3][1]=49,WW[3][2]=84,WW[3][3]=0;
								WW[4][0]=48,WW[4][1]=49,WW[4][2]=85,WW[4][3]=0;

								display_menu(line_2,4);
								if(line_2<3)
								{
									send2_hex((u8)(address[0]>>4),60,0);send2_hex((u8)(address[0]&0x0f),68,0);
									send2_hex((u8)(address[1]>>4),60,2);send2_hex((u8)(address[1]&0x0f),68,2);
									send2_hex((u8)(address[2]>>4),60,4);send2_hex((u8)(address[2]&0x0f),68,4);
									send2_hex((u8)(address[3]>>4),60,6);send2_hex((u8)(address[3]&0x0f),68,6);
								}
								else 
								{
									send2_hex((u8)(address[1]>>4),60,0);send2_hex((u8)(address[1]&0x0f),68,0);
									send2_hex((u8)(address[2]>>4),60,2);send2_hex((u8)(address[2]&0x0f),68,2);
									send2_hex((u8)(address[3]>>4),60,4);send2_hex((u8)(address[3]&0x0f),68,4);
									send2_hex((u8)(address[4]>>4),60,6);send2_hex((u8)(address[4]&0x0f),68,6);

								}
								
								if(increase)
								{
									increase=0;
									address[line_2]++;Data_change(1),NRF_change=1;
								}
								if(decrease)
								{
									decrease=0;
									address[line_2]--;Data_change(1),NRF_change=1;
								}
								
							}
							else if(menu_2==8)//跳频
							{
								line_2=move_cursor(line_2,4);
								
								WW[0][0]=53,WW[0][1]=51,WW[0][2]=81,WW[0][3]=0;
								WW[1][0]=53,WW[1][1]=51,WW[1][2]=82,WW[1][3]=0;
								WW[2][0]=53,WW[2][1]=51,WW[2][2]=83,WW[2][3]=0;
								WW[3][0]=53,WW[3][1]=51,WW[3][2]=84,WW[3][3]=0;
								WW[4][0]=53,WW[4][1]=51,WW[4][2]=85,WW[4][3]=0;
								display_menu(line_2,4);
								
								if(line_2<3)
								{
									write_num100(hopping[0],60,0,0);
									write_num100(hopping[1],60,2,0);
									write_num100(hopping[2],60,4,0);
									write_num100(hopping[3],60,6,0);
								}
								else 
								{
									write_num100(hopping[1],60,0,0);
									write_num100(hopping[2],60,2,0);
									write_num100(hopping[3],60,4,0);
									write_num100(hopping[4],60,6,0);
								}
								
								if(increase)
								{
									increase=0;
									if(hopping[line_2]<125)hopping[line_2]++;Data_change(1);
								}
								if(decrease)
								{
									decrease=0;
									if(hopping[line_2]>0)hopping[line_2]--;Data_change(1);
								}
								
							}
							else if(menu_2==9)//通道映射
							{
								line_2=move_cursor(line_2,7);
								
								WW[0][0]=16,WW[0][1]=17,WW[0][2]=81,WW[0][3]=0;
								WW[1][0]=16,WW[1][1]=17,WW[1][2]=82,WW[1][3]=0;
								WW[2][0]=16,WW[2][1]=17,WW[2][2]=83,WW[2][3]=0;
								WW[3][0]=16,WW[3][1]=17,WW[3][2]=84,WW[3][3]=0;
								WW[4][0]=16,WW[4][1]=17,WW[4][2]=85,WW[4][3]=0;
								WW[5][0]=16,WW[5][1]=17,WW[5][2]=86,WW[5][3]=0;
								WW[6][0]=16,WW[6][1]=17,WW[6][2]=87,WW[6][3]=0;
								WW[7][0]=16,WW[7][1]=17,WW[7][2]=88,WW[7][3]=0;
								display_menu(line_2,7);
								
								word[0]=4;word[1]=94;word[2]=95;word[3]=0;write(60,0);
								write(60,2);write(60,4);write(60,6);
								
								if(increase)
								{
									increase=0;
									if(NRF_mapping[line_2]<7)NRF_mapping[line_2]++;Data_change(2);
								}
								if(decrease)
								{
									decrease=0;
									if(NRF_mapping[line_2]>0)NRF_mapping[line_2]--;Data_change(2);
								}
								
								if(line_2<3)
								{
									send2(NRF_mapping[0]+49,96,0);
									send2(NRF_mapping[1]+49,96,2);
									send2(NRF_mapping[2]+49,96,4);
									send2(NRF_mapping[3]+49,96,6);
								}
								else if(line_2==7)
								{
									send2(NRF_mapping[4]+49,96,0);
									send2(NRF_mapping[5]+49,96,2);
									send2(NRF_mapping[6]+49,96,4);
									send2(NRF_mapping[7]+49,96,6);
								}
								else 
								{
									send2(NRF_mapping[line_2-2]+49,96,0);
									send2(NRF_mapping[line_2-1]+49,96,2);
									send2(NRF_mapping[line_2]+49,96,4);
									send2(NRF_mapping[line_2+1]+49,96,6);
								}

							}
							else if(menu_2==10)//版本信息
							{
								photo(bmp0,4,0,120,3);
								send3(hzk[73],56,6);send3(hzk[74],68,6);
								send2('V',80,6);send2('3',88,6);send2('.',96,6);
								send2('A',104,6);send2('1',112,6);send2('2',120,6);
								
							}
							else if(menu_2==11)//对频
							{
								word[0]=121;word[1]=122;word[2]=43;word[3]=44;word[4]=123;word[5]=0;write(24,0);
								send2('.',84,0);send2('.',92,0);send2('.',100,0);
								
															
								device_connect();

								if(cancel)
								{
									word[0]=16;word[1]=47;word[2]=126;word[3]=127;word[4]=0;write(40,4);
									menu_2=0;cancel=0;
								}
								else
								{
									word[0]=16;word[1]=47;word[2]=124;word[3]=125;word[4]=0;write(40,4);
									menu_2=0;menu=0;
								}	
								delay_ms(500);star=1;

							}
							else if(menu_2==12)//恢复出厂
							{
								word[0]=106,word[1]=107,word[2]=32,word[3]=136,word[4]=0,write(40,2);
								word[0]=137,word[1]=109,word[2]=0,write(52,4);send2('?',80,4);
								if(right)
								{								
									IAP_CONTR=0x83;		//擦除所有数据
									EEPROM_clean(0);
									EEPROM_clean(2);
									EEPROM_clean(4);
									IAP_CONTR=0;
									send3(hzk[3],80,4);
									word[0]=138,word[1]=139,word[2]=140,word[3]=57,word[4]=0,write(40,6);
									TR0=0,TR1=0,CCON=0;while(1);
								}
							}
							else if(menu_2==13)//回传报警
							{
								line_2=move_cursor(line_2,5);
								
								WW[0][0]=160,WW[0][1]=161,WW[0][2]=0;
								WW[1][0]=58,WW[1][1]=59,WW[1][2]=0;
								WW[2][0]=105,WW[2][1]=206,WW[2][2]=0;
								WW[3][0]=207,WW[3][1]=208,WW[3][2]=0;
								WW[4][0]=29,WW[4][1]=30,WW[4][2]=0;
								WW[5][0]=66,WW[5][1]=67,WW[5][2]=0;
								
								display_menu(line_2,5);
								
								
								if(line_2<3)
								{
									if(Switch_Check(Return_alarm_switch)==3)send3(hzk[58],64,0);
									else send3(hzk[59],64,0);
								}
								
								
								
								if(line_2==1)
								{
									if(increase)
									{
										increase=0;
										if(Return_alarm_switch<5)Return_alarm_switch++,Data_change(1);
										
									}
									if(decrease)
									{
										decrease=0;
										if(Return_alarm_switch>0)Return_alarm_switch--,Data_change(1);

									}
									
									display_switch(Return_alarm_switch,60,2);
								}
								else if(line_2==2)
								{
									if(increase)
									{
										increase=0;
										if(Power_V<200)
										{
											Power_V++,Data_change(1);
										}
									}
									if(decrease)
									{
										decrease=0;
										if(Power_V>0)
										{
											Power_V--,Data_change(1);
										}
									}	

									write_num100(Power_V,60,4,'.');send2('V',92,4);
									
								}
								else if(line_2==3)
								{
									if(increase)
									{
										increase=0;
										if(Chip_V<50)
										{
											Chip_V++,Data_change(1);
										}
									}
									if(decrease)
									{
										decrease=0;
										if(Chip_V>0)
										{
											Chip_V--,Data_change(1);
										}
									}

									write_num100(Chip_V,60,4,'.');send2('V',92,4);									
									
								}
								else if(line_2==4)
								{
									if(increase)
									{
										increase=0;
										if(TX_min<100)
										{
											TX_min++,Data_change(1);
										}
									}
									if(decrease)
									{
										decrease=0;
										if(TX_min>0)
										{
											TX_min--,Data_change(1);
										}
									}

									write_num100(TX_min,68,4,'%');
									
								}
								else if(line_2==5)
								{
									if(increase)
									{
										increase=0;
										if(RX_min<100)
										{
											RX_min++,Data_change(1);
										}
									}
									if(decrease)
									{
										decrease=0;
										if(RX_min>0)
										{
											RX_min--,Data_change(1);
										}
									}

									write_num100(RX_min,68,6,'%');
									
								}
								
								
							}
							else if(menu_2==14)//显示设置
							{
								line_2=move_cursor(line_2,3);
								
								WW[0][0]=187,WW[0][1]=188,WW[0][2]=103,WW[0][3]=0;
								WW[1][0]=178,WW[1][1]=179,WW[1][2]=64,WW[1][3]=0;
								WW[2][0]=60,WW[2][1]=61,WW[2][2]=0;
								WW[3][0]=213,WW[3][1]=79,WW[3][2]=0;							
								
								display_menu(line_2,3);
								
								write_num100(LCD_vop,60,0,0);
								if(LCD_big)send3(hzk[178],68,2);
								else send3(hzk[179],68,2);
								
								if(LCD_light)send3(hzk[58],68,4);
								else send3(hzk[59],68,4);
								if(LCD_reverse)send3(hzk[35],68,6);
								else send3(hzk[34],68,6);
								
								if(line_2==0)
								{
									if(increase)
									{
										increase=0;
										if(LCD_vop<60)
										{
											LCD_vop++;Data_change(1);
											
											RS=0;
											LCD(0x81);
											LCD(LCD_vop);
											RS=1;
										}
									}
									
									if(decrease)
									{
										decrease=0;
										if(LCD_vop>10)
										{
											LCD_vop--;Data_change(1);
											
											RS=0;
											LCD(0x81);
											LCD(LCD_vop);
											RS=1;
										}
									}								
									
								}
								else if(line_2==1)
								{
									if(increase|decrease)
									{
										increase=0;decrease=0;

										LCD_big=!LCD_big;Data_change(1);
										RS=0;
										if(LCD_big)LCD(0x60);
										else LCD(0x40);
										RS=1;

									}																			
									
								}
								else if(line_2==2)
								{
									if(increase|decrease)
									{
										increase=0;decrease=0;Data_change(1);

											LCD_light=!LCD_light;LEDA=LCD_light;
									

									}																		
									
								}
								else
								{
									if(increase|decrease)
									{
										increase=0;decrease=0;

										LCD_reverse=!LCD_reverse;Data_change(1);
										RS=0;
										if(LCD_reverse)LCD(0xc0),LCD(0xa1);
										else LCD(0xc8),LCD(0xa0);;
										RS=1;
										
										star=1;

									}
								}
							
							}
							else if(menu_2==15)//PPM映射
							{
								line_2=move_cursor(line_2,7);
								
								WW[0][0]=192,WW[0][1]=193,WW[0][2]=81,WW[0][3]=0;
								WW[1][0]=192,WW[1][1]=193,WW[1][2]=82,WW[1][3]=0;
								WW[2][0]=192,WW[2][1]=193,WW[2][2]=83,WW[2][3]=0;
								WW[3][0]=192,WW[3][1]=193,WW[3][2]=84,WW[3][3]=0;
								WW[4][0]=192,WW[4][1]=193,WW[4][2]=85,WW[4][3]=0;
								WW[5][0]=192,WW[5][1]=193,WW[5][2]=86,WW[5][3]=0;
								WW[6][0]=192,WW[6][1]=193,WW[6][2]=87,WW[6][3]=0;
								WW[7][0]=192,WW[7][1]=193,WW[7][2]=88,WW[7][3]=0;
								display_menu(line_2,7);
								
								word[0]=4;word[1]=94;word[2]=95;word[3]=0;write(60,0);
								write(60,2);write(60,4);write(60,6);
								
								if(increase)
								{
									increase=0;
									if(PPM_mapping[line_2]<7)PPM_mapping[line_2]++;Data_change(1);
								}
								if(decrease)
								{
									decrease=0;
									if(PPM_mapping[line_2]>0)PPM_mapping[line_2]--;Data_change(1);
								}
								
								if(line_2<3)
								{
									send2(PPM_mapping[0]+49,96,0);
									send2(PPM_mapping[1]+49,96,2);
									send2(PPM_mapping[2]+49,96,4);
									send2(PPM_mapping[3]+49,96,6);
								}
								else if(line_2==7)
								{
									send2(PPM_mapping[4]+49,96,0);
									send2(PPM_mapping[5]+49,96,2);
									send2(PPM_mapping[6]+49,96,4);
									send2(PPM_mapping[7]+49,96,6);
								}
								else 
								{
									send2(PPM_mapping[line_2-2]+49,96,0);
									send2(PPM_mapping[line_2-1]+49,96,2);
									send2(PPM_mapping[line_2]+49,96,4);
									send2(PPM_mapping[line_2+1]+49,96,6);
								}
							}
							else if(menu_2==16)//大小舵
							{
								line_2=move_cursor(line_2,3);
								
								WW[0][0]=160,WW[0][1]=161,WW[0][2]=0;
								WW[1][0]=58,WW[1][1]=59,WW[1][2]=0;
								WW[2][0]=202,WW[2][1]=34,WW[2][2]=36,WW[2][3]=37;
								WW[3][0]=202,WW[3][1]=134,WW[3][2]=36,WW[3][3]=37;
								
								display_menu(line_2,3);
								
								
									if(use_new_ratio)send3(hzk[202],60,0),send3(hzk[36],72,0),send3(hzk[37],84,0);
									else send3(hzk[201],60,0),send3(hzk[36],72,0),send3(hzk[37],84,0);
								
								display_switch(ratio_switch,60,2);
								
								if(line_2==1)
								{
									if(increase)
									{
										increase=0;
										if(ratio_switch<5)ratio_switch++,Data_change(1);
										
									}
									if(decrease)
									{
										decrease=0;
										if(ratio_switch>0)ratio_switch--,Data_change(1);

									}
								}
								if(line_2==2)
								{
									if(right)
									{
										right=0;menu_3=1,line_3=0,ratio_mode=1;
										
									}
								}
								if(line_2==3)
								{
									if(right)
									{
										right=0;menu_3=1,line_3=0,ratio_mode=0;
										
									}
								}
								
								
						
							}
							else if(menu_2==17)//原始数据
							{
								for(i=0;i<8;i++)
								{
									location(0,i);
									send('A');send('D');send('1'+i);send(':');
									write_num1000(adc[7-i],0);
								}
								
								
															
							}
							else if(menu_2==18)//无线协议
							{
								line_2=move_cursor(line_2,2);
								
								WW[0][0]=65,WW[0][1]=79,WW[0][2]=50,WW[0][3]=53,WW[0][4]=51,WW[0][5]=0;
								WW[1][0]=66,WW[1][1]=67,WW[1][2]=65,WW[1][3]=53,WW[1][4]=51,WW[1][5]=0;
								WW[2][0]=66,WW[2][1]=67,WW[2][2]=50,WW[2][3]=53,WW[2][4]=51,WW[2][5]=0;
								
	//							WW[3][0]=69,WW[3][1]=70,WW[3][2]=71,WW[3][3]=72,WW[3][4]=0,WW[3][5]=0;
								
								display_menu(line_2,2);
								
								if(right)
								{
									right=0;
									TX_protocol=line_2+1;Data_change(1);
								}
								
								if(TX_protocol==1)send3(hzk[3],80,0);
								if(TX_protocol==2)send3(hzk[3],80,2);
								if(TX_protocol==3)send3(hzk[3],80,4);
							}
							else if(menu_2==19)//全频扫描
							{
								
								word[0]=76;word[1]=53;word[2]=77;word[3]=78;word[4]=0;write(0,0);
								location(54,0);send('2');send('4');send('0');send('0');send('-');
								send('2');send('5');send('2');send('5');send('M');send('h');send('z');
								
								location(0,2);LCD(0xff);
								location(0,4);LCD(0xff);
								location(0,6);LCD(0xff);
								location(127,2);LCD(0xff);
								location(127,4);LCD(0xff);
								location(127,6);LCD(0xff);
								
								location(63,3);LCD(0x3f);LCD(0x3f);
								location(63,5);LCD(0x3f);LCD(0x3f);
								location(63,7);LCD(0x3f);LCD(0x3f);
								location(31,3);LCD(0x0f);
								location(31,5);LCD(0x0f);
								location(31,7);LCD(0x0f);
								location(95,3);LCD(0x0f);
								location(95,5);LCD(0x0f);
								location(95,7);LCD(0x0f);
								location(0,3);send('0');
								location(116,3);send('4');send('1');
								location(0,5);send('4');send('2');
								location(116,5);send('8');send('3');
											location(0,7);send('8');send('4');
								location(110,7);send('1');send('2');send('5');
							
								
								
								for(i=0;i<126;i++)
								{
									if(i==0)location(1,2);
									else if(i==42)location(1,4);
									else if(i==84)location(1,6);
									
									if(CD_data[i/8]&(1<<(i%8)))LCD(0xbd),LCD(0xbd),LCD(0xbd);
									else LCD(0x81),LCD(0x81),LCD(0x81);
									
								}

								
								if(left)left=0,menu_2=0,star=1,RC_check=0;
							}
							else if(menu_2==20)//摇杆校准
							{
								
								
								if(CH1_max<adc[6])CH1_max=adc[6];		 //获取最大值
								if(CH2_max<adc[7])CH2_max=adc[7];
								if(CH3_max<adc[4])CH3_max=adc[4];
								if(CH4_max<adc[5])CH4_max=adc[5];
								
								if(CH1_min>adc[6])CH1_min=adc[6];		 //获取最小值
								if(CH2_min>adc[7])CH2_min=adc[7];
								if(CH3_min>adc[4])CH3_min=adc[4];
								if(CH4_min>adc[5])CH4_min=adc[5];

								location(0,0);						  //显示数值
								send('C');send('H');send('1');send(' ');send('<');write_num1000(CH1_min,0);
								send(' ');send('-');send(' ');write_num1000(CH1_max,0);send('>');
								location(0,1);
								send('C');send('H');send('2');send(' ');send('<');write_num1000(CH2_min,0);
								send(' ');send('-');send(' ');write_num1000(CH2_max,0);send('>');
								location(0,2);
								send('C');send('H');send('3');send(' ');send('<');write_num1000(CH3_min,0);
								send(' ');send('-');send(' ');write_num1000(CH3_max,0);send('>');
								location(0,3);
								send('C');send('H');send('4');send(' ');send('<');write_num1000(CH4_min,0);
								send(' ');send('-');send(' ');write_num1000(CH4_max,0);send('>');

								
								
								if(right)
								{
									right=0;
									calibration_action=1;
									
									if(512-CH1_min<CH1_max-512)CH1_offset=(512-CH1_min)/10;//获取最小偏差
									else CH1_offset=(CH1_max-512)/10;
									if(512-CH2_min<CH2_max-512)CH2_offset=(512-CH2_min)/10;
									else CH2_offset=(CH2_max-512)/10;
									if(512-CH3_min<CH3_max-512)CH3_offset=(512-CH3_min)/10;
									else CH3_offset=(CH3_max-512)/10;
									if(512-CH4_min<CH4_max-512)CH4_offset=(512-CH4_min)/10;
									else CH4_offset=(CH4_max-512)/10;
									
									Data_change(1);
									
							
								}
								
								send3(hzk[209],24,6);send3(hzk[210],36,6);
								send3(hzk[98],48,6);send3(hzk[99],60,6);
								
								if(calibration_action)
								{
									
									send3(hzk[3],84,6);	
						
								
								}
								else send2('?',84,6);	
								
																			
								
								
						

							}
							else if(menu_2==21)//PPM输入显示
							{
								location(0,0);send('I');send('N');send('1');
								location(0,1);send('I');send('N');send('2');
								location(0,2);send('I');send('N');send('3');
								location(0,3);send('I');send('N');send('4');
								location(0,4);send('I');send('N');send('5');
								location(0,5);send('I');send('N');send('6');
								location(0,6);send('I');send('N');send('7');
								location(0,7);send('I');send('N');send('8');
								
								if(increase)
								{
									increase=0;delay_time=5;
									if(PPM_offset<100)PPM_offset++,Data_change(1);
								}
								if(decrease)
								{
									decrease=0;delay_time=5;
									if(PPM_offset>-100)PPM_offset--,Data_change(1);
								}
								
								display_bar(PPM_DATA);
								
								
								if(delay_time>0)
								{
									delay_time--;
								
									photo(bmp3,57,5,32,3);
									location(60,6);
													
									write_num1000(PPM_offset,'-');
								}
							}
							else if(menu_2==22)//舵量监视
							{
								location(0,0);send('C');send('H');send('1');
								location(0,1);send('C');send('H');send('2');
								location(0,2);send('C');send('H');send('3');
								location(0,3);send('C');send('H');send('4');
								location(0,4);send('C');send('H');send('5');
								location(0,5);send('C');send('H');send('6');
								location(0,6);send('C');send('H');send('7');
								location(0,7);send('C');send('H');send('8');
			
								display_bar(operation);
							}
							else if(menu_2==23)//自动扫描
							{
								line_2=move_cursor(line_2,2);
								
								WW[0][0]=172,WW[0][1]=51,WW[0][2]=0;
								WW[1][0]=173,WW[1][1]=51,WW[1][2]=0;
								WW[2][0]=114,WW[2][1]=103,WW[2][2]=0;
								display_menu(line_2,2);
								
								if(line_2<1)
								{
									if(increase)
									{
										increase=0;
										if(auto_star<120)auto_star++,Data_change(1);
										auto_min=auto_star*4.25+511;
									}
									if(decrease)
									{
										decrease=0;
										if(auto_star>-120)auto_star--,Data_change(1);
										auto_min=auto_star*4.25+511;
									}
								}
								else if(line_2<2)
								{
									if(increase)
									{
										increase=0;
										if(auto_end<120)auto_end++,Data_change(1);
										auto_max=auto_end*4.25+511;
		
									}
									if(decrease)
									{
										decrease=0;
										if(auto_end>-120)auto_end--,Data_change(1);
										auto_max=auto_end*4.25+511;
									}
								}
								else if(line_2<3)
								{
									if(increase)
									{
										increase=0;
										if(auto_speed<100)auto_speed++,Data_change(1);
									}
									if(decrease)
									{
										decrease=0;
										if(auto_speed>0)auto_speed--,Data_change(1);
									}
								}
								write_num100(auto_star,66,0,'/');
								write_num100(auto_end,66,2,'/');
								write_num100(auto_speed,66,4,0);
								
								
							}
							else if(menu_2==24)//电压设置
							{
								line_2=move_cursor(line_2,2);
								
								WW[0][0]=175,WW[0][1]=70,WW[0][2]=0;
								WW[1][0]=71,WW[1][1]=72,WW[1][2]=0;
								WW[2][0]=39,WW[2][1]=40,WW[2][2]=0;							
								
							
								display_menu(line_2,2);
								
								temp=battery_voltage;
								if(temp>999)send2(temp%10000/1000+48,52,0);
								send2(temp%1000/100+48,60,0);
								send2('.',68,0);							
								send2(temp%100/10+48,76,0);
								send2(temp%10+48,84,0);
								send2('V',92,0);
								
								
								if(Battery_1s==1)
								{
									write_num100(VLD1,60,2,'.');send2('V',92,2);
									write_num100(v_trim1,60,4,'#');
								}
								else if(Battery_1s==0)
								{
									write_num100(VLD2,60,2,'.');send2('V',92,2);
									write_num100(v_trim2,60,4,'#');
								}
								
								if(line_2<1)
								{
									
								}
								else if(line_2<2)
								{

									if(increase)
									{
										increase=0;
										if(Battery_1s==1)
										{
											if(VLD1<80)VLD1++;Data_change(1);
										}
										else if(Battery_1s==0)
										{
											if(VLD2<120)VLD2++;Data_change(1);
										}
									}
									
									if(decrease)
									{
										decrease=0;
										if(Battery_1s==1)
										{
											if(VLD1>33)VLD1--;Data_change(1);
										}
										else if(Battery_1s==0)
										{
											if(VLD2>66)VLD2--;Data_change(1);
										}
									}
									
									
										
									
								}
								else if(line_2<3)
								{
										
									if(increase)
									{
										increase=0;
										if(Battery_1s==1)
										{
											if(v_trim1<100)v_trim1++;Data_change(1);
										}
										else if(Battery_1s==0)
										{
											if(v_trim2<100)v_trim2++;Data_change(1);
										}
									}
									
									if(decrease)
									{
										decrease=0;
										if(Battery_1s==1)
										{
											if(v_trim1>-100)v_trim1--;Data_change(1);
										}
										else if(Battery_1s==0)
										{
											if(v_trim2>-100)v_trim2--;Data_change(1);
										}
									}
								}
							
							}
							else if(menu_2==25)//接收机输出模式
							{
								line_2=move_cursor(line_2,7);
								
								WW[0][0]=16,WW[0][1]=17,WW[0][2]=81,WW[0][3]=0;
								WW[1][0]=16,WW[1][1]=17,WW[1][2]=82,WW[1][3]=0;
								WW[2][0]=16,WW[2][1]=17,WW[2][2]=83,WW[2][3]=0;
								WW[3][0]=16,WW[3][1]=17,WW[3][2]=84,WW[3][3]=0;
								WW[4][0]=16,WW[4][1]=17,WW[4][2]=85,WW[4][3]=0;
								WW[5][0]=16,WW[5][1]=17,WW[5][2]=86,WW[5][3]=0;
								WW[6][0]=16,WW[6][1]=17,WW[6][2]=87,WW[6][3]=0;
								WW[7][0]=16,WW[7][1]=17,WW[7][2]=88,WW[7][3]=0;
								display_menu(line_2,7);
								send3(hzk[1],60,0);
								send3(hzk[1],60,2);
								send3(hzk[1],60,4);
								send3(hzk[1],60,6);
							
								
								if(increase)
								{
									increase=0;
									if(line_2==0)
									{
										if(receiver_mode[0]<3)receiver_mode[0]++,config_change=1,Data_change(2);
									}
									else if(line_2==1)
									{
										if(receiver_mode[1]<2)receiver_mode[1]++,config_change=1,Data_change(2);
									}
									else if(line_2==4)
									{
										if(receiver_mode[4]<4)receiver_mode[4]++;
										if(receiver_mode[4]==2)
										{
											receiver_mode[4]=4;
										}
										
										config_change=1,Data_change(2);
									}
									else if(line_2==6)
									{
										if(receiver_mode[6]<2)receiver_mode[6]++,config_change=1,Data_change(2);
									}
									else
									{
										if(receiver_mode[line_2]<1)receiver_mode[line_2]++,config_change=1,Data_change(2);
									}
									
								}
								if(decrease)
								{
									decrease=0;
									if(line_2==4)
									{
										if(receiver_mode[4]>0)receiver_mode[4]--;
										if(receiver_mode[4]==3)
										{
											receiver_mode[4]=1;
										}
										config_change=1,Data_change(2);
									}
									else
									{
										if(receiver_mode[line_2]>0)receiver_mode[line_2]--,config_change=1,Data_change(2);
										
									}

									
								}
								
								if(config_change)
								{
									config_change=0;
									tx_order[1]=0;
									if(receiver_mode[0]==3)
									{
										tx_order[1]|=0x80;
										
									}
									if(receiver_mode[4]==4)
									{
										tx_order[1]|=0x40;
										
									}
									
									if(receiver_mode[0]==2)
									{
										tx_order[1]|=0x08;
									}
									if(receiver_mode[1]==2)
									{
										tx_order[1]|=0x04;
									}
									if(receiver_mode[6]==2)
									{
										tx_order[1]|=0x02;
									}
									
									tx_order[2]=0;
									if(receiver_mode[0]==1)
									{
										tx_order[2]|=0x80;
									}
									if(receiver_mode[1]==1)
									{
										tx_order[2]|=0x40;
									}
									if(receiver_mode[2]==1)
									{
										tx_order[2]|=0x20;
									}
									if(receiver_mode[3]==1)
									{
										tx_order[2]|=0x10;
									}
									if(receiver_mode[4]==1)
									{
										tx_order[2]|=0x08;
									}
									if(receiver_mode[5]==1)
									{
										tx_order[2]|=0x04;
									}
									if(receiver_mode[6]==1)
									{
										tx_order[2]|=0x02;
									}
									if(receiver_mode[7]==1)
									{
										tx_order[2]|=0x01;
									}
									
									Receiver_change=1;
								}
								
								if(line_2<3)
								{
									display_receiver_mode(receiver_mode[0],72,0);
									display_receiver_mode(receiver_mode[1],72,2);
									display_receiver_mode(receiver_mode[2],72,4);
									display_receiver_mode(receiver_mode[3],72,6);
								}
								else if(line_2==7)
								{
									display_receiver_mode(receiver_mode[4],72,0);
									display_receiver_mode(receiver_mode[5],72,2);
									display_receiver_mode(receiver_mode[6],72,4);
									display_receiver_mode(receiver_mode[7],72,6);
								}
								else 
								{
									display_receiver_mode(receiver_mode[line_2-2],72,0);
									display_receiver_mode(receiver_mode[line_2-1],72,2);
									display_receiver_mode(receiver_mode[line_2],72,4);
									display_receiver_mode(receiver_mode[line_2+1],72,6);
								}
							}
							else if(menu_2==26)//定时器
							{
								line_2=move_cursor(line_2,2);
								
								WW[0][0]=160,WW[0][1]=161,WW[0][2]=0;
								WW[1][0]=58,WW[1][1]=59,WW[1][2]=0;
								WW[2][0]=71,WW[2][1]=72,WW[2][2]=0;							
								
							
								display_menu(line_2,2);
								
								if(clock_switch)
								{
									if(clock_go)send3(hzk[196],60,0),send3(hzk[182],72,0);
									else
									{
										
										if(Switch_Check(clock_switch)==2)send3(hzk[199],60,0),send3(hzk[200],72,0);
										else send3(hzk[211],60,0),send3(hzk[212],72,0);
									
									}
								}
								else send3(hzk[0],60,0),send3(hzk[0],72,0);
									
										
								
								display_switch(clock_switch,60,2);
								write_num100(clock_set,60,4,0);send3(hzk[174],84,4);send3(hzk[135],96,4);
								
								if(line_2==1)
								{
									if(increase)
									{
										increase=0;
										if(clock_switch<5)clock_switch++,Data_change(1);
										
									}
									if(decrease)
									{
										decrease=0;
										if(clock_switch>0)clock_switch--,Data_change(1);

									}
								}
								if(line_2==2)
								{
									if(increase)
									{
										increase=0;
										
											if(clock_set<60)clock_set++;Data_change(1);
										
									}
									
									if(decrease)
									{
										decrease=0;
										
											if(clock_set>0)clock_set--;Data_change(1);
										
									}
								}								
								
								
							}
							else if(menu_2==27)//开机画面
							{
								line_2=move_cursor(line_2,1);
								
								if(line_2<1)
								{
									photo(screen3,0,0,128,8);
								}
								else if(line_2<2)
								{
									photo(screen4,0,0,128,8);
								}
								
								if(right)
								{
									right=0;
									boot_screen=line_2;Data_change(1);
									menu_2=0;
								}
								
							}
							else if(menu_2==28)//偏好设置
							{
								line_2=move_cursor(line_2,3);
								
								WW[0][0]=56,WW[0][1]=57,WW[0][2]=0;
								WW[1][0]=141,WW[1][1]=142,WW[1][2]=104,WW[1][3]=194,WW[1][4]=0;
								WW[2][0]=141,WW[2][1]=142,WW[2][2]=195,WW[2][3]=196,WW[2][4]=0;
								WW[3][0]=39,WW[3][1]=40,WW[3][2]=197,WW[3][3]=198,WW[3][4]=0;
								
								display_menu(line_2,3);
								
								if(Sound_enable)send3(hzk[58],72,0);
								else send3(hzk[59],72,0);
								if(Throttle_check)send3(hzk[58],72,2),send3(hzk[132],84,2);
								else send3(hzk[10],72,2);
								if(Auto_lock)send3(hzk[104],72,4),send3(hzk[105],84,4);
								else send3(hzk[191],72,4),send3(hzk[105],84,4);
								if(Trim_step)send2('1',72,6);
								else send2('5',72,6);
					//			if(Right_throttle)send3(hzk[190],68,2),send3(hzk[191],80,2);
					//			else send3(hzk[189],68,2),send3(hzk[191],80,2);
								
								if(line_2<1)
								{
									if(increase|decrease)
									{
										increase=0;decrease=0;
										
											Sound_enable=!Sound_enable;Data_change(1);
										
									}	
								}
								if(line_2<2)
								{
									if(increase|decrease)
									{
										increase=0;decrease=0;
										
											Throttle_check=!Throttle_check;Data_change(1);
										
									}	
								}
								if(line_2<3)
								{
									if(increase|decrease)
									{
										increase=0;decrease=0;
										
											Auto_lock=!Auto_lock;Data_change(1);
										
									}	
								}
								if(line_2<4)
								{
									if(increase|decrease)
									{
										increase=0;decrease=0;
										
											Trim_step=!Trim_step;Data_change(1);
										
									}	
								}
				/*				else if(line_2<2)
								{
									if(increase|decrease)
									{
										increase=0;decrease=0;
										
											Right_throttle=!Right_throttle;Data_change(1);
										
									}	
								}*/
							}
							else if(menu_2==29)//指数曲线
							{
								line_2=move_cursor(line_2,1);
								
								if(line_2==0)
								{
									if(increase)
									{									
										increase=0;
										if(index_number<2)index_number++;
										
									}
									if(decrease)
									{									
										decrease=0;
										if(index_number>0)index_number--;
										
									}
								}
								if(line_2==1)
								{
									if(increase)
									{									
										increase=0;
										if(CH_index[index_number]<100)CH_index[index_number]++,Data_change(2);
										
									}
									if(decrease)
									{									
										decrease=0;
										if(CH_index[index_number]>-100)CH_index[index_number]--,Data_change(2);
										
									}
								}
								
								send3(hzk[1],0,line_2+line_2);
								send3(hzk[2],0,2-line_2-line_2);

								send3(hzk[94],12,0);send3(hzk[95],24,0);
								
								if(index_number==2)send3(hzk[84],36,0);
								else send3(hzk[81+index_number],36,0);

								write_num100(CH_index[index_number],24,2,'/');
								
								if(last_index!=CH_index[index_number])
								{
									
									last_index=CH_index[index_number];
									index_change=1;
								}
								
								
								display_curve();
								
							}
							else if(menu_2==30)//多点曲线
							{
								line_2=move_cursor(line_2,2);														
								
								
								if(line_2==0)
								{
									if(increase)
									{									
										increase=0;
										if(curve_num<4)curve_num++;
										
									}
									if(decrease)
									{									
										decrease=0;
										if(curve_num>0)curve_num--;
										
									}
									
									send3(hzk[1],0,0);
									send3(hzk[2],0,2);
									send3(hzk[2],0,4);
								}
								else if(line_2==1)
								{
									if(increase)
									{									
										increase=0;
										if(point_num<6)point_num++;
										
									}
									if(decrease)
									{									
										decrease=0;
										if(point_num>0)point_num--;
										
									}
									
									send3(hzk[2],0,0);
									send3(hzk[1],0,2);
									send3(hzk[2],0,4);
								}
								else if(line_2==2)
								{
									if(increase)
									{
										increase=0;
										if(turnning_point[curve_num][point_num]<100)
										{
											turnning_point[curve_num][point_num]++,Data_change(2);
										}
									}
									if(decrease)
									{
										decrease=0;
										if(turnning_point[curve_num][point_num]>0)
										{
											turnning_point[curve_num][point_num]--,Data_change(2);
										}
									}	
									
									send3(hzk[2],0,0);
									send3(hzk[2],0,2);
									send3(hzk[1],0,4);
								}
															
								
								
				
								send3(hzk[94],12,0);send3(hzk[95],24,0);
								if(curve_num==0)send3(hzk[83],40,0);
								else if(curve_num>0)send3(hzk[84+curve_num],40,0);
									
				
								send3(hzk[51],12,2);
								send3(hzk[81+point_num],28,2);
								
								write_num100(turnning_point[curve_num][point_num],12,4,'%');
								get_curve2();
								display_curve2();
							}
							else if(menu_2==31)//组合开关
							{
								line_2=move_cursor(line_2,4);
							
								WW[0][0]=160,WW[0][1]=161,WW[0][2]=0;
								WW[1][0]=83,WW[1][1]=205,WW[1][2]=0;
								WW[2][0]=82,WW[2][1]=205,WW[2][2]=0;
								WW[3][0]=38,WW[3][1]=30,WW[3][2]=0;
								WW[4][0]=36,WW[4][1]=37,WW[4][2]=0;
								
								display_menu(line_2,4);
								
								if(line_2<3)
								{
									if(Com_Swt_IN1)
									{
										send3(hzk[8],60,0),send3(hzk[9],72,0);								
										send3(hzk[81+Combination_Switch],84,0);	
									}
									else
									{
										send3(hzk[2],60,0),send3(hzk[59],72,0);send3(hzk[2],84,0);
									}
									
									
									display_switch(Com_Swt_IN1,60,2);
									display_switch(Com_Swt_IN2,60,4);	
									send3(hzk[94],60,6);send3(hzk[95],72,6);send3(hzk[Com_Swt_mapping+81],84,6);
								}
								else
								{
									display_switch(Com_Swt_IN1,60,0);
									display_switch(Com_Swt_IN2,60,2);
									send3(hzk[94],60,4);send3(hzk[95],72,4);send3(hzk[Com_Swt_mapping+81],84,4);
								}
								
								
								
								if(line_2==1)
								{
									if(increase)
									{
										increase=0;
										if(Com_Swt_IN1==0)Com_Swt_IN1=2,Data_change(1);
										else if(Com_Swt_IN1==2)Com_Swt_IN1=3,Data_change(1);
									}
									if(decrease)
									{
										decrease=0;
										if(Com_Swt_IN1==2)Com_Swt_IN1=0,Data_change(1);
										else if(Com_Swt_IN1==3)Com_Swt_IN1=2,Data_change(1);

									}
								}
								if(line_2==2)
								{
									if(increase)
									{
										increase=0;
										if(Com_Swt_IN2<5)Com_Swt_IN2++,Data_change(1);
										if(Com_Swt_IN2==2)Com_Swt_IN2=4;
									}
									if(decrease)
									{
										decrease=0;
										if(Com_Swt_IN2>0)Com_Swt_IN2--,Data_change(1);
										if(Com_Swt_IN2==3)Com_Swt_IN2=1;
									}
								}
								if(line_2==3)
								{
									if(increase)
									{
										increase=0;
										if(Com_Swt_mapping<7)Com_Swt_mapping++,Data_change(1);
									}
									if(decrease)
									{
										decrease=0;
										if(Com_Swt_mapping>0)Com_Swt_mapping--,Data_change(1);

									}
								}
								if(line_2==4)
								{
									if(right)
									{
										right=0;menu_3=2,line_3=0;
										
									}
								}
								
								
							}
							else if(menu_2==32)//
							{
								line_2=move_cursor(line_2,1);
								
								
							}
							else if(menu_2==33)//
							{
								line_2=move_cursor(line_2,1);
								
								
							}
							
							
							if(left)left=0,menu_2=0,star=1;
							increase=0;decrease=0;right=0;	
						}
						
						
					}
					else 
					{

						if(menu_1==1)//工作模式
						{
							line_1=move_cursor(line_1,2);
							
							WW[0][0]=10,WW[0][1]=11,WW[0][2]=29,WW[0][3]=30,WW[0][4]=0;
							WW[1][0]=192,WW[1][1]=193,WW[1][2]=31,WW[1][3]=32,WW[1][4]=0;
							WW[2][0]=192,WW[2][1]=193,WW[2][2]=31,WW[2][3]=33,WW[2][4]=0;
							
							display_menu(line_1,2);
							
							if(line_1==0)//无线
							{								
								if(right)
								{
									right=0;
									if(RC_TX)
									{
										RC_TX=0;Data_change(1);
									}
									else
									{
										IR_TX=0,RC_TX=1;Data_change(1);
										
									}
								}
							}
							else if(line_1==1)//PPM
							{
								if(right)
								{
									right=0;
									if(PPM_OUT)
									{
										PPM_OUT=0;Data_change(1);
										ET1=0;P3M0=0x00;
									}
									else
									{
										PPM_OUT=1,PPM_IN=0;config_change=1;Data_change(1);
										EX1=0;ET1=1;TMOD=0x11;P3M0=0x08;
									}
								}
								 
							}
							else 
							{
								if(right)
								{
									right=0;
									if(PPM_IN)
									{
										PPM_IN=0;EX1=0;Data_change(1);
									}
									else
									{
										PPM_IN=1,PPM_OUT=0;config_change=1;Data_change(1);
										TMOD=0x91;
										EX1=1;ET1=0;P3M0=0x00;
								//		P3M0=0x60;			 
										
									}
								}
								
							}
							if(RC_TX)send3(hzk[3],62,0);
							else send3(hzk[2],62,0);
							if(PPM_IN)send3(hzk[3],62,4);
							else send3(hzk[2],62,4);
							if(PPM_OUT)send3(hzk[3],62,2);
							else send3(hzk[2],62,2);
						}
						else if(menu_1==2)//快捷功能
						{
							line_1=move_cursor(line_1,3);
							
							WW[0][0]=181,WW[0][1]=182,WW[0][2]=80,WW[0][3]=0;
							WW[1][0]=183,WW[1][1]=184,WW[1][2]=36,WW[1][3]=0;
							WW[2][0]=203,WW[2][1]=204,WW[2][2]=58,WW[2][3]=59,WW[2][4]=0;
							WW[3][0]=66,WW[3][1]=67,WW[3][2]=71,WW[3][3]=72,WW[3][4]=0;

							display_menu(line_1,3);
							
							
							
							if(line_1==0)
							{

								if(right)right=0,menu_2=26,line_2=0,star=1;
							}
							else if(line_1==1)
							{

								if(right)right=0,menu_2=16,line_2=0;
							}
							else if(line_1==2)
							{

								if(right)right=0,menu_2=31,line_2=0;
							}
							else if(line_1==3)
							{

								if(right)right=0,menu_2=13,line_2=0;
							}

							
						}
						else if(menu_1==3)//通道设置
						{
							line_1=move_cursor(line_1,6);
							
							WW[0][0]=34,WW[0][1]=35,WW[0][2]=0;
							WW[1][0]=39,WW[1][1]=40,WW[1][2]=0;
							WW[2][0]=34,WW[2][1]=36,WW[2][2]=37,WW[2][3]=0;
							WW[3][0]=134,WW[3][1]=36,WW[3][2]=37,WW[3][3]=0;
							WW[4][0]=31,WW[4][1]=33,WW[4][2]=38,WW[4][3]=30,WW[4][4]=0;
							WW[5][0]=143,WW[5][1]=144,WW[5][2]=118,WW[5][3]=11,WW[5][4]=0;
							WW[6][0]=50,WW[6][1]=51,WW[6][2]=118,WW[6][3]=11,WW[6][4]=0;
							display_menu(line_1,6);
							
							
							if(line_1==0)
							{
								if(right)right=0,menu_2=1,line_2=0;
							}
							else if(line_1==1)
							{
								if(right)right=0,menu_2=4,line_2=0;
							}
							else if(line_1==2)
							{
								if(right)right=0,menu_2=2,line_2=0,ratio_mode=1;
							}
							else if(line_1==3)
							{
								if(right)right=0,menu_2=2,line_2=0,ratio_mode=0;
							}
							else if(line_1==4)
							{
								if(right)right=0,menu_2=3,line_2=0;
							}
							else if(line_1==5)
							{
								if(right)right=0,menu_2=29,line_2=0,index_change=1;
							}
							else 
							{
								if(right)right=0,menu_2=30,line_2=0;
							}
						}
						else if(menu_1==4)//高级混控
						{
							line_1=move_cursor(line_1,3);
							
							WW[0][0]=21,WW[0][1]=22,WW[0][2]=80,WW[0][3]=81,WW[0][4]=0;
							WW[1][0]=21,WW[1][1]=22,WW[1][2]=80,WW[1][3]=82,WW[1][4]=0;
							WW[2][0]=21,WW[2][1]=22,WW[2][2]=80,WW[2][3]=83,WW[2][4]=0;
							WW[3][0]=21,WW[3][1]=22,WW[3][2]=80,WW[3][3]=84,WW[3][4]=0;
							display_menu(line_1,3);
							
							if(increase|decrease)
							{
								increase=0;decrease=0;
								if(line_1<1)mixer1=!mixer1,Data_change(2);
								else if(line_1<2)mixer2=!mixer2,Data_change(2);
								else if(line_1<3)mixer3=!mixer3,Data_change(2);
								else if(line_1<4)mixer4=!mixer4,Data_change(2);
							}
							
							if(mixer1)send3(hzk[58],75,0);
							else send3(hzk[59],75,0);
							if(mixer2)send3(hzk[58],75,2);
							else send3(hzk[59],75,2);
							if(mixer3)send3(hzk[58],75,4);
							else send3(hzk[59],75,4);
							if(mixer4)send3(hzk[58],75,6);
							else send3(hzk[59],75,6);
							if(right)right=0,menu_2=5,line_2=0,star=1,mixer_name=line_1+1;						
							
						}
						else if(menu_1==5)//无线配置
						{
							line_1=move_cursor(line_1,4);
							
													
							WW[0][0]=16,WW[0][1]=47,WW[0][2]=48,WW[0][3]=49,WW[0][4]=0;
							WW[1][0]=50,WW[1][1]=51,WW[1][2]=52,WW[1][3]=53,WW[1][4]=0;
							WW[2][0]=76,WW[2][1]=53,WW[2][2]=77,WW[2][3]=78,WW[2][4]=0;							
							WW[3][0]=16,WW[3][1]=47,WW[3][2]=119,WW[3][3]=120,WW[3][4]=0;
							WW[4][0]=29,WW[4][1]=30,WW[4][2]=27,WW[4][3]=45,WW[4][4]=0;
							display_menu(line_1,4);
							

							if(line_1<1)
							{

								if(right)right=0,menu_2=7,line_2=0,star=1;
							}
							else if(line_1<2)
							{

								if(right)right=0,menu_2=8,line_2=0;
							}
							else if(line_1<3)
							{

								if(right)
								{
									right=0;
									menu_2=19,star=1,line_2=0;
									RC_check=1,RC_check_star=1;
									
								}
								
							}
							else if(line_1<4)
							{
								if(right)right=0,menu_2=18,line_2=0;
							}
							else if(line_1<5)
							{
								if(increase)
								{
									increase=0;	   
									if(TX_power<3)TX_power++,Data_change(1),NRF_change=1;	
								}
								if(decrease)
								{
									decrease=0;
									if(TX_power>0)TX_power--,Data_change(1),NRF_change=1;
								}
								
								if(TX_power==3)
								{
									send2('1',60,6);send2('0',68,6);send2('0',76,6);send2('m',84,6);send2('W',92,6);
								}
								else if(TX_power==2)
								{
									send2(' ',60,6);send2('2',68,6);send2('5',76,6);send2('m',84,6);send2('W',92,6);
								}
								else if(TX_power==1)
								{
									send2(' ',60,6);send2(' ',68,6);send2('6',76,6);send2('m',84,6);send2('W',92,6);
								}
								else if(TX_power==0)
								{
									send2(' ',60,6);send2(' ',68,6);send2('2',76,6);send2('m',84,6);send2('W',92,6);
								}
								else 
								{
									send2('E',60,6);send2('R',68,6);send2('R',76,6);send2('O',84,6);send2('R',92,6);

								}
								
							}						
							
						}
						else if(menu_1==6)//红外配置
						{

						}
						else if(menu_1==7)//PPM配置
						{		
							
							line_1=move_cursor(line_1,3);
							
							WW[0][0]=31,WW[0][1]=32,WW[0][2]=38,WW[0][3]=30,WW[0][4]=0;
							WW[1][0]=31,WW[1][1]=33,WW[1][2]=98,WW[1][3]=99,WW[1][4]=0;
							WW[2][0]=54,WW[2][1]=55,WW[2][2]=160,WW[2][3]=161,WW[2][4]=0;
							WW[3][0]=54,WW[3][1]=55,WW[3][2]=58,WW[3][3]=59,WW[3][4]=0;
							display_menu(line_1,3);
							
							if(line_1<1)
							{
								if(right)right=0,menu_2=15,line_2=0,star=1;
							}
							else if(line_1<2)
							{
							if(right)right=0,menu_2=21,line_2=0,delay_time=20,star=1;
							}
							else if(line_1<3)
							{
									
							}
							else 
							{
								if(increase)
								{
									increase=0;
									if(train_switch<5)train_switch++,Data_change(1);
								
									
								}
								if(decrease)
								{
									decrease=0;
									if(train_switch>0)train_switch--,Data_change(1);

								}

			
							}
							if(PPM_updata>0)
							{
								if(train_output)
								{
									send3(hzk[2],72,4),send3(hzk[58],84,4),send3(hzk[2],96,4);
								}
								else send3(hzk[2],72,4),send3(hzk[59],84,4),send3(hzk[2],96,4);

							}
							else send3(hzk[10],72,4),send3(hzk[47],84,4),send3(hzk[68],96,4);
							
							display_switch(train_switch,72,6);

						}
						else if(menu_1==8)//系统设置
						{
							line_1=move_cursor(line_1,8);
							
							WW[0][0]=62,WW[0][1]=63,WW[0][2]=18,WW[0][3]=13,WW[0][4]=0;
							WW[1][0]=133,WW[1][1]=152,WW[1][2]=18,WW[1][3]=13,WW[1][4]=0;
							WW[2][0]=175,WW[2][1]=70,WW[2][2]=18,WW[2][3]=13,WW[2][4]=0;
							WW[3][0]=58,WW[3][1]=132,WW[3][2]=145,WW[3][3]=146,WW[3][4]=0;
							WW[4][0]=154,WW[4][1]=155,WW[4][2]=144,WW[4][3]=156,WW[4][4]=0;
							WW[5][0]=36,WW[5][1]=37,WW[5][2]=167,WW[5][3]=168,WW[5][4]=0;
							WW[6][0]=96,WW[6][1]=97,WW[6][2]=98,WW[6][3]=99,WW[6][4]=0;
							WW[7][0]=73,WW[7][1]=74,WW[7][2]=47,WW[7][3]=75,WW[7][4]=0;
							
							WW[8][0]=106,WW[8][1]=107,WW[8][2]=32,WW[8][3]=136,WW[8][4]=0;
							display_menu(line_1,8);
							
							if(line_1<1)
							{
								if(right)right=0,menu_2=14,line_2=0,star=1;
							}
							else if(line_1<2)
							{
								if(right)right=0,menu_2=28,line_2=0;
							}
							else if(line_1<3)
							{
								if(right)right=0,menu_2=24,line_2=0;
								
							}
							else if(line_1<4)
							{
								if(right)right=0,menu_2=27,line_2=0;
								if(increase|decrease)
								{
									increase=0;decrease=0;
									boot_screen_en=!boot_screen_en;
										Data_change(1);
								}
								if(boot_screen_en)send3(hzk[58],72,4);
								else send3(hzk[59],72,4);
								
								
							}
							else if(line_1<5)
							{
								if(right)right=0,menu_2=17,line_2=0,star=1;
									
							}
							else if(line_1<6)
							{
								if(right)right=0,menu_2=22,star=1,line_2=0;	
							}
							else if(line_1<7)
							{
								if(right)right=0,menu_2=20,line_2=0,star=1;
							}
							else if(line_1<8)
							{
								if(right)right=0,menu_2=10,line_2=0,star=1;
							}
							else if(line_1<9)
							{
								if(right)right=0,menu_2=12,line_2=0,star=1;

							}
							
						}
						else if(menu_1==9)//角度传感
						{
							
						}
						else if(menu_1==10)//自动控制
						{
							line_1=move_cursor(line_1,3);
							
							WW[0][0]=160,WW[0][1]=161,WW[0][2]=0;
							WW[1][0]=58,WW[1][1]=59,WW[1][2]=0;
							
							WW[2][0]=170,WW[2][1]=171,WW[2][2]=0;
							WW[3][0]=77,WW[3][1]=78,WW[3][2]=0;
							display_menu(line_1,3);
							
							
								if(auto_switch)
								{
									if(auto_mode==1)send3(hzk[170],60,0),send3(hzk[171],72,0);
									else if(auto_mode==2)send3(hzk[199],60,0),send3(hzk[200],72,0);	
									else if(auto_mode==3)send3(hzk[77],60,0),send3(hzk[78],72,0);
								}
								else send3(hzk[59],60,0),send3(hzk[2],72,0);
							
								display_switch(auto_switch,60,2);
							write_num100(auto_middle,66,4,'/');
							
							
							if(line_1==1)
							{
								
								if(increase)
								{
									increase=0;
									if(auto_switch==0)auto_switch=2,Data_change(1);
									else if(auto_switch==2)auto_switch=3,Data_change(1);
								}
								if(decrease)
								{
									decrease=0;
									if(auto_switch==2)auto_switch=0,Data_change(1);
									else if(auto_switch==3)auto_switch=2,Data_change(1);

								}
							
							}							
							if(line_1==2)
							{
								if(increase)
								{
									increase=0;
									if(auto_middle<120)auto_middle++,Data_change(1);
								}
								if(decrease)
								{
									decrease=0;
									if(auto_middle>-120)auto_middle--,Data_change(1);
								}
								
							}
							if(line_1==3)
							{
								if(right)right=0,menu_2=23,line_2=0;
							}
							
							
						}
						else if(menu_1==11)//模型管理
						{
							line_1=move_cursor(line_1,3);
							
							WW[0][0]=162,WW[0][1]=163,WW[0][2]=0;
							WW[1][0]=164,WW[1][1]=165,WW[1][2]=0;
							WW[2][0]=166,WW[2][1]=169,WW[2][2]=0;
							WW[3][0]=166,WW[3][1]=171,WW[3][2]=0;
							display_menu(line_1,3);
							
							
							send3(hzk[Model_number+81],60,0);send3(hzk[68],72,0);
							
							
							if(line_1<1)
							{
								if(increase)
								{
									increase=0;
									if(Model_number<5)
									{
										Model_current=Model_number;
										Model_number++;
										ROM_enable=1,Data_system=1,Model_change=1;
										/**
										+军刀破+
										重置命名索引
										*/
										Name_idx=0;
									}
								}
								if(decrease)
								{
									decrease=0;
									if(Model_number>0)
									{
										Model_current=Model_number;
										Model_number--;
										ROM_enable=1,Data_system=1,Model_change=1;
										/**
										+军刀破+
										重置命名索引
										*/
										Name_idx=0;
									}
								}
							}
							else if(line_1<2)
							{
								/*************+军刀破+************/
								//模型命名，最多用七个字符命名，只能以A－Z，a-z，0-9，及常用符号输入
								//按加减输入字符，确定键输入下个字符
								u8 i=0;
								for(i=0;i<7;i++)
								{
									if(i==Name_idx)
									{
										send2_rev(Model_name[i]+32,60+8*i,2);
									}
									else
									{
										send2(Model_name[i]+32,60+8*i,2);
									}
								}
								if(increase)
								{
									increase=0;
									Model_name[Name_idx]++;
									if(Model_name[Name_idx]>90) Model_name[Name_idx]=0;
									send2_rev(Model_name[Name_idx]+32,60+Name_idx*8,2);									
									ROM_enable=1,Data_model=1,Model_change=1;
								}
								if(decrease)
								{
									decrease=0;
									if(Model_name[Name_idx]>0) Model_name[Name_idx]--;
									else Model_name[Name_idx]=90;
									send2_rev(Model_name[Name_idx]+32,60+Name_idx*8,2);
									ROM_enable=1,Data_model=1,Model_change=1;
								}
								if(right)
								{
									right=0;
									Name_idx++;
									if(Name_idx>6) Name_idx=0;
								}
								/*************************/
							}
							else if(line_1<3)
							{
/*								if(increase)
								{
									increase=0;
									if(Model_target<4)Model_target++;
								}
								if(decrease)
								{
									decrease=0;
									if(Model_target>0)Model_target--;
								}
								
								if(right)
								{
									right=0;
									if(copy_ready)
									{
										paste_ready=1;
											
									}
									else 
									{
										copy_ready=1;
									}								
								}
								
								
								
								if(copy_ready)
								{
									if(left)
									{
										left=0;
										copy_ready=0;paste_ready=0;
										send3(hzk[2],60,4);send3(hzk[2],72,4);send3(hzk[2],84,4);send3(hzk[2],96,4);
									}
									else
									{
										send3(hzk[176],60,4);send3(hzk[Model_target+81],72,4);send3(hzk[68],84,4);
										if(paste_ready)
										{
											send3(hzk[3],96,4);
											delay_time++;
											if(delay_time>4)
											{
												delay_time=0;
												Model_copy();
												Data_model=1,ROM_enable=1;	
												copy_ready=0;paste_ready=0;
											}
										}
										else send2('?',96,4);	
									}


									
								}*/
							}
							else if(line_1<4)
							{
								if(right)
								{
									right=0;
									if(reset_action)
									{
										Model_reset=1;
										
									}
									else
									{
										reset_action=1;
										
									}
									
								}
								
								
								
								
								if(reset_action)
								{
									if(left)
									{
										left=0;
										reset_action=0;Model_reset=0;
										send3(hzk[2],60,6);send3(hzk[2],72,6);send3(hzk[2],84,6);
									}	
									else
									{
										send3(hzk[137],60,6);send3(hzk[109],72,6);
										if(Model_reset)
										{
											send3(hzk[3],84,6);
											delay_time++;
											if(delay_time>4)
											{
												delay_time=0;
												Model_data_reset();
												reset_action=0;Model_reset=0;Data_model=1,ROM_enable=1;	
											}
											
										}
										else send2('?',84,6);	
									}
									
								}
							}
							
						}
						else if(menu_1==12)//接收机管理
						{
							line_1=move_cursor(line_1,3);
							
							WW[0][0]=41,WW[0][1]=42,WW[0][2]=43,WW[0][3]=44,WW[0][4]=0;
							WW[1][0]=46,WW[1][1]=22,WW[1][2]=36,WW[1][3]=37,WW[1][4]=0;
							WW[2][0]=31,WW[2][1]=32,WW[2][2]=8,WW[2][3]=9,WW[2][4]=0;
							WW[3][0]=16,WW[3][1]=17,WW[3][2]=38,WW[3][3]=30,WW[3][4]=0;

							display_menu(line_1,3);
							
							
							if(line_1<1)
							{

								if(right)right=0,menu_2=11,star=1;
							}
							else if(line_1<2)
							{

								if(right)right=0,menu_2=6,line_2=0;
							}
							else if(line_1<3)
							{
					  		
								if(right)right=0,menu_2=25,line_2=0;
							}
							else if(line_1<4)
							{
								if(right)right=0,menu_2=9,line_2=0;
							}

							
						}

						if(left)left=0,menu_1=0,star=1;
						increase=0;decrease=0;right=0;	
					}
				}
				else						 //主菜单
				{	


					

					line=move_cursor(line,10);
					WW[0][0]=209,WW[0][1]=210,WW[0][2]=39,WW[0][3]=40,WW[0][4]=0;
					
					WW[1][0]=6,WW[1][1]=7,WW[1][2]=8,WW[1][3]=9,WW[1][4]=0;
					WW[2][0]=25,WW[2][1]=26,WW[2][2]=27,WW[2][3]=28,WW[2][4]=0;
					WW[3][0]=16,WW[3][1]=17,WW[3][2]=18,WW[3][3]=13,WW[3][4]=0;							
					WW[4][0]=19,WW[4][1]=20,WW[4][2]=21,WW[4][3]=22,WW[4][4]=0;
					WW[5][0]=10,WW[5][1]=11,WW[5][2]=12,WW[5][3]=13,WW[5][4]=0;					
					WW[6][0]=192,WW[6][1]=193,WW[6][2]=12,WW[6][3]=13,WW[6][4]=0;
					WW[7][0]=104,WW[7][1]=105,WW[7][2]=22,WW[7][3]=169,WW[7][4]=0;
					WW[8][0]=8,WW[8][1]=159,WW[8][2]=14,WW[8][3]=15,WW[8][4]=0;
					WW[9][0]=43,WW[9][1]=44,WW[9][2]=14,WW[9][3]=15,WW[9][4]=0;
					WW[10][0]=23,WW[10][1]=24,WW[10][2]=18,WW[10][3]=13,WW[10][4]=0;


					display_menu(line,10);
					if(line==0)
					{

						if(right)right=0,save_action=1;
						if(save_action)
						{
							Data_change(2);
							send3(hzk[3],80,0);
						}
					}
					else if(line==1)		  
					{
						
						if(right)right=0,menu_1=1,line_1=0,star=1;
					}
					else if(line==2)
					{

						if(right)right=0,menu_1=2,line_1=0;
					}
					else if(line==3)
					{
					if(right)right=0,menu_1=3,line_1=0;	
					}
					else if(line==4)
					{
							if(right)right=0,menu_1=4,line_1=0;
					}
					else if(line==5)
					{
						if(right)right=0,menu_1=5,line_1=0;							
					}		  
					else if(line==6) 
					{
						if(right)right=0,menu_1=7,line_1=0;							
					}
					else if(line==7) 
					{
						if(right)right=0,menu_1=10,line_1=0;
					}
					else if(line==8) 
					{
						if(right)right=0,menu_1=11,line_1=0;
					}
					else if(line==9) 
					{
					
						if(right)right=0,menu_1=12,line_1=0;
					}
					else  
					{
					
						if(right)right=0,menu_1=8,line_1=0;
					}
					
					if(left)left=0,menu=0,line=0,star=1,save_action=0;
					if(right)right=0;
					if(increase)increase=0;
					if(decrease)decrease=0;

				}
			}
			else 
			{

				if(star)			 //star标志位：页面第一次载入
				{
					star=0;
					photo(screen1,0,0,128,8);
					
					//photo(name6,76,3,36,2);
					displayModel();

				}	
				
				
				
				send2(hour/10+48,64,0);send2(hour%10+48,72,0);				
				send2(minute/10+48,85,0);send2(minute%10+48,93,0);				
				send2(second/10+48,106,0);send2(second%10+48,114,0);
				
				location(40,2);
				send(minute2/10+48);send(minute2%10+48);
				LCD(0);LCD(0x24);
				send(second2/10+48);send(second2%10+48);
				
				if(SA)send2(123,74,5);
				else send2(124,74,5);
				
				if(SB1&SB2)send2(126,82,5);
				else if(SB1&!SB2)send2(125,82,5);
				else send2(127,82,5);
				
				if(SC1&SC2)send2(126,90,5);
				else if(SC1&!SC2)send2(125,90,5);
				else send2(127,90,5);
				
				if(SD)send2(123,98,5);
				else send2(124,98,5);
				
				if(SE)send2(123,106,5);
				else send2(124,106,5);
				
/*				
				location(55,5);
				temp=operation[0];
	//			temp=angle_x;
				if(temp>0)send('+');
				else if(temp<0)temp=-temp,send('-');
				else send(' ');
				send(temp/1000+48);
				send(temp%1000/100+48);
				send(temp%100/10+48);
				send(temp%10+48);
				
				location(55,6);
				temp=NRF_buff[0];
	//			temp=angle_y;
				if(temp>0)send('+');
				else if(temp<0)temp=-temp,send('-');
				else send(' ');
				send(temp/1000+48);
				send(temp%1000/100+48);
				send(temp%100/10+48);
				send(temp%10+48);
				

				
				location(95,5);temp=PPM_DATA[0];
				send(temp/1000+48);
				send(temp%1000/100+48);
				send(temp%100/10+48);
				send(temp%10+48);
			
				location(95,6);temp=operation[0];
				send(temp/1000+48);
				send(temp%1000/100+48);
				send(temp%100/10+48);
				send(temp%10+48);
*/	
				
				
				
				
				
				if(USB_5V)
				{
					location(78,2);
					send(' ');				
					
					send('5');send('V');LCD(0x00);send('-');LCD(0x00);
					
					
					
					send('U');send('S');send('B');
				}
				else
				{
					location(78,2);temp=battery_voltage;
					write_num1000(temp,'.');
						
				}

				
				location(23,4);
				if(rx[1])
				{
					temp=rx[1];
					temp<<=8;
					temp+=rx[2];
				}
				else temp=0;
				write_num1000(temp,'.');
				
				if(Switch_Check(Return_alarm_switch)==3)
				{
					if(temp<Power_V)
					{
						warning(52,3,1);Message_DataReturn_bad|=0x01;	
					}
					else
					{
						warning(52,3,0);Message_DataReturn_bad&=~0x01;
					}	
				}
				else warning(52,3,0),Message_DataReturn_bad=0;	
				
				
				
				
				location(23,3);
				if(rx[3])
				{
					temp=rx[3];
					temp<<=8;
					temp+=rx[4];
				}
				else temp=0;
				write_num1000(temp,'.');
				
				if(Switch_Check(Return_alarm_switch)==3)
				{
					if(temp<Chip_V)
					{
						warning(52,4,1);Message_DataReturn_bad|=0x02;	
					}
					else
					{
						warning(52,4,0);Message_DataReturn_bad&=~0x02;
					}
				}
				else warning(52,4,0),Message_DataReturn_bad=0;
				
				location(23,5);temp=rx[0]*2;
				if(temp>99)send('1'),send('0'),send('0');
				else send(' '),send(temp/10+48),send(temp%10+48);
				
				
				if(Switch_Check(Return_alarm_switch)==3)
				{
					if(temp<TX_min)
					{
						warning(52,5,1);Message_DataReturn_bad|=0x04;	
					}
					else
					{
						warning(52,5,0);Message_DataReturn_bad&=~0x04;
					}
				}
				else warning(52,5,0),Message_DataReturn_bad=0;
				
				temp=FUCK_sum*2;
				location(23,6);
				if(temp>99)send('1'),send('0'),send('0');
				else send(' '),send(temp/10+48),send(temp%10+48);

				if(Switch_Check(Return_alarm_switch)==3)
				{
					if(temp<RX_min)warning(52,6,1),Message_DataReturn_bad|=0x08;
					else warning(52,6,0),Message_DataReturn_bad&=~0x08;
				}
				else warning(52,6,0),Message_DataReturn_bad=0;
				
/*				flash_time++;
				if(flash_time<7)
				{
					
					warning(27,6);
				}
				else if(flash_time<14)
				{
					
					location(23,6);//write_num1000(temp,0);
					if(temp>99)send('1'),send('0'),send('0');
				else send(' '),send(temp/10+48),send(temp%10+48);
				}
				else flash_time=0;
				*/
								
				
				if(throttle_lock)
				{
					display_throttle(0);
				}
				else
				{
					if(CH_inverted[2])
					{
						display_throttle(255-throttle*10/34);
						location(6,2);
						temp=100-throttle*20/169;
					}
					else
					{
						display_throttle(throttle*10/34);
						location(6,2);
						temp=throttle*20/169;
					}
						
					if(temp==100)
					{
						send('A');send('L');send('L');
					}
					else
					{
						send(temp/10+48);send(temp%10+48);send('%');
					}

					
				}
				
				
				if(event1)//微调更新
				{
					if(middle_change_last==CH_middle_change)
					{
						if(CH_middle_change==0)
						{
							
							photo(bmp3,77,4,32,3);
							location(80,5);
						}
						else if(CH_middle_change==1)
						{
							
							photo(bmp3,90,2,32,3);
							location(93,3);
						}
						else if(CH_middle_change==2)
						{
							
							photo(bmp3,5,2,32,3);
							location(8,3);
						}
						else if(CH_middle_change==3)
						{
							
							photo(bmp3,17,4,32,3);
							location(20,5);
								
						}							
						
						write_num1000(CH_middle[CH_middle_change],'-');
					}
					else
					{
						star=1;
					}
					middle_change_last=CH_middle_change;							
					
				}
				
				location(6,7);
				if(CH_inverted[3]==1)display_trim2(125-CH_middle[3]);
				else display_trim2(CH_middle[3]+125);
				location(67,7);
				if(CH_inverted[0]==1)display_trim2(125-CH_middle[0]);
				else display_trim2(CH_middle[0]+125);

				if(CH_inverted[2]==1)display_trim1(125-CH_middle[2],0);
				else display_trim1(CH_middle[2]+125,0);
				if(CH_inverted[1]==1)display_trim1(125-CH_middle[1],123);
				else display_trim1(CH_middle[1]+125,123);
				
				LCD_OFF();
			}
		}
		else if(tt==3) //保留
		{
			
		}
		else if(tt==4)
		{//SA=!SA;
			if(ROM_enable)	//避免数据重复保存
			{
				ROM_enable=0;
				if(Data_system)		//系统参数
				{
					Data_system=0;
					DATA_save1();
				}
				if(Data_model) 	//机型参数
				{
					Data_model=0;
					DATA_save2();
				}
				
			}
			
			if(Model_change)	//读取另一个机型数据
			{
				Model_change=0;
				Model_current=Model_number;
				DATA_read2();
				
			}
			
		//	SA=!SA;
		}
		else if(tt==5)//保留
		{
			tt=0;
		}
	

	}

}


void ET0_isr()__interrupt 1	  //数据处理定时器	 低优先级
{

	TL0=0xe0,TH0=0xb1;		 //20ms
	flag_20ms=1;
//SA=!SA;
//	SC2=!SC2;
	Master_clock++;				   //主时钟计时
	if(Master_clock>49)Master_clock=0,second++;
	if(second>59)second=0,minute++;
	if(minute>59)minute=0,hour++;
	if(hour>99)hour=99;
	
	if(clock_go)//时钟计时
	{
		clock2++;
		if(clock2>49)clock2=0,second2++;
		if(second2>59)second2=0,minute2++;
		if(minute2>59)minute2=99;				
		
		if(minute2>=clock_set)Message_AlarmClock=1;
		else Message_AlarmClock=0;
	}
	else 
	{
		Message_AlarmClock=0;
	}
	
	
	

	if(KEY)		//菜单键检测			
	{
		KS_press_time=0;
		KS_long_press=0;
		if(KS)				   
		{					   
			KS=0;Message_KeyTone=1;			  
			
			if(menu)
			{
				if(menu_2==11)
				{
					cancel=1;
				}
				else if(menu_2==19)
				{
					RC_check=0,menu=0,star=1;
				}
				else
				{
					star=1,menu=0,save_action=0;
				}
				
			}
			else 
			{
				star=1;menu=1,menu_1=0,menu_2=0;
			}	
			
		} 
	}
	else 
	{
		KS=1;
		if(KS_long_press)KS=0;		  //长按菜单键锁定油门
		else KS_press_time++;
		
		if(KS_press_time>100)
		{
			KS_press_time=0;KS=0;
			KS_long_press=1;
			if(menu==0)
			{
				throttle_lock=!throttle_lock;
				Message_KeyTone=2;star=1;	
			}
			
		}
	}
	
	if(Auto_lock)//自动锁定油门
	{
		if(throttle_lock==0)
		{
			throttle_idle++;
			if(throttle_idle>500)
			{
				throttle_idle=0;
				throttle_lock=1;star=1;Message_KeyTone=2;
			}	
		}	
	}
	
			
	
	if(event1)		//1秒清0
	{
		t1++;
		if(t1>49)t1=0,event1=0,star=1;
	}
/*	
	if(event2)		//5秒清0
	{
		t2++;
		if(t2>250)
		{
			t2=0;
			throttle_lock=1;
		}
		
	}
	*/
	if(event3)		//1.5秒清0
	{
		t3++;
		if(t3>75)t3=0,event3=0,ROM_enable=1;//避免频繁擦写ROM
	}
	
/*	if(event3)		//0.5秒清0
	{
		t3++;
		if(t3>25)t3=0,event3=0,star=1;
	}*/
	
	if(Battery_1s)//低压报警处理
	{
		battery_voltage=((long)adc[0]*1000/(1000-v_trim1))*0.097;		   
		if(battery_voltage<VLD1*10)Message_LowPower=1;
		else Message_LowPower=0;
	}
	else
	{

		battery_voltage=((long)adc[0]*1000/(1000-v_trim2))*0.146;
		
		if(battery_voltage<VLD2*10)
		{
			if(battery_voltage>400&&battery_voltage<600)
			{
				USB_5V=1;
				Message_LowPower=0;
			}
			else USB_5V=0,Message_LowPower=1;
		}
		else Message_LowPower=0;
		

	}
	
	
	if(Message_AlarmClock||Message_LowPower||Message_DataReturn_bad)
  {
    beep=3;
  }
  else if(Message_KeyTone)
  {
    beep=Message_KeyTone;
		Message_KeyTone=0;
  }
	else beep=0;
	

	if(Sound_enable)//蜂鸣器处理
	{//蜂鸣器负极接单片机引脚，所以buzzer=0时才是蜂鸣器发出声音
		sound++;				   
		if(beep)
		{																				  
			if(beep==1)
			{ buzzer=0;if(sound>3)beep=0,buzzer=1;}
			if(beep==2)
			{buzzer=0;if(sound>30)beep=0,buzzer=1;}
			/*start-----改自“柴门”-------*/
			/*if(beep==3)
			{
				if(sound%40>25)buzzer=0;
				else buzzer=1;
			}*/
			//用不同的报警声表示不同的警报信息：
			if(Message_DataReturn_bad) //回传报警（信号弱报警、接收端低压报警，这里或许还可以再考虑分拆一下警报类型）
			{//bi _ bi _ bi - - _ bi _ bi _ bi - - _ bi _ bi _ bi - - _
				u8 ZhouQi = sound%80;
				if( ((ZhouQi>=0) && (ZhouQi<10))  ||  ((ZhouQi>=20) && (ZhouQi<30))  ||  ((ZhouQi>=40) && (ZhouQi<70)) )
					buzzer=0;
				else buzzer=1;
			}
			else if(Message_LowPower) //低压报警（应该是遥控器低压报警），蜂鸣器响10停10，bi _ bi _ bi _
			{
				if(sound%20>=10)buzzer=0;
				else buzzer=1;
			}
			else if(Message_AlarmClock) //超时报警（菜单中设置超时时间），蜂鸣器响20停30，bi - _ _ _ bi - _ _ _
			{
				if(sound%50>=30)buzzer=0;
				else buzzer=1;
			}
			/*end-----改自“柴门”-------*/
		}
		else sound=0,buzzer=1;	
	}
	else sound=0,buzzer=1;
	

/**************************************************************************/
//开关处理

    /*start------改自“柴门”--------*/
	//舵量增加时遥控首界面中的开关状态显示却是往下，改为开关在上时是舵量增加
	/*
	if(SA)Switch_A=1;
	else Switch_A=3;
	
	if(SB1&SB2)testljsf234099asfljl=2;
	else if(SB1&!SB2)testljsf234099asfljl=1;
	else testljsf234099asfljl=3;

	if(SC1&SC2)Switch_C=2;
	else if(SC1&!SC2)Switch_C=1;
	else Switch_C=3;
	
	if(SD)Switch_D=1;
	else Switch_D=3;
	
	if(SE)Switch_E=1;
	else Switch_E=3;
	*/
	if(SA)Switch_A=3;
	else Switch_A=1;
	
	if(SB1&SB2)testljsf234099asfljl=2;
	else if(SB1&!SB2)testljsf234099asfljl=3;
	else testljsf234099asfljl=1;

	if(SC1&SC2)Switch_C=2;
	else if(SC1&!SC2)Switch_C=3;
	else Switch_C=1;
	
	if(SD)Switch_D=3;
	else Switch_D=1;
	
	if(SE)Switch_E=3;
	else Switch_E=1;
	/*end------改自“柴门”--------*/
	
	if(Com_Swt_IN1)
	{
		
		
		if(Switch_Check(Com_Swt_IN1)==1)
		{
			if(Com_Swt_IN2)
			{
				if(Switch_Check(Com_Swt_IN2)==1)Combination_Switch=0;	
				else Combination_Switch=3;				
			}
			else Combination_Switch=0;
		}
		else if(Switch_Check(Com_Swt_IN1)==2)
		{
			if(Com_Swt_IN2)
			{
				if(Switch_Check(Com_Swt_IN2)==1)Combination_Switch=1;
				else Combination_Switch=4;
			}
			else Combination_Switch=1;
		}
		else
		{
			if(Com_Swt_IN2)
			{
				if(Switch_Check(Com_Swt_IN2)==1)Combination_Switch=2;
				else Combination_Switch=5;
			}
			else Combination_Switch=2;
		}
		
	}
	
	
	
	
	
	if(train_switch)
	{
		if(Switch_Check(train_switch)==3)train_output=1;
		else train_output=0;	
	}
	else train_output=0;
	
	if(clock_switch)
	{		
			if(Switch_Check(clock_switch)==3)clock_go=1;
			else if(Switch_Check(clock_switch)==2)clock_go=0;
			else clock_go=0,clock2=0,second2=0,minute2=0;		
		
	}
	else clock_go=0;
	
	if(ratio_switch)
	{
		if(Switch_Check(ratio_switch)==3)use_new_ratio=1;
		else use_new_ratio=0;
	}
	else use_new_ratio=0;
	
	if(auto_switch)
	{
		auto_mode=Switch_Check(auto_switch);
	}
	
	if(auto_mode==1)
	{
		auto_value=auto_middle*4.25+511;
	}
	else if(auto_mode==3)
	{
		if(auto_value>auto_max)auto_value=auto_max,auto_h=1,auto_l=0;
		if(auto_value<auto_min)auto_value=auto_min,auto_h=0,auto_l=1;
		if(auto_l)
		{
			auto_value+=auto_speed;
		}
		else if(auto_h)
		{
			auto_value-=auto_speed;
		}	
	}
	
	
	
	
	
	
/************************************************************************/
//数据运算	
	
	function_filter(0);
	function_filter(1);
	function_filter(2);
	function_filter(3);
	function_filter(4);
	function_filter(5);
	function_filter(6);
	function_filter(7);
	
	
	CH_value[0]=CH_calib(adc[6],CH1_offset);
	CH_value[1]=CH_calib(adc[7],CH2_offset);
	CH_value[2]=CH_calib(adc[4],CH3_offset);
	CH_value[3]=CH_calib(adc[5],CH4_offset);


	if(throttle_lock)
	{
		if(CH_inverted[2])
		{
			CH_value[2]=1023;
		}
		else
		{
			CH_value[2]=0;
		}
	}
	
	if(Auto_lock)
	{
		if(CH_inverted[2])
		{
			if(CH_value[2]<1013)throttle_idle=0;
		}
		else
		{
			if(CH_value[2]>10)throttle_idle=0;
		}
	}
	
	
	
	function_mapping(0);
	function_mapping(1);
	function_mapping(2);
	function_mapping(3);
	function_mapping(4);
	function_mapping(5);
	function_mapping(6);
	function_mapping(7);
	
	
	function_curve(0);//1.5ms
	function_curve(1);
	function_curve(2);
	
	function_curve2(0);
	function_curve2(1);
	function_curve2(2);
	function_curve2(3);
	function_curve2(4);
	

	if(use_new_ratio)
	{
		function_size(0,1);
		function_size(1,1);
		function_size(2,1);
		function_size(3,1);	
	}
	else 
	{
		function_size(0,0);
		function_size(1,0);
		function_size(2,0);
		function_size(3,0);
	}

	
	
	function_size(4,0);
	function_size(5,0);
	function_size(6,0);
	function_size(7,0);
	
	
	
	function_inverted(0);
	function_inverted(1);
	function_inverted(2);
	function_inverted(3);
	function_inverted(4);
	function_inverted(5);
	function_inverted(6);
	function_inverted(7);
	

	
	if(mixer1)
	{
		function_mix(mixer1_same,mixer1_contrary);
	}
	if(mixer2)
	{
		function_mix(mixer2_same,mixer2_contrary);
	}
	if(mixer3)
	{
		function_mix(mixer3_same,mixer3_contrary);
	}
	if(mixer4)
	{
		function_mix(mixer4_same,mixer4_contrary);
	}
	
	
	
	
	operation[0]+=511;
	operation[1]+=511;
	operation[2]+=511;
	operation[3]+=511;
	operation[4]+=511;
	operation[5]+=511;
	operation[6]+=511;
	operation[7]+=511;
	
	
	
	if(Com_Swt_IN1)
	{
		operation[Com_Swt_mapping]=740*Com_Swt_out[Combination_Switch]/100+143;	
	}
	
	
	
	
	Xdata_check(operation);
	
	
	NRF_buff[0]=operation[NRF_mapping[0]];
	NRF_buff[1]=operation[NRF_mapping[1]];
	NRF_buff[2]=operation[NRF_mapping[2]];
	NRF_buff[3]=operation[NRF_mapping[3]];
	NRF_buff[4]=operation[NRF_mapping[4]];
	NRF_buff[5]=operation[NRF_mapping[5]];
	NRF_buff[6]=operation[NRF_mapping[6]];
	NRF_buff[7]=operation[NRF_mapping[7]];
	
	if(PPM_updata>0)
	{
		PPM_updata--;
		if(train_output)
		{
			NRF_buff[0]=PPM_DATA[NRF_mapping[0]];
			NRF_buff[1]=PPM_DATA[NRF_mapping[1]];
			NRF_buff[2]=PPM_DATA[NRF_mapping[2]];
			NRF_buff[3]=PPM_DATA[NRF_mapping[3]];
			NRF_buff[4]=PPM_DATA[NRF_mapping[4]];
			NRF_buff[5]=PPM_DATA[NRF_mapping[5]];
			NRF_buff[6]=PPM_DATA[NRF_mapping[6]];
			NRF_buff[7]=PPM_DATA[NRF_mapping[7]];	
		}
		
	}
	
	PPMout[0]=operation[PPM_mapping[0]];
	PPMout[1]=operation[PPM_mapping[1]];
	PPMout[2]=operation[PPM_mapping[2]];
	PPMout[3]=operation[PPM_mapping[3]];
	PPMout[4]=operation[PPM_mapping[4]];
	PPMout[5]=operation[PPM_mapping[5]];
	PPMout[6]=operation[PPM_mapping[6]];
	PPMout[7]=operation[PPM_mapping[7]];

	
//operation[0]=adc[4];
//operation[1]=adc[5];	
//operation[2]=adc[6];
//operation[3]=adc[7];
/*
NRF_buff[0]=512;
NRF_buff[1]=512;
NRF_buff[2]=512;
NRF_buff[3]=512;
NRF_buff[4]=512;
NRF_buff[5]=512;
NRF_buff[6]=512;
NRF_buff[7]=512;*/


		tx_buff[0]=0xa1;
		tx_buff[1]=NRF_buff[0]>>2;
		tx_buff[2]=NRF_buff[0]<<6;
  	tx_buff[2]+=NRF_buff[1]>>4;
		tx_buff[3]=NRF_buff[1]<<4;
		tx_buff[3]+=NRF_buff[2]>>6;
		tx_buff[4]=NRF_buff[2]<<2;
		tx_buff[4]+=NRF_buff[3]>>8;
		tx_buff[5]=NRF_buff[3];

		tx_buff[6]=NRF_buff[4]>>2;
		tx_buff[7]=NRF_buff[4]<<6;
  	tx_buff[7]+=NRF_buff[5]>>4;
		tx_buff[8]=NRF_buff[5]<<4;
		tx_buff[8]+=NRF_buff[6]>>6;
		tx_buff[9]=NRF_buff[6]<<2;
		tx_buff[9]+=NRF_buff[7]>>8;
		tx_buff[10]=NRF_buff[7];

			
		tx_data[0]=tx_buff[0];
		tx_data[1]=tx_buff[1];
		tx_data[2]=tx_buff[2];
		tx_data[3]=tx_buff[3];
		tx_data[4]=tx_buff[4];
		tx_data[5]=tx_buff[5];
		tx_data[6]=tx_buff[6];
		tx_data[7]=tx_buff[7];
		tx_data[8]=tx_buff[8];
		tx_data[9]=tx_buff[9];
		tx_data[10]=tx_buff[10];
			
			
	

if(index_change)
{
	index_change=0;
	get_curve(last_index);//40ms	
	
}

/*
SBUF=adc[6]/1000+48;  while(!TI);TI=0;
SBUF=adc[6]%1000/100+48;  while(!TI);TI=0;
	SBUF=adc[6]%100/10+48;  while(!TI);TI=0; 
SBUF=adc[6]%10+48;  while(!TI);TI=0;
SBUF=' ';  while(!TI);TI=0;
SBUF=adc_buff[6]/1000+48;  while(!TI);TI=0;
SBUF=adc_buff[6]%1000/100+48;  while(!TI);TI=0;
	SBUF=adc_buff[6]%100/10+48;  while(!TI);TI=0; 
SBUF=adc_buff[6]%10+48;  while(!TI);TI=0;
SBUF='\n';  while(!TI);TI=0;

	SBUF=0x88;  while(!TI);TI=0;
SBUF=0xa1;  while(!TI);TI=0;
	SBUF=0x16;  while(!TI);TI=0; 
SBUF=adc[0]>>8;  while(!TI);TI=0;
SBUF=adc[0];  while(!TI);TI=0;
SBUF=adc_buff[0]>>8;  while(!TI);TI=0;
SBUF=adc_buff[0];  while(!TI);TI=0;
SBUF=adc_buff[6]>>8;  while(!TI);TI=0;
SBUF=adc_buff[6];  while(!TI);TI=0;
SBUF=adc[3]>>8;  while(!TI);TI=0;
SBUF=adc[3];  while(!TI);TI=0;
SBUF=adc[4]>>8;  while(!TI);TI=0;
SBUF=adc[4];  while(!TI);TI=0;
SBUF=adc[5]>>8;  while(!TI);TI=0;
SBUF=adc[5];  while(!TI);TI=0;
SBUF=adc[6]>>8;  while(!TI);TI=0;
SBUF=adc[6];  while(!TI);TI=0;
SBUF=adc[7]>>8;  while(!TI);TI=0;
SBUF=adc[7];  while(!TI);TI=0;
*/

//SC2=!SC2;		
//SA=!SA;

}

void EX1_isr()__interrupt 2	  //脉宽测量	 高优先级
{
	u16 width;u8 i;
	width=TH1;
	width<<=8;
	width+=TL1;
	TL1=0,TH1=0;
	
	if(width<500)
	{
		t_ppm=0;
	}
	else if(width<1750)
	{
		if(t_ppm<8)PPM_buff[t_ppm]=width-605;
		t_ppm++;
		
	}
	else if(width>3000&&width<14000)
	{
		
		if(t_ppm>5&&t_ppm<11)
		{
			t_ppm=0;
			if(PPM_updata<20)PPM_updata+=5;
			for(i=0;i<8;i++)
			PPM_DATA[i]=PPM_buff[i]+PPM_offset+PPM_offset;
			Xdata_check(PPM_DATA);
		}
	}
	else 
	{
		t_ppm=0;
	}

	
}

void ET1_isr()__interrupt 3	  //脉宽输出	 高优先级
{
	u16 temp;//	SC1=0;
	TR1=0;
	if(PPM_OUT)			  //输出8通道PPM，PPM信号总周期20ms，
	{	

							//每通道固定0.4ms低电平开始，0.6到1.6ms高电平结束
		t_PPMout++;
		switch(t_PPMout)
		{
			case 1:PPM=0;TL1=0x70,TH1=0xfe;temp=64935-PPMout[0];
							T_l=temp;T_h=temp>>8;break;				
			case 2:PPM=1;TL1=T_l;TH1=T_h;break;
			case 3:PPM=0;TL1=0x70,TH1=0xfe;temp=64935-PPMout[1];
							T_l=temp;T_h=temp>>8;break;				
			case 4:PPM=1;TL1=T_l;TH1=T_h;break;
			case 5:PPM=0;TL1=0x70,TH1=0xfe;temp=64935-PPMout[2];
							T_l=temp;T_h=temp>>8;break;				
			case 6:PPM=1;TL1=T_l;TH1=T_h;break;
			case 7:PPM=0;TL1=0x70,TH1=0xfe;temp=64935-PPMout[3];
							T_l=temp;T_h=temp>>8;break;				
			case 8:PPM=1;TL1=T_l;TH1=T_h;break;
			case 9:PPM=0;TL1=0x70,TH1=0xfe;temp=64935-PPMout[4];
							T_l=temp;T_h=temp>>8;break;				
			case 10:PPM=1;TL1=T_l;TH1=T_h;break;
			case 11:PPM=0;TL1=0x70,TH1=0xfe;temp=64935-PPMout[5];
							T_l=temp;T_h=temp>>8;break;				
			case 12:PPM=1;TL1=T_l;TH1=T_h;break;
			case 13:PPM=0;TL1=0x70,TH1=0xfe;temp=64935-PPMout[6];
							T_l=temp;T_h=temp>>8;break;				
			case 14:PPM=1;TL1=T_l;TH1=T_h;break;
			case 15:PPM=0;TL1=0x70,TH1=0xfe;temp=64935-PPMout[7];
							T_l=temp;T_h=temp>>8;break;				
			case 16:PPM=1;TL1=T_l;TH1=T_h;break;
			
			case 17:PPM=0;TL1=0x70,TH1=0xfe;break;				
			case 18:PPM=1;TL1=0xf0,TH1=0xd8;t_PPMout=0;break;
/*			
			case 13:PPM=0;TL1=0xe0,TH1=0xfc;break;				
			case 14:PPM=1;TL1=0x00,TH1=0xb0;t_PPMout=0;break;*/
			
		}
	}
	TR1=1;//SC1=1;
}

void PCA_isr()__interrupt 7	//任务定时器   中优先级
{//SB2=0;  
	if(CCF0)				//电压采集
	{
		CCF0=0;
		CCAP0L = value0;
    	CCAP0H = value0>> 8;            
    	value0 += 2500;	   //2.5ms
			  
//		out=!out;
		t_adc++;
		switch(t_adc)	//读取上次转换结果,开始下次转换；
		{			 
			case 1:adc_buff[0]=ADC_RES,adc_buff[0]<<=2,adc_buff[0]+=ADC_RESL;ADC_CONTR=0x89;break;
			case 2:adc_buff[1]=ADC_RES,adc_buff[1]<<=2,adc_buff[1]+=ADC_RESL;ADC_CONTR=0x8a;break;
			case 3:adc_buff[2]=ADC_RES,adc_buff[2]<<=2,adc_buff[2]+=ADC_RESL;ADC_CONTR=0x8b;break;
			case 4:adc_buff[3]=ADC_RES,adc_buff[3]<<=2,adc_buff[3]+=ADC_RESL;ADC_CONTR=0x8c;break;
			case 5:adc_buff[4]=ADC_RES,adc_buff[4]<<=2,adc_buff[4]+=ADC_RESL;ADC_CONTR=0x8d;break;
			case 6:adc_buff[5]=ADC_RES,adc_buff[5]<<=2,adc_buff[5]+=ADC_RESL;ADC_CONTR=0x8e;break;
			case 7:adc_buff[6]=ADC_RES,adc_buff[6]<<=2,adc_buff[6]+=ADC_RESL;ADC_CONTR=0x8f;break;
			case 8:adc_buff[7]=ADC_RES,adc_buff[7]<<=2,adc_buff[7]+=ADC_RESL;ADC_CONTR=0x88;t_adc=0;break;
		 }

	}								   
	else					//无线通信  
	{	CCF1=0;
			
	  
		if(RC_check)		//全频扫描
		{
			CCAP1L = value1;
    	CCAP1H = value1>> 8;            
    	value1 += 1234;	   //2ms
			
			if(RC_check_star)		
			{
				RC_check_star=0;
				RX_mode();
			}
			
			t_check++;
			if(t_check>125)t_check=0;
			
			
			if(REG_read(0x09))
			{
				CD_data[t_check/8]|=1<<(t_check%8);
			}
			else CD_data[t_check/8]&=~(1<<(t_check%8));

			
			NRF_channel(t_check);
			
		}
		else if(RC_TX)			  //NRF定时发射信号
		{
			RC_TX_state++;
	
			if(RC_TX_state==1)//TX
			{
				CCAP1L = value1;
				CCAP1H = value1>> 8;            
				value1 += 2000;	   //2ms
				
				t_hopping++;
				if(t_hopping>4)t_hopping=0;
				NRF_channel(hopping[t_hopping]);//顺序跳频
				
				if(NRF_change)
				{
					NRF_change=0;
					NRF_power(TX_power);
					TX_address(address);
					RX_address(address);
				}

				if(NRF_config_change)
				{
					NRF_config_change=0;
					tx_order[0]=0xa0;
					FIFO_write2(tx_order,11);
					
			
				}
				else if(Receiver_change)
				{
					Receiver_change=0;
					tx_order[0]=0xa2;
				
					FIFO_write2(tx_order,11);
					
					
				}
				else
				{

						FIFO_write2(tx_data,11);

				}
				
				
				
			}
			else if(RC_TX_state==2)//RX
			{
				CCAP1L = value1;
				CCAP1H = value1>> 8;            
				value1 += 16600;	   //17ms
				
				if(IRQ==0)
				{
					CE=0;
					REG_write(0x07,0x20);	//清除TX中断信号
					CE=1;	
				}
				
				RX_mode();
			}
			else if(RC_TX_state==3)
			{
				CCAP1L = value1;
				CCAP1H = value1>> 8;            
				value1 += 1400;	   //1ms
		
				RC_TX_state=0;
				if(IRQ==0)
				{
					FIFO_read2(rx,11);		//读取接收数据
					CE=0;
					REG_write(0x07,0x40);	//清除RX中断信号
					CE=1;	
					receive++;
					timer3=0;
				}
				else
				{
					timer3++;
					if(timer3>49)timer3=49,receive=0;
					
				}
				
				timer2++;
				if(timer2>49)timer2=0,FUCK_sum=receive,receive=0;
				TX_mode();	
				
			}

		}

		
	}
//SB2=1;	
} 
