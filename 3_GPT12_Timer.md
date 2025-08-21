# 내용
GPT12 타이머를 사용하여 타이머 인터럽트를 만들어보자






##
GPT
Hw_Gpt12_Timer


## 구현 코드

#include "Hw_Gpt12_Timer.h"
#include "IfxGpt12.h"



#define ISR_PROVIDER_GPT12_TIMER    IfxSrc_Tos_cpu0
#define RELOAD_VALUE_100MS          9765u //48828u
#define RELOAD_VALUE_500MS          24414u//48828u

IFX_INTERRUPT(interruptGpt12_100ms, 0, ISR_PRIORITY_GPT12_TIMER_100MS);
IFX_INTERRUPT(interruptGpt12_500ms, 0, ISR_PRIORITY_GPT12_TIMER_500MS);

float32 gpt_freq[3];

void Hw_Gpt12_Init(void)
{
    /* Initialize the GPT12 module */
    IfxGpt12_enableModule(&MODULE_GPT120);
    IfxGpt12_setGpt1BlockPrescaler(&MODULE_GPT120, IfxGpt12_Gpt1BlockPrescaler_16);


    /* Initialize the Timer T3 */
    IfxGpt12_T3_setMode(&MODULE_GPT120, IfxGpt12_Mode_timer);
    IfxGpt12_T3_setTimerDirection(&MODULE_GPT120, IfxGpt12_TimerDirection_down);
    IfxGpt12_T3_setTimerPrescaler(&MODULE_GPT120, IfxGpt12_TimerInputPrescaler_64);
    IfxGpt12_T3_setTimerValue(&MODULE_GPT120, RELOAD_VALUE_100MS);

    /* Initialize the Timer T4 */
    IfxGpt12_T4_setMode(&MODULE_GPT120, IfxGpt12_Mode_timer);
    IfxGpt12_T4_setTimerDirection(&MODULE_GPT120, IfxGpt12_TimerDirection_down);
    IfxGpt12_T4_setTimerPrescaler(&MODULE_GPT120, IfxGpt12_TimerInputPrescaler_128);
    IfxGpt12_T4_setTimerValue(&MODULE_GPT120, RELOAD_VALUE_500MS);

    /* Initialize the interrupt */
    IfxSrc_init(IfxGpt12_T3_getSrc(&MODULE_GPT120), IfxSrc_Tos_cpu0, ISR_PRIORITY_GPT12_TIMER_100MS);
    IfxSrc_enable(IfxGpt12_T3_getSrc(&MODULE_GPT120));

    IfxSrc_init(IfxGpt12_T4_getSrc(&MODULE_GPT120), IfxSrc_Tos_cpu0, ISR_PRIORITY_GPT12_TIMER_500MS);
    IfxSrc_enable(IfxGpt12_T4_getSrc(&MODULE_GPT120));


    /* Timer Start */
    IfxGpt12_T3_run(&MODULE_GPT120, IfxGpt12_TimerRun_start);
    IfxGpt12_T4_run(&MODULE_GPT120, IfxGpt12_TimerRun_start);

    /* Get GPT Frequency */
    gpt_freq[0]=IfxGpt12_getModuleFrequency(&MODULE_GPT120);
    gpt_freq[1]=IfxGpt12_T3_getFrequency(&MODULE_GPT120);
    gpt_freq[2]=IfxGpt12_T4_getFrequency(&MODULE_GPT120);

}



/* Interrupt Service Routine of the GPT12 */
void interruptGpt12_100ms(void)
{
    Hw_Gpio_Toggle(_GPIO_CH5);
    IfxGpt12_T3_setTimerValue(&MODULE_GPT120, RELOAD_VALUE_100MS);
}

void interruptGpt12_500ms(void)
{
    //Hw_Gpio_Toggle(_GPIO_CH5);
    IfxGpt12_T4_setTimerValue(&MODULE_GPT120, RELOAD_VALUE_500MS);

}
