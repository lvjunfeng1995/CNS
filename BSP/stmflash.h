#ifndef __STMFLASH_H__
#define __STMFLASH_H__
#include "cns.h"  
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

//////////////////////////////////////////////////////////////////////////////////////////////////////
//�û������Լ�����Ҫ����
 	 		//��ѡSTM32��FLASH������С(��λΪK)
#define STM32_FLASH_WREN 1              //ʹ��FLASHд��(0��������;1��ʹ��)
//////////////////////////////////////////////////////////////////////////////////////////////////////

//FLASH��ʼ��ַ
 	//STM32 FLASH����ʼ��ַ
//FLASH������ֵ
 

INT16U STMFLASH_ReadHalfWord(INT32U faddr);		  //��������  
void STMFLASH_WriteLenByte(INT32U WriteAddr,INT32U DataToWrite,INT16U Len);	//ָ����ַ��ʼд��ָ�����ȵ�����
INT32U STMFLASH_ReadLenByte(INT32U ReadAddr,INT16U Len);						//ָ����ַ��ʼ��ȡָ����������
INT32U STMFLASH_Write(INT32U WriteAddr,INT16U *pBuffer,INT16U NumToWrite);		//��ָ����ַ��ʼд��ָ�����ȵ�����
void STMFLASH_Read(INT32U ReadAddr,INT16U *pBuffer,INT16U NumToRead);   		//��ָ����ַ��ʼ����ָ�����ȵ�����

//����д��
void Test_Write(INT32U WriteAddr,INT16U WriteData);								   
#endif

















