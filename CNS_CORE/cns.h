/* ************************************************************************** */
/** Descriptive File Name

  @Company
    NewDoon

  @File Name
    cns.h

  @Description
    Contains all the methods and data structures
 */
/* ************************************************************************** */

#ifndef _CNS_H    /* Guard against multiple inclusion */
#define _CNS_H


#ifdef __cplusplus
extern "C" {
#endif
	
#include "cns_type.h"    
    
#define         NULL                         ((void *)0)
    
#define         REC_DATA_NONE                 0

#define         CMN_LENGTH                    64
#define         REPLY_LENGTH                  512
    
#define         INTERRUPT_ENABLE              INTX_ENABLE()             /*Enable interrupt*/
#define         INTERRUPT_DISABLE             INTX_DISABLE()           /*Disable interrupt*/   
 
#define         CNS_CFG_CMD_NUM                7
    
#define         EVENT_NUM                       10                               /*Number of task*/
    

#define         SAMPLING_FREQUENCY             200                              /*Sampling frequency*/

#define         True                           1
#define         False                          0

#define         STM32_FLASH_SIZE               256
#define         STM32_FLASH_BASE               0x08000000
#define         STM_PAGE_SIZE	                 2048
#define         STMFLASH_EEGDATA_BEGIN_PAGE    8
#define         STMFLASH_WRITESIZE_ONCE        406

#define         STM_PAGE_NUM                   STM32_FLASH_SIZE/(STM_PAGE_SIZE/1024)

#define         STM_WRITEADDRPOINTER_END       STM32_FLASH_BASE+ \
                                               STM32_FLASH_SIZE*1024 - 424

#define         FIRST_WRITEADDRPOINTER_HEAD    STM32_FLASH_BASE+ \
                                               STMFLASH_EEGDATA_BEGIN_PAGE* \
																							 STM_PAGE_SIZE+ \
																							 STM_PAGE_SIZE- 8
																							 
#define         FIRST_WRITEADDRPOINTER_END     STM32_FLASH_BASE+ \
                                               STMFLASH_EEGDATA_BEGIN_PAGE* \
																							 STM_PAGE_SIZE+ \
																							 STM_PAGE_SIZE- 4

#define         SECOND_WRITEADDRPOINTER_HEAD   STM32_FLASH_BASE+ \
                                               STMFLASH_EEGDATA_BEGIN_PAGE* \
																							 STM_PAGE_SIZE+ \
																							 STM_PAGE_SIZE*2 - 8
																							 
#define         SECOND_WRITEADDRPOINTER_END    STM32_FLASH_BASE+ \
                                               STMFLASH_EEGDATA_BEGIN_PAGE* \
																							 STM_PAGE_SIZE+ \
																							 STM_PAGE_SIZE*2 - 4

      			    
      typedef enum cns_rec_data_state    CNS_REC_DATA_STATE;
      typedef enum cns_event             CNS_EVENT;
      typedef enum cns_request           CNS_REQUEST;
      typedef enum cns_reply             CNS_REPLY;
			typedef enum cns_stimulate_state   STIMULATE_STATE;
			typedef enum cns_data_err          CNS_DATA_ERR;
			typedef enum cns_pointer_loc       POINTER_LOC;
			
			typedef struct stimulate           STIMULATE;
			typedef struct event_ct            EVENT_CT;
		
		enum cns_rec_data_state{
		    CNS_REC_DATA_START = 1,
			  CNS_REC_DATA_FIRST,
			  CNS_REC_DATA_CMD
		};

    enum cns_pointer_loc{
		    POINTER_LOC_FIRST,
			  POINTER_LOC_SECOND
		};		
                      
    
    enum cns_event                                                             /*Task type*/
    {
        CNS_EVENT_DATACHECK=0,
			  CNS_EVENT_STIMULATE,
			  CNS_EVENT_CHARGE,
        CNS_EVENT_WRITEFLASH,
        CNS_EVENT_READFLASH,
        CNS_EVENT_ELEANDIMP,        
        CNS_EVENT_ELELACK,
        CNS_EVENT_SAMPLEANDERRCHK,
        CNS_EVENT_ERRCHECK,
        CNS_EVENT_SLEEPMODE
    };
		
		enum cns_data_err                                                            /*Data err type*/
    {
        DATA_NONE=0,
        DATA_ERR,
        DATA_IMPCHK,
        DATA_STM,
        DATA_WF,
        DATA_RESEND,
			  DATA_CONNECT,
			  DATA_CHARGE,
			  WRITE_ERR,
			  WRITE_SUCCESS,
			  KEEP_LIVE,
			  DATA_EEG
    };
		
		enum cns_request
		{
		    Req_connect = 0,
			  Req_stimulate,
			  Req_charge,
			  Req_EEG,
			  Req_resend,
			  Keep_live,
			  Req_flasheeg
		};
		
		enum cns_reply
		{
		    Rep_confirm = 0,
			  Rep_stimulate,
			  Rep_charge,
			  Rep_EEG,
			  Rep_resend,
			  Rep_alive,
			  Rep_write,
			  Rep_end,
			  Rep_read
		};
		
		enum cns_stimulate_state
		{
		    Stimulate_Start,
			  Stimulate_Positive,
			  Stimulate_Negative,
			  Stimulate_End
		};   
         
		
		struct stimulate{
			
	      INT32U Freq;
			
	      INT32U Pwper;
			
	      INT32U BurstDuration;
			
			  INT8U Current;
    };
		
		struct event_ct
    {
		    ACTION_PTR action;
		};
        
		
            extern    volatile    CNS_REC_DATA_STATE Rec_State;
            extern    volatile    INT8U              Rec_Data_Ctr;	
            extern    volatile    MSG_SIZE           Reply_length;	
		
		        extern    volatile    INT8U             Stimulate_100us_Cnt;
            extern    volatile    INT8U             Stimulate_not_Cnt;  
            extern    volatile    INT8U             Stimulate_100us_Ctr;		
		        extern    volatile    INT8U             Stimulate_not_Ctr;
		        extern    volatile    STIMULATE_STATE   Stimulate_state;
            extern    volatile    INT32U            StimulateBurst;
            extern    volatile    INT32U            Stimulatethorugh;
		
		        extern    STIMULATE         Stimulate_execute;
		        extern    STIMULATE         Stimulate_default;


             extern   volatile    INT8U       CnsRdyTbl[EVENT_NUM];
             extern   EVENT_CT*   CnsEveTbl[EVENT_NUM];
		
		         extern      INT8U             CNSCmdTable[CNS_CFG_CMD_NUM];
		
            
		extern INT8U    *CMN_buf;                                       /*Point to the USART interrupt data buffer*/
		extern EEG_ADDR *Mem_tail;                                      /*Point to the current eeg*/
    extern EEG_ADDR *Head_buf;                                      /*Point to the eeg buffer bound*/ 
    extern EEG_ADDR *End_buf;
    extern EEG_ADDR *Mem_head;    
    extern INT8U    *Reply_buf;
		extern volatile  MSG_SIZE  CMN_Length;
		
		extern volatile  INT32U      WriteAddr_Head;
		extern volatile  INT32U      WriteAddr_End;
		extern volatile  POINTER_LOC CurPointerLoc;
		
		extern void CNS_EventHandle ();
		extern void CNS_EventPost   (CNS_EVENT event);
		extern void CNS_init        ();
               				          
   
#ifdef __cplusplus
}
#endif

#endif 

/* *****************************************************************************
 End of File
 */
