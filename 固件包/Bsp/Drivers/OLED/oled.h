#ifndef __OLED_H
#define __OLED_H			  	 
#include "sys.h"
#include "stdlib.h"	    
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板V3
//SSD1306 OLED 驱动IC驱动代码
//驱动方式:8080并口/4线串口
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2015/1/14
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 


//OLED模式设置
//0: 4线串行模式  （模块的BS1，BS2均接GND）
//1: 并行8080模式 （模块的BS1，BS2均接VCC）
#define OLED_MODE 	0 
		    						  
//---------------------------OLED端口定义--------------------------  
#define OLED_SCLK_Clr() GPIO_ResetBits(GPIOA,GPIO_Pin_5)
#define OLED_SCLK_Set() GPIO_SetBits(GPIOA,GPIO_Pin_5)

#define OLED_SDIN_Clr() GPIO_ResetBits(GPIOA,GPIO_Pin_7)
#define OLED_SDIN_Set() GPIO_SetBits(GPIOA,GPIO_Pin_7)
		     
#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据

#define SIZE 16
#define XLevelL		0x02
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xFF 
#define X_WIDTH 	128
#define Y_WIDTH 	64	   
//OLED控制用函数
void OLED_WR_Byte(unsigned dat,unsigned cmd);  
void OLED_Display_On(void);
void OLED_Display_Off(void);	   							   		    
void OLED_Init(void);
void OLED_Clear(void);
void OLED_DrawPoint(u8 x,u8 y,u8 t);
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot);
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 Char_Size);
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size);
void OLED_ShowString(u8 x,u8 y, u8 *p,u8 Char_Size);	 
void OLED_Set_Pos(unsigned char x, unsigned char y);
void OLED_ShowCHinese(u8 x,u8 y,u8 no);
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[]);
void fill_picture(unsigned char fill_Data);
void Picture();
void IIC_Start();
void IIC_Stop();
void Write_IIC_Command(unsigned char IIC_Command);
void Write_IIC_Data(unsigned char IIC_Data);
void Write_IIC_Byte(unsigned char IIC_Byte);
void IIC_Wait_Ack();	 
#endif  
	 







 

