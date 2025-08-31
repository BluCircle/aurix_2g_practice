#include "Hw_Adc.h"
#include "IfxEvadc_Adc.h"


IfxEvadc_Adc         g_evadc;
IfxEvadc_Adc_Group   g_adcGroup;
IfxEvadc_Adc_Channel g_adcChannel;



void Hw_Adc_Init(void)
{
    /* 모듈 초기화 */
    IfxEvadc_Adc_Config adcConfig;
    IfxEvadc_Adc_initModuleConfig(&adcConfig, &MODULE_EVADC);
    IfxEvadc_Adc_initModule(&g_evadc, &adcConfig);

    /* 그룹 초기화 */
    IfxEvadc_Adc_GroupConfig adcGroupConfig;
    IfxEvadc_Adc_initGroupConfig(&adcGroupConfig, &g_evadc);
    adcGroupConfig.groupId = IfxEvadc_GroupId_0;
    adcGroupConfig.master  = IfxEvadc_GroupId_0;
    adcGroupConfig.arbiter.requestSlotQueue0Enabled = TRUE;
    adcGroupConfig.queueRequest[0].triggerConfig.gatingMode  = IfxEvadc_GatingMode_always;
    IfxEvadc_Adc_initGroup(&g_adcGroup, &adcGroupConfig);

    /* 채널 초기화 */
    IfxEvadc_Adc_ChannelConfig adcChannelConfig;
    IfxEvadc_Adc_initChannelConfig(&adcChannelConfig, &g_adcGroup);
    adcChannelConfig.channelId     = (IfxEvadc_ChannelId)(0);     // 채널0
    adcChannelConfig.resultRegister= (IfxEvadc_ChannelResult)(0); // 결과레지스터0
    IfxEvadc_Adc_initChannel(&g_adcChannel, &adcChannelConfig);

    /* 큐 시작 */
    IfxEvadc_Adc_addToQueue(&g_adcChannel, IfxEvadc_RequestSource_queue0, IFXEVADC_QUEUE_REFILL);
    IfxEvadc_Adc_startQueue(&g_adcGroup, IfxEvadc_RequestSource_queue0);
}




uint16 Hw_Adc_Single_Read(void)
{
    uint16 ret = 0;
    Ifx_EVADC_G_RES conversionResult;

    do
    {
        conversionResult = IfxEvadc_Adc_getResult(&g_adcChannel);
    } while(!conversionResult.B.VF);

    ret = (uint16)conversionResult.B.RESULT;

    return ret;
}
