#include "stmflash.h"
#include "stm32f10x.h"
 
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//STM32 FLASH ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/13
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////
 
//��ȡָ����ַ�İ���(16λ����)
//faddr:����ַ(�˵�ַ����Ϊ2�ı���!!)
//����ֵ:��Ӧ����.
u16 STMFLASH_ReadHalfWord(u32 faddr)
{
	return *(vu16*)faddr; 
}

u32 STMFLASH_ReadWord(u32 faddr)
{
	return *(vu32*)faddr; 
}
#if STM32_FLASH_WREN	//���ʹ����д   
//������д��
//WriteAddr:��ʼ��ַ
//pBuffer:����ָ��
//NumToWrite:����(16λ)��   
void STMFLASH_Write_NoCheck(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite)   
{ 			 		 
	u16 i;
	for(i=0;i<NumToWrite;i++)
	{
		FLASH_ProgramHalfWord(WriteAddr,pBuffer[i]);
	    WriteAddr+=2;//��ַ����2.
	}  
}

u32 NextWriteAddr(u32 WriteAddr,u16 NumToWrite)
{
  u32 secpos;	   //������ַ
	u16 secoff;	   //������ƫ�Ƶ�ַ(16λ�ּ���)
	u16 secremain; //������ʣ���ַ(16λ�ּ���)	     
	u32 offaddr;   //ȥ��0X08000000��ĵ�ַ
	
	if(WriteAddr<STM32_FLASH_BASE||(WriteAddr>=(STM32_FLASH_BASE+1024*STM32_FLASH_SIZE))||(NumToWrite > (STM_PAGE_SIZE/2)))return 0;
	
	offaddr=WriteAddr-STM32_FLASH_BASE;		//ʵ��ƫ�Ƶ�ַ.
	secpos=offaddr/STM_PAGE_SIZE;			//������ַ  0~127 for STM32F103RBT6
	secoff=(offaddr%STM_PAGE_SIZE)/2;		//�������ڵ�ƫ��(2���ֽ�Ϊ������λ.)
	secremain=STM_PAGE_SIZE/2-secoff;		//����ʣ��ռ��С

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
//��ָ����ַ��ʼд��ָ�����ȵ�����
//WriteAddr:��ʼ��ַ(�˵�ַ����Ϊ2�ı���!!)
//pBuffer:����ָ��
//NumToWrite:����(16λ)��(����Ҫд���16λ���ݵĸ���.)


u16 STMFLASH_BUF[STM_PAGE_SIZE/2];//�����2K�ֽ�
u32 STMFLASH_Write(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite)	
{	
	u32 secpos;	   //������ַ
	u16 secoff;	   //������ƫ�Ƶ�ַ(16λ�ּ���)
	u16 secremain; //������ʣ���ַ(16λ�ּ���)	   
 	u16 i;    
	u32 offaddr;   //ȥ��0X08000000��ĵ�ַ
	u32 newaddr = WriteAddr + STMFLASH_WRITESIZE_ONCE;
	if(WriteAddr<STM32_FLASH_BASE||(WriteAddr>=(STM32_FLASH_BASE+1024*STM32_FLASH_SIZE))||(NumToWrite > (STM_PAGE_SIZE/2)))
	{
	  return WriteAddr;//�Ƿ���ַ
	}
	FLASH_Unlock();						//����
	offaddr=newaddr-STM32_FLASH_BASE;		//ʵ��ƫ�Ƶ�ַ.
	secpos=offaddr/STM_PAGE_SIZE;			//������ַ  0~127 for STM32F103RBT6
	secoff=(offaddr%STM_PAGE_SIZE)/2;		//�������ڵ�ƫ��(2���ֽ�Ϊ������λ.)
	secremain=STM_PAGE_SIZE/2-secoff;		//����ʣ��ռ��С   
	if(NumToWrite<=secremain)
	{
	  secremain=NumToWrite;//�����ڸ�������Χ
		
		STMFLASH_Read(newaddr,STMFLASH_BUF,NumToWrite);
	
	  for(i=0;i<NumToWrite;i++)
	  {
	    if(STMFLASH_BUF[i]!=0xFFFF)
			  break;
	  }
	
	  if(i < NumToWrite)
	  {
		  STMFLASH_Read(secpos*STM_PAGE_SIZE+STM32_FLASH_BASE,STMFLASH_BUF,secoff);
		
	    FLASH_ErasePage(secpos*STM_PAGE_SIZE+STM32_FLASH_BASE);//�����������
		
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
		
		FLASH_ErasePage(secpos*STM_PAGE_SIZE+STM32_FLASH_BASE);//�����������
		
		newaddr = secpos*STM_PAGE_SIZE + STM32_FLASH_BASE;
		
		secremain = NumToWrite;
		
		STMFLASH_Write_NoCheck(newaddr,pBuffer,secremain);//д�Ѿ������˵�,ֱ��д������ʣ������.
	}
				
	FLASH_Lock();//����
	
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

//��ָ����ַ��ʼ����ָ�����ȵ�����
//ReadAddr:��ʼ��ַ
//pBuffer:����ָ��
//NumToWrite:����(16λ)��
void STMFLASH_Read(u32 ReadAddr,u16 *pBuffer,u16 NumToRead)   	
{
	u16 i;
	for(i=0;i<NumToRead;i++)
	{
		pBuffer[i]=STMFLASH_ReadHalfWord(ReadAddr);//��ȡ2���ֽ�.
		ReadAddr+=2;//ƫ��2���ֽ�.	
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//WriteAddr:��ʼ��ַ
//WriteData:Ҫд�������
void Test_Write(u32 WriteAddr,u16 WriteData)   	
{
	STMFLASH_Write(WriteAddr,&WriteData,1);//д��һ���� 
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
















