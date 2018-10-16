#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "stm32f10x.h"
#include "bsp_usart.h"
#include "can.h"
#include "bsp_key.h"  
#include "iap.h"
#include "stmflash.h"
#include "LQ12864.h"
#include "bsp_i2c_ee.h"
#include "timer.h"
#include "bsp_i2c_gpio.h"

/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
//u8  Read_buff[2048];
int main(void)
{	
    PWR->CR |= 1<<8;//取消备份区写保护
    RCC->BDCR &= 0xFFFFFFFE; //关闭外部低速振荡器，PC14，PC15成为普通IO  
    BKP->CR &= 0xFFFFFFFE; //侵入检测TAMPER引脚(PC13)作为通用IO口使用
    PWR->CR &= 0xFFFFFEFF; //备份区写保护</span>
    
    u8 jump[2]={230,230},k[5]={0},ACK[1],rec_flag=1,ACK_p=0,addr_l=0,addr_h=0;
    u32 app_size=0,*p=&app_size;
    u32 i=0,app_size_c=0,addr_now=0,addr_n=0;
    u32 Read_Addr=FLASH_APP1_ADDR;
    Read_fifo:
    rec_flag=1;app_size_c=0;addr_now=0;USART_RX_CNT=0;
    Key_GPIO_Config();
	delay_init();	    	 //延时函数初始化	  
    OLED_Init();
    i2c_CfgGpio();
 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	uart_init(115200);	 	//串口初始化为115200
    TIM3_Int_Init(1000,720-1);
  	CAN_Mode_Init(CAN_SJW_1tq,CAN_BS2_8tq,CAN_BS1_9tq,4,CAN_Mode_Normal);//CAN普通模式初始化, 波特率500Kbps 
    if (ee_CheckOk() == 0)
	{
		/* 没有检测到EEPROM */
		printf("没有检测到串行!\r\n");
        OLED_P8x16Str(8,0,"NO EEPROM");
        delay_ms(1000);
	}
     
    printf("开始发送数据\n\n\n\n");
    OLED_P8x16Str(8,0,"CAN_BUS update");
    ee_ReadBytes(&addr_h,0,1);
    ee_ReadBytes(&addr_l,1,1);
    addr_now=(u16)(addr_h<<8)|(addr_l);
    k[0]=132; 
    k[3]=addr_h;k[4]=addr_l;
    while(rec_flag)
    {
        if(USART_RX_CNT)
	   	{
            if(rec_flag!=2)
            {
                OLED_P8x16Str(8,0,"              ");
                OLED_P8x16Str(8,0,"Receive...");
                k[1]=app_size>>8&0xff;
                k[2]=app_size&0xff; 
                rec_flag=2;
            }
            
            if(USART_RX_CNT!=app_size)
            {
                app_size=USART_RX_CNT;
				printf("代码长度:%dBytes\r\n",app_size);
                OLED_P8x16Str(5,2,"Size:");
                OLED_ShowNum(45,2,app_size,5,16);
                OLED_P8x16Str(82,2," Byte");
                k[1]=app_size>>8&0xff;
                k[2]=app_size&0xff; 
            }
		}
            if(addr_now!=0)
            {
                OLED_P8x16Str(8,0,"ERROR Address: ");
                OLED_ShowNum(60,2,addr_now,5,16);
                delay_ms(300);
            }
//        if(Key_Scan(KEY1_GPIO_PORT,KEY1_GPIO_PIN))
//        {
//            rec_flag=1;
//            USART_RX_CNT=0;
//            OLED_Fill(0X00);
//            OLED_P8x16Str(20,0,"Reading..!");
//			printf("%dBytes个字节代码读出缓冲区完毕\r\n",app_size);
//            if(app_size!=0)
//            {
//                OLED_P8x16Str(5,0,"prepare ok");
//            }
//            else
//            {
//                OLED_P8x16Str(5,0,"    faill   ");
//            }
//            
//        }
         if(Key_Scan(KEY3_GPIO_PORT,KEY3_GPIO_PIN))
         {
              OLED_Fill(0X00);
              Can_Send_Msg(jump,2);delay_ms(2);
              Can_Send_Msg(jump,2);delay_ms(2);
              OLED_P8x16Str(5,0,"Prepare Send");
         }
         if(Key_Scan(KEY4_GPIO_PORT,KEY4_GPIO_PIN)||Key_Scan(KEY2_GPIO_PORT,KEY2_GPIO_PIN))
        {
            rec_flag=0;
        }
        
    }
    OLED_Fill(0X00);
    OLED_P8x16Str(5,0,"Prepare Send");
    Continue:
    k[0]=132; 
    k[3]=addr_h;k[4]=addr_l;
    k[1]=app_size>>8&0xff; k[2]=app_size&0xff; 
    while(1)
	{	
        if(Key_Scan(KEY1_GPIO_PORT,KEY1_GPIO_PIN))
        {
            goto Read_fifo;
        }
        
        if(Key_Scan(KEY3_GPIO_PORT,KEY3_GPIO_PIN))
        {
              OLED_Fill(0X00);
              Can_Send_Msg(jump,2);delay_ms(2);
              Can_Send_Msg(jump,2);delay_ms(2);
              OLED_P8x16Str(5,0,"Prepare Send");
        }
        
        if(Key_Scan(KEY2_GPIO_PORT,KEY2_GPIO_PIN))
        {
            
            if(app_size!=0)
            {
                    TIM_Cmd(TIM3, ENABLE);  //使能TIMx	
                    OLED_P8x16Str(5,0,"Sending... ");
                    OLED_P8x16Str(110,0,"%");
                    Can_Send_Msg(k,5);delay_ms(2);
                    Can_Send_Msg(k,5);delay_ms(2);
                    Can_Send_Msg(k,5);delay_ms(2);
                    delay_ms(100);
                    i=0;app_size_c=app_size-(app_size%1024);
                    printf("addr_now%d\r\n",addr_now);
                        while(i<=app_size-addr_now)
                        {
                            app_buf[0]=USART_RX_BUF[0+i+addr_now];
                            app_buf[1]=USART_RX_BUF[1+i+addr_now];
                            app_buf[2]=USART_RX_BUF[2+i+addr_now];
                            app_buf[3]=USART_RX_BUF[3+i+addr_now];
                            app_buf[4]=USART_RX_BUF[4+i+addr_now];
                            app_buf[5]=USART_RX_BUF[5+i+addr_now];
                            app_buf[6]=USART_RX_BUF[6+i+addr_now];
                            app_buf[7]=USART_RX_BUF[7+i+addr_now];
                            Can_Send_Msg(app_buf,8);
                            i=i+8;
                            time_s=0;
                            ACK_p=1;
                            ACK[0]=0;
                            if((i+8)%1024==0&&i<=app_size_c)
                            {
                                    addr_h=i>>8&0xff;
                                    addr_l=i&0xff;
//                                    ee_WriteBytes(&addr_h,0,1);
//                                    ee_WriteBytes(&addr_l,1,1);
                                    addr_n=i;
                                    OLED_ShowNum(90,0,(i+addr_now)*100/app_size,2,16);
                            }
                            while(Can_Receive_Msg(ACK)!=1)
                            {
                                if(time_s>=100)
                                {
                                    TIM_Cmd(TIM3, DISABLE);  //使能TIMx	
                                    OLED_P8x16Str(5,3,"  Time out !    ");
                                    addr_now=addr_n;
                                    while(Key_Scan(KEY4_GPIO_PORT,KEY4_GPIO_PIN));
                                    while(Key_Scan(KEY4_GPIO_PORT,KEY4_GPIO_PIN)==0);
                                    
                                    OLED_P8x16Str(5,3,"  Continue !    ");
                                    goto Continue;

                                }
                            }
                            while(ACK[0]!=112);
                        }
                         addr_h=0;
                         addr_l=0;
                         ee_WriteBytes(&addr_h,0,1);
                         ee_WriteBytes(&addr_l,1,1);
                         printf("发送结束");
//                        for(i=0;i<=app_size;i++)
//                        {
//                            printf("%d:%d\r\n",i,USART_RX_BUF[i]);
//                        }
                        
                    OLED_P8x16Str(5,0,"Send OK !       ");
              }
             else{OLED_P8x16Str(5,0,"NO Data      ");}
        
        }
//        *****************/
	}	
}
