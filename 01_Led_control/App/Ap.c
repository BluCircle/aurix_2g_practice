#include "Ap.h"
#include "Hw_Gpio.h"

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

    while(1)
    {

    }

}
