#ifndef APP_TYPEDEFS_H_
#define APP_TYPEDEFS_H_


#include "Ifx_Types.h"
#include "IfxPort_PinMap.h"


/* GPIO Configuration*/
#define HW_GPIO_CH    8
#define   _GPIO_CH1   0
#define   _GPIO_CH2   1
#define   _GPIO_CH3   2
#define   _GPIO_CH4   3
#define   _GPIO_CH5   4
#define   _GPIO_CH6   5
#define   _GPIO_CH7   6
#define   _GPIO_CH8   7

#define   GPIO_HIGH   1
#define   GPIO_LOW    0


/* GPT12 Timer Configuration */
#define RELOAD_T3_100MS      9766u
#define RELOAD_T4_500MS      24414u


/* CANFD Configuration */
#define MAXIMUM_CAN_FD_DATA_PAYLOAD        64
#define CANFD_BAUDRATE                     500000
#define CANFD_FASTBAUDRATE                 2000000
#define CANFD_RXBUFF_NUM                   3
#define CANFD_RXFIFO0_NUM                  3
#define CANFD_FILTER_NUM                   CANFD_RXBUFF_NUM+CANFD_RXFIFO0_NUM


/* Interrupt Priorities */
#define ISR_PRIORITY_GPT12_TIMER_100MS     11
#define ISR_PRIORITY_GPT12_TIMER_500MS     12

#define ISR_PRIORITY_CANFD_RXBUFF          21
#define ISR_PRIORITY_CANFD_RXFIFO0         22
#define ISR_PRIORITY_CANFD_TXBUFF          30





#endif /* APP_TYPEDEFS_H_ */
