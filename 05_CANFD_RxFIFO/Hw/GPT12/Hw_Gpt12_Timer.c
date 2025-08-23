#include "Hw_Gpt12_Timer.h"
#include "IfxGpt12.h"
#include "Ap.h"




IFX_INTERRUPT(interruptGpt12_100ms, 0, ISR_PRIORITY_GPT12_TIMER_100MS);
IFX_INTERRUPT(interruptGpt12_500ms, 0, ISR_PRIORITY_GPT12_TIMER_500MS);

extern void apTimer_100ms(void);
extern void apTimer_500ms(void);

void interruptGpt12_100ms(void)
{
    IfxGpt12_T3_setTimerValue(&MODULE_GPT120, RELOAD_T3_100MS);
    apTimer_100ms();
}

void interruptGpt12_500ms(void)
{
    IfxGpt12_T4_setTimerValue(&MODULE_GPT120, RELOAD_T4_500MS);
    apTimer_500ms();
}


void Hw_Gpt12_Init(void)
{
    IfxGpt12_enableModule(&MODULE_GPT120);
    IfxGpt12_setGpt1BlockPrescaler(&MODULE_GPT120, IfxGpt12_Gpt1BlockPrescaler_16);

    /* GPT1 T3 Timer Init */
    IfxGpt12_T3_setMode(&MODULE_GPT120, IfxGpt12_Mode_timer);
    IfxGpt12_T3_setTimerDirection(&MODULE_GPT120, IfxGpt12_TimerDirection_down);
    IfxGpt12_T3_setTimerPrescaler(&MODULE_GPT120, IfxGpt12_TimerInputPrescaler_64);
    IfxGpt12_T3_setTimerValue(&MODULE_GPT120, RELOAD_T3_100MS);


    /* GPT1 T4 Timer Init */
    IfxGpt12_T4_setMode(&MODULE_GPT120, IfxGpt12_Mode_timer);
    IfxGpt12_T4_setTimerDirection(&MODULE_GPT120, IfxGpt12_TimerDirection_down);
    IfxGpt12_T4_setTimerPrescaler(&MODULE_GPT120, IfxGpt12_TimerInputPrescaler_128);
    IfxGpt12_T4_setTimerValue(&MODULE_GPT120, RELOAD_T4_500MS);


    /* Initialize the interrupt */
    IfxSrc_init(IfxGpt12_T3_getSrc(&MODULE_GPT120), IfxSrc_Tos_cpu0, ISR_PRIORITY_GPT12_TIMER_100MS);
    IfxSrc_enable(IfxGpt12_T3_getSrc(&MODULE_GPT120));

    IfxSrc_init(IfxGpt12_T4_getSrc(&MODULE_GPT120), IfxSrc_Tos_cpu0, ISR_PRIORITY_GPT12_TIMER_500MS);
    IfxSrc_enable(IfxGpt12_T4_getSrc(&MODULE_GPT120));

    /* Timer Start */

    IfxGpt12_T3_run(&MODULE_GPT120, IfxGpt12_TimerRun_start);
    IfxGpt12_T4_run(&MODULE_GPT120, IfxGpt12_TimerRun_start);
}


