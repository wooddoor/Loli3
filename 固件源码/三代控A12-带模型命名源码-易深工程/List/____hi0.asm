;--------------------------------------------------------
; File Created by SDCC : free open source ANSI-C Compiler
; Version 3.9.0 #11195 (MINGW32)
;--------------------------------------------------------
	.module ____hi0
	.optsdcc -mmcs51 --model-small
	
;--------------------------------------------------------
; Public variables in this module
;--------------------------------------------------------
	.globl _main
	.globl ____TR1
	.globl ____TR0
	.globl ____TI
	.globl ____TF1
	.globl ____TF0
	.globl ____TB8
	.globl ____SM2
	.globl ____SM1
	.globl ____SM0
	.globl ____RS1
	.globl ____RS0
	.globl ____RI
	.globl ____REN
	.globl ____RB8
	.globl ____PX1
	.globl ____PX0
	.globl ____PT1
	.globl ____PT0
	.globl ____PS
	.globl ____PPCA
	.globl ____PLVD
	.globl ____PADC
	.globl ____P53
	.globl ____P52
	.globl ____P51
	.globl ____P50
	.globl ____P47
	.globl ____P46
	.globl ____P45
	.globl ____P44
	.globl ____P43
	.globl ____P42
	.globl ____P41
	.globl ____P40
	.globl ____P37
	.globl ____P36
	.globl ____P35
	.globl ____P34
	.globl ____P33
	.globl ____P32
	.globl ____P31
	.globl ____P30
	.globl ____P27
	.globl ____P26
	.globl ____P25
	.globl ____P24
	.globl ____P23
	.globl ____P22
	.globl ____P21
	.globl ____P20
	.globl ____P17
	.globl ____P16
	.globl ____P15
	.globl ____P14
	.globl ____P13
	.globl ____P12
	.globl ____P11
	.globl ____P10
	.globl ____P07
	.globl ____P06
	.globl ____P05
	.globl ____P04
	.globl ____P03
	.globl ____P02
	.globl ____P01
	.globl ____P00
	.globl ____P
	.globl ____OV
	.globl ____IT1
	.globl ____IT0
	.globl ____IE1
	.globl ____IE0
	.globl ____F0
	.globl ____EX1
	.globl ____EX0
	.globl ____ET1
	.globl ____ET0
	.globl ____ES
	.globl ____ELVD
	.globl ____EADC
	.globl ____EA
	.globl ____CY
	.globl ____CR
	.globl ____CF
	.globl ____CCF1
	.globl ____CCF0
	.globl ____AC
	.globl ____WDT_CONTR
	.globl ____WAKE_CLKO
	.globl ____TMOD
	.globl ____TL1
	.globl ____TL0
	.globl ____TH1
	.globl ____TH0
	.globl ____TCON
	.globl ____SPSTAT
	.globl ____SPDAT
	.globl ____SPCTL
	.globl ____SP
	.globl ____SCON
	.globl ____SBUF
	.globl ____SADEN
	.globl ____SADDR
	.globl ____S2CON
	.globl ____S2BUF
	.globl ____PSW
	.globl ____PCON
	.globl ____PCA_PWM1
	.globl ____PCA_PWM0
	.globl ____P5M1
	.globl ____P5M0
	.globl ____P5
	.globl ____P4SW
	.globl ____P4M1
	.globl ____P4M0
	.globl ____P4
	.globl ____P3M1
	.globl ____P3M0
	.globl ____P3
	.globl ____P2M1
	.globl ____P2M0
	.globl ____P2
	.globl ____P1M1
	.globl ____P1M0
	.globl ____P1ASF
	.globl ____P1
	.globl ____P0M1
	.globl ____P0M0
	.globl ____P0
	.globl ____IPH2
	.globl ____IPH
	.globl ____IP2
	.globl ____IP
	.globl ____IE2
	.globl ____IE
	.globl ____IAP_TRIG
	.globl ____IAP_DATA
	.globl ____IAP_CONTR
	.globl ____IAP_CMD
	.globl ____IAP_ADDRL
	.globl ____IAP_ADDRH
	.globl ____DPL
	.globl ____DPH
	.globl ____CMOD
	.globl ____CLK_DIV
	.globl ____CL
	.globl ____CH
	.globl ____CCON
	.globl ____CCAPM1
	.globl ____CCAPM0
	.globl ____CCAP1L
	.globl ____CCAP1H
	.globl ____CCAP0L
	.globl ____CCAP0H
	.globl ____BUS_SPEED
	.globl ____BRT
	.globl ____B
	.globl ____AUXR1
	.globl ____AUXR
	.globl ____ADC_RESL
	.globl ____ADC_RES
	.globl ____ADC_CONTR
	.globl ____ACC
;--------------------------------------------------------
; special function registers
;--------------------------------------------------------
	.area RSEG    (ABS,DATA)
	.org 0x0000
G$___ACC$0_0$0 == 0x00e0
____ACC	=	0x00e0
G$___ADC_CONTR$0_0$0 == 0x00bc
____ADC_CONTR	=	0x00bc
G$___ADC_RES$0_0$0 == 0x00bd
____ADC_RES	=	0x00bd
G$___ADC_RESL$0_0$0 == 0x00be
____ADC_RESL	=	0x00be
G$___AUXR$0_0$0 == 0x008e
____AUXR	=	0x008e
G$___AUXR1$0_0$0 == 0x00a2
____AUXR1	=	0x00a2
G$___B$0_0$0 == 0x00f0
____B	=	0x00f0
G$___BRT$0_0$0 == 0x009c
____BRT	=	0x009c
G$___BUS_SPEED$0_0$0 == 0x00a1
____BUS_SPEED	=	0x00a1
G$___CCAP0H$0_0$0 == 0x00fa
____CCAP0H	=	0x00fa
G$___CCAP0L$0_0$0 == 0x00ea
____CCAP0L	=	0x00ea
G$___CCAP1H$0_0$0 == 0x00fb
____CCAP1H	=	0x00fb
G$___CCAP1L$0_0$0 == 0x00eb
____CCAP1L	=	0x00eb
G$___CCAPM0$0_0$0 == 0x00da
____CCAPM0	=	0x00da
G$___CCAPM1$0_0$0 == 0x00db
____CCAPM1	=	0x00db
G$___CCON$0_0$0 == 0x00d8
____CCON	=	0x00d8
G$___CH$0_0$0 == 0x00f9
____CH	=	0x00f9
G$___CL$0_0$0 == 0x00e9
____CL	=	0x00e9
G$___CLK_DIV$0_0$0 == 0x0097
____CLK_DIV	=	0x0097
G$___CMOD$0_0$0 == 0x00d9
____CMOD	=	0x00d9
G$___DPH$0_0$0 == 0x0083
____DPH	=	0x0083
G$___DPL$0_0$0 == 0x0082
____DPL	=	0x0082
G$___IAP_ADDRH$0_0$0 == 0x00c3
____IAP_ADDRH	=	0x00c3
G$___IAP_ADDRL$0_0$0 == 0x00c4
____IAP_ADDRL	=	0x00c4
G$___IAP_CMD$0_0$0 == 0x00c5
____IAP_CMD	=	0x00c5
G$___IAP_CONTR$0_0$0 == 0x00c7
____IAP_CONTR	=	0x00c7
G$___IAP_DATA$0_0$0 == 0x00c2
____IAP_DATA	=	0x00c2
G$___IAP_TRIG$0_0$0 == 0x00c6
____IAP_TRIG	=	0x00c6
G$___IE$0_0$0 == 0x00a8
____IE	=	0x00a8
G$___IE2$0_0$0 == 0x00af
____IE2	=	0x00af
G$___IP$0_0$0 == 0x00b8
____IP	=	0x00b8
G$___IP2$0_0$0 == 0x00b5
____IP2	=	0x00b5
G$___IPH$0_0$0 == 0x00b7
____IPH	=	0x00b7
G$___IPH2$0_0$0 == 0x00b6
____IPH2	=	0x00b6
G$___P0$0_0$0 == 0x0080
____P0	=	0x0080
G$___P0M0$0_0$0 == 0x0094
____P0M0	=	0x0094
G$___P0M1$0_0$0 == 0x0093
____P0M1	=	0x0093
G$___P1$0_0$0 == 0x0090
____P1	=	0x0090
G$___P1ASF$0_0$0 == 0x009d
____P1ASF	=	0x009d
G$___P1M0$0_0$0 == 0x0092
____P1M0	=	0x0092
G$___P1M1$0_0$0 == 0x0091
____P1M1	=	0x0091
G$___P2$0_0$0 == 0x00a0
____P2	=	0x00a0
G$___P2M0$0_0$0 == 0x0096
____P2M0	=	0x0096
G$___P2M1$0_0$0 == 0x0095
____P2M1	=	0x0095
G$___P3$0_0$0 == 0x00b0
____P3	=	0x00b0
G$___P3M0$0_0$0 == 0x00b2
____P3M0	=	0x00b2
G$___P3M1$0_0$0 == 0x00b1
____P3M1	=	0x00b1
G$___P4$0_0$0 == 0x00c0
____P4	=	0x00c0
G$___P4M0$0_0$0 == 0x00b4
____P4M0	=	0x00b4
G$___P4M1$0_0$0 == 0x00b3
____P4M1	=	0x00b3
G$___P4SW$0_0$0 == 0x00bb
____P4SW	=	0x00bb
G$___P5$0_0$0 == 0x00c8
____P5	=	0x00c8
G$___P5M0$0_0$0 == 0x00ca
____P5M0	=	0x00ca
G$___P5M1$0_0$0 == 0x00c9
____P5M1	=	0x00c9
G$___PCA_PWM0$0_0$0 == 0x00f2
____PCA_PWM0	=	0x00f2
G$___PCA_PWM1$0_0$0 == 0x00f3
____PCA_PWM1	=	0x00f3
G$___PCON$0_0$0 == 0x0087
____PCON	=	0x0087
G$___PSW$0_0$0 == 0x00d0
____PSW	=	0x00d0
G$___S2BUF$0_0$0 == 0x009b
____S2BUF	=	0x009b
G$___S2CON$0_0$0 == 0x009a
____S2CON	=	0x009a
G$___SADDR$0_0$0 == 0x00a9
____SADDR	=	0x00a9
G$___SADEN$0_0$0 == 0x00b9
____SADEN	=	0x00b9
G$___SBUF$0_0$0 == 0x0099
____SBUF	=	0x0099
G$___SCON$0_0$0 == 0x0098
____SCON	=	0x0098
G$___SP$0_0$0 == 0x0081
____SP	=	0x0081
G$___SPCTL$0_0$0 == 0x00ce
____SPCTL	=	0x00ce
G$___SPDAT$0_0$0 == 0x00cf
____SPDAT	=	0x00cf
G$___SPSTAT$0_0$0 == 0x00cd
____SPSTAT	=	0x00cd
G$___TCON$0_0$0 == 0x0088
____TCON	=	0x0088
G$___TH0$0_0$0 == 0x008c
____TH0	=	0x008c
G$___TH1$0_0$0 == 0x008d
____TH1	=	0x008d
G$___TL0$0_0$0 == 0x008a
____TL0	=	0x008a
G$___TL1$0_0$0 == 0x008b
____TL1	=	0x008b
G$___TMOD$0_0$0 == 0x0089
____TMOD	=	0x0089
G$___WAKE_CLKO$0_0$0 == 0x008f
____WAKE_CLKO	=	0x008f
G$___WDT_CONTR$0_0$0 == 0x00c1
____WDT_CONTR	=	0x00c1
;--------------------------------------------------------
; special function bits
;--------------------------------------------------------
	.area RSEG    (ABS,DATA)
	.org 0x0000
G$___AC$0_0$0 == 0x00d6
____AC	=	0x00d6
G$___CCF0$0_0$0 == 0x00d8
____CCF0	=	0x00d8
G$___CCF1$0_0$0 == 0x00d9
____CCF1	=	0x00d9
G$___CF$0_0$0 == 0x00df
____CF	=	0x00df
G$___CR$0_0$0 == 0x00de
____CR	=	0x00de
G$___CY$0_0$0 == 0x00d7
____CY	=	0x00d7
G$___EA$0_0$0 == 0x00af
____EA	=	0x00af
G$___EADC$0_0$0 == 0x00ad
____EADC	=	0x00ad
G$___ELVD$0_0$0 == 0x00ae
____ELVD	=	0x00ae
G$___ES$0_0$0 == 0x00ac
____ES	=	0x00ac
G$___ET0$0_0$0 == 0x00a9
____ET0	=	0x00a9
G$___ET1$0_0$0 == 0x00ab
____ET1	=	0x00ab
G$___EX0$0_0$0 == 0x00a8
____EX0	=	0x00a8
G$___EX1$0_0$0 == 0x00aa
____EX1	=	0x00aa
G$___F0$0_0$0 == 0x00d5
____F0	=	0x00d5
G$___IE0$0_0$0 == 0x0089
____IE0	=	0x0089
G$___IE1$0_0$0 == 0x008b
____IE1	=	0x008b
G$___IT0$0_0$0 == 0x0088
____IT0	=	0x0088
G$___IT1$0_0$0 == 0x008a
____IT1	=	0x008a
G$___OV$0_0$0 == 0x00d2
____OV	=	0x00d2
G$___P$0_0$0 == 0x00d0
____P	=	0x00d0
G$___P00$0_0$0 == 0x0080
____P00	=	0x0080
G$___P01$0_0$0 == 0x0081
____P01	=	0x0081
G$___P02$0_0$0 == 0x0082
____P02	=	0x0082
G$___P03$0_0$0 == 0x0083
____P03	=	0x0083
G$___P04$0_0$0 == 0x0084
____P04	=	0x0084
G$___P05$0_0$0 == 0x0085
____P05	=	0x0085
G$___P06$0_0$0 == 0x0086
____P06	=	0x0086
G$___P07$0_0$0 == 0x0087
____P07	=	0x0087
G$___P10$0_0$0 == 0x0090
____P10	=	0x0090
G$___P11$0_0$0 == 0x0091
____P11	=	0x0091
G$___P12$0_0$0 == 0x0092
____P12	=	0x0092
G$___P13$0_0$0 == 0x0093
____P13	=	0x0093
G$___P14$0_0$0 == 0x0094
____P14	=	0x0094
G$___P15$0_0$0 == 0x0095
____P15	=	0x0095
G$___P16$0_0$0 == 0x0096
____P16	=	0x0096
G$___P17$0_0$0 == 0x0097
____P17	=	0x0097
G$___P20$0_0$0 == 0x00a0
____P20	=	0x00a0
G$___P21$0_0$0 == 0x00a1
____P21	=	0x00a1
G$___P22$0_0$0 == 0x00a2
____P22	=	0x00a2
G$___P23$0_0$0 == 0x00a3
____P23	=	0x00a3
G$___P24$0_0$0 == 0x00a4
____P24	=	0x00a4
G$___P25$0_0$0 == 0x00a5
____P25	=	0x00a5
G$___P26$0_0$0 == 0x00a6
____P26	=	0x00a6
G$___P27$0_0$0 == 0x00a7
____P27	=	0x00a7
G$___P30$0_0$0 == 0x00b0
____P30	=	0x00b0
G$___P31$0_0$0 == 0x00b1
____P31	=	0x00b1
G$___P32$0_0$0 == 0x00b2
____P32	=	0x00b2
G$___P33$0_0$0 == 0x00b3
____P33	=	0x00b3
G$___P34$0_0$0 == 0x00b4
____P34	=	0x00b4
G$___P35$0_0$0 == 0x00b5
____P35	=	0x00b5
G$___P36$0_0$0 == 0x00b6
____P36	=	0x00b6
G$___P37$0_0$0 == 0x00b7
____P37	=	0x00b7
G$___P40$0_0$0 == 0x00c0
____P40	=	0x00c0
G$___P41$0_0$0 == 0x00c1
____P41	=	0x00c1
G$___P42$0_0$0 == 0x00c2
____P42	=	0x00c2
G$___P43$0_0$0 == 0x00c3
____P43	=	0x00c3
G$___P44$0_0$0 == 0x00c4
____P44	=	0x00c4
G$___P45$0_0$0 == 0x00c5
____P45	=	0x00c5
G$___P46$0_0$0 == 0x00c6
____P46	=	0x00c6
G$___P47$0_0$0 == 0x00c7
____P47	=	0x00c7
G$___P50$0_0$0 == 0x00c8
____P50	=	0x00c8
G$___P51$0_0$0 == 0x00c9
____P51	=	0x00c9
G$___P52$0_0$0 == 0x00ca
____P52	=	0x00ca
G$___P53$0_0$0 == 0x00cb
____P53	=	0x00cb
G$___PADC$0_0$0 == 0x00bd
____PADC	=	0x00bd
G$___PLVD$0_0$0 == 0x00be
____PLVD	=	0x00be
G$___PPCA$0_0$0 == 0x00bf
____PPCA	=	0x00bf
G$___PS$0_0$0 == 0x00bc
____PS	=	0x00bc
G$___PT0$0_0$0 == 0x00b9
____PT0	=	0x00b9
G$___PT1$0_0$0 == 0x00bb
____PT1	=	0x00bb
G$___PX0$0_0$0 == 0x00b8
____PX0	=	0x00b8
G$___PX1$0_0$0 == 0x00ba
____PX1	=	0x00ba
G$___RB8$0_0$0 == 0x009a
____RB8	=	0x009a
G$___REN$0_0$0 == 0x009c
____REN	=	0x009c
G$___RI$0_0$0 == 0x0098
____RI	=	0x0098
G$___RS0$0_0$0 == 0x00d3
____RS0	=	0x00d3
G$___RS1$0_0$0 == 0x00d4
____RS1	=	0x00d4
G$___SM0$0_0$0 == 0x009f
____SM0	=	0x009f
G$___SM1$0_0$0 == 0x009e
____SM1	=	0x009e
G$___SM2$0_0$0 == 0x009d
____SM2	=	0x009d
G$___TB8$0_0$0 == 0x009b
____TB8	=	0x009b
G$___TF0$0_0$0 == 0x008d
____TF0	=	0x008d
G$___TF1$0_0$0 == 0x008f
____TF1	=	0x008f
G$___TI$0_0$0 == 0x0099
____TI	=	0x0099
G$___TR0$0_0$0 == 0x008c
____TR0	=	0x008c
G$___TR1$0_0$0 == 0x008e
____TR1	=	0x008e
;--------------------------------------------------------
; overlayable register banks
;--------------------------------------------------------
	.area REG_BANK_0	(REL,OVR,DATA)
	.ds 8
;--------------------------------------------------------
; internal ram data
;--------------------------------------------------------
	.area DSEG    (DATA)
;--------------------------------------------------------
; overlayable items in internal ram 
;--------------------------------------------------------
;--------------------------------------------------------
; Stack segment in internal ram 
;--------------------------------------------------------
	.area	SSEG
__start__stack:
	.ds	1

;--------------------------------------------------------
; indirectly addressable internal ram data
;--------------------------------------------------------
	.area ISEG    (DATA)
;--------------------------------------------------------
; absolute internal ram data
;--------------------------------------------------------
	.area IABS    (ABS,DATA)
	.area IABS    (ABS,DATA)
;--------------------------------------------------------
; bit data
;--------------------------------------------------------
	.area BSEG    (BIT)
;--------------------------------------------------------
; paged external ram data
;--------------------------------------------------------
	.area PSEG    (PAG,XDATA)
;--------------------------------------------------------
; external ram data
;--------------------------------------------------------
	.area XSEG    (XDATA)
;--------------------------------------------------------
; absolute external ram data
;--------------------------------------------------------
	.area XABS    (ABS,XDATA)
;--------------------------------------------------------
; external initialized ram data
;--------------------------------------------------------
	.area XISEG   (XDATA)
	.area HOME    (CODE)
	.area GSINIT0 (CODE)
	.area GSINIT1 (CODE)
	.area GSINIT2 (CODE)
	.area GSINIT3 (CODE)
	.area GSINIT4 (CODE)
	.area GSINIT5 (CODE)
	.area GSINIT  (CODE)
	.area GSFINAL (CODE)
	.area CSEG    (CODE)
;--------------------------------------------------------
; interrupt vector 
;--------------------------------------------------------
	.area HOME    (CODE)
__interrupt_vect:
	ljmp	__sdcc_gsinit_startup
;--------------------------------------------------------
; global & static initialisations
;--------------------------------------------------------
	.area HOME    (CODE)
	.area GSINIT  (CODE)
	.area GSFINAL (CODE)
	.area GSINIT  (CODE)
	.globl __sdcc_gsinit_startup
	.globl __sdcc_program_startup
	.globl __start__stack
	.globl __mcs51_genXINIT
	.globl __mcs51_genXRAMCLEAR
	.globl __mcs51_genRAMCLEAR
	.area GSFINAL (CODE)
	ljmp	__sdcc_program_startup
;--------------------------------------------------------
; Home
;--------------------------------------------------------
	.area HOME    (CODE)
	.area HOME    (CODE)
__sdcc_program_startup:
	ljmp	_main
;	return from main will return to caller
;--------------------------------------------------------
; code
;--------------------------------------------------------
	.area CSEG    (CODE)
;------------------------------------------------------------
;Allocation info for local variables in function 'main'
;------------------------------------------------------------
	G$main$0$0 ==.
	C$____hi0.c$3$0_0$1 ==.
;	____hi0.c:3: int main()
;	-----------------------------------------
;	 function main
;	-----------------------------------------
_main:
	ar7 = 0x07
	ar6 = 0x06
	ar5 = 0x05
	ar4 = 0x04
	ar3 = 0x03
	ar2 = 0x02
	ar1 = 0x01
	ar0 = 0x00
	C$____hi0.c$5$1_0$1 ==.
;	____hi0.c:5: return 0;
	mov	dptr,#0x0000
	C$____hi0.c$6$1_0$1 ==.
;	____hi0.c:6: }
	C$____hi0.c$6$1_0$1 ==.
	XG$main$0$0 ==.
	ret
	.area CSEG    (CODE)
	.area CONST   (CODE)
	.area XINIT   (CODE)
	.area CABS    (ABS,CODE)
