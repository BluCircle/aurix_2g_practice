#ifndef HW_PWM_OUT_HW_GTM_TOM_PWM_H_
#define HW_PWM_OUT_HW_GTM_TOM_PWM_H_

#include "typeDefs.h"
#include "IfxGtm_Tom_Pwm.h"

void Hw_Gtm_Pwm_Init(void);
void Hw_Gtm_Pwm_On(uint8 ch);
void Hw_Gtm_Pwm_Off(uint8 ch);
void Hw_Gtm_Pwm_Set_Duty(uint8 ch, uint16 duty);
void Hw_Gtm_Pwm_Set_Period(uint8 ch, uint16 Period);


#endif /* HW_PWM_OUT_HW_GTM_TOM_PWM_H_ */
