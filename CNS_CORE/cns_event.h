/* ************************************************************************** */
/** Descriptive File Name

  @Company
    NewDoon

  @File Name
    cns_event.h

  @Summary
    Brief description of the file.

  @Description
    Describe the purpose of this file.
 */
/* ************************************************************************** */

#ifndef _CNS_EVENT_H    /* Guard against multiple inclusion */
#define _CNS_EVENT_H

#include "cns.h"

/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif

    extern   void     UsartSend(INT8U * p_Send, MSG_SIZE len);
	
	  extern   INT32U   ItanTriWRITE(INT32U  reg, INT16U  dat);
	
    extern   INT32U   ItanWRITE(INT32U reg, INT16U dat);
	
	  extern   MSG_SIZE RestoreData(INT8U *mem,MSG_SIZE msg_size);
	
	  extern   void     ReplyConfirm(void);
	
	  extern   void     ReplyInf(CNS_REPLY reply,INT8U request);
	
	  extern   void     CNS_DataErrCheck(CNS_DATA_ERR  *data_err);
	
	  extern   void     CNS_WriteFlash(CNS_DATA_ERR *data_err);
	
	  extern   INT32U   ItanSTIMULATE(INT32U ch);
	
	  extern   void     CNS_Stimulate(CNS_DATA_ERR *data_err);
		
		extern   void     CNS_ReadFlash(CNS_DATA_ERR *data_err);
		
		extern   void     CNS_EleAndImpSend(CNS_DATA_ERR *data_err);
		
		extern   void     Charge_Init(void);
		
		extern   void     Charge_start (void);
		
		extern   void     Charge_stop (void);
		
		extern   INT8U    GetChargeState (void);
		
		extern   INT32U   ItanCONVERT(INT8U ch);
		
		extern   void     CNS_ErrCheck();
		
		extern   INT32U   ItanCLEAR(void);
		
		extern   void     ItanInit(void);
		
		extern   void     CNS_OSEnterSleepMode(void);
		
		extern   void     CNS_GetSample();

    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _EXAMPLE_FILE_NAME_H */

/* *****************************************************************************
 End of File
 */