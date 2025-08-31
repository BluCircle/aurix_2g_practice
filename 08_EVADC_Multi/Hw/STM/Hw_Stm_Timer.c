#include "Hw_Stm_Timer.h"
#include "IfxStm.h"



uint64 millis(void)
{
    return IfxStm_get(&MODULE_STM0) / IfxStm_getFrequency(&MODULE_STM0) * 1000;
}
