#ifndef HW_ADC_HW_ADC_H_
#define HW_ADC_HW_ADC_H_

#include "typeDefs.h"
#include "IfxEvadc_Adc.h"

typedef struct
{
        IfxEvadc_GroupId       groupId;
        IfxEvadc_ChannelId     channelId;
        IfxEvadc_ChannelResult resultReg;
}adcInitCfg;


void Hw_Adc_Init(void);
uint16 Hw_Adc_Single_Read(uint8 ch);


#endif /* HW_ADC_HW_ADC_H_ */
