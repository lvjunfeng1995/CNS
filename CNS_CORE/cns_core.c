/* ************************************************************************** */
/** Descriptive File Name

  @Company
    NewDoon

  @File Name
    cns_core.c

  @Description
    Task priority related operations.
 */
/* ************************************************************************** */

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

#include "cns.h"
#include "cns_action.h"

/* ************************************************************************** */
/* ************************************************************************** */
// Section: Local Functions                                                   */
/* ************************************************************************** */
/* ************************************************************************** */



/* ************************************************************************** */ 

static   EVENT_CT   EventArry     [EVENT_NUM]          = {0};
static   INT8U      CMN_BUF       [CMN_LENGTH]         = {0};
static   INT8U      REPLY_BUF     [REPLY_LENGTH]       = {0};
static   EEG_ADDR   EEG_BUF       [SAMPLING_FREQUENCY+3] = {0};

volatile    INT8U       CnsRdyTbl[EVENT_NUM] = {0};

EVENT_CT*   CnsEveTbl[EVENT_NUM] = {&EventArry[CNS_EVENT_DATACHECK],
                                    &EventArry[CNS_EVENT_STIMULATE],
                                    &EventArry[CNS_EVENT_CHARGE],
                                    &EventArry[CNS_EVENT_WRITEFLASH],
                                    &EventArry[CNS_EVENT_READFLASH],
                                    &EventArry[CNS_EVENT_ELEANDIMP],
                                    &EventArry[CNS_EVENT_ELELACK],
                                    &EventArry[CNS_EVENT_SAMPLEANDERRCHK],
                                    &EventArry[CNS_EVENT_ERRCHECK],
                                    &EventArry[CNS_EVENT_SLEEPMODE]};

volatile    CNS_REC_DATA_STATE Rec_State           =   CNS_REC_DATA_START; 
volatile    MSG_SIZE           CMN_Length          =   REC_DATA_NONE;
volatile    INT8U              Rec_Data_Ctr        =   REC_DATA_NONE;
volatile    INT8U              Stimulate_100us_Cnt =   0;
volatile    INT8U              Stimulate_not_Cnt   =   0;
volatile    INT8U              Stimulate_100us_Ctr =   0;
volatile    INT8U              Stimulate_not_Ctr   =   0;
volatile    INT32U             StimulateBurst      =   0;
volatile    INT32U             Stimulatethorugh    =   0;
volatile    STIMULATE_STATE    Stimulate_state     =   Stimulate_Start;
volatile    MSG_SIZE           Reply_length        =   0;
volatile    INT32U             WriteAddr_Head      =   0;
volatile    INT32U             WriteAddr_End       =   0;
volatile    POINTER_LOC        CurPointerLoc       =   POINTER_LOC_SECOND;																		
            INT8U             *CMN_buf             =   &CMN_BUF[0];
				    INT8U             *Reply_buf           =   &REPLY_BUF[0];
																		
						INT8U       CNSCmdTable[CNS_CFG_CMD_NUM] =   {5,31,7,9,11,13,15};
						
						STIMULATE   Stimulate_default            =   {0x1F4,2,0x64,0x50};
						STIMULATE   Stimulate_execute            =   {0x1F4,2,0x64,0x50}; 
						
						EEG_ADDR   *Head_buf                     =  &EEG_BUF[2];
						EEG_ADDR   *End_buf                      =  &EEG_BUF[SAMPLING_FREQUENCY+2];
						EEG_ADDR   *Mem_head                     =  &EEG_BUF[2];
						EEG_ADDR   *Mem_tail                     =  &EEG_BUF[2];
				
extern INT32U CurPoint;						
void CNS_EventRegist(EVENT_CT* const event_ct,ACTION_PTR action)
{
    if(NULL != event_ct)
		{
		  event_ct -> action = action;
		}
}

void WriteAddrPointerInit()
{
    INT32U first_end_addr   = STMFLASH_ReadWord(FIRST_WRITEADDRPOINTER_END);
	  INT32U first_head_addr  = STMFLASH_ReadWord(FIRST_WRITEADDRPOINTER_HEAD);
	
	  INT32U second_end_addr  = STMFLASH_ReadWord(SECOND_WRITEADDRPOINTER_END);
	  INT32U second_head_addr = STMFLASH_ReadWord(SECOND_WRITEADDRPOINTER_HEAD);
	
	  if(first_end_addr == 0xFFFFFFFF && second_end_addr == 0xFFFFFFFF)
		{
		  WriteAddr_Head = STM32_FLASH_BASE + STM_PAGE_SIZE*STMFLASH_EEGDATA_BEGIN_PAGE;
			WriteAddr_End  = STM32_FLASH_BASE + STM_PAGE_SIZE*STMFLASH_EEGDATA_BEGIN_PAGE;
			
			CurPointerLoc  = POINTER_LOC_SECOND;
		}
		else if(first_end_addr != 0xFFFFFFFF && second_end_addr == 0xFFFFFFFF)
		{
			WriteAddr_Head = first_head_addr;
			WriteAddr_End  = first_end_addr;
			
			CurPointerLoc  = POINTER_LOC_FIRST;
		}
		else if(first_end_addr == 0xFFFFFFFF && second_end_addr != 0xFFFFFFFF)
		{
		  WriteAddr_Head = second_head_addr;
			WriteAddr_End  = second_end_addr;
			
			CurPointerLoc  = POINTER_LOC_SECOND;
		}
		else if(first_end_addr != 0xFFFFFFFF && second_end_addr != 0xFFFFFFFF)
		{
			INT32U first_time  = *((INT32U *)first_end_addr);
			INT32U second_time = *((INT32U *)second_end_addr);
			
			if(first_time > second_time)
			{
				WriteAddr_Head = first_head_addr;
			  WriteAddr_End  = first_end_addr;
				
				CurPointerLoc  = POINTER_LOC_FIRST;
			}
			else
			{
			  WriteAddr_Head = second_head_addr;
			  WriteAddr_End  = second_end_addr;
				
				CurPointerLoc  = POINTER_LOC_SECOND;
			}
		}
}

void CNS_init()
{    	
	  CNS_EventRegist( (EVENT_CT*)  CnsEveTbl[CNS_EVENT_DATACHECK],
	                   (ACTION_PTR) DataCheckEvent);
	
	  CNS_EventRegist( (EVENT_CT*)  CnsEveTbl[CNS_EVENT_WRITEFLASH],
	                   (ACTION_PTR) WriteFlashEvent);
	
	  CNS_EventRegist( (EVENT_CT*)  CnsEveTbl[CNS_EVENT_STIMULATE],
	                   (ACTION_PTR) StimulateEvent);
	
	  CNS_EventRegist( (EVENT_CT*)  CnsEveTbl[CNS_EVENT_READFLASH],
	                   (ACTION_PTR) ReadFlashEvent);
	
	  CNS_EventRegist( (EVENT_CT*)  CnsEveTbl[CNS_EVENT_ELEANDIMP],
	                   (ACTION_PTR) EleAndImpEvent);
										 
    CNS_EventRegist( (EVENT_CT*)  CnsEveTbl[CNS_EVENT_CHARGE],
	                   (ACTION_PTR) ChargeEvent);
										 
	//	CNS_EventRegist( (EVENT_CT*)  CnsEveTbl[CNS_EVENT_SAMPLEANDERRCHK],
	//                   (ACTION_PTR) SampleAndErrChkEvent);
	
		SPI1_Init();
    Charge_Init();	  
	  ItanCLEAR();	 	  
	  ItanInit();
    RTC_Init();		
		uart_init(19200);
		TIM3_Int_Init();
	  TIM4_Int_Init();
		WriteAddrPointerInit();
		CurPoint = WriteAddr_Head;
}

void CNS_EventHandle()
{
	  CNS_EVENT NextEvent = CNS_EVENT_SAMPLEANDERRCHK;
	
    INT8U index = 0;
	
	  for(;index < EVENT_NUM;index++)
	  {
			if(CnsRdyTbl[index] != 0)
			{
			  NextEvent = (CNS_EVENT)index;
				
				break;
			}
		}
		
		if(NextEvent != CNS_EVENT_SAMPLEANDERRCHK)
		{
		  EVENT_CT* CurEvent_ct = CnsEveTbl[NextEvent];
			
			ACTION_PTR CurAction = CurEvent_ct -> action;
			
			if(CurAction != NULL)
			{
			  CurAction();
				
				if(CnsRdyTbl[NextEvent] == 0)
				{
				  while(1);
				}
				
				CnsRdyTbl[NextEvent] --;
			}
		}
}

void CNS_EventPost(CNS_EVENT event)
{
    CnsRdyTbl[event] ++;
}


/* *****************************************************************************
 End of File
 */
