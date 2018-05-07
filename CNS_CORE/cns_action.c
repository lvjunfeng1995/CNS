/* ************************************************************************** */
/** Descriptive File Name

  @Company
    NewDoon

  @File Name
    cns_action.c

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
#include "cns_event.h"
/* ************************************************************************** */
/* ************************************************************************** */
// Section: Local Functions                                                   */
/* ************************************************************************** */
/* ************************************************************************** */



/* ************************************************************************** */ 
int recnum = 0;
void DataCheckEvent(void)
{
	  CNS_DATA_ERR data_err;
	
    CNS_DataErrCheck(&data_err);
     recnum++;       
               switch(data_err)
               {
                   case DATA_ERR:										 								 
										 
									     UsartSend(Reply_buf,Reply_length);
									 
									     break;
                
                   case DATA_IMPCHK:                       									 
                        
                       CNS_EventPost(CNS_EVENT_ELEANDIMP);
                       
                       break;
                    
                   case DATA_STM:                                             									     
									 
									     UsartSend(Reply_buf,Reply_length);
									 
									     CNS_EventPost(CNS_EVENT_STIMULATE);									 									     
                       
                       break;
                
                   case DATA_WF:
                       
                       CNS_EventPost(CNS_EVENT_WRITEFLASH);
                       
                       break;
									 
									 case DATA_CONNECT:										 								 
										 
									     UsartSend(Reply_buf,Reply_length);
									 
									     break;
									 
									 case DATA_CHARGE:
										 
									     CNS_EventPost(CNS_EVENT_CHARGE);
									 
									     break;
									 
									 case DATA_RESEND:										 								
										 
									     UsartSend(Reply_buf,Reply_length);
									 
									     break;
									 
									 case KEEP_LIVE:										   							 
										 
									     UsartSend(Reply_buf,Reply_length);
									 
									     break;
									 									 
									 case DATA_EEG:
										 
									     UsartSend(Reply_buf,Reply_length);
									 
										   break;
               }    	
    
}

void WriteFlashEvent()
{
	  CNS_DATA_ERR data_err;
	
    CNS_WriteFlash(&data_err);
                
     switch(data_err)
     {
         case WRITE_ERR:					 
					
         //ReplyInf(Rep_flash,0x01);

        // UsartSend(Reply_buf,Reply_length);		
				 
				 break;
                    
         case WRITE_SUCCESS:

        // ReplyInf(Rep_flash,0x01);

        // UsartSend(Reply_buf,Reply_length);				 
				 
				 break;
     }          
                               
}

void StimulateEvent()
{
	  CNS_DATA_ERR data_err;
	
    CNS_Stimulate(&data_err);
}

void ReadFlashEvent()
{
	  CNS_DATA_ERR data_err;
	
    CNS_ReadFlash(&data_err);
}

void EleAndImpEvent()
{
	  CNS_DATA_ERR data_err;
	
	  CNS_EleAndImpSend(&data_err);
}

void ChargeEvent()
{
    CNS_DATA_ERR data_err = DATA_NONE;
	
	  INT8U  pre_charge_state = 0;
	  INT8U  post_charge_state = 0;
	
	  pre_charge_state = GetChargeState();
							  
		if(pre_charge_state)
		{
							
				Charge_start();
							
				post_charge_state = pre_charge_state - 1;
							
		}
		else
		{
							
			  Charge_stop();
							
				post_charge_state = pre_charge_state + 1;
							
		}
												
		ReplyInf(Rep_charge,post_charge_state);
												
		UsartSend(Reply_buf,Reply_length);				
}

void SampleAndErrChkEvent()
{
	  CNS_DATA_ERR data_err = DATA_NONE;
	
    CNS_GetSample();						    						 
                
    CNS_ErrCheck();
}


/* *****************************************************************************
 End of File
 */