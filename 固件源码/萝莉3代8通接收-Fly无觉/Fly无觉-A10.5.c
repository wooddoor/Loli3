#include<reg51.h>
#include<intrins.h>
#define u8 unsigned char
#define u16 unsigned int

//15系列新增寄存器
sfr IAP_DATA=0xc2;
sfr IAP_ADDRH=0xc3;
sfr IAP_ADDRL=0xc4;
sfr IAP_CMD=0xc5;
sfr IAP_TRIG=0xc6;
sfr IAP_CONTR=0xc7;

sfr AUXR=0x8e;
sfr T2H=0xd6;
sfr T2L=0xd7;
sfr IE2=0xaf;
sfr P1ASF=0x9d;
sfr ADC_CONTR=0xbc;
sfr ADC_RES=0xbd;
sfr ADC_RESL=0xbe;

sfr P5=0xc8;
sfr P0M0=0x94;
sfr P1M0=0x92;
sfr P2M0=0x96;
sfr P3M0=0xb2;
sfr P4M0=0xb4;
sfr P1M1=0x91;

sfr P_SW1   = 0xA2;             //外设功能切换寄存器1
sfr CMOD=0xd9;
sfr CCON=0xd8;
sfr CCAPM0=0xda;
sfr CCAPM1=0xdb;
sfr CCAPM2=0xdc;
sfr CCAP0H=0xfa;
sfr CCAP1H=0xfb;
sfr CCAP2H=0xfc;
sfr CL=0xe9;
sfr CH=0xf9;
sfr PCA_PWM0=0xf2;
sfr PCA_PWM1=0xf3;
sfr PCA_PWM2=0xf4;



/**************************************************************************/
//引脚定义
sbit LED=P1^0;	//定义指示灯

/*
sbit CH1=P3^7;	//定义8个通道输出
sbit CH2=P3^6;
sbit CH3=P3^3;
sbit CH4=P3^2;
sbit CH5=P3^1;
sbit CH6=P3^0;
sbit CH7=P3^4;
sbit CH8=P3^5;

sbit MDO=P1^5;	//定义无线模块的管脚
sbit SCK=P5^4;
sbit CE=P5^5;
sbit IRQ=P1^4;
sbit MDI=P1^3;
sbit CSN=P1^2;
*/
sbit CH1=P3^7;	//定义8个通道输出
sbit CH2=P3^6;
sbit CH3=P3^5;
sbit CH4=P3^4;
sbit CH5=P3^3;
sbit CH6=P3^2;
sbit CH7=P3^1;
sbit CH8=P3^0;

sbit MDO=P1^5;	//定义无线模块的管脚
sbit SCK=P1^7;
sbit CE=P5^5;
sbit IRQ=P1^4;
sbit MDI=P1^6;
sbit CSN=P5^4;




/**************************************************************************/

u8 code random[100]={4,1,3,2,2,1,0,0,2,2,2,3,4,1,2,1,4,3,3,4,//随机跳频序列
										 2,0,2,2,3,1,2,3,2,2,2,4,2,4,0,3,4,2,3,1,
										 0,3,1,3,3,0,2,0,4,3,3,3,3,3,4,1,1,4,3,0,
	                   1,0,3,2,3,2,3,3,4,4,1,3,0,0,3,1,3,3,3,0,
	                   3,3,4,1,2,4,1,3,0,1,3,4,4,3,2,3,1,2,3,3};

//数据保存
bit Data_change;
u8 xdata TEMP_DATA[50];
											

//无线通信
u8 rx[11];		//接收的11字节数据
u8 tx[11];
u8 m;
u8 hopping_turn,hopping_num,hopping_count;

u8 xdata address[5];
u8 code address_0[5]={'L','O','V','E','!'};//使用LOVE作为对频暗语

u8 hopping[5]={10,35,60,85,110};
bit first,restar;
bit connecting,lose;
bit jump_1,jump_2,jump_mode;

u8 NRF_error;


u8 TX_power=3;



//通道输出
u16 xdata timer1=0,timer2=0,timer3=0,timer4=0,timer5=0;
u8 xdata rx_num=0,receive=0;
u8 xdata Light_LED=0;

u16 xdata out_control_data[8]={511,511,80,511,511,511,511,511};

u16 CH_data[8]={0,1024,0,1024,0,1024};
u16 data buff[8];
u16 temp1,temp2;
u8 t_output;



//电压检测
u8 t_adc;
u16 adc1,adc2;
long idata last_adc1,last_adc2;
u16 voltage_bec,voltage_total;



//PPM
u8 t_PPMout;
u8 T_h,T_l;
bit PPM_OUT;


bit CH1_SW,CH2_SW,CH3_SW,CH4_SW,CH5_SW,CH6_SW,CH7_SW,CH8_SW;
bit CH1_PWM,CH2_PWM,CH7_PWM;



//SBUS
bit SBUS;
u8 t_sbus;
u8 xdata Sbus_buff[25],Sbus_tx[25];
u16 xdata Sbus_data[8];


/**************************************************************************/
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

void delay_ms(u8 i)
{
	while(i--)
	Delay1ms();
}

/**************************************************************************/
//ROM存储
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

u8 EEPROM_test(u8 address_H)
{
	IAP_CONTR=0x83;
	
	EEPROM_clean(address_H);
	EEPROM_write(address_H,0,0x88);
	if(EEPROM_read(address_H,0)==0x88)
	{
		EEPROM_clean(address_H);
		EEPROM_write(address_H,0,0x55);
		if(EEPROM_read(address_H,0)==0x55)
		{
			return 1;
		}
	}
	
	return 0;
}



void DATA_read()
{
	u8 i;
	u16 sum=0,sum_read;
	
	IAP_CONTR=0x83;
	for(i=0;i<40;i++)
	{
		TEMP_DATA[i]=EEPROM_read(0,i);	
		sum+=TEMP_DATA[i];
	}
	
	sum_read=EEPROM_read(0,40);//读校验和
	sum_read<<=8;
	sum_read+=EEPROM_read(0,41);
	
	if(TEMP_DATA[0]!=0xab||sum!=sum_read)//比较校验和
	{
		
	   first=1;

	}
	else
	{
	  hopping[0]=TEMP_DATA[1];
		hopping[1]=TEMP_DATA[2];
		hopping[2]=TEMP_DATA[3];
		hopping[3]=TEMP_DATA[4];
		hopping[4]=TEMP_DATA[5];
		address[0]=TEMP_DATA[6];
		address[1]=TEMP_DATA[7];
		address[2]=TEMP_DATA[8];
		address[3]=TEMP_DATA[9];
		address[4]=TEMP_DATA[10];
		
		out_control_data[0]=TEMP_DATA[11];
		out_control_data[0]<<=8;
		out_control_data[0]+=TEMP_DATA[12];
		out_control_data[1]=TEMP_DATA[13];
		out_control_data[1]<<=8;
		out_control_data[1]+=TEMP_DATA[14];
		out_control_data[2]=TEMP_DATA[15];
		out_control_data[2]<<=8;
		out_control_data[2]+=TEMP_DATA[16];
		out_control_data[3]=TEMP_DATA[17];
		out_control_data[3]<<=8;
		out_control_data[3]+=TEMP_DATA[18];
		out_control_data[4]=TEMP_DATA[19];
		out_control_data[4]<<=8;
		out_control_data[4]+=TEMP_DATA[20];
		out_control_data[5]=TEMP_DATA[21];
		out_control_data[5]<<=8;
		out_control_data[5]+=TEMP_DATA[22];
		out_control_data[6]=TEMP_DATA[23];
		out_control_data[6]<<=8;
		out_control_data[6]+=TEMP_DATA[24];
		out_control_data[7]=TEMP_DATA[25];
		out_control_data[7]<<=8;
		out_control_data[7]+=TEMP_DATA[26];
		
		PPM_OUT=TEMP_DATA[27];
		CH1_PWM=TEMP_DATA[28];
		CH2_PWM=TEMP_DATA[29];
		CH7_PWM=TEMP_DATA[30];
		CH1_SW=TEMP_DATA[31];
		CH2_SW=TEMP_DATA[32];
		CH3_SW=TEMP_DATA[33];
		CH4_SW=TEMP_DATA[34];
		CH5_SW=TEMP_DATA[35];
		CH6_SW=TEMP_DATA[36];
		CH7_SW=TEMP_DATA[37];
		CH8_SW=TEMP_DATA[38];
		SBUS=TEMP_DATA[39];
		
	} 
	
	IAP_CONTR=0;
}
void DATA_save()
{
	u8 i;u16 sum;
	
	TEMP_DATA[0]=0xab;

	TEMP_DATA[1]=hopping[0];	//修改目标模型数据
	TEMP_DATA[2]=hopping[1];
	TEMP_DATA[3]=hopping[2];
	TEMP_DATA[4]=hopping[3];
	TEMP_DATA[5]=hopping[4];
	TEMP_DATA[6]=address[0];	//修改目标模型数据
	TEMP_DATA[7]=address[1];
	TEMP_DATA[8]=address[2];
	TEMP_DATA[9]=address[3];
	TEMP_DATA[10]=address[4];
	
	TEMP_DATA[11]=out_control_data[0]>>8;
	TEMP_DATA[12]=out_control_data[0];
	TEMP_DATA[13]=out_control_data[1]>>8;
	TEMP_DATA[14]=out_control_data[1];
	TEMP_DATA[15]=out_control_data[2]>>8;
	TEMP_DATA[16]=out_control_data[2];
	TEMP_DATA[17]=out_control_data[3]>>8;
	TEMP_DATA[18]=out_control_data[3];
	TEMP_DATA[19]=out_control_data[4]>>8;
	TEMP_DATA[20]=out_control_data[4];
	TEMP_DATA[21]=out_control_data[5]>>8;
	TEMP_DATA[22]=out_control_data[5];
	TEMP_DATA[23]=out_control_data[6]>>8;
	TEMP_DATA[24]=out_control_data[6];
	TEMP_DATA[25]=out_control_data[7]>>8;
	TEMP_DATA[26]=out_control_data[7];
	
	TEMP_DATA[27]=PPM_OUT;
	TEMP_DATA[28]=CH1_PWM;
	TEMP_DATA[29]=CH2_PWM;
	TEMP_DATA[30]=CH7_PWM;
	TEMP_DATA[31]=CH1_SW;
	TEMP_DATA[32]=CH2_SW;
	TEMP_DATA[33]=CH3_SW;
	TEMP_DATA[34]=CH4_SW;
	TEMP_DATA[35]=CH5_SW;
	TEMP_DATA[36]=CH6_SW;
	TEMP_DATA[37]=CH7_SW;
	TEMP_DATA[38]=CH8_SW;
	TEMP_DATA[39]=SBUS;
	
	sum=0;
	for(i=0;i<40;i++)
	{
		sum+=TEMP_DATA[i];
	}
	TEMP_DATA[40]=sum>>8;//写入校验和
	TEMP_DATA[41]=sum;
	
	
	IAP_CONTR=0x83;
		EEPROM_clean(0);
					
		for(i=0;i<42;i++)
		{
			EEPROM_write(0,i,TEMP_DATA[i]);
		}
	
		IAP_CONTR=0;
}
/**************************************************************************/
//NRF24L01
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

void FIFO_write(u8 DATA_OUT[],u8 lengh)
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
	REG_write(0x00,0x3b); //CRC,8 bit,Power on,RX
	CE=1;
} 				   
	
void TX_mode()				 
{
	CE=0;
	REG_write(0x00,0x0a);
  CE=1;
}

void  NRF_power(u8 P)				//发射功率设置 250k
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
	RX_mode(); 		//	 REG_write(0x1d,0x01);
	NRF_channel(66);
	NRF_power(TX_power);
	NRF_size(11);
	RX_address(address);
	TX_address(address);
}

void NRF_test()	//无线模块终极测试
{	
	u8 reset_err=0;
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
	
	FIFO_write(tx,1);
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

void receiver_connect()
{
	u8 t;
	NRF_power(0);
	NRF_channel(33);
	TX_address(address_0);
	RX_address(address_0);
	while(IRQ);
	FIFO_read(rx,11);		//读取接收数据
	CE=0;
	REG_write(0x07,0x40);	//清除无线模块中断信号
	CE=1;
	if(rx[0]==0xa0)
	{
		hopping[0]=rx[1];
		hopping[1]=rx[2];
		hopping[2]=rx[3];
		hopping[3]=rx[4];
		hopping[4]=rx[5];
		address[0]=rx[6];
		address[1]=rx[7];
		address[2]=rx[8];
		address[3]=rx[9];
		address[4]=rx[10];
	}
	
	tx[0]='O',tx[1]='K';
	connecting=1;
	while(connecting)
	{
		TX_mode();		
		NRF_channel(33);
		TX_address(address_0);
		RX_address(address_0);
		FIFO_write(tx,11);
		Delay1ms();
		
		RX_mode();
		NRF_channel(hopping[0]);
		TX_address(address);
		RX_address(address);
		while(1)
		{
			Delay1ms();
			if(IRQ==0)
			{
				FIFO_read(rx,11);		//读取接收数据
				CE=0;
				REG_write(0x07,0x40);	//清除无线模块中断信号
				CE=1;	 
				connecting=0;break;
			}
			t++;if(t>100){t=0;break;}
		}			  
	}
		DATA_save();
	RX_address(address);
	TX_address(address);
	NRF_power(3);
}

/**************************************************************************/

void data_check(int x,int max,int min)
{
	
	if(x>max)x=max;
	if(x<min)x=min;

}

void Get_Sbus_data()
{
	Sbus_data[0]=CH_data[0]<<1;
	Sbus_data[1]=CH_data[1]<<1;
	Sbus_data[2]=CH_data[2]<<1;
	Sbus_data[3]=CH_data[3]<<1;
	Sbus_data[4]=CH_data[4]<<1;
	Sbus_data[5]=CH_data[5]<<1;
	Sbus_data[6]=CH_data[6]<<1;
	Sbus_data[7]=CH_data[7]<<1;

	

	Sbus_buff[1]=Sbus_data[0];//CH1~CH8
	Sbus_buff[2]=Sbus_data[0]>>8;
	Sbus_buff[2]+=Sbus_data[1]<<3;

	Sbus_buff[3]=Sbus_data[1]>>5;
	Sbus_buff[3]+=Sbus_data[2]<<6;

	Sbus_buff[4]=Sbus_data[2]>>2;
	
	Sbus_buff[5]=Sbus_data[2]>>10;
		Sbus_buff[5]+=Sbus_data[3]<<1;
	
	Sbus_buff[6]=Sbus_data[3]>>7;
		Sbus_buff[6]+=Sbus_data[4]<<4;
	
	Sbus_buff[7]=Sbus_data[4]>>4;
		Sbus_buff[7]+=Sbus_data[5]<<7;
		
	Sbus_buff[8]=Sbus_data[5]>>1;
		
	Sbus_buff[9]=Sbus_data[5]>>9;
		Sbus_buff[9]+=Sbus_data[6]<<2;
		
	Sbus_buff[10]=Sbus_data[6]>>6;
		Sbus_buff[10]+=Sbus_data[7]<<5;
		
	Sbus_buff[11]=Sbus_data[7]>>3;
	
	Sbus_tx[0]=0x0f;//Star				
	Sbus_tx[1]=Sbus_buff[1];//CH1~CH8
	Sbus_tx[2]=Sbus_buff[2];
	Sbus_tx[3]=Sbus_buff[3];
	Sbus_tx[4]=Sbus_buff[4];
	Sbus_tx[5]=Sbus_buff[5];
	Sbus_tx[6]=Sbus_buff[6];
	Sbus_tx[7]=Sbus_buff[7];
	Sbus_tx[8]=Sbus_buff[8];
	Sbus_tx[9]=Sbus_buff[9];
	Sbus_tx[10]=Sbus_buff[10];
	Sbus_tx[11]=Sbus_buff[11];				
	Sbus_tx[12]=0;//CH9~CH16
	Sbus_tx[13]=0;
	Sbus_tx[14]=0;
	Sbus_tx[15]=0;
	Sbus_tx[16]=0;
	Sbus_tx[17]=0;
	Sbus_tx[18]=0;
	Sbus_tx[19]=0;
	Sbus_tx[20]=0;
	Sbus_tx[21]=0;
	Sbus_tx[22]=0;				
	Sbus_tx[23]=0;//flag
	Sbus_tx[24]=0;//End
}

void Cycle()
{
	timer1++;
	if(timer1>333)			//每秒统计接收到的有效信号，小于20个点亮LED示警，表示信号微弱
	{
		timer1=333;
		rx_num=0;
		Light_LED=2;
	}
	
	
	timer2++;
	if(timer2>333)
	{
		timer2=0;
		rx_num=receive;
		receive=0;
	}
	
	timer3++;
	if(timer3>666)//信号丢失2秒触发失控保护
	{
		timer3=666;
		CH_data[0]=out_control_data[0];
		CH_data[1]=out_control_data[1];
		CH_data[2]=out_control_data[2];
		CH_data[3]=out_control_data[3];
		CH_data[4]=out_control_data[4];
		CH_data[5]=out_control_data[5];
		CH_data[6]=out_control_data[6];
		CH_data[7]=out_control_data[7];
		
		Get_Sbus_data();
	}
	
	if(Data_change)
	{
		timer4++;
		if(timer4>333)
		{
			timer4=0;
			Data_change=0;
			DATA_save();
		}	
	}
	
	if(Light_LED==1)
	{
		LED=0;
		timer5++;
		if(timer5>10)
		{
			timer5=0;
			Light_LED=0;
			
		}
	}
	else if(Light_LED==2)LED=0;
	else LED=1;

		
	m++;					//两个标志位循环跳频
	if(jump_1==0)
	{
		if(m>7)lose=0,jump_1=1,jump_2=0;	  //未收到信号，开始第一跳
	}
	else
	{
		if(jump_2==0)
		{
			if(m>14)lose=0,jump_2=1;		 //第一跳之后仍无信号，开始第二跳
		}
		else
		{
			 if(m>22)m=0,lose=0,jump_1=0;	 //第二跳之后仍无信号，复位重新跳。
			
		}		
	}	
	
	t_adc++;
	if(t_adc==1)
	{
		adc1=ADC_RES;
		adc1<<=2,adc1+=ADC_RESL;
		
		last_adc1=(adc1+last_adc1*9/10);
		
		P1ASF=0x02;
		ADC_CONTR=0x89;	
	}
	else if(t_adc==2)
	{
		t_adc=0;
		adc2=ADC_RES;
		adc2<<=2,adc2+=ADC_RESL;
		
		last_adc2=(adc2+last_adc2*9/10);
		
		P1ASF=0x00;
		ADC_CONTR=0x88;	
	}
	
}

/*************************************************************/
//串口通信


void UartInit(void)		//100000bps@12.000MHz
{
	SCON = 0xD0;		//9位数据,可变波特率
	AUXR |= 0x01;		//串口1选择定时器2为波特率发生器
	AUXR |= 0x04;		//定时器2时钟为Fosc,即1T
	T2L = 0xE2;		//设定定时初值
	T2H = 0xFF;		//设定定时初值

}

void LED_flash(u8 t)
{
	
	while(t)
	{
		LED=0;		
		delay_ms(50);
		LED=1;		
		delay_ms(50);
		t--;
	}
}

void initial()
{	

	
	
	CH5=0;CH6=1;			//如果CH5与CH6被短接，重新对码
	Delay1ms();
	if(CH6==0)		  //修正通道6插上舵机不能用
	{
		P3M0=0x02;	  //插上舵机也会使CH6为0，所以将CH5设为推挽
		CH5=1;
		Delay1ms();
		if(CH6)restar=1;   //如果CH6被拉高，说明5/6通道短接，启动重新对码
	}
	CH5=0;CH6=0;
	 	
	
	P3M0=0xcf;	 //
	P1M1=0x02;	//将通道输出IO口配置为推挽模式，保证正常驱动电调与舵机
		
	
	while(1)
	{
		if(EEPROM_test(2))break;
		else delay_ms(100);
	}
	
	DATA_read();
	data_check(out_control_data[0],1023,0);
	data_check(out_control_data[1],1023,0);
	data_check(out_control_data[2],1023,0);
	data_check(out_control_data[3],1023,0);
	data_check(out_control_data[4],1023,0);
	data_check(out_control_data[5],1023,0);
	data_check(out_control_data[6],1023,0);
	data_check(out_control_data[7],1023,0);
	
	
	

	NRF_test();
	if(NRF_error)//模块错误闪灯
	{
		LED_flash(10);
	}
	
	NRF_init();

	

	if(restar)first=1;
	if(first)
	{
		receiver_connect();
	}
	
}

																	     

main()
{	
	CH1=0;
	CH2=0;
	CH3=0;
	CH4=0;
	CH5=0;
	CH6=0;
	CH7=0;
	CH8=0;
	
/*	while(PCON)
	{
		PCON=0;
		Delay1ms();
	}*/
	
		
	delay_ms(200);
	LED=0;
	delay_ms(200);//开机延时以避过电源波动
	
	initial();
	LED=1;		//点亮指示灯再关闭，表示单片机正常工作
	

	ADC_CONTR=0x80;		 //ADC上电

	P_SW1=0x10;//PWM引脚切换到P3口
	PCA_PWM2=0x03;
	PCA_PWM1=0x03;
	if(CH1_PWM)CCAPM2=0x42;
	if(CH2_PWM)CCAPM1=0x42;

	  	
	IP=0x02; //T0高优先
	IE=0x82; //允许T0中断
	

	if(SBUS)
	{
		UartInit();
		CMOD=0x01;//允许PCA中断
		IE2=0;CH5=1;
	}
	else
	{
		T2L=0x47,T2H=0xf4;//T2 3ms
		IE2=0x04;//允许T2中断
		CMOD=0;
	}
					

	
	TMOD=0x00;	//定时器模式0

	AUXR|=0x10; 	  //打开定时器2
	CCON=0x40;			 //打开PCA定时器
	
/*	
while(1)
{
	SBUF=0xf0;
	delay_ms(10);
}*/
	
	NRF_channel(hopping[0]);
	
	
	while(IRQ);
	TR0=1;
	lose=1;
	
	while(1)
	{	
		while(IRQ&lose);
		if(lose)
		{	
			jump_1=0;jump_2=0;hopping_count=0;//收到有效信号后刷新跳频器
			receive++;timer1=0;timer3=0;m=0;	
			if(Light_LED)Light_LED=0;
			
			FIFO_read(rx,11);		//读取接收数据
			CE=0;
			REG_write(0x07,0x40);	//清除无线模块中断信号
			CE=1;
			
			TX_mode();
			tx[0]=rx_num;
			voltage_bec=1299210/last_adc1;
			tx[1]=voltage_bec>>8;
			tx[2]=voltage_bec;
			voltage_total=(long)last_adc2*voltage_bec*3/10240;
			tx[3]=voltage_total>>8;
			tx[4]=voltage_total;
			
			FIFO_write(tx,11);
			Delay1ms();

			RX_mode();
						
			if(jump_mode)
			{
				jump_mode=0;
				hopping_turn=random[hopping_num];
			}
			hopping_turn++;
			if(hopping_turn>4)hopping_turn=0;
			NRF_channel(hopping[hopping_turn]);
			


			if(rx[0]==0xa2)
			{
				Light_LED=1;
				
				if(rx[1]&0x80)
				{
					PPM_OUT=1;
				}
				else 
				{
					PPM_OUT=0;
				}
				
				if(rx[1]&0x40)
				{
					SBUS=1;
				}
				else 
				{
					SBUS=0;
				}
				
				if(SBUS)
				{
					UartInit();
					CMOD=0x01;//允许PCA中断
					IE2=0;
				}
				else
				{
					T2L=0x47,T2H=0xf4;//T2 3ms
					AUXR &= 0xFB;//12T
					IE2=0x04;//允许T2中断
					CMOD=0;
				}
				
				if(rx[1]&0x08)
				{
					CH1_PWM=1;CCAPM2=0x42;
				}
				else 
				{
					CH1_PWM=0;CCAPM2=0;
				}
				
				if(rx[1]&0x04)
				{
					CH2_PWM=1;CCAPM1=0x42;
				}
				else 
				{
					CH2_PWM=0;CCAPM1=0;
				}
				
				if(rx[1]&0x02)
				{
					CH7_PWM=1;CCAPM0=0x42;
				}
				else 
				{
					CH7_PWM=0;CCAPM0=0;
				}
				
				if(rx[2]&0x80)CH1_SW=1;
				else CH1_SW=0;
				if(rx[2]&0x40)CH2_SW=1;
				else CH2_SW=0;
				if(rx[2]&0x20)CH3_SW=1;
				else CH3_SW=0;
				if(rx[2]&0x10)CH4_SW=1;
				else CH4_SW=0;
				if(rx[2]&0x08)CH5_SW=1;
				else CH5_SW=0;
				if(rx[2]&0x04)CH6_SW=1;
				else CH6_SW=0;
				if(rx[2]&0x02)CH7_SW=1;
				else CH7_SW=0;
				if(rx[2]&0x01)CH8_SW=1;
				else CH8_SW=0;
				
				
				Data_change=1;timer4=0;
			}
			else if(rx[0]==0xa0)
			{
				Light_LED=1;LED=0;
				
				buff[0]=rx[1];
				buff[0]<<=2;
				buff[0]+=rx[2]>>6;
				buff[1]=rx[2]&0x3f;
				buff[1]<<=4;
				buff[1]+=rx[3]>>4;			
				buff[2]=rx[3]&0x0f;
				buff[2]<<=6;
				buff[2]+=rx[4]>>2;
				buff[3]=rx[4]&0x03;
				buff[3]<<=8;
				buff[3]+=rx[5];

				buff[4]=rx[6];
				buff[4]<<=2;
				buff[4]+=rx[7]>>6;
				buff[5]=rx[7]&0x3f;
				buff[5]<<=4;
				buff[5]+=rx[8]>>4;			
				buff[6]=rx[8]&0x0f;
				buff[6]<<=6;
				buff[6]+=rx[9]>>2;
				buff[7]=rx[9]&0x03;
				buff[7]<<=8;
				buff[7]+=rx[10];
				
				
				data_check(buff[0],1023,0);
				data_check(buff[1],1023,0);
				data_check(buff[2],1023,0);
				data_check(buff[3],1023,0);
				data_check(buff[4],1023,0);
				data_check(buff[5],1023,0);
				data_check(buff[6],1023,0);
				data_check(buff[7],1023,0);
				
				out_control_data[0]=buff[0];
				out_control_data[1]=buff[1];
				out_control_data[2]=buff[2];
				out_control_data[3]=buff[3];
				out_control_data[4]=buff[4];
				out_control_data[5]=buff[5];
				out_control_data[6]=buff[6];
				out_control_data[7]=buff[7];
				
				
				Data_change=1;timer4=0;
				
				
			}
			else if(rx[0]==0xa1)
			{
				buff[0]=rx[1];
				buff[0]<<=2;
				buff[0]+=rx[2]>>6;
				buff[1]=rx[2]&0x3f;
				buff[1]<<=4;
				buff[1]+=rx[3]>>4;			
				buff[2]=rx[3]&0x0f;
				buff[2]<<=6;
				buff[2]+=rx[4]>>2;
				buff[3]=rx[4]&0x03;
				buff[3]<<=8;
				buff[3]+=rx[5];

				buff[4]=rx[6];
				buff[4]<<=2;
				buff[4]+=rx[7]>>6;
				buff[5]=rx[7]&0x3f;
				buff[5]<<=4;
				buff[5]+=rx[8]>>4;			
				buff[6]=rx[8]&0x0f;
				buff[6]<<=6;
				buff[6]+=rx[9]>>2;
				buff[7]=rx[9]&0x03;
				buff[7]<<=8;
				buff[7]+=rx[10];
				
				
				data_check(buff[0],1023,0);
				data_check(buff[1],1023,0);
				data_check(buff[2],1023,0);
				data_check(buff[3],1023,0);
				data_check(buff[4],1023,0);
				data_check(buff[5],1023,0);
				data_check(buff[6],1023,0);
				data_check(buff[7],1023,0);
				
				CH_data[0]=buff[0];
				CH_data[1]=buff[1];
				CH_data[2]=buff[2];
				CH_data[3]=buff[3];
				CH_data[4]=buff[4];
				CH_data[5]=buff[5];
				CH_data[6]=buff[6];
				CH_data[7]=buff[7];
				
				Get_Sbus_data();




			}
			
			


		}
		else
		{
		 	hopping_count++;
			if(hopping_count>5)jump_mode=1;
			
			if(jump_mode)
			{
				hopping_num++;
				if(hopping_num>99)hopping_num=0;
				NRF_channel(hopping[random[hopping_num]]);
				lose=1;
			}
			else
			{
				hopping_turn++;
				if(hopping_turn>4)hopping_turn=0;
				NRF_channel(hopping[hopping_turn]);
				lose=1;
			}
			
		}
		
	}
	
}


void ET0_isr()interrupt 1		//定时器0用作信号输出	
{
	u16 temp;
	if(PPM_OUT)			  //输出8通道PPM，PPM信号总周期20ms，
	{	
							//每通道固定0.4ms低电平开始，0.6到1.6ms高电平结束
		t_PPMout++;
		switch(t_PPMout)
		{
			case 1:CH1=1;TL0=0x70,TH0=0xfe;temp=64935-CH_data[0];
							T_l=temp;T_h=temp>>8;break;				
			case 2:CH1=0;TL0=T_l;TH0=T_h;break;
			case 3:CH1=1;TL0=0x70,TH0=0xfe;temp=64935-CH_data[1];
							T_l=temp;T_h=temp>>8;break;				
			case 4:CH1=0;TL0=T_l;TH0=T_h;break;
			case 5:CH1=1;TL0=0x70,TH0=0xfe;temp=64935-CH_data[2];
							T_l=temp;T_h=temp>>8;break;				
			case 6:CH1=0;TL0=T_l;TH0=T_h;break;
			case 7:CH1=1;TL0=0x70,TH0=0xfe;temp=64935-CH_data[3];
							T_l=temp;T_h=temp>>8;break;				
			case 8:CH1=0;TL0=T_l;TH0=T_h;break;
			case 9:CH1=1;TL0=0x70,TH0=0xfe;temp=64935-CH_data[4];
							T_l=temp;T_h=temp>>8;break;				
			case 10:CH1=0;TL0=T_l;TH0=T_h;break;
			case 11:CH1=1;TL0=0x70,TH0=0xfe;temp=64935-CH_data[5];
							T_l=temp;T_h=temp>>8;break;				
			case 12:CH1=0;TL0=T_l;TH0=T_h;break;
			case 13:CH1=1;TL0=0x70,TH0=0xfe;temp=64935-CH_data[6];
							T_l=temp;T_h=temp>>8;break;				
			case 14:CH1=0;TL0=T_l;TH0=T_h;break;
			case 15:CH1=1;TL0=0x70,TH0=0xfe;temp=64935-CH_data[7];
							T_l=temp;T_h=temp>>8;break;				
			case 16:CH1=0;TL0=T_l;TH0=T_h;break;
			
			case 17:CH1=1;TL0=0x70,TH0=0xfe;break;				
			case 18:CH1=0;TL0=0xf0,TH0=0xd8;t_PPMout=0;break;
			
			
		}
	}
	else if(SBUS)
	{
		CH5=1;
		if(t_sbus>24)
		{
			t_sbus=0;
		}
		else
		{
		
			if(TI)TI=0;
			ACC=Sbus_tx[t_sbus];
			TB8=P;
			SBUF=ACC;	
			
			t_sbus++;
		}
		
		
		if(t_sbus==25)
		{
			TL0=0xF0;TH0=0xD8;//10ms
		}
		else 
		{
			TL0=0x88;TH0=0xFF;//120us
		}

		
	}
	else 
	{
		t_output++;
		switch(t_output)
		{
			
			case 1:if(CH1_PWM)//输出PWM
							{
								if(CH_data[0]<100)
								{
									PCA_PWM2=0x03;	
								}
								else
								{
									PCA_PWM2=0x00;
								}
								CCAP2H=255-CH_data[0]/4;
							}
							else
							{
								if(CH1_SW)//输出电平信号
								{
									if(CH_data[0]<500)CH1=0;
									else CH1=1;
								}
								else CH1=1;	//输出舵量
							}
									
							temp2=128600-temp1;
							TL0=temp2,TH0=temp2>>8;break;	
							
			case 2:if(CH1_PWM==0)
							{
								if(CH1_SW==0)CH1=0;
							}
							
							temp1=64725-CH_data[1]*27/20;
							TL0=temp1,TH0=temp1>>8;break;				
							
							
			case 3:if(CH2_PWM)
							{
								if(CH_data[1]<100)
								{
									PCA_PWM1=0x03;	
								}
								else
								{
									PCA_PWM1=0x00;
								}
								CCAP1H=255-CH_data[1]/4;
							}
							else
							{
								if(CH2_SW)
								{
									if(CH_data[1]<500)CH2=0;
									else CH2=1;
								}
								else CH2=1;
							}
							
							temp2=128600-temp1;
							TL0=temp2,TH0=temp2>>8;break;
														
			case 4:if(CH2_PWM==0)
							{
								if(CH2_SW==0)CH2=0;
							}
							
							temp1=64725-CH_data[2]*27/20;
							TL0=temp1,TH0=temp1>>8;break;
							
							
							
			case 5:if(CH3_SW)
							{
								if(CH_data[2]<500)CH3=0;
								else CH3=1;
							}
							else CH3=1;
							
							temp2=128600-temp1;
							TL0=temp2,TH0=temp2>>8;break;
							
			case 6:if(CH3_SW==0)CH3=0;
							
							temp1=64725-CH_data[3]*27/20;
							TL0=temp1,TH0=temp1>>8;break;
							
							
			case 7:if(CH4_SW)
							{
								if(CH_data[3]<500)CH4=0;
								else CH4=1;
							}
							else CH4=1;
							
							temp2=128600-temp1;
							TL0=temp2,TH0=temp2>>8;break;
							
			case 8:if(CH4_SW==0)CH4=0;
							
							temp1=64725-CH_data[4]*27/20;
							TL0=temp1,TH0=temp1>>8;break;
							
							
			case 9:if(CH5_SW)
							{
								if(CH_data[4]<500)CH5=0;
								else CH5=1;
							}
							else CH5=1;
							
							temp2=128600-temp1;
							TL0=temp2,TH0=temp2>>8;break;
							
			case 10:if(CH5_SW==0)CH5=0;
							
							temp1=64725-CH_data[5]*27/20;
							TL0=temp1,TH0=temp1>>8;break;
							
							
			case 11:if(CH6_SW)
							{
								if(CH_data[5]<500)CH6=0;
								else CH6=1;
							}
							else CH6=1;
							
							temp2=128600-temp1;
							TL0=temp2,TH0=temp2>>8;break;
							
			case 12:if(CH6_SW==0)CH6=0;
							
							temp1=64725-CH_data[6]*27/20;
							TL0=temp1,TH0=temp1>>8;break;
													
			case 13:if(CH7_SW)
							{
								if(CH_data[6]<500)CH7=0;
								else CH7=1;
							}
							else CH7=1;
							
							temp2=128600-temp1;
							TL0=temp2,TH0=temp2>>8;break;
							
			case 14:if(CH7_SW==0)CH7=0;
							
							temp1=64725-CH_data[7]*27/20;
							TL0=temp1,TH0=temp1>>8;break;
							
			case 15:if(CH8_SW)
							{
								if(CH_data[7]<500)CH8=0;
								else CH8=1;
							}
							else CH8=1;
							
							temp2=128600-temp1;
							TL0=temp2,TH0=temp2>>8;break;
							
			case 16:if(CH8_SW==0)CH8=0;
							
							temp1=64725-CH_data[0]*27/20;
							TL0=temp1,TH0=temp1>>8;t_output=0;break;
		}	
	}
}

void PCA_isr()interrupt 7
{
	CL=0x48,CH=0xf4;	  //3ms
	CCON=0x40;
	
	
	Cycle();
	
//CH4=!CH4;
}

void et2()interrupt 12	//跳频定时器 3ms		
{	
	
	Cycle();
//CH2=!CH2;
//CH3=!CH3;	
}	
	
