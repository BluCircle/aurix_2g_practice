#ifndef HW_CANFD_HW_CANFD_H_
#define HW_CANFD_HW_CANFD_H_

#include "typeDefs.h"
#include "IfxCan_Can.h"


typedef struct
{
    IfxCan_Can_Config canConfig;
    IfxCan_Can canModule;
    IfxCan_Can_Node canNode;
    IfxCan_Can_NodeConfig canNodeConfig;
    IfxCan_Filter canFilter;
    IfxCan_Message txMsg;
    IfxCan_Message rxMsg;
    uint8 txData[MAXIMUM_CAN_FD_DATA_PAYLOAD];
    uint8 rxData[MAXIMUM_CAN_FD_DATA_PAYLOAD];
} mcmcanType;


void Hw_Canfd_Init(void);
void Hw_Can_Transmit_data(uint8* data);


#endif /* HW_CANFD_HW_CANFD_H_ */
