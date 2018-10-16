#ifndef __IAP_H__
#define __IAP_H__
#include "sys.h"  



typedef  void (*iapfun)(void);				//定义一个函数类型的参数.

#define FLASH_APP1_ADDR		0x08010000  	//第一个应用程序起始地址(存放在FLASH)
											//保留0X08000000~0X0800FFFF的空间为IAP使用

extern u16 iapbuf[1024];  
extern u8  buff[8];
extern u8  app_buff[2*1024];

void iap_load_app(u32 appxaddr);			//执行flash里面的app程序
void iap_load_appsram(u32 appxaddr);		//执行sram里面的app程序
void iap_write_appbin_8(u32 appxaddr,u8 *appbuf);
void iap_write_appbin(u32 appxaddr,u8 *appbuf,u32 applen);	//在指定地址开始,写入bin
#endif







































