#include "Ap.h"


uint8 led_state[HW_GPIO_CH];


void apInit(void)
{
    hwInit();

    Hw_Gpio_Write(_GPIO_CH1, GPIO_HIGH);

    for(uint8 i = 0; i < HW_GPIO_CH;i++)
    {
        led_state[i] = Hw_Gpio_Read(i);
    }

}


void apMain(void)
{
    uint64 pre_time = millis();

    while(1)
    {
        if( millis()- pre_time >= 500)
        {
            Hw_Gpio_Toggle(_GPIO_CH1);
            pre_time = millis();
        }
    }
}


void apTimer_100ms(void)
{
    uint8 txData[64];
    txData[0] = 0x1;
    txData[1] = 0x2;
    txData[2] = 0x3;
    txData[3] = 0x4;

    Hw_Can_Transmit_data(&txData[0]);
    Hw_Gpio_Toggle(_GPIO_CH3);
}

void apTimer_500ms(void)
{
    Hw_Gpio_Toggle(_GPIO_CH2);
}
