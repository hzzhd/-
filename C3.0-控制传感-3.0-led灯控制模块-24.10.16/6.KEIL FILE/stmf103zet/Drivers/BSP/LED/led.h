#ifndef _LED_H
#define _LED_H
#include "./SYSTEM/sys/sys.h"

#define LED0_GPIO_PORT              GPIOB
#define LED0_GPIO_PIN               GPIO_PIN_5
#define LED0_GPIO_CLK_ENABLE()        do { __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)

#define LED1_GPIO_PORT              GPIOE
#define LED1_GPIO_PIN               GPIO_PIN_5
#define LED1_GPIO_CLK_ENABLE()        do { __HAL_RCC_GPIOE_CLK_ENABLE(); }while(0)

/*
#define LED0(x)  do{ x ? \
                    HAL_GPIO_WritePin(LED0_GPIO_PORT, LED0_GPIO_PIN,GPIO_PIN_SET): \
                    HAL_GPIO_WritePin(LED0_GPIO_PORT, LED0_GPIO_PIN,GPIO_PIN_RESET);\
                }while(0)

#define LED1(x)  do{ x ? \
                    HAL_GPIO_WritePin(LED1_GPIO_PORT, LED0_GPIO_PIN,GPIO_PIN_SET): \
                    HAL_GPIO_WritePin(LED1_GPIO_PORT, LED1_GPIO_PIN,GPIO_PIN_RESET);\
                }while(0)

#define LED0_TOGGLE()   do{ HAL_GPIO_TogglePin(LED0_GPIO_PORT,LED0_GPIO_PIN); }while(0)
#define LED1_TOGGLE()   do{ HAL_GPIO_TogglePin(LED1_GPIO_PORT,LED1_GPIO_PIN); }while(0)
*/

void led_init(void);
void led0(int x);
void led1(int x);
void led0_toggle(void);
void led1_toggle(void);

#endif
