#include "./BSP/TIMER/gtim.h"
#include "./BSP/LED/led.h"

TIM_HandleTypeDef g_timx_handle;

void gtim_timx_int_init(uint16_t arr,uint16_t psc)
{
    GTIM_TIMX_INT_CLK_ENABLE();
    
    g_timx_handle.Instance = GTIM_TIMX_INT;
    g_timx_handle.Init.Prescaler = psc;
    g_timx_handle.Init.CounterMode = TIM_COUNTERMODE_UP;
    g_timx_handle.Init.Period = arr;
    HAL_TIM_Base_Init(&g_timx_handle);
    
    HAL_NVIC_SetPriority(GTIM_TIMX_INT_IRQn,1,3);
    HAL_NVIC_EnableIRQ(GTIM_TIMX_INT_IRQn);
    HAL_TIM_Base_Start_IT(&g_timx_handle);
}

void GTIM_TIMX_INT_IRQHandler(void)
{
    if (__HAL_TIM_GET_FLAG(&g_timx_handle,TIM_FLAG_UPDATE) != RESET)
    {
        led1_toggle();
        __HAL_TIM_CLEAR_IT(&g_timx_handle,TIM_IT_UPDATE);
    }
}

TIM_HandleTypeDef g_timx_pwm_chy_handle;

void gtim_timx_pwm_chy_init(uint16_t arr,uint16_t psc)
{
    TIM_OC_InitTypeDef timx_oc_pwm_chy;
    
    g_timx_pwm_chy_handle.Instance = GTIM_TIMX_PWM;
    g_timx_pwm_chy_handle.Init.Prescaler = psc;
    g_timx_pwm_chy_handle.Init.CounterMode = TIM_COUNTERMODE_UP;
    g_timx_pwm_chy_handle.Init.Period = arr;
    HAL_TIM_PWM_Init(&g_timx_pwm_chy_handle);
    
    timx_oc_pwm_chy.OCMode = TIM_OCMODE_PWM1;
    timx_oc_pwm_chy.Pulse = arr/2;
    timx_oc_pwm_chy.OCPolarity = TIM_OCPOLARITY_LOW;
    HAL_TIM_PWM_ConfigChannel(&g_timx_pwm_chy_handle,&timx_oc_pwm_chy,GTIM_TIMX_PWM_CHY);
    HAL_TIM_PWM_Start(&g_timx_pwm_chy_handle,GTIM_TIMX_PWM_CHY);
}

void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == GTIM_TIMX_PWM)
    {
        GPIO_InitTypeDef gpio_init_struct;
        GTIM_TIMX_PWM_CHY_GPIO_CLK_ENABLE();
        GTIM_TIMX_PWM_CHY_CLK_ENABLE();
        
        gpio_init_struct.Pin = GTIM_TIMX_PWM_CHY_GPIO_PIN;
        gpio_init_struct.Mode = GPIO_MODE_AF_PP;
        gpio_init_struct.Pull = GPIO_PULLUP;
        gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
        HAL_GPIO_Init(GTIM_TIMX_PWM_CHY_GPIO_PORT,&gpio_init_struct);
        GTIM_TIMX_PWM_CHY_GPIO_REMAP();
    }
}
