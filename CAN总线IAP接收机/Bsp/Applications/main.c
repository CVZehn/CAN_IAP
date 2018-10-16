#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "stm32f10x.h"
#include "bsp_usart.h"
#include "can.h"
#include "iap.h"
#include "bsp_key.h"
#include "stmflash.h"
#include "iap.h"
#include "LQ12864.h"
#include "bsp_i2c_ee.h"
#include "timer.h"

/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{	
    
    u8 k[5]={0},ACK[1]={112},data_d[3]={0};
    u32 i=0,j=0,size=0,app_addr_c=0;
    u32 app_addr=FLASH_APP1_ADDR;
    Key_GPIO_Config();
	delay_init();	    	 //延时函数初始化	 
    TIM3_Int_Init(10000,7200-1);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
    uart_init(115200);	 	//串口初始化为115200
    OLED_Init();
  	CAN_Mode_Init(CAN_SJW_1tq,CAN_BS2_8tq,CAN_BS1_9tq,4,CAN_Mode_Normal);//CAN普通模式初始化, 波特率500Kbps 

 
    printf("自检程序。。。\r\n");  
      
//    
    printf("等待帧头\r\n");  

   
	TIM_Cmd(TIM3, ENABLE);  //使能TIMx	
      OLED_P8x16Str(0,0,"wait");
    
      OLED_P8x16Str(0,0,"PREP");
      OLED_P8x16Str(0,3,"Running APP ");
      OLED_P8x16Str(0,5,"in    seconds.");
      
    while(1)
    {
        
            OLED_ShowNum(20,5,time,3,16);
            if(Can_Receive_Msg(k)==5)
            {
                
                OLED_Fill(0X00);
                TIM_Cmd(TIM3, DISABLE);  //使能TIMx		
                delay_ms(10);
                OLED_P8x16Str(0,0,"STAR");
                if(k[0]==132)
                {
                    size=(u16)(k[1]<<8)|k[2];
                    app_addr_c=(u16)(k[3]<<8)|k[4];
                    if(app_addr_c!=0)
                    {
                        OLED_P8x16Str(0,2,"ERROR PROGRAM");
                        printf("app_addr_c:%d\r\n",app_addr_c);
                    }
                    printf(" %d个字节 ",size);
                    
                    OLED_P8x16Str(0,0,"UPDATING..");
                    OLED_P8x16Str(110,0,"%");
                    while(i<=size-app_addr_c)
                    {
                        if(Can_Receive_Msg(buff)==8)
                        {
                            app_buff[0+j]=buff[0];
                            app_buff[1+j]=buff[1]; 
                            app_buff[2+j]=buff[2];
                            app_buff[3+j]=buff[3];
                            app_buff[4+j]=buff[4];
                            app_buff[5+j]=buff[5];
                            app_buff[6+j]=buff[6];
                            app_buff[7+j]=buff[7];
                            
                            if(j==2040)
                            {
                                iap_write_appbin(app_addr+app_addr_c,app_buff,2048);
                                app_addr=app_addr+2048;
                                OLED_ShowNum(90,0,(i+app_addr_c)*100/size,2,16);
                                j=0;
                            } 
                            else
                            {
                                j=j+8;
                            }
                                i=i+8;
                                Can_Send_Msg(ACK,1);
                        }
                    }
                    if(j)
                    {
                        iap_write_appbin(app_addr+app_addr_c,app_buff,j);
                        printf("写入完成\r\n");
                        OLED_P8x16Str(0,0,"   Update  OK  !    ");
                    }
                    printf("发送完成\r\n");
                    
                    OLED_P8x16Str(0,3,"Running APP ");
                    OLED_P8x16Str(0,5,"in    seconds.");
//                for(i=0;i<=size;i=i+1)
//                {
//                    printf("%d:%d\r\n",i,(u8)STMFLASH_ReadHalfWord(FLASH_APP1_ADDR+i));
//                }
                }
                
                TIM_Cmd(TIM3, ENABLE);  //使能TIMx	
           } 
            if(Key_Scan(KEY1_GPIO_PORT,KEY1_GPIO_PIN)||time<=0)
            {
                    printf("开始执行FLASH用户代码!!\r\n");
                    
                    printf("%d\r\n",((*(vu32*)(+4))&0xFF000000));
                    __set_PRIMASK(1);
                    if(((*(vu32*)(FLASH_APP1_ADDR+4))&0xFF000000)==0x08000000)//判断是否为0X08XXXXXX.
                    {	
                        OLED_Fill(0x00);
                        OLED_P8x16Str(0,1,"     USER APP     ");
                        printf("%d\r\n",((*(vu32*)(FLASH_APP1_ADDR+4))&0xFF000000));
                        iap_load_app(FLASH_APP1_ADDR);//执行FLASH APP代码
                    }else 
                    {
                        OLED_Fill(0x00);
                        OLED_P8x16Str(0,1,"  USER APP ERROR    ");
                        printf("%d\r\n",((*(vu32*)(FLASH_APP1_ADDR+4))&0xFF000000));
                        printf("非FLASH应用程序,无法执行!\r\n");   
                    }			
            }
                   
        }
           
}	

