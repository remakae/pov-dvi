/*************************************************************
                   旋转LED IO口 测试程序

单片机 STC89c51
由于用的LED比较多 所以焊接虚焊是正常的，用此流水灯程序测试下
有没虚焊或者LED灯损坏的问题

*************************************************************/
#include<stc89c51rc.h>
#define uchar unsigned char //
#define uint unsigned int   //
void delay(uint);           //
void main(void);            //

void main(void) 

{  

uint i; 

uchar temp; 



   
while(1) 

{  

 temp=0x01; 

    for(i=0;i<8;i++)  

    {  

     P0=~temp;  
	 P1=~temp;
	 P2=~temp;
	 P3=~temp;
   delay(100);    

     temp<<=1; 

    }  
temp=0x80; 

for(i=0;i<8;i++)  

   {  

   
     P0=~temp;  
	 P1=~temp;
	 P2=~temp;
	 P3=~temp;

    delay(100);   

    temp>>=1; 

   }  
   temp=0xFE; 

for(i=0;i<8;i++)   

    { 

    
     P0=~temp;  
	 P1=~temp;
	 P2=~temp;
	 P3=~temp;

     delay(100);    

     temp<<=1; 

    }  
  temp=0xFE; 

for(i=0;i<8;i++)   

    {  

     
     P0=~temp;  
	 P1=~temp;
	 P2=~temp;
	 P3=~temp;

     delay(100);    

     temp<<=1; 

    }  
  temp=0x7F; 

   for(i=0;i<8;i++)  

    { 

     
     P0=~temp;  
	 P1=~temp;
	 P2=~temp;
	 P3=~temp;

     delay(100);    

     temp>>=1; 

    }  
   temp=0x7F; 

   for(i=0;i<8;i++)  

    {  

    
     P0=~temp;  
	 P1=~temp;
	 P2=~temp;
	 P3=~temp;

     delay(100);    

     temp>>=1; 

    }  
   temp=0xfF; 

   for(i=0;i<8;i++)  

    { 

    
     P0=~temp;  
	 P1=~temp;
	 P2=~temp;
	 P3=~temp;

     delay(100);    

     
     P0=~temp;  
	 P1=~temp;
	 P2=~temp;
	 P3=~temp;

     delay(100); 

    }  
   temp=0xAA; 

   for(i=0;i<8;i++)  

    { 

    
     P0=~temp;  
	 P1=~temp;
	 P2=~temp;
	 P3=~temp;

     delay(100);     
  
     P0=~temp;  
	 P1=~temp;
	 P2=~temp;
	 P3=~temp;

     delay(100); 

    }  
 
     P0=~temp;  
	 P1=~temp;
	 P2=~temp;
	 P3=~temp;

    delay(200); 

   for(i=0;i<8;i++)  

    { 

     
     P0=~temp;  
	 P1=~temp;
	 P2=~temp;
	 P3=~temp;

     delay(100);  

     
     P0=~temp;  
	 P1=~temp;
	 P2=~temp;
	 P3=~temp;

     delay(100);    

     
     P0=~temp;  
	 P1=~temp;
	 P2=~temp;
	 P3=~temp;

     delay(100);    

     
     P0=~temp;  
	 P1=~temp;
	 P2=~temp;
	 P3=~temp;

     delay(100);    

    }  
   
     P0=~temp;  
	 P1=~temp;
	 P2=~temp;
	 P3=~temp;

    delay(200); 

   for(i=0;i<8;i++)  

    { 

    
     P0=~temp;  
	 P1=~temp;
	 P2=~temp;
	 P3=~temp;

     delay(100);  

     
     P0=~temp;  
	 P1=~temp;
	 P2=~temp;
	 P3=~temp;

     delay(100);    

     
     P0=~temp;  
	 P1=~temp;
	 P2=~temp;
	 P3=~temp;

     delay(100);    

    
     P0=~temp;  
	 P1=~temp;
	 P2=~temp;
	 P3=~temp;

     delay(100);    

    } 

}  
}  void delay(uint t)   //



 

      {  

register uint bt; 

        for(;t;t--)  

        for(bt=0;bt<255;bt++); 

      }  
  
