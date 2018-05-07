#ifndef __STMFLASH_H__
#define __STMFLASH_H__
#include "cns.h"  
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//STM32 FLASH 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/13
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////
//用户根据自己的需要设置
 	 		//所选STM32的FLASH容量大小(单位为K)
#define STM32_FLASH_WREN 1              //使能FLASH写入(0，不是能;1，使能)
//////////////////////////////////////////////////////////////////////////////////////////////////////

//FLASH起始地址
 	//STM32 FLASH的起始地址
//FLASH解锁键值
 

INT16U STMFLASH_ReadHalfWord(INT32U faddr);		  //读出半字  
void STMFLASH_WriteLenByte(INT32U WriteAddr,INT32U DataToWrite,INT16U Len);	//指定地址开始写入指定长度的数据
INT32U STMFLASH_ReadLenByte(INT32U ReadAddr,INT16U Len);						//指定地址开始读取指定长度数据
INT32U STMFLASH_Write(INT32U WriteAddr,INT16U *pBuffer,INT16U NumToWrite);		//从指定地址开始写入指定长度的数据
void STMFLASH_Read(INT32U ReadAddr,INT16U *pBuffer,INT16U NumToRead);   		//从指定地址开始读出指定长度的数据

//测试写入
void Test_Write(INT32U WriteAddr,INT16U WriteData);								   
#endif

















