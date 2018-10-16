/************************************************************************************
* Description: 128*64点整OLED模块功能演示程序的主函数，仅适用heltec.taobao.com所售产品
* 1. void delay(unsigned int z) -- 主函数中用于调整显示效果的延时函数,STC89C52 12MHZ z=1时大约延时1ms,其他频率需要自己计算
* 2. void main(void) -- 主函数
*************************************************************************************/

//#include <STC12C5A60S2.H>
//#include "intrins.h"
#include "codetab.h"
#include "LQ12864.h"
#include "stm32f10x.h"


 //初始化硬件IIC引脚
void I2C_Configuration(void)
{
	
	GPIO_InitTypeDef  GPIO_InitStructure; 

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);

	/*STM32F103C8T6芯片的硬件I2C: PC8 -- SDA; PC9 -- CLK */
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

    GPIO_SetBits(GPIOC,GPIO_Pin_8);
    GPIO_SetBits(GPIOC,GPIO_Pin_9);

}

void SCL_high()
{
    GPIO_SetBits(GPIOC,GPIO_Pin_9);
}

void SCL_low()
{
    GPIO_ResetBits(GPIOC,GPIO_Pin_9);
}

void SDA_high()
{
    GPIO_SetBits(GPIOC,GPIO_Pin_8);
}

void SDA_low()
{
    GPIO_ResetBits(GPIOC,GPIO_Pin_8);
}

/**
**  设置SDA为输出
**/
void SDA_OUT(void)
{
    GPIO_InitTypeDef GPIO_InitStructer;
    GPIO_InitStructer.GPIO_Pin= GPIO_Pin_8;
    GPIO_InitStructer.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_InitStructer.GPIO_Mode=GPIO_Mode_Out_PP;
    GPIO_Init(GPIOC, &GPIO_InitStructer);
}


/**
**  设置SDA为输入
**/
void SDA_IN(void)
{
    GPIO_InitTypeDef GPIO_InitStructer;
    GPIO_InitStructer.GPIO_Pin= GPIO_Pin_8;
    GPIO_InitStructer.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_InitStructer.GPIO_Mode=GPIO_Mode_IPU;
    GPIO_Init(GPIOC, &GPIO_InitStructer);
}


/**********************************************
//IIC Start
**********************************************/
void IIC_Start()
{
   SDA_OUT();
   SCL_high();		
   SDA_high();
    delay_us(2);
   SDA_low();
    delay_us(2);
   SCL_low();
    delay_us(2);
}

/**********************************************
//IIC Stop
**********************************************/
void IIC_Stop()
{
   SCL_low();
   SDA_low();
    delay_us(2);
   SCL_high();
    delay_us(2);
   SDA_high();
    delay_us(2);
}

/*
*   返回1--应答出错
*   放回0--应答正确
*/
u8 IIC_Wait_Ask(void)
{
    int count=0;
    SDA_IN();
    SCL_high();
    delay_us(2);
    while(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_9) == 0)
    {
        count++;
        if(count>250)
        {
            IIC_Stop();
            return 1;
        }   
    }
    SCL_low();
    delay_us(2);
    return 0;
}

/**********************************************
// 通过I2C总线写一个字节
**********************************************/
void Write_IIC_Byte(unsigned char IIC_Byte)
{
	unsigned char i;
    SDA_OUT();
	for(i=0;i<8;i++)
	{
         SCL_low();
        delay_us(2);
		if(IIC_Byte & 0x80)
			SDA_high();
		else
			SDA_low();
		SCL_high();
        delay_us(2);
		SCL_low();
		IIC_Byte<<=1;
	}
//	SDA_high();
//	SCL_high();
//	SCL_low();
}

u8 IIC_ReadByte(void)
{
    u8 data,i;
    SDA_IN();
    //SDA_high();
    delay_us(2);
    for(i=0;i<8;i++)
    {
        data<<=1;
        SCL_low();
        delay_us(2);
        SCL_high();
        delay_us(2);
        if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_9))
            data=data | 0x01;
        else 
            data=data & 0xFE;

    }
    SCL_low();
    delay_us(2);
    return data;

}

/*********************OLED写数据************************************/ 
void OLED_WrDat(unsigned char IIC_Data)
{
	IIC_Start();
	Write_IIC_Byte(0x78);
    IIC_Wait_Ask();
	Write_IIC_Byte(0x40);			//write data
    IIC_Wait_Ask();
	Write_IIC_Byte(IIC_Data);
    IIC_Wait_Ask();
	IIC_Stop();
}
/*********************OLED写命令************************************/
void OLED_WrCmd(unsigned char IIC_Command)
{
	IIC_Start();
	Write_IIC_Byte(0x78);            //Slave address,SA0=0
    IIC_Wait_Ask();
	Write_IIC_Byte(0x00);			//write command
    IIC_Wait_Ask();
	Write_IIC_Byte(IIC_Command);
    IIC_Wait_Ask();
	IIC_Stop();
}
/*********************OLED 设置坐标************************************/
void OLED_Set_Pos(unsigned char x, unsigned char y) 
{ 
	OLED_WrCmd(0xb0+y);
	OLED_WrCmd(((x&0xf0)>>4)|0x10);
	OLED_WrCmd((x&0x0f)|0x01);
} 
/*********************OLED全屏************************************/
void OLED_Fill(unsigned char bmp_dat) 
{
	unsigned char y,x;
	for(y=0;y<8;y++)
	{
		OLED_WrCmd(0xb0+y);
		OLED_WrCmd(0x01);
		OLED_WrCmd(0x10);
		for(x=0;x<X_WIDTH;x++)
		OLED_WrDat(bmp_dat);
	}
}
/*********************OLED复位************************************/
void OLED_CLS(void)
{
	unsigned char y,x;
	for(y=0;y<8;y++)
	{
		OLED_WrCmd(0xb0+y);
		OLED_WrCmd(0x01);
		OLED_WrCmd(0x10);
		for(x=0;x<X_WIDTH;x++)
		OLED_WrDat(0);
	}
}
/*********************OLED初始化************************************/
void OLED_Init(void)
{
    I2C_Configuration();
    
	delay_ms(500);//初始化之前的延时很重要！
	OLED_WrCmd(0xae);//--turn off oled panel
	OLED_WrCmd(0x00);//---set low column address
	OLED_WrCmd(0x10);//---set high column address
	OLED_WrCmd(0x40);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	OLED_WrCmd(0x81);//--set contrast control register
	OLED_WrCmd(Brightness); // Set SEG Output Current Brightness
	OLED_WrCmd(0xa0);//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
	OLED_WrCmd(0xc0);//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
	OLED_WrCmd(0xa6);//--set normal display
	OLED_WrCmd(0xa8);//--set multiplex ratio(1 to 64)
	OLED_WrCmd(0x3f);//--1/64 duty
	OLED_WrCmd(0xd3);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
	OLED_WrCmd(0x00);//-not offset
	OLED_WrCmd(0xd5);//--set display clock divide ratio/oscillator frequency
	OLED_WrCmd(0x80);//--set divide ratio, Set Clock as 100 Frames/Sec
	OLED_WrCmd(0xd9);//--set pre-charge period
	OLED_WrCmd(0xf1);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	OLED_WrCmd(0xda);//--set com pins hardware configuration
	OLED_WrCmd(0x12);
	OLED_WrCmd(0xdb);//--set vcomh
	OLED_WrCmd(0x40);//Set VCOM Deselect Level
	OLED_WrCmd(0x20);//-Set Page Addressing Mode (0x00/0x01/0x02)
	OLED_WrCmd(0x02);//
	OLED_WrCmd(0x8d);//--set Charge Pump enable/disable
	OLED_WrCmd(0x14);//--set(0x10) disable
	OLED_WrCmd(0xa4);// Disable Entire Display On (0xa4/0xa5)
	OLED_WrCmd(0xa6);// Disable Inverse Display On (0xa6/a7) 
	OLED_WrCmd(0xaf);//--turn on oled panel
	OLED_Fill(0x00); //初始清屏
	OLED_Set_Pos(0,0);
} 
/***************功能描述：显示6*8一组标准ASCII字符串	显示的坐标（x,y），y为页范围0～7****************/
void OLED_P6x8Str(unsigned char x,unsigned char y,unsigned char ch[])
{
	unsigned char c=0,i=0,j=0;
	while (ch[j]!='\0')
	{
		c =ch[j]-32;
		if(x>126){x=0;y++;}
		OLED_Set_Pos(x,y);
		for(i=0;i<6;i++)
		OLED_WrDat(F6x8[c][i]);
		x+=6;
		j++;
	}
}/*******************功能描述：显示8*16标准ASCII字符	 显示的坐标（x,y），y为页范围0～7****************/
void OLED_P8x16(unsigned char x,unsigned char y,unsigned char ch)
{
        unsigned char i=0;
		OLED_Set_Pos(x,y);
		for(i=0;i<8;i++)
		OLED_WrDat(F8X16[ch*16+i]);
		OLED_Set_Pos(x,y+1);
		for(i=0;i<8;i++)
		OLED_WrDat(F8X16[ch*16+i+8]);
	
}
/*******************功能描述：显示8*16一组标准ASCII字符串	 显示的坐标（x,y），y为页范围0～7****************/
void OLED_P8x16Str(unsigned char x,unsigned char y,unsigned char ch[])
{
	unsigned char c=0,i=0,j=0;
	while (ch[j]!='\0')
	{
		c =ch[j]-32;
		if(x>120){x=0;y++;}
		OLED_Set_Pos(x,y);
		for(i=0;i<8;i++)
		OLED_WrDat(F8X16[c*16+i]);
		OLED_Set_Pos(x,y+1);
		for(i=0;i<8;i++)
		OLED_WrDat(F8X16[c*16+i+8]);
		x+=8;
		j++;
	}
}
/*****************功能描述：显示16*16点阵  显示的坐标（x,y），y为页范围0～7****************************/
void OLED_P16x16Ch(unsigned char x,unsigned char y,unsigned char N)
{
	unsigned char wm=0;
	unsigned int adder=32*N;
	OLED_Set_Pos(x , y);
	for(wm = 0;wm < 16;wm++)
	{
		OLED_WrDat(F16x16[adder]);
		adder += 1;
	}
	OLED_Set_Pos(x,y + 1);
	for(wm = 0;wm < 16;wm++)
	{
		OLED_WrDat(F16x16[adder]);
		adder += 1;
	} 	  	
}
u32 mypow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}				  
//显示2个数字
//x,y :起点坐标	 
//len :数字的位数
//size:字体大小
//mode:模式	0,填充模式;1,叠加模式
//num:数值(0~4294967295);	 		  
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size)
{         	
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/mypow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				OLED_P8x16(x+(size/2)*t,y,0);
				continue;
			}else enshow=1; 
		 	 
		}
	 	OLED_P8x16(x+(size/2)*t,y,temp+16); 
	}
}

//显示汉字
void OLED_ShowCHinese(u8 x,u8 y,u8 no)
{      			    
	u8 t,adder=0;
	OLED_Set_Pos(x,y);	
    for(t=0;t<16;t++)
		{
				OLED_WrDat(Hzk[2*no][t]);
				adder+=1;
     }	
		OLED_Set_Pos(x,y+1);	
    for(t=0;t<16;t++)
			{	
				OLED_WrDat(Hzk[2*no+1][t]);
				adder+=1;
      }					
}
/***********功能描述：显示显示BMP图片128×64起始点坐标(x,y),x的范围0～127，y为页的范围0～8*****************/
void Draw_BMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[])
{
	unsigned int j=0;
	unsigned char x,y;

  if(y1%8==0) y=y1/8;      
  else y=y1/8+1;
	for(y=y0;y<y1;y++)
	{
		OLED_Set_Pos(x0,y);
    for(x=x0;x<x1;x++)
	    {      
	    	OLED_WrDat(BMP[j++]);
	    }
	}
}

/**
  * @brief  OLED_ON，将OLED从休眠中唤醒
  * @param  无
    * @retval 无
  */
void OLED_ON(void)
{
    OLED_WrCmd(0X8D);  //设置电荷泵
    OLED_WrCmd(0X14);  //开启电荷泵
    OLED_WrCmd(0XAF);  //OLED唤醒
}


