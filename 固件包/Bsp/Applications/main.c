#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "stm32f10x.h"
#include "bsp_usart.h"
#include "can.h"
#include "iap.h"
#include "bsp_key.h"
#include "LQ12864.h"

const u8 Data_32k[32*1024]=
{


};
/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int main(void)
{	
	SCB->VTOR = FLASH_BASE | 0x10000; /* Vector Table Relocation in Internal FLASH. */
    u8 k[5]={0};
    u32 i=0;
    Key_GPIO_Config();
	delay_init();	    	 //��ʱ������ʼ��	  
    
    
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
    uart_init(115200);	 	//���ڳ�ʼ��Ϊ115200
  	CAN_Mode_Init(CAN_SJW_1tq,CAN_BS2_8tq,CAN_BS1_9tq,4,CAN_Mode_Normal);//CAN��ͨģʽ��ʼ��, ������500Kbps 

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO ,ENABLE);//��ӳ����Ҫ��ʹ��AFIOʱ��
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);//ֻ�ر�JTAG������SWD

    OLED_Init();
 
	OLED_Fill(0x00); //��ȫ��
	delay_ms(500);
     printf("���Բ��Բ���\r\n");
     printf("���Բ��Բ���\r\n");
     printf("���Բ��Բ���\r\n");
     printf("���Բ��Բ���\r\n");

   
      OLED_P8x16Str(0,0,"USER APP RUNING");
    
    for(i=0;i<=500;i++)
    {
        printf("  %d  ",Data_32k[i]);
    }
\
    while(1)
	{	            
        if(Can_Receive_Msg(k)==2)
            {
                delay_ms(10);
                OLED_P8x16Str(0,0,"  JUMP IAP   ");
                if(k[0]==230)
                {
                    NVIC_SystemReset();
                }
            }
        
    }
           
}	

