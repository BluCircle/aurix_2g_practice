#include "Hw_Gpio.h"



typedef struct
{
  Ifx_P *port;
  uint8 pinIndex;
  IfxPort_Mode mode;
  IfxPort_State init_state;
}Hw_Gpio_ConfigType;

Hw_Gpio_ConfigType Hw_Gpio_Config_Tbl[HW_GPIO_CH] =
{
        {&MODULE_P33, 4, IfxPort_Mode_outputPushPullGeneral, IfxPort_State_high},
        {&MODULE_P33, 5, IfxPort_Mode_outputPushPullGeneral, IfxPort_State_high},
        {&MODULE_P33, 6, IfxPort_Mode_outputPushPullGeneral, IfxPort_State_high},
        {&MODULE_P33, 7, IfxPort_Mode_outputPushPullGeneral, IfxPort_State_high},
        {&MODULE_P20, 11, IfxPort_Mode_outputPushPullGeneral, IfxPort_State_high},
        {&MODULE_P20, 12, IfxPort_Mode_outputPushPullGeneral, IfxPort_State_high},
        {&MODULE_P20, 13, IfxPort_Mode_outputPushPullGeneral, IfxPort_State_high},
        {&MODULE_P20, 14, IfxPort_Mode_outputPushPullGeneral, IfxPort_State_high}
};



void Hw_Gpio_Init(void)
{
    for(uint8 idx = 0; idx < HW_GPIO_CH ; idx++)
    {
        /*Mode 설정*/
        IfxPort_setPinMode(Hw_Gpio_Config_Tbl[idx].port, Hw_Gpio_Config_Tbl[idx].pinIndex, Hw_Gpio_Config_Tbl[idx].mode);
        /*초기 상태 설정*/
        IfxPort_setPinState(Hw_Gpio_Config_Tbl[idx].port, Hw_Gpio_Config_Tbl[idx].pinIndex, Hw_Gpio_Config_Tbl[idx].init_state);
    }
}

uint8 Hw_Gpio_Read(uint8 ch)
{
    uint8 ret = 0;
    ret = IfxPort_getPinState(Hw_Gpio_Config_Tbl[ch].port, Hw_Gpio_Config_Tbl[ch].pinIndex);
    return ret;
}

void Hw_Gpio_Write(uint8 ch, uint8 cmd)
{
    if(cmd == GPIO_HIGH)
        IfxPort_setPinState(Hw_Gpio_Config_Tbl[ch].port, Hw_Gpio_Config_Tbl[ch].pinIndex, IfxPort_State_low);

    if(cmd == GPIO_LOW)
        IfxPort_setPinState(Hw_Gpio_Config_Tbl[ch].port, Hw_Gpio_Config_Tbl[ch].pinIndex, IfxPort_State_high);
}

void Hw_Gpio_Toggle(uint8 ch)
{
    IfxPort_togglePin(Hw_Gpio_Config_Tbl[ch].port, Hw_Gpio_Config_Tbl[ch].pinIndex);
}
