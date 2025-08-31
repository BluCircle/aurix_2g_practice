#ifndef HW_GPIO_HW_GPIO_H_
#define HW_GPIO_HW_GPIO_H_

#include "typeDefs.h"


void Hw_Gpio_Init(void);
uint8 Hw_Gpio_Read(uint8 ch);
void Hw_Gpio_Write(uint8 ch, uint8 cmd);
void Hw_Gpio_Toggle(uint8 ch);


#endif /* HW_GPIO_HW_GPIO_H_ */
