#ifndef __RTC_H
#define __RTC_H	    
//Mini STM32开发板
//RTC实时时钟 驱动代码			 
//正点原子@ALIENTEK
//2010/6/6

//时间结构体
#include "cns.h"
typedef struct 
{
	INT8U hour;
	INT8U min;
	INT8U sec;			
	//公历日月年周
	INT16U w_year;
	INT8U  w_month;
	INT8U  w_date;
	INT8U  week;		 
}_calendar_obj;					 
extern _calendar_obj calendar;	//日历结构体

extern INT8U const mon_table[12];	//月份日期数据表
void Disp_Time(INT8U x,INT8U y,INT8U size);//在制定位置开始显示时间
void Disp_Week(INT8U x,INT8U y,INT8U size,INT8U lang);//在指定位置显示星期
INT8U RTC_Init(void);        //初始化RTC,返回0,失败;1,成功;
INT8U Is_Leap_Year(INT16U year);//平年,闰年判断
INT8U RTC_Alarm_Set(INT16U syear,INT8U smon,INT8U sday,INT8U hour,INT8U min,INT8U sec);
INT8U RTC_Get(void);         //更新时间   
INT8U RTC_Get_Week(INT16U year,INT8U month,INT8U day);
INT8U RTC_Set(INT16U syear,INT8U smon,INT8U sday,INT8U hour,INT8U min,INT8U sec);//设置时间			 
#endif


