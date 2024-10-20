#ifndef __GTIM_H
#define __GTIM_H

#include "./SYSTEM/sys/sys.h"

#define GTIM_TIMX_INT                TIM3
#define GTIM_TIMX_INT_IRQn           TIM3_IRQn
#define GTIM_TIMX_INT_IRQHandler     TIM3_IRQHandler
#define GTIM_TIMX_INT_CLK_ENABLE()   __HAL_RCC_TIM3_CLK_ENABLE()

#define GTIM_TIMX_PWM_CHY_GPIO_PORT         GPIOB
#define GTIM_TIMX_PWM_CHY_GPIO_PIN          GPIO_PIN_5
#define GTIM_TIMX_PWM_CHY_GPIO_CLK_ENABLE() __HAL_RCC_GPIOB_CLK_ENABLE()
#define GTIM_TIMX_PWM_CHY_GPIO_REMAP()      do{ __HAL_RCC_AFIO_CLK_ENABLE(); __HAL_AFIO_REMAP_TIM3_PARTIAL();}while(0)

#define GTIM_TIMX_PWM                       TIM3
#define GTIM_TIMX_PWM_CHY                   TIM_CHANNEL_2
#define GTIM_TIMX_PWM_CHY_CCRX              TIM3->CCR2
#define GTIM_TIMX_PWM_CHY_CLK_ENABLE()      __HAL_RCC_TIM3_CLK_ENABLE()

void gtim_timx_int_init(uint16_t arr,uint16_t psc);
void gtim_timx_pwm_chy_init(uint16_t arr,uint16_t psc);

#endif
