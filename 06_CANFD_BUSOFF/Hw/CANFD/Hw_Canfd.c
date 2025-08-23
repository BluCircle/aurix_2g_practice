#include "Hw_Canfd.h"
#include "Hw_Gpio.h"


mcmcanType g_mcmcan;

IfxCan_Filter g_canfilter_tbl[CANFD_FILTER_NUM]=
{
        {0,IfxCan_FilterElementConfiguration_storeInRxBuffer,IfxCan_FilterType_none,0x100,0x0,IfxCan_RxBufferId_0},
        {1,IfxCan_FilterElementConfiguration_storeInRxBuffer,IfxCan_FilterType_none,0x101,0x0,IfxCan_RxBufferId_1},
        {2,IfxCan_FilterElementConfiguration_storeInRxBuffer,IfxCan_FilterType_none,0x102,0x0,IfxCan_RxBufferId_2},
        {3,IfxCan_FilterElementConfiguration_storeInRxFifo0,IfxCan_FilterType_dualId,0x200,0x0202,0},
        {4,IfxCan_FilterElementConfiguration_storeInRxFifo0,IfxCan_FilterType_range,0x204,0x206,0},
        {5,IfxCan_FilterElementConfiguration_storeInRxFifo0,IfxCan_FilterType_classic,0x210,0x7FC,0}
};


IFX_INTERRUPT(canfd_isr_rxbuff, 0, ISR_PRIORITY_CANFD_RXBUFF);
IFX_INTERRUPT(canfd_isr_rxfifo0, 0, ISR_PRIORITY_CANFD_RXFIFO0);
IFX_INTERRUPT(canfd_isr_txbuff, 0, ISR_PRIORITY_CANFD_TXBUFF);
IFX_INTERRUPT(canfd_isr_busoff, 0, ISR_PRIORITY_CANFD_BUSOFF);


void canfd_isr_rxbuff(void)
{
    IfxCan_RxBufferId rxBufferId = IfxCan_RxBufferId_0;

    IfxCan_Node_clearInterruptFlag(g_mcmcan.canNode.node, IfxCan_Interrupt_messageStoredToDedicatedRxBuffer);

    while(!IfxCan_Node_isRxBufferNewDataUpdated(g_mcmcan.canNode.node, rxBufferId))
    {
        rxBufferId++;
    }

    g_mcmcan.rxMsg.readFromRxFifo0 = FALSE;
    g_mcmcan.rxMsg.readFromRxFifo1 = FALSE;
    g_mcmcan.rxMsg.bufferNumber = rxBufferId;
    IfxCan_Can_readMessage(&g_mcmcan.canNode, &g_mcmcan.rxMsg, (uint32*)&g_mcmcan.rxData[0]);

    Hw_Gpio_Toggle(rxBufferId+3);
}

void canfd_isr_rxfifo0(void)
{
    IfxCan_Node_clearInterruptFlag(g_mcmcan.canNode.node, IfxCan_Interrupt_rxFifo0NewMessage);

    g_mcmcan.rxMsg.readFromRxFifo0 = TRUE;
    g_mcmcan.rxMsg.readFromRxFifo1 = FALSE;
    IfxCan_Can_readMessage(&g_mcmcan.canNode, &g_mcmcan.rxMsg, (uint32*)&g_mcmcan.rxData[0]);

    Hw_Gpio_Toggle(_GPIO_CH7);
}


void canfd_isr_txbuff(void)
{

}

void canfd_isr_busoff(void)
{
    IfxCan_Node_clearInterruptFlag(g_mcmcan.canNode.node, IfxCan_Interrupt_busOffStatus);

    if(g_mcmcan.canNode.node->CCCR.B.INIT != 0)
        g_mcmcan.canNode.node->CCCR.B.INIT = 0;

}


void Hw_Canfd_Init(void)
{
    IfxCan_Can_initModuleConfig(&g_mcmcan.canConfig, &MODULE_CAN0);
    IfxCan_Can_initModule(&g_mcmcan.canModule, &g_mcmcan.canConfig);

    IfxCan_Can_initNodeConfig(&g_mcmcan.canNodeConfig, &g_mcmcan.canModule);

    g_mcmcan.canNodeConfig.nodeId = IfxCan_NodeId_0;
    g_mcmcan.canNodeConfig.frame.type = IfxCan_FrameType_transmitAndReceive;
    g_mcmcan.canNodeConfig.frame.mode = IfxCan_FrameMode_fdLongAndFast;
    g_mcmcan.canNodeConfig.baudRate.baudrate = CANFD_BAUDRATE;
    g_mcmcan.canNodeConfig.fastBaudRate.baudrate = CANFD_FASTBAUDRATE;

    g_mcmcan.canNodeConfig.txConfig.txMode = IfxCan_TxMode_dedicatedBuffers;
    g_mcmcan.canNodeConfig.txConfig.dedicatedTxBuffersNumber = 1;
    g_mcmcan.canNodeConfig.txConfig.txBufferDataFieldSize = IfxCan_DataFieldSize_64;

    g_mcmcan.canNodeConfig.rxConfig.rxMode = IfxCan_RxMode_sharedAll;
    g_mcmcan.canNodeConfig.rxConfig.rxBufferDataFieldSize = IfxCan_DataFieldSize_64;
    g_mcmcan.canNodeConfig.rxConfig.rxFifo0Size = 8;

    IfxCan_Can_Pins canPins = {
        .rxPin      = &IfxCan_RXD00B_P20_7_IN,
        .rxPinMode  = IfxPort_InputMode_pullUp,
        .txPin      = &IfxCan_TXD00_P20_8_OUT,
        .txPinMode  = IfxPort_OutputMode_pushPull,
        .padDriver  = IfxPort_PadDriver_cmosAutomotiveSpeed2
    };
    g_mcmcan.canNodeConfig.pins = &canPins;


    g_mcmcan.canNodeConfig.interruptConfig.messageStoredToDedicatedRxBufferEnabled =TRUE;
    g_mcmcan.canNodeConfig.interruptConfig.reint.priority = ISR_PRIORITY_CANFD_RXBUFF;
    g_mcmcan.canNodeConfig.interruptConfig.reint.typeOfService =IfxSrc_Tos_cpu0;
    g_mcmcan.canNodeConfig.interruptConfig.reint.interruptLine = IfxCan_InterruptLine_0;


    g_mcmcan.canNodeConfig.interruptConfig.rxFifo0NewMessageEnabled = TRUE;
    g_mcmcan.canNodeConfig.interruptConfig.rxf0n.priority = ISR_PRIORITY_CANFD_RXFIFO0;
    g_mcmcan.canNodeConfig.interruptConfig.rxf0n.typeOfService = IfxSrc_Tos_cpu0;
    g_mcmcan.canNodeConfig.interruptConfig.rxf0n.interruptLine = IfxCan_InterruptLine_1;


    g_mcmcan.canNodeConfig.interruptConfig.transmissionCompletedEnabled = TRUE;
    g_mcmcan.canNodeConfig.interruptConfig.traco.priority = ISR_PRIORITY_CANFD_TXBUFF;
    g_mcmcan.canNodeConfig.interruptConfig.traco.typeOfService = IfxSrc_Tos_cpu0;
    g_mcmcan.canNodeConfig.interruptConfig.traco.interruptLine = IfxCan_InterruptLine_2;

    g_mcmcan.canNodeConfig.interruptConfig.busOffStatusEnabled = TRUE;
    g_mcmcan.canNodeConfig.interruptConfig.boff.priority = ISR_PRIORITY_CANFD_BUSOFF;
    g_mcmcan.canNodeConfig.interruptConfig.boff.typeOfService = IfxSrc_Tos_cpu0;
    g_mcmcan.canNodeConfig.interruptConfig.boff.interruptLine = IfxCan_InterruptLine_3;


    g_mcmcan.canNodeConfig.filterConfig.messageIdLength = IfxCan_MessageIdLength_standard;
    g_mcmcan.canNodeConfig.filterConfig.standardListSize =  CANFD_RXBUFF_NUM + CANFD_RXFIFO0_NUM;
    g_mcmcan.canNodeConfig.filterConfig.rejectRemoteFramesWithStandardId = TRUE;
    g_mcmcan.canNodeConfig.filterConfig.standardFilterForNonMatchingFrames = IfxCan_NonMatchingFrame_reject;

    IfxCan_Can_initNode(&g_mcmcan.canNode, &g_mcmcan.canNodeConfig);


    for(int i = 0 ; i < CANFD_FILTER_NUM ; i++)
    {
        g_mcmcan.canFilter.number = g_canfilter_tbl[i].number;
        g_mcmcan.canFilter.elementConfiguration = g_canfilter_tbl[i].elementConfiguration;
        g_mcmcan.canFilter.id1 = g_canfilter_tbl[i].id1;
        g_mcmcan.canFilter.id2 = g_canfilter_tbl[i].id2;
        g_mcmcan.canFilter.type = g_canfilter_tbl[i].type;
        g_mcmcan.canFilter.rxBufferOffset = g_canfilter_tbl[i].rxBufferOffset;
        IfxCan_Can_setStandardFilter(&g_mcmcan.canNode, &g_mcmcan.canFilter);
    }
}


void Hw_Can_Transmit_data(uint8* data)
{
    IfxCan_Status status;
    IfxCan_Can_initMessage(&g_mcmcan.txMsg);

    g_mcmcan.txMsg.messageId = MY_CAN_ID;
    g_mcmcan.txMsg.messageIdLength = IfxCan_MessageIdLength_standard;
    g_mcmcan.txMsg.frameMode = IfxCan_FrameMode_fdLongAndFast;
    g_mcmcan.txMsg.dataLengthCode = IfxCan_DataLengthCode_64;
    g_mcmcan.txMsg.bufferNumber = 0;

    // 데이터를 32비트 단위로 변환해서 txData에 저장
    memcpy(g_mcmcan.txData, data, 64); // 전제: Data는 64바이트

    // CAN 데이터 전송 (uint32* 형으로 캐스팅 필요)
    status = IfxCan_Can_sendMessage(&g_mcmcan.canNode, &g_mcmcan.txMsg, (uint32*)&g_mcmcan.txData[0]);
}

