#ifndef __STC12C5Axx_H_
#define __STC12C5Axx_H_
__sfr __at (0xe0) ___ACC;
__sfr __at (0xbc) ___ADC_CONTR;
__sfr __at (0xbd) ___ADC_RES;
__sfr __at (0xbe) ___ADC_RESL;
__sfr __at (0x8e) ___AUXR;
__sfr __at (0xa2) ___AUXR1;
__sfr __at (0xf0) ___B;
__sfr __at (0x9c) ___BRT;
__sfr __at (0xa1) ___BUS_SPEED;
__sfr __at (0xfa) ___CCAP0H;
__sfr __at (0xea) ___CCAP0L;
__sfr __at (0xfb) ___CCAP1H;
__sfr __at (0xeb) ___CCAP1L;
__sfr __at (0xda) ___CCAPM0;
__sfr __at (0xdb) ___CCAPM1;
__sfr __at (0xd8) ___CCON;
__sfr __at (0xf9) ___CH;
__sfr __at (0xe9) ___CL;
__sfr __at (0x97) ___CLK_DIV;
__sfr __at (0xd9) ___CMOD;
__sfr __at (0x83) ___DPH;
__sfr __at (0x82) ___DPL;
__sfr __at (0xc3) ___IAP_ADDRH;
__sfr __at (0xc4) ___IAP_ADDRL;
__sfr __at (0xc5) ___IAP_CMD;
__sfr __at (0xc7) ___IAP_CONTR;
__sfr __at (0xc2) ___IAP_DATA;
__sfr __at (0xc6) ___IAP_TRIG;
__sfr __at (0xa8) ___IE;
__sfr __at (0xaf) ___IE2;
__sfr __at (0xb8) ___IP;
__sfr __at (0xb5) ___IP2;
__sfr __at (0xb7) ___IPH;
__sfr __at (0xb6) ___IPH2;
__sfr __at (0x80) ___P0;
__sfr __at (0x94) ___P0M0;
__sfr __at (0x93) ___P0M1;
__sfr __at (0x90) ___P1;
__sfr __at (0x9d) ___P1ASF;
__sfr __at (0x92) ___P1M0;
__sfr __at (0x91) ___P1M1;
__sfr __at (0xa0) ___P2;
__sfr __at (0x96) ___P2M0;
__sfr __at (0x95) ___P2M1;
__sfr __at (0xb0) ___P3;
__sfr __at (0xb2) ___P3M0;
__sfr __at (0xb1) ___P3M1;
__sfr __at (0xc0) ___P4;
__sfr __at (0xb4) ___P4M0;
__sfr __at (0xb3) ___P4M1;
__sfr __at (0xbb) ___P4SW;
__sfr __at (0xc8) ___P5;
__sfr __at (0xca) ___P5M0;
__sfr __at (0xc9) ___P5M1;
__sfr __at (0xf2) ___PCA_PWM0;
__sfr __at (0xf3) ___PCA_PWM1;
__sfr __at (0x87) ___PCON;
__sfr __at (0xd0) ___PSW;
__sfr __at (0x9b) ___S2BUF;
__sfr __at (0x9a) ___S2CON;
__sfr __at (0xa9) ___SADDR;
__sfr __at (0xb9) ___SADEN;
__sfr __at (0x99) ___SBUF;
__sfr __at (0x98) ___SCON;
__sfr __at (0x81) ___SP;
__sfr __at (0xce) ___SPCTL;
__sfr __at (0xcf) ___SPDAT;
__sfr __at (0xcd) ___SPSTAT;
__sfr __at (0x88) ___TCON;
__sfr __at (0x8c) ___TH0;
__sfr __at (0x8d) ___TH1;
__sfr __at (0x8a) ___TL0;
__sfr __at (0x8b) ___TL1;
__sfr __at (0x89) ___TMOD;
__sfr __at (0x8f) ___WAKE_CLKO;
__sfr __at (0xc1) ___WDT_CONTR;
__sbit __at (0xd6) ___AC;
__sbit __at (0xd8) ___CCF0;
__sbit __at (0xd9) ___CCF1;
__sbit __at (0xdf) ___CF;
__sbit __at (0xde) ___CR;
__sbit __at (0xd7) ___CY;
__sbit __at (0xaf) ___EA;
__sbit __at (0xad) ___EADC;
__sbit __at (0xae) ___ELVD;
__sbit __at (0xac) ___ES;
__sbit __at (0xa9) ___ET0;
__sbit __at (0xab) ___ET1;
__sbit __at (0xa8) ___EX0;
__sbit __at (0xaa) ___EX1;
__sbit __at (0xd5) ___F0;
__sbit __at (0x89) ___IE0;
__sbit __at (0x8b) ___IE1;
__sbit __at (0x88) ___IT0;
__sbit __at (0x8a) ___IT1;
__sbit __at (0xd2) ___OV;
__sbit __at (0xd0) ___P;
__sbit __at (0x80) ___P00;
__sbit __at (0x81) ___P01;
__sbit __at (0x82) ___P02;
__sbit __at (0x83) ___P03;
__sbit __at (0x84) ___P04;
__sbit __at (0x85) ___P05;
__sbit __at (0x86) ___P06;
__sbit __at (0x87) ___P07;
__sbit __at (0x90) ___P10;
__sbit __at (0x91) ___P11;
__sbit __at (0x92) ___P12;
__sbit __at (0x93) ___P13;
__sbit __at (0x94) ___P14;
__sbit __at (0x95) ___P15;
__sbit __at (0x96) ___P16;
__sbit __at (0x97) ___P17;
__sbit __at (0xa0) ___P20;
__sbit __at (0xa1) ___P21;
__sbit __at (0xa2) ___P22;
__sbit __at (0xa3) ___P23;
__sbit __at (0xa4) ___P24;
__sbit __at (0xa5) ___P25;
__sbit __at (0xa6) ___P26;
__sbit __at (0xa7) ___P27;
__sbit __at (0xb0) ___P30;
__sbit __at (0xb1) ___P31;
__sbit __at (0xb2) ___P32;
__sbit __at (0xb3) ___P33;
__sbit __at (0xb4) ___P34;
__sbit __at (0xb5) ___P35;
__sbit __at (0xb6) ___P36;
__sbit __at (0xb7) ___P37;
__sbit __at (0xc0) ___P40;
__sbit __at (0xc1) ___P41;
__sbit __at (0xc2) ___P42;
__sbit __at (0xc3) ___P43;
__sbit __at (0xc4) ___P44;
__sbit __at (0xc5) ___P45;
__sbit __at (0xc6) ___P46;
__sbit __at (0xc7) ___P47;
__sbit __at (0xc8) ___P50;
__sbit __at (0xc9) ___P51;
__sbit __at (0xca) ___P52;
__sbit __at (0xcb) ___P53;
__sbit __at (0xbd) ___PADC;
__sbit __at (0xbe) ___PLVD;
__sbit __at (0xbf) ___PPCA;
__sbit __at (0xbc) ___PS;
__sbit __at (0xb9) ___PT0;
__sbit __at (0xbb) ___PT1;
__sbit __at (0xb8) ___PX0;
__sbit __at (0xba) ___PX1;
__sbit __at (0x9a) ___RB8;
__sbit __at (0x9c) ___REN;
__sbit __at (0x98) ___RI;
__sbit __at (0xd3) ___RS0;
__sbit __at (0xd4) ___RS1;
__sbit __at (0x9f) ___SM0;
__sbit __at (0x9e) ___SM1;
__sbit __at (0x9d) ___SM2;
__sbit __at (0x9b) ___TB8;
__sbit __at (0x8d) ___TF0;
__sbit __at (0x8f) ___TF1;
__sbit __at (0x99) ___TI;
__sbit __at (0x8c) ___TR0;
__sbit __at (0x8e) ___TR1;


#define ACC ___ACC
#define ADC_CONTR ___ADC_CONTR
#define ADC_RES ___ADC_RES
#define ADC_RESL ___ADC_RESL
#define AUXR ___AUXR
#define AUXR1 ___AUXR1
#define B ___B
#define BRT ___BRT
#define BUS_SPEED ___BUS_SPEED
#define CCAP0H ___CCAP0H
#define CCAP0L ___CCAP0L
#define CCAP1H ___CCAP1H
#define CCAP1L ___CCAP1L
#define CCAPM0 ___CCAPM0
#define CCAPM1 ___CCAPM1
#define CCON ___CCON
#define CH ___CH
#define CL ___CL
#define CLK_DIV ___CLK_DIV
#define CMOD ___CMOD
#define DPH ___DPH
#define DPL ___DPL
#define IAP_ADDRH ___IAP_ADDRH
#define IAP_ADDRL ___IAP_ADDRL
#define IAP_CMD ___IAP_CMD
#define IAP_CONTR ___IAP_CONTR
#define IAP_DATA ___IAP_DATA
#define IAP_TRIG ___IAP_TRIG
#define IE ___IE
#define IE2 ___IE2
#define IP ___IP
#define IP2 ___IP2
#define IPH ___IPH
#define IPH2 ___IPH2
#define P0 ___P0
#define P0M0 ___P0M0
#define P0M1 ___P0M1
#define P1 ___P1
#define P1ASF ___P1ASF
#define P1M0 ___P1M0
#define P1M1 ___P1M1
#define P2 ___P2
#define P2M0 ___P2M0
#define P2M1 ___P2M1
#define P3 ___P3
#define P3M0 ___P3M0
#define P3M1 ___P3M1
#define P4 ___P4
#define P4M0 ___P4M0
#define P4M1 ___P4M1
#define P4SW ___P4SW
#define P5 ___P5
#define P5M0 ___P5M0
#define P5M1 ___P5M1
#define PCA_PWM0 ___PCA_PWM0
#define PCA_PWM1 ___PCA_PWM1
#define PCON ___PCON
#define PSW ___PSW
#define S2BUF ___S2BUF
#define S2CON ___S2CON
#define SADDR ___SADDR
#define SADEN ___SADEN
#define SBUF ___SBUF
#define SCON ___SCON
#define SP ___SP
#define SPCTL ___SPCTL
#define SPDAT ___SPDAT
#define SPSTAT ___SPSTAT
#define TCON ___TCON
#define TH0 ___TH0
#define TH1 ___TH1
#define TL0 ___TL0
#define TL1 ___TL1
#define TMOD ___TMOD
#define WAKE_CLKO ___WAKE_CLKO
#define WDT_CONTR ___WDT_CONTR
#define AC ___AC
#define CCF0 ___CCF0
#define CCF1 ___CCF1
#define CF ___CF
#define CR ___CR
#define CY ___CY
#define EA ___EA
#define EADC ___EADC
#define ELVD ___ELVD
#define ES ___ES
#define ET0 ___ET0
#define ET1 ___ET1
#define EX0 ___EX0
#define EX1 ___EX1
#define F0 ___F0
#define IE0 ___IE0
#define IE1 ___IE1
#define IT0 ___IT0
#define IT1 ___IT1
#define OV ___OV
#define P ___P
#define P00 ___P00
#define P01 ___P01
#define P02 ___P02
#define P03 ___P03
#define P04 ___P04
#define P05 ___P05
#define P06 ___P06
#define P07 ___P07
#define P10 ___P10
#define P11 ___P11
#define P12 ___P12
#define P13 ___P13
#define P14 ___P14
#define P15 ___P15
#define P16 ___P16
#define P17 ___P17
#define P20 ___P20
#define P21 ___P21
#define P22 ___P22
#define P23 ___P23
#define P24 ___P24
#define P25 ___P25
#define P26 ___P26
#define P27 ___P27
#define P30 ___P30
#define P31 ___P31
#define P32 ___P32
#define P33 ___P33
#define P34 ___P34
#define P35 ___P35
#define P36 ___P36
#define P37 ___P37
#define P40 ___P40
#define P41 ___P41
#define P42 ___P42
#define P43 ___P43
#define P44 ___P44
#define P45 ___P45
#define P46 ___P46
#define P47 ___P47
#define P50 ___P50
#define P51 ___P51
#define P52 ___P52
#define P53 ___P53
#define PADC ___PADC
#define PLVD ___PLVD
#define PPCA ___PPCA
#define PS ___PS
#define PT0 ___PT0
#define PT1 ___PT1
#define PX0 ___PX0
#define PX1 ___PX1
#define RB8 ___RB8
#define REN ___REN
#define RI ___RI
#define RS0 ___RS0
#define RS1 ___RS1
#define SM0 ___SM0
#define SM1 ___SM1
#define SM2 ___SM2
#define TB8 ___TB8
#define TF0 ___TF0
#define TF1 ___TF1
#define TI ___TI
#define TR0 ___TR0
#define TR1 ___TR1
#endif