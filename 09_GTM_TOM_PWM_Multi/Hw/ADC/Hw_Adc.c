#include "Hw_Adc.h"

IfxEvadc_Adc         g_evadc;
IfxEvadc_Adc_Group   g_adcGroup[HW_ADC_NUM];
IfxEvadc_Adc_Channel g_adcChannel[HW_ADC_NUM];

static const adcInitCfg adcInitCfgTbl[HW_ADC_NUM] = {
        {IfxEvadc_GroupId_0, 7, 7},    // AN7
        {IfxEvadc_GroupId_2, 4, 4},    // AN20
        {IfxEvadc_GroupId_2, 5, 5},    // AN21
        {IfxEvadc_GroupId_8, 12, 12}   // AN44
};


void Hw_Adc_Init(void)
{
    /* 모듈 초기화 */
    IfxEvadc_Adc_Config adcConfig;
    IfxEvadc_Adc_initModuleConfig(&adcConfig, &MODULE_EVADC);
    IfxEvadc_Adc_initModule(&g_evadc, &adcConfig);

    /* 그룹 초기화 */
    IfxEvadc_Adc_GroupConfig adcGroupConfig;
    for(uint8 idx=0;idx<HW_ADC_NUM;idx++)
    {
        IfxEvadc_Adc_initGroupConfig(&adcGroupConfig, &g_evadc);
        adcGroupConfig.groupId = adcInitCfgTbl[idx].groupId;
        adcGroupConfig.master  = adcInitCfgTbl[idx].groupId;
        adcGroupConfig.arbiter.requestSlotQueue0Enabled = TRUE;
        adcGroupConfig.queueRequest[0].triggerConfig.gatingMode  = IfxEvadc_GatingMode_always;
        IfxEvadc_Adc_initGroup(&g_adcGroup[idx], &adcGroupConfig);
    }

    /* 채널 초기화 */
    IfxEvadc_Adc_ChannelConfig adcChannelConfig;
    for(uint8 idx=0;idx<HW_ADC_NUM;idx++)
    {
        IfxEvadc_Adc_initChannelConfig(&adcChannelConfig, &g_adcGroup[idx]);
        adcChannelConfig.channelId     = adcInitCfgTbl[idx].channelId;     // 채널0
        adcChannelConfig.resultRegister= adcInitCfgTbl[idx].resultReg; // 결과레지스터0
        IfxEvadc_Adc_initChannel(&g_adcChannel[idx], &adcChannelConfig);
    }

    /* 큐 시작 */
    for(uint8 idx=0;idx<HW_ADC_NUM;idx++)
    {
        IfxEvadc_Adc_addToQueue(&g_adcChannel[idx], IfxEvadc_RequestSource_queue0, IFXEVADC_QUEUE_REFILL);
        IfxEvadc_Adc_startQueue(&g_adcGroup[idx], IfxEvadc_RequestSource_queue0);
    }
}




uint16 Hw_Adc_Single_Read(uint8 ch)
{
    Ifx_EVADC_G_RES result;

    do
    {
        result = IfxEvadc_Adc_getResult(&g_adcChannel[ch]);
    } while(!result.B.VF);

    return (uint16)result.B.RESULT;
}
