#include "Hw_Canfd.h"
#include "Hw_Gpio.h"


mcmcanType g_mcmcan;

IFX_INTERRUPT(canfd_isr_rxbuff, 0, ISR_PRIORITY_CANFD_RXBUFF);
IFX_INTERRUPT(canfd_isr_txbuff, 0, ISR_PRIORITY_CANFD_TXBUFF);


void canfd_isr_rxbuff(void)
{
    IfxCan_RxBufferId rxBufferId = IfxCan_RxBufferId_0;

    IfxCan_Node_clearInterruptFlag(g_mcmcan.canNode.node, IfxCan_Interrupt_messageStoredToDedicatedRxBuffer);

    while(!IfxCan_Node_isRxBufferNewDataUpdated(g_mcmcan.canNode.node, rxBufferId))
    {
        rxBufferId++;
    }

    g_mcmcan.rxMsg.bufferNumber = rxBufferId;
    IfxCan_Can_readMessage(&g_mcmcan.canNode, &g_mcmcan.rxMsg, (uint32*)&g_mcmcan.rxData[0]);

    Hw_Gpio_Toggle(rxBufferId+3);
}

void canfd_isr_txbuff(void)
{

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

    g_mcmcan.canNodeConfig.rxConfig.rxMode =IfxCan_RxMode_dedicatedBuffers;
    g_mcmcan.canNodeConfig.rxConfig.rxBufferDataFieldSize = IfxCan_DataFieldSize_64;

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


    g_mcmcan.canNodeConfig.interruptConfig.transmissionCompletedEnabled = TRUE;
    g_mcmcan.canNodeConfig.interruptConfig.traco.priority = ISR_PRIORITY_CANFD_TXBUFF;
    g_mcmcan.canNodeConfig.interruptConfig.traco.typeOfService = IfxSrc_Tos_cpu0;
    g_mcmcan.canNodeConfig.interruptConfig.traco.interruptLine = IfxCan_InterruptLine_1;

    /*add*/
    g_mcmcan.canNodeConfig.filterConfig.standardListSize = 3;

    IfxCan_Can_initNode(&g_mcmcan.canNode, &g_mcmcan.canNodeConfig);



    g_mcmcan.canFilter.number = 0;
    g_mcmcan.canFilter.elementConfiguration = IfxCan_FilterElementConfiguration_storeInRxBuffer;
    g_mcmcan.canFilter.id1 = 0x100;
    g_mcmcan.canFilter.type = IfxCan_FilterType_none;
    g_mcmcan.canFilter.rxBufferOffset = IfxCan_RxBufferId_0;
    IfxCan_Can_setStandardFilter(&g_mcmcan.canNode, &g_mcmcan.canFilter);

    /*add*/
    g_mcmcan.canFilter.number = 1;
    g_mcmcan.canFilter.elementConfiguration = IfxCan_FilterElementConfiguration_storeInRxBuffer;
    g_mcmcan.canFilter.id1 = 0x101;
    g_mcmcan.canFilter.type = IfxCan_FilterType_none;
    g_mcmcan.canFilter.rxBufferOffset = IfxCan_RxBufferId_1;
    IfxCan_Can_setStandardFilter(&g_mcmcan.canNode, &g_mcmcan.canFilter);

    /*add*/
    g_mcmcan.canFilter.number = 2;
    g_mcmcan.canFilter.elementConfiguration = IfxCan_FilterElementConfiguration_storeInRxBuffer;
    g_mcmcan.canFilter.id1 = 0x102;
    g_mcmcan.canFilter.type = IfxCan_FilterType_none;
    g_mcmcan.canFilter.rxBufferOffset = IfxCan_RxBufferId_2;
    IfxCan_Can_setStandardFilter(&g_mcmcan.canNode, &g_mcmcan.canFilter);

}

