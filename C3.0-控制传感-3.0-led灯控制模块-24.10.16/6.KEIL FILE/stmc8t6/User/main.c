#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/delay/delay.h"
#include "./SYSTEM/usart/usart.h"
#include "./BSP/LED/led.h"

int main(void)
{
    HAL_Init();
    sys_stm32_clock_init(RCC_PLL_MUL9);
    delay_init(72);
    led_init();
    
    delay_ms(1000);
    led0(0);
    led1(0);
    delay_ms(1000);
    
    while(1)
    {
        
        led0(0);
        led1(1);
        delay_ms(500);
        led0(1);
        led1(0);
        delay_ms(500);
        
//        led0_toggle();
//        led1_toggle();
//        delay_ms(500);

    }
}
