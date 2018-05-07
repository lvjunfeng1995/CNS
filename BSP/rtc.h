#ifndef __RTC_H
#define __RTC_H	    
//Mini STM32������
//RTCʵʱʱ�� ��������			 
//����ԭ��@ALIENTEK
//2010/6/6

//ʱ��ṹ��
#include "cns.h"
typedef struct 
{
	INT8U hour;
	INT8U min;
	INT8U sec;			
	//������������
	INT16U w_year;
	INT8U  w_month;
	INT8U  w_date;
	INT8U  week;		 
}_calendar_obj;					 
extern _calendar_obj calendar;	//�����ṹ��

extern INT8U const mon_table[12];	//�·��������ݱ�
void Disp_Time(INT8U x,INT8U y,INT8U size);//���ƶ�λ�ÿ�ʼ��ʾʱ��
void Disp_Week(INT8U x,INT8U y,INT8U size,INT8U lang);//��ָ��λ����ʾ����
INT8U RTC_Init(void);        //��ʼ��RTC,����0,ʧ��;1,�ɹ�;
INT8U Is_Leap_Year(INT16U year);//ƽ��,�����ж�
INT8U RTC_Alarm_Set(INT16U syear,INT8U smon,INT8U sday,INT8U hour,INT8U min,INT8U sec);
INT8U RTC_Get(void);         //����ʱ��   
INT8U RTC_Get_Week(INT16U year,INT8U month,INT8U day);
INT8U RTC_Set(INT16U syear,INT8U smon,INT8U sday,INT8U hour,INT8U min,INT8U sec);//����ʱ��			 
#endif


