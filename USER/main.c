
#include "cns.h"

int main()
{
	CNS_init();
	
	while(1)
	{
	  CNS_EventHandle();
	
		//STMFLASH_Write(STM32_FLASH_BASE+STM_PAGE_SIZE*STMFLASH_EEGDATA_BEGIN_PAGE,Mem_head,200);

    //RTC_Get();
	}
}