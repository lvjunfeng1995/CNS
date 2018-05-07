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

#ifndef _CNS_TYPE_H    /* Guard against multiple inclusion */
#define _CNS_TYPE_H 

#ifdef __cplusplus
extern "C" {
#endif
	
    typedef     unsigned char        INT8U;
    typedef     unsigned short       INT16U;
    typedef     unsigned int         INT32U;
    typedef     char                 CHAR;
    
    typedef     unsigned short       EEG_ADDR;
	  typedef     unsigned short       MSG_SIZE;
	
	  typedef     void(*ACTION_PTR)      (void);
    

#ifdef __cplusplus
}
#endif

#endif 
/* *****************************************************************************
 End of File
*/