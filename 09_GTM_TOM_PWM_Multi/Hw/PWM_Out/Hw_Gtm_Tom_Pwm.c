#include "Hw_Gtm_Tom_Pwm.h"




#define PWM_CH1  IfxGtm_TOM1_6_TOUT83_P14_3_OUT
#define PWM_CH2  IfxGtm_TOM1_12_TOUT34_P33_12_OUT
#define PWM_CH3  IfxGtm_TOM1_7_TOUT6_P02_6_OUT
#define PWM_CH4  IfxGtm_TOM1_2_TOUT33_P33_11_OUT

IfxGtm_Tom_Pwm_Config g_tomConfig[HW_PWM_NUM];
IfxGtm_Tom_Pwm_Driver g_tomDriver[HW_PWM_NUM];


IfxGtm_Tom_ToutMap *g_pwmConfig_Tbl[HW_PWM_NUM] = { &PWM_CH1, &PWM_CH2, &PWM_CH3, &PWM_CH4 };


void Hw_Gtm_Pwm_Init(void)
{
    IfxGtm_enable(&MODULE_GTM);
    IfxGtm_Cmu_enableClocks(&MODULE_GTM, IFXGTM_CMU_CLKEN_FXCLK);


    for(uint8 idx = 0; idx < HW_PWM_NUM; idx++)
    {
        IfxGtm_Tom_Pwm_initConfig(&g_tomConfig[idx], &MODULE_GTM);
        g_tomConfig[idx].tom = g_pwmConfig_Tbl[idx]->tom;
        g_tomConfig[idx].tomChannel = g_pwmConfig_Tbl[idx]->channel;

        g_tomConfig[idx].clock = IfxGtm_Tom_Ch_ClkSrc_cmuFxclk0;
        g_tomConfig[idx].period = PWM_PERIOD;
        g_tomConfig[idx].dutyCycle = PWM_PERIOD*0.5;
        g_tomConfig[idx].pin.outputPin = g_pwmConfig_Tbl[idx];
        g_tomConfig[idx].synchronousUpdateEnabled = TRUE;

        IfxGtm_Tom_Pwm_init(&g_tomDriver[idx], &g_tomConfig[idx]);
        IfxGtm_Tom_Pwm_start(&g_tomDriver[idx], TRUE);
    }
}



void Hw_Gtm_Pwm_On(uint8 ch)
{
    IfxGtm_Tom_Pwm_start(&g_tomDriver[ch], TRUE);
}


void Hw_Gtm_Pwm_Off(uint8 ch)
{
    IfxGtm_Tom_Pwm_stop(&g_tomDriver[ch], TRUE);
}


void Hw_Gtm_Pwm_Set_Duty(uint8 ch, uint16 duty)
{
    g_tomConfig[ch].dutyCycle = PWM_PERIOD*duty/100;
    IfxGtm_Tom_Pwm_init(&g_tomDriver[ch], &g_tomConfig[ch]);
}


void Hw_Gtm_Pwm_Set_Period(uint8 ch, uint16 Period)
{
    g_tomConfig[ch].period = Period;
    IfxGtm_Tom_Pwm_init(&g_tomDriver[ch], &g_tomConfig[ch]);
}
