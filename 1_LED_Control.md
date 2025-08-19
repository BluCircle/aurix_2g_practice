# 내용
GPIO 제어 함수를 생성하여 LED를 제어해보자.

# 주요 함수
void Hw_Gpio_Init(void);
uint8 Hw_Gpio_Read(uint8 ch);
void Hw_Gpio_Write(uint8 ch, uint8 cmd);
void Hw_Gpio_Toggle(uint8 ch);


# 구현 단계

## 파일 포함
#include "Hw_Gpio.h"
#include "IfxPort.h"
#include "IfxPort_PinMap.h"

## 구조체 

typedef struct
{
  Ifx_P *port;
  uint8 pinIndex;
  IfxPort_Mode mode;
  IfxPort_State init_state;
}Hw_Gpio_ConfigType;


## 설정 테이블
const Hw_Gpio_ConfigType Hw_Gpio_Config_Tbl[HW_GPIO_CH] =
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


## 주요 함수
void Hw_Gpio_Init(void)
{
    for(int i = 0; i < HW_GPIO_CH; i++)
    {
        IfxPort_setPinMode(Hw_Gpio_Config_Tbl[i].port, Hw_Gpio_Config_Tbl[i].pinIndex, Hw_Gpio_Config_Tbl[i].mode);
        IfxPort_setPinState(Hw_Gpio_Config_Tbl[i].port, Hw_Gpio_Config_Tbl[i].pinIndex, Hw_Gpio_Config_Tbl[i].init_state);
    }
}

uint8 Hw_Gpio_Read(uint8 ch)
{
    uint8 ret;
    ret = IfxPort_getPinState(Hw_Gpio_Config_Tbl[ch].port, Hw_Gpio_Config_Tbl[ch].pinIndex);
    return ret;
}

void Hw_Gpio_Write(uint8 ch, uint8 cmd)
{
    if(cmd == GPIO_HIGH)
        IfxPort_setPinState(Hw_Gpio_Config_Tbl[ch].port, Hw_Gpio_Config_Tbl[ch].pinIndex, IfxPort_State_high);
    else if(cmd == GPIO_LOW)
        IfxPort_setPinState(Hw_Gpio_Config_Tbl[ch].port, Hw_Gpio_Config_Tbl[ch].pinIndex, IfxPort_State_low);
}

void Hw_Gpio_Toggle(uint8 ch)
{
    IfxPort_togglePin(Hw_Gpio_Config_Tbl[ch].port, Hw_Gpio_Config_Tbl[ch].pinIndex);
}
