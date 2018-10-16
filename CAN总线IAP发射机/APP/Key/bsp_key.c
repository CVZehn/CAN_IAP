/**
  ******************************************************************************
  * @file    bsp_key.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   按键应用bsp（扫描模式）
  ******************************************************************************
  * @attention
  *
  * 实验平台:秉火 F103-指南者 STM32 开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */ 
  
#include "bsp_key.h"  
#include "bsp_i2c_ee.h"
#include "bsp_i2c_gpio.h"

/**
  * @brief  配置按键用到的I/O口
  * @param  无
  * @retval 无
  */
void Key_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	
	/*开启按键端口的时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	//选择按键的引脚
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15; 
	// 设置按键的引脚为浮空输入
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
	//使用结构体初始化按键
	GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_SetBits (GPIOB,GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);


//	RCC_APB2PeriphClockCmd(EEPROM_RCC_I2C_PORT, ENABLE);	/* 打开GPIO时钟 */

//	GPIO_InitStructure.GPIO_Pin = EEPROM_I2C_SCL_PIN | EEPROM_I2C_SDA_PIN;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;  	/* 开漏输出 */
//	GPIO_Init(EEPROM_GPIO_PORT_I2C, &GPIO_InitStructure);

//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15; 
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; 
//	GPIO_Init(GPIOC, &GPIO_InitStructure);
//    GPIO_ResetBits(EEPROM_GPIO_PORT_I2C,GPIO_Pin_15);
}

 /*
 * 函数名：Key_Scan
 * 描述  ：检测是否有按键按下
 * 输入  ：GPIOx：x 可以是 A，B，C，D或者 E
 *		     GPIO_Pin：待读取的端口位 	
 * 输出  ：KEY_OFF(没按下按键)、KEY_ON（按下按键）


 */
uint8_t Key_Scan(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin)
{			
	/*检测是否有按键按下 */
	if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == 0 )  
	{	 
		/*等待按键释放 */
		while(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == 0);   
		return 	1;	 
	}
	else
		return 0;
}
/*********************************************END OF FILE**********************/
