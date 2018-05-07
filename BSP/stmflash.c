#include "stmflash.h"
#include "stm32f10x.h"
 
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
 
//读取指定地址的半字(16位数据)
//faddr:读地址(此地址必须为2的倍数!!)
//返回值:对应数据.
u16 STMFLASH_ReadHalfWord(u32 faddr)
{
	return *(vu16*)faddr; 
}

u32 STMFLASH_ReadWord(u32 faddr)
{
	return *(vu32*)faddr; 
}
#if STM32_FLASH_WREN	//如果使能了写   
//不检查的写入
//WriteAddr:起始地址
//pBuffer:数据指针
//NumToWrite:半字(16位)数   
void STMFLASH_Write_NoCheck(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite)   
{ 			 		 
	u16 i;
	for(i=0;i<NumToWrite;i++)
	{
		FLASH_ProgramHalfWord(WriteAddr,pBuffer[i]);
	    WriteAddr+=2;//地址增加2.
	}  
}

u32 NextWriteAddr(u32 WriteAddr,u16 NumToWrite)
{
  u32 secpos;	   //扇区地址
	u16 secoff;	   //扇区内偏移地址(16位字计算)
	u16 secremain; //扇区内剩余地址(16位字计算)	     
	u32 offaddr;   //去掉0X08000000后的地址
	
	if(WriteAddr<STM32_FLASH_BASE||(WriteAddr>=(STM32_FLASH_BASE+1024*STM32_FLASH_SIZE))||(NumToWrite > (STM_PAGE_SIZE/2)))return 0;
	
	offaddr=WriteAddr-STM32_FLASH_BASE;		//实际偏移地址.
	secpos=offaddr/STM_PAGE_SIZE;			//扇区地址  0~127 for STM32F103RBT6
	secoff=(offaddr%STM_PAGE_SIZE)/2;		//在扇区内的偏移(2个字节为基本单位.)
	secremain=STM_PAGE_SIZE/2-secoff;		//扇区剩余空间大小

  if(NumToWrite<=secremain)
	{
	  return (WriteAddr+NumToWrite*2);
	}

  secpos++;
		
	if(secpos == STM_PAGE_NUM)
	{
		secpos = STMFLASH_EEGDATA_BEGIN_PAGE;
	}

  return (secpos*STM_PAGE_SIZE+STM32_FLASH_BASE);
}

u32 CheckFlashData(u32 StarAddr,u16 DataNum,u32 CheckCodeAddr)
{
  u16 i = 0;
	u16 *start_addr = (u16 *)StarAddr;
	u16 checkcode   = *((u16 *)CheckCodeAddr); 
	u16 sum = 0;
	
	for(;i<DataNum;i++)
	{
	  sum += *(start_addr);
		
		start_addr ++;
	}
	
	if(sum == checkcode)
	{
	  return 1;
	}
	
	return 0;	
}
//从指定地址开始写入指定长度的数据
//WriteAddr:起始地址(此地址必须为2的倍数!!)
//pBuffer:数据指针
//NumToWrite:半字(16位)数(就是要写入的16位数据的个数.)


u16 STMFLASH_BUF[STM_PAGE_SIZE/2];//最多是2K字节
u32 STMFLASH_Write(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite)	
{	
	u32 secpos;	   //扇区地址
	u16 secoff;	   //扇区内偏移地址(16位字计算)
	u16 secremain; //扇区内剩余地址(16位字计算)	   
 	u16 i;    
	u32 offaddr;   //去掉0X08000000后的地址
	u32 newaddr = WriteAddr + STMFLASH_WRITESIZE_ONCE;
	if(WriteAddr<STM32_FLASH_BASE||(WriteAddr>=(STM32_FLASH_BASE+1024*STM32_FLASH_SIZE))||(NumToWrite > (STM_PAGE_SIZE/2)))
	{
	  return WriteAddr;//非法地址
	}
	FLASH_Unlock();						//解锁
	offaddr=newaddr-STM32_FLASH_BASE;		//实际偏移地址.
	secpos=offaddr/STM_PAGE_SIZE;			//扇区地址  0~127 for STM32F103RBT6
	secoff=(offaddr%STM_PAGE_SIZE)/2;		//在扇区内的偏移(2个字节为基本单位.)
	secremain=STM_PAGE_SIZE/2-secoff;		//扇区剩余空间大小   
	if(NumToWrite<=secremain)
	{
	  secremain=NumToWrite;//不大于该扇区范围
		
		STMFLASH_Read(newaddr,STMFLASH_BUF,NumToWrite);
	
	  for(i=0;i<NumToWrite;i++)
	  {
	    if(STMFLASH_BUF[i]!=0xFFFF)
			  break;
	  }
	
	  if(i < NumToWrite)
	  {
		  STMFLASH_Read(secpos*STM_PAGE_SIZE+STM32_FLASH_BASE,STMFLASH_BUF,secoff);
		
	    FLASH_ErasePage(secpos*STM_PAGE_SIZE+STM32_FLASH_BASE);//擦除这个扇区
		
		  INT16U     j = secoff;
			INT16U *addr = pBuffer; 
		
	  	for(;j<secoff+secremain;j++)
	  	{
	  	  STMFLASH_BUF[j] = *(addr);
			
	  		addr ++;
	  	}
		
	  	STMFLASH_Write_NoCheck(secpos*STM_PAGE_SIZE+STM32_FLASH_BASE,STMFLASH_BUF,secremain+secoff);		
	  }
		else
    {
		  STMFLASH_Write_NoCheck(newaddr,pBuffer,secremain);
		}
	
	}
	else
	{
	  secpos++;
		
		if(secpos == STM_PAGE_NUM)
		{
			secpos = STMFLASH_EEGDATA_BEGIN_PAGE;
		}
		
		FLASH_ErasePage(secpos*STM_PAGE_SIZE+STM32_FLASH_BASE);//擦除这个扇区
		
		newaddr = secpos*STM_PAGE_SIZE + STM32_FLASH_BASE;
		
		secremain = NumToWrite;
		
		STMFLASH_Write_NoCheck(newaddr,pBuffer,secremain);//写已经擦除了的,直接写入扇区剩余区间.
	}
				
	FLASH_Lock();//上锁
	
	u8 num = 0;
	
	while(!CheckFlashData(newaddr+4,SAMPLING_FREQUENCY,newaddr+4+SAMPLING_FREQUENCY*2))
	{
	  num ++;
		
		if(num > 2) break;
	}
	
	if(num > 2)
	{
	  return WriteAddr;
	}
	
	return newaddr;
}
#endif

//从指定地址开始读出指定长度的数据
//ReadAddr:起始地址
//pBuffer:数据指针
//NumToWrite:半字(16位)数
void STMFLASH_Read(u32 ReadAddr,u16 *pBuffer,u16 NumToRead)   	
{
	u16 i;
	for(i=0;i<NumToRead;i++)
	{
		pBuffer[i]=STMFLASH_ReadHalfWord(ReadAddr);//读取2个字节.
		ReadAddr+=2;//偏移2个字节.	
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//WriteAddr:起始地址
//WriteData:要写入的数据
void Test_Write(u32 WriteAddr,u16 WriteData)   	
{
	STMFLASH_Write(WriteAddr,&WriteData,1);//写入一个字 
}

void STMFLASH_WriteAddrPointer()
{
	switch(CurPointerLoc)
	{
	  case POINTER_LOC_FIRST:
			
			STMFLASH_Read(STM32_FLASH_BASE+STM_PAGE_SIZE*STMFLASH_EEGDATA_BEGIN_PAGE+STM_PAGE_SIZE,
		                STMFLASH_BUF,
		                STM_PAGE_SIZE/2);
		
		  STMFLASH_BUF[STM_PAGE_SIZE/2-4] =  WriteAddr_Head;
		  STMFLASH_BUF[STM_PAGE_SIZE/2-3] =  WriteAddr_Head >>16;
		
		  STMFLASH_BUF[STM_PAGE_SIZE/2-2] =  WriteAddr_End;
		  STMFLASH_BUF[STM_PAGE_SIZE/2-1] =  WriteAddr_End >>16;
		
		  FLASH_Unlock();
		
		  FLASH_ErasePage(STM32_FLASH_BASE+STM_PAGE_SIZE*STMFLASH_EEGDATA_BEGIN_PAGE+STM_PAGE_SIZE);
		
		  STMFLASH_Write_NoCheck(STM32_FLASH_BASE+STM_PAGE_SIZE*STMFLASH_EEGDATA_BEGIN_PAGE+STM_PAGE_SIZE,
		                         STMFLASH_BUF,
		                         STM_PAGE_SIZE/2);
		
		  FLASH_Lock();
		
		  CurPointerLoc = POINTER_LOC_SECOND;
		
			break;
		
		case POINTER_LOC_SECOND:
			
		  STMFLASH_Read(STM32_FLASH_BASE+STM_PAGE_SIZE*STMFLASH_EEGDATA_BEGIN_PAGE,
		                STMFLASH_BUF,
		                STM_PAGE_SIZE/2);
		
		  STMFLASH_BUF[STM_PAGE_SIZE/2-4] =  WriteAddr_Head;
		  STMFLASH_BUF[STM_PAGE_SIZE/2-3] =  WriteAddr_Head >>16;
		
		  STMFLASH_BUF[STM_PAGE_SIZE/2-2] =  WriteAddr_End;
		  STMFLASH_BUF[STM_PAGE_SIZE/2-1] =  WriteAddr_End >>16;
		
		  FLASH_Unlock();
		
		  FLASH_ErasePage(STM32_FLASH_BASE+STM_PAGE_SIZE*STMFLASH_EEGDATA_BEGIN_PAGE);				  
		
		  STMFLASH_Write_NoCheck(STM32_FLASH_BASE+STM_PAGE_SIZE*STMFLASH_EEGDATA_BEGIN_PAGE,
		                         STMFLASH_BUF,
		                         STM_PAGE_SIZE/2);
		
		  FLASH_Lock();
		
		  CurPointerLoc = POINTER_LOC_FIRST;
		
			break;
	}
}
















