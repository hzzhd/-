#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/LED/led.h"
#include "./BSP/TIMER/gtim.h"
#include "./BSP/ADC/adc.h"

extern TIM_HandleTypeDef g_timx_pwm_chy_handle;

#define ADC_DMA_BUF_SIZE        100         /* ADC DMA采集 BUF大小 */
uint16_t g_adc_dma_buf[ADC_DMA_BUF_SIZE];   /* ADC DMA BUF */

extern uint8_t g_adc_dma_sta;               /* DMA传输状态标志, 0,未完成; 1, 已完成 */

int main(void)
{
    uint16_t ledrpwmval = 0;
    uint8_t dir = 1;
    
    uint16_t i;
    uint16_t adcx;
    uint32_t sum;
    float temp;
    
    HAL_Init();
    sys_stm32_clock_init(RCC_PLL_MUL9);
    delay_init(72);
    usart_init(115200);
    led_init();
    gtim_timx_pwm_chy_init(65534-1,2-1);
    
    adc_dma_init((uint32_t)&g_adc_dma_buf); /* 初始化ADC DMA采集 */
    adc_dma_enable(ADC_DMA_BUF_SIZE);   /* 启动ADC DMA采集 */
    
    led1(0);
    
    while(1)
    {
        if (g_adc_dma_sta == 1)
        {
            /* 计算DMA 采集到的ADC数据的平均值 */
            sum = 0;

            for (i = 0; i < ADC_DMA_BUF_SIZE; i++)   /* 累加 */
            {
                sum += g_adc_dma_buf[i];
            }

            adcx = sum / ADC_DMA_BUF_SIZE;           /* 取平均值 */
            g_adc_dma_sta = 0;                                  /* 清除DMA采集完成状态标志 */
            adc_dma_enable(ADC_DMA_BUF_SIZE);                   /* 启动下一次ADC DMA采集 */
        }
        delay_ms(10);
        
        adcx /= 4;
        if (dir) ledrpwmval += (adcx + 1);
        else ledrpwmval -= (adcx + 1);
        
        if (ledrpwmval > 60000) dir = 0;
        if (ledrpwmval == 0) dir = 1;
        __HAL_TIM_SET_COMPARE(&g_timx_pwm_chy_handle,GTIM_TIMX_PWM_CHY,ledrpwmval);
    }
}
