/**
******************************************************************************
* @filename			vermanage.h
* @author				祝世叶
* @Description	vermanage文件，在此文件内主要是当前系统的编译版本号、系统的
                 硬件信息以及作者和使用的OS！
*/

 /* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _VerManage_H_  //头文件
#define _VerManage_H_ 

/*     FirstBuiltDate：2018-10-04 17:50:57      */
/*    工程最新编译时间：2018-10-16 17:13:03   */

#define _BuiltDate __DATE__
#define _BuiltTime __TIME__

const unsigned char BuiltDate[] = _BuiltDate;
const unsigned char BuiltTime[] = _BuiltTime;
const unsigned char MCUName[] = {"STM32F103CBT6"};
const unsigned char MCUKernel[] = {"M3"};
const unsigned char RunFOSC[] = {"72MHz"};
const unsigned char Ver[] = {"1.0"};
const unsigned char VerSet[] = {"1"};
const unsigned char ProjectName[] = {"IAP_CAN"};
const unsigned char Author[] = {"ZEHN"};
const unsigned char OSName[] = {"RECIVER"};


#endif

