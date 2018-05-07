/* ************************************************************************** */
/** Descriptive File Name

  @Company
    NewDoon

  @File Name
    cns_type.h

  @Description
    Contains all value type
 */
/* ************************************************************************** */

#ifndef _CNS_ACTION_H    /* Guard against multiple inclusion */
#define _CNS_ACTION_H 

#ifdef __cplusplus
extern "C" {
#endif
	
    extern void DataCheckEvent(void);
    
    extern void WriteFlashEvent(void);
    
    extern void StimulateEvent(void);
    
    extern void ReadFlashEvent(void);
    
    extern void ManageErrEvent(void);
    
    extern void EleAndImpEvent(void);
    
    extern void ChargeEvent(void);
    
    extern void NoticeEleEvent(void);
    
    extern void SampleAndErrChkEvent(void);
    
    extern void SleepModeEvent(void);
    

#ifdef __cplusplus
}
#endif

#endif 
/* *****************************************************************************
 End of File
*/