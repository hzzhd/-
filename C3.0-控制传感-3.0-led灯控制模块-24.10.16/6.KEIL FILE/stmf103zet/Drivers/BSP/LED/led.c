#include "./BSP/LED/led.h"

void led_init(void)
{
    GPIO_InitTypeDef gpio_init_struct;
    LED0_GPIO_CLK_ENABLE();
    LED1_GPIO_CLK_ENABLE();
    
    gpio_init_struct.Pin = LED0_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;
    gpio_init_struct.Pull = GPIO_PULLUP;
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(LED0_GPIO_PORT,&gpio_init_struct);
    
    gpio_init_struct.Pin = LED1_GPIO_PIN;
    HAL_GPIO_Init(LED1_GPIO_PORT,&gpio_init_struct);
    
//    LED0(1);
//    LED1(1);
}

void led0(int x)
{
    if (x) 
        HAL_GPIO_WritePin(LED0_GPIO_PORT, LED0_GPIO_PIN,GPIO_PIN_RESET);
    else 
        HAL_GPIO_WritePin(LED0_GPIO_PORT, LED0_GPIO_PIN,GPIO_PIN_SET);
}

void led1(int x)
{
    if (x) 
        HAL_GPIO_WritePin(LED1_GPIO_PORT, LED1_GPIO_PIN,GPIO_PIN_RESET);
    else 
        HAL_GPIO_WritePin(LED1_GPIO_PORT, LED1_GPIO_PIN,GPIO_PIN_SET);
}

void led0_toggle(void)
{
    HAL_GPIO_TogglePin(LED0_GPIO_PORT,LED0_GPIO_PIN);
}

void led1_toggle(void)
{
    HAL_GPIO_TogglePin(LED1_GPIO_PORT,LED1_GPIO_PIN);
}
