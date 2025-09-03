#include "Ap.h"


uint8 led_state[HW_GPIO_CH];
uint16 adc_res[HW_ADC_NUM];

void apInit(void)
{
    hwInit();

    Hw_Gpio_Write(_GPIO_CH1, GPIO_HIGH);

    for(uint8 i = 0; i < HW_GPIO_CH;i++)
    {
        led_state[i] = Hw_Gpio_Read(i);
    }

    Hw_Gtm_Pwm_Set_Duty(_PWM_CH1, 20); //1V , 334
    Hw_Gtm_Pwm_Set_Duty(_PWM_CH2, 40); //2V , 668
    Hw_Gtm_Pwm_Set_Duty(_PWM_CH3, 60); //3V , 99C
    Hw_Gtm_Pwm_Set_Duty(_PWM_CH4, 80); //4V , CD0
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
    for(uint8 idx=0;idx<HW_ADC_NUM;idx++)
    {
        adc_res[idx] = Hw_Adc_Single_Read(idx);
    }

    uint8 txData[64];
    txData[0] = 0x1;
    txData[1] = (adc_res[_ADC_CH1] >> 8) & 0xFF;
    txData[2] =  adc_res[_ADC_CH1]       & 0xFF;
    txData[3] = 0x2;
    txData[4] = (adc_res[_ADC_CH2] >> 8) & 0xFF;
    txData[5] =  adc_res[_ADC_CH2]       & 0xFF;
    txData[6] = 0x3;
    txData[7] = (adc_res[_ADC_CH3] >> 8) & 0xFF;
    txData[8] =  adc_res[_ADC_CH3]       & 0xFF;
    txData[9] = 0x4;
    txData[10] = (adc_res[_ADC_CH4] >> 8) & 0xFF;
    txData[11] =  adc_res[_ADC_CH4]       & 0xFF;

    Hw_Can_Transmit_data(&txData[0]);
    Hw_Gpio_Toggle(_GPIO_CH3);
}

void apTimer_500ms(void)
{
    Hw_Gpio_Toggle(_GPIO_CH2);
}
