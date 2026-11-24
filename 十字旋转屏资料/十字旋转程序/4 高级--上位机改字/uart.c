#include "STC89C5xRC.H"


INT8U sector_erase(INT16U sector_addr);
INT8U byte_program(INT16U byte_addr, INT8U original_data);
INT8U byte_read(INT16U byte_addr);
//起始标志 0x5A 结束标志 0xA5  0x00 无反应
#define START 0X5A //起始位
#define END   0XA5 //结束位
/*
	0x5a+C+L+DATA+CS+0xA5
	C 控制码 
	L 长度
	DATA 数据
	CS  较验和  C+L+DATA


 和取模
    接收成功返回1字节：0x06
*/

bit uart_rBIT=0;
uchar ReceVeryfy=0;
uchar ReceState=0;
uchar ucDataBuff[200];
uchar ReceCount=0;
uchar BuffCount=0;
uchar ucControlCode;
uchar ucLength;
data uint uiBaseAddress=0;
/**********************************/
void Send_data(unsigned char d)	  /*从串口发送数据*/
{
	  ES=0;
      SBUF=d;
	  while(TI==0);       //等待发送完毕
      TI=0;
	  ES=1;
}
/*******************************************/
void MAIN_RS_INT(void) interrupt 4
{
    uchar DataTemp;
	uchar i;
     //uart_rBIT=1;
    if(RI)  // 接收
    {
		RI = 0;
		//Error=1;
		DataTemp = SBUF;
		/************************************/
		switch (ReceState)
		{
			case 0:
				if(START == DataTemp)
        {
          ReceState = 1;
        }
				break;
			case 1:
				ucControlCode=DataTemp;	//接收控制码
				ReceVeryfy += ucControlCode;//开始计算校验和			
				 ReceState = 2;							
				break;
			case 2:
				ucLength=DataTemp;
				ReceVeryfy += ucLength;//计算校验和
				ReceState = 3;		
				break;
			case 3:			
				if(ReceCount<ucLength)//开始接收数据
				{
					ReceVeryfy += DataTemp;
					ucDataBuff[ReceCount]=DataTemp;
					ReceCount++;					
					
				}
				else
				{
					if(ReceVeryfy == DataTemp)//校验和
                     {
                        
						ReceVeryfy=0;
						ReceCount=0;
						ReceState=4;
						//LED1=~LED1;
                     }
					 else
					 {
					  	ReceState=0;
					 }	
				}
				break;
			   	case 4:
					 if(END == DataTemp) //接收到结束码后 串口接收标志位置1
					 {
					 	uart_rBIT=1;
						ReceState=0;
					 }
				break;
			default:				 
				ReceState=0;
				break;
		}
/**************************************************/
	if( uart_rBIT )
	{
		uart_rBIT = 0;
		switch (ucControlCode)
		{
			case 0x01://扇区擦除命令
					for(i=0;i<10;i++)
				    {
						sector_erase(i*0x200+0x2000);//擦除扇区 STC89C52的第一个扇区地址是0x2000 共4K 8个扇区 一个扇区是512字节 可存160个字符
					}
					uiBaseAddress=0;
					Send_data(0x06);					
				break;
			
			case 0x03://立体字幕数据命令
					
					for(BuffCount=0;BuffCount<ucLength;BuffCount++)
					{
						byte_program(BuffCount+uiBaseAddress+0x2000,ucDataBuff[BuffCount]);//把字幕数据写入EEP 第一个扇区地址是0x2000
					}
					uiBaseAddress=uiBaseAddress+ucLength;

					Send_data(0x06);
				break;				
				case 0x07:
					ES = 0;
							
					for( i=0;i<250;i++ )
					{
						SBUF = byte_read(0x2000+i);//测试用
						while( TI == 0 );
						TI = 0;
					}		
					ES = 1;
				 break;
			default:
				break;
		}

	}
			
	}

}
