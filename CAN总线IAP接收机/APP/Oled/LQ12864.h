/************************************************************************************
* File name: LQ12864.h
* Project  : OLED
* Processor: STC15W4K48

* Description:128*64����OLEDģ�������ļ���������heltec.taobao.com���۲�Ʒ

* 1. void delay(unsigned int z) -- ��ʱ����,����
* 2. void IIC_Start() -- ����I2C����
* 3. void IIC_Stop() -- �ر�I2C����
* 4. void Write_IIC_Byte(unsigned char IIC_Byte) -- ͨ��I2C����дһ��byte������
* 5. void OLED_WrDat(unsigned char dat) -- ��OLED��д����
* 6. void OLED_WrCmd(unsigned char cmd) -- ��OLED��д����
* 7. void OLED_Set_Pos(unsigned char x, unsigned char y) -- ������ʾ����
* 8. void OLED_Fill(unsigned char bmp_dat) -- ȫ����ʾ(��ʾBMPͼƬʱ�Ż��õ��˹���)
* 9. void OLED_CLS(void) -- ��λ/����
* 10. void OLED_Init(void) -- OLED����ʼ�����򣬴˺���Ӧ�ڲ�����Ļ֮ǰ���ȵ���
* 11. void OLED_P6x8Str(unsigned char x, y,unsigned char ch[]) -- 6x8������������ʾASCII�����С���У���̫����
* 12. void OLED_P8x16Str(unsigned char x, y,unsigned char ch[]) -- 8x16������������ʾASCII�룬�ǳ�����
* 13.void OLED_P16x16Ch(unsigned char x, y, N) -- 16x16������������ʾ���ֵ���С���У������ø������塢�Ӵ֡���б���»��ߵ�
* 14.void Draw_BMP(unsigned char x0, y0,x1, y1,unsigned char BMP[]) -- ��128x64���ص�BMPλͼ��ȡ�����������ֱ�Ȼ���Ƶ�codetab�У��˺������ü���
*
* History: none;
*
*************************************************************************************/
#include "stm32f10x.h"
#include "delay.h"

#define	Brightness	0xCF 
#define X_WIDTH 	128
#define Y_WIDTH 	64

void OLED_Init(void);
void OLED_P6x8Str(unsigned char x,unsigned char y,unsigned char ch[]);
void OLED_P8x16Str(unsigned char x,unsigned char y,unsigned char ch[]);
void OLED_P16x16Ch(unsigned char x,unsigned char y,unsigned char N);
void Draw_BMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[]);
void OLED_Fill(unsigned char bmp_dat) ;
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size);
