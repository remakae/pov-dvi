#include "STC89C5xRC.H"


uchar byte_read(uint byte_addr);
void Send_data(unsigned char d);

uint uiFontNumber=0;//EEP 中的字总数

sbit extern_int = P4^3;
void init()
{
 	//int2初始化
	EA = 1;			//开中断
    IT2 = 0;    //外部中断为边沿触发
    EX2 = 0;    //允许外部中断 



	 //串口1初始化
	PCON &= 0x7F;		//波特率不倍速
	SCON = 0x50;		//8位数据,可变波特率
	AUXR &= 0xBF;		//定时器1时钟为Fosc/12,即12T
	AUXR &= 0xFE;		//串口1选择定时器1为波特率发生器
	TMOD &= 0x0F;		//清除定时器1模式位
	TMOD |= 0x20;		//设定定时器1为8位自动重装方式
	TL1 = 0xFD;		//设定定时初值
	TH1 = 0xFD;		//设定定时器重装值
	ET1 = 0;		//禁止定时器1中断
	TR1 = 1;		//启动定时器1
	extern_int = 1;
	P1=0XFF;
	P2=0XFF;
	P0=0XFF;
	P3=0XFF;
	uiFontNumber=((byte_read(0x2000)<<8)+byte_read(0x2001))*16; //读取EEP立体数据总数 
	Send_data(0X55);//测试串口



}

bit BIT_timeout=0;

/*********中断子程序**********/
void int2() interrupt 6	//使用外部中断0
{
	BIT_timeout=1; //起点标志位	
	EX2 = 0;
	
}
/********************************/

/*********延时子程序**********/
void DelayUs(uint N)
{
    uint x ;
    for(x=0;x<=N;x++);
}







uint i=0,j=0;
uchar Data_L=0xff;
uchar Data_H=0xff;
uchar time_delay=0;
uchar k;
bit Scan_bit=0;
#define ucNumRows 186 //定义一周显示字个数 一个字16列 160/16=10个字
uchar code liushui[8]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};//流水扫描数据

/***************************************/
void main_loop(void)
{
	static bit l;
	while((extern_int ==1)&&(BIT_timeout == 0));	//起点判断
 		BIT_timeout = 0;
	
		j++; //移动文字显示				           
    if( j>uiFontNumber )//显示完文字后循环
    {
			j=0 ;
			l^=1;
		}	                        
		for(i=ucNumRows+j;i>j;i--)  //立体显示176列数据176/16=11个字
		{      
			Data_L=byte_read(i*2+0x2002);	//缓冲数据低位
			Data_H=byte_read(i*2+1+0x2002);//缓冲数据高位	
			if(l)
			{
				P2=Data_L;	//送数据低位显示
				P0=Data_H;//送数据高位显示
			}
			else
			{
				P1=Data_L;
				P3=Data_H;
			}		
		
			DelayUs(0);//延时值 可以改变字体的宽度  值越大 字体越宽
			
      if(i==(ucNumRows+j-50))
			{
				EX2 = 1;
			}
			if(BIT_timeout ==1)
			{
				return;
				
			}
		}
		
		EX2 = 0;
		P0 = P1 = P2 =P3 =0xff;
		   
	
}


int main(void)
{
    init();

				    
	while(1)
	{
       main_loop();
    }
}

