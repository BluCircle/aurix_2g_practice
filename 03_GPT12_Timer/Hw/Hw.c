#include "Hw.h"



void hwInit(void)
{
    Hw_Gpio_Init();
    Hw_Gpt12_Init();
}
