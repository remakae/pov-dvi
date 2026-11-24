             /* ---  STC International Limited ---------------- */
/* ---  宏晶科技 姚永平  设计 2004/9/11   V1.0 --- */
/* ---  宏晶科技 姚永平  设计 2004/12/4   V2.0 --- */
/* ---  RD+/RC系列   Flash IAP/ISP Operation ----- */
/* ---  STC89C54RD+, STC89C58RD+,              --- */
/* ---  STC89LE54RD+,STC89LE58RD+,              -- */
/* ---  STC89C51RC,  STC89C52RC,             ----- */
/* ---  STC89LE51RC, STC89LE52RC,             ---- */
/* ---  Mobile: 13922805190 ---------------------- */
/* ---  Fax: 0755-82944243 ----------------------- */
/* ---  Tel: 0755-82908285 ----------------------- */
/* ---  Web  : www.mcu-memory.com ---------------- */
/* ---  感谢网友2004/12/3的指正,送样品以示感谢 --- */
#include "STC89C5xRC.H"

#include <intrins.h>        /* use _nop_() function */
//sfr16 DPTR = 0x82;



/* 定义命令 */
#define READ_AP_and_Data_Memory_Command				0x01        /*  字节读数据存储区   */
#define PROGRAM_AP_and_Data_Memory_Command          0x02        /*  字节编程数据存储区 */
#define SECTOR_ERASE_AP_and_Data_Memory_Command     0x03        /*  扇区擦除数据存储区 */


#define		DELAY_CONST         60000

/* 定义常量 */
#define ERROR   0
#define OK      1


/* 定义Flash 操作等待时间 */
//#define        MCU_CLOCK_40MHz
#define         MCU_CLOCK_20MHz
//#define        MCU_CLOCK_10MHz
//#define        MCU_CLOCK_5MHz
#ifdef MCU_CLOCK_40MHz
        #define WAIT_TIME        0x00
#endif
#ifdef MCU_CLOCK_20MHz
        #define WAIT_TIME        0x01
#endif
#ifdef MCU_CLOCK_10MHz
        #define WAIT_TIME        0x02
#endif
#ifdef MCU_CLOCK_5MHz
        #define WAIT_TIME        0x03
#endif

/* 调试控制项 */
//#define         DEBUG_STC89C_LE58RD+

#define         DEBUG_STC89C_LE52RC

//#define USED_BYTE_QTY_IN_ONE_SECTOR                1
//#define USED_BYTE_QTY_IN_ONE_SECTOR                2
//#define USED_BYTE_QTY_IN_ONE_SECTOR                4
//#define USED_BYTE_QTY_IN_ONE_SECTOR                8
//#define USED_BYTE_QTY_IN_ONE_SECTOR                16
//#define USED_BYTE_QTY_IN_ONE_SECTOR                32
//#define USED_BYTE_QTY_IN_ONE_SECTOR                64
//#define USED_BYTE_QTY_IN_ONE_SECTOR                128
//#define USED_BYTE_QTY_IN_ONE_SECTOR                256
//#define USED_BYTE_QTY_IN_ONE_SECTOR                512

//INT8U xdata protect_buffer[USED_BYTE_QTY_IN_ONE_SECTOR];
//#ifdef DEBUG_STC89C_LE58RD+                        //STC89C58RD+,  89LE58RD+
//        #define DEBUG_AP_Memory_Begin_Sector_addr		0x0000
//        #define DEBUG_AP_Memory_End_Sector_addr         0x7e00
//        #define DEBUG_AP_Memory_End_Byte_addr           0x7fff
//
//        #define DEBUG_Data_Memory_Begin_Sector_addr     0x8000
//#endif
#ifdef DEBUG_STC89C_LE52RC                        //STC89C52RC,        89LE52RC
        #define DEBUG_AP_Memory_Begin_Sector_addr		0x0000
        #define DEBUG_AP_Memory_End_Sector_addr			0x1e00
        #define DEBUG_AP_Memory_End_Byte_addr			0x1fff

        #define DEBUG_Data_Memory_Begin_Sector_addr		0x2000
#endif


/* 打开 ISP,IAP 功能 */
void ISP_IAP_enable(void)
{
	EA	=	0;	/* 关中断 */
	ISP_CONTR	=	ISP_CONTR & 0x18;       /* 0001,1000 */
	ISP_CONTR	=	ISP_CONTR | WAIT_TIME;
	ISP_CONTR	=	ISP_CONTR | 0x80;       /* 1000,0000 */
}

/* 关闭 ISP,IAP 功能 */
void ISP_IAP_disable(void)
{
	ISP_CONTR	=	ISP_CONTR & 0x7f;	/* 0111,1111 */
	ISP_TRIG	=	0x00;
	EA			=   1;                	/* 开中断 */
}

/* 字节读 */
INT8U byte_read(INT16U byte_addr)
{
	ISP_ADDRH	=	(INT8U)(byte_addr >> 8);
	ISP_ADDRL	=	(INT8U)(byte_addr & 0x00ff);

	ISP_CMD		=	ISP_CMD	&	0xf8;        /* 1111,1000 */
	ISP_CMD		=	ISP_CMD	|	READ_AP_and_Data_Memory_Command;        /* 0000,0001 */

	ISP_IAP_enable();

	ISP_TRIG	=	0x46;
	ISP_TRIG	=	0xb9;
	_nop_();

	ISP_IAP_disable();
	return (ISP_DATA);
}

/* 扇区擦除 */
INT8U sector_erase(INT16U sector_addr)
{
	INT16U get_sector_addr	=	0;
	get_sector_addr			=	(sector_addr & 0xfe00); /* 1111,1110,0000,0000; 取扇区地址 */
	ISP_ADDRH        		=	(INT8U)(get_sector_addr >> 8);
	ISP_ADDRL		        =	0x00;

	ISP_CMD	=	ISP_CMD	&	0xf8;	/* 1111,1000 */
	ISP_CMD	=	ISP_CMD	|	SECTOR_ERASE_AP_and_Data_Memory_Command;	/* 0000,0011 */

	ISP_IAP_enable();
	ISP_TRIG	=	0x46;        /* 触发ISP_IAP命令 */
	ISP_TRIG	=	0xb9;        /* 触发ISP_IAP命令 */
	 _nop_();

	ISP_IAP_disable();
	return OK;
}

/* 字节编程 */
INT8U byte_program(INT16U byte_addr, INT8U original_data)
{
	ISP_ADDRH	=	(INT8U)(byte_addr >> 8);
	ISP_ADDRL	=	(INT8U)(byte_addr & 0x00ff);

	ISP_CMD		=	ISP_CMD	&	0xf8;        /* 1111,1000 */
	ISP_CMD		=	ISP_CMD	|	PROGRAM_AP_and_Data_Memory_Command;		/* 0000,0010 */
	ISP_DATA	=	original_data;

	ISP_IAP_enable();
	ISP_TRIG	=	0x46;        /* 触发ISP_IAP命令 */
	ISP_TRIG	=	0xb9;        /* 触发ISP_IAP命令 */
	_nop_();

	ISP_IAP_disable();
	return	OK;
}