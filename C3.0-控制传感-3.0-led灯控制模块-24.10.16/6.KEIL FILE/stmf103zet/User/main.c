#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/LED/led.h"
#include "./BSP/TIMER/gtim.h"
#include "./BSP/ADC/adc.h"

extern TIM_HandleTypeDef g_timx_pwm_chy_handle;

#define ADC_DMA_BUF_SIZE        100         /* ADC DMA�ɼ� BUF��С */
uint16_t g_adc_dma_buf[ADC_DMA_BUF_SIZE];   /* ADC DMA BUF */

extern uint8_t g_adc_dma_sta;               /* DMA����״̬��־, 0,δ���; 1, ����� */

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
    
    adc_dma_init((uint32_t)&g_adc_dma_buf); /* ��ʼ��ADC DMA�ɼ� */
    adc_dma_enable(ADC_DMA_BUF_SIZE);   /* ����ADC DMA�ɼ� */
    
    led1(0);
    
    while(1)
    {
        if (g_adc_dma_sta == 1)
        {
            /* ����DMA �ɼ�����ADC���ݵ�ƽ��ֵ */
            sum = 0;

            for (i = 0; i < ADC_DMA_BUF_SIZE; i++)   /* �ۼ� */
            {
                sum += g_adc_dma_buf[i];
            }

            adcx = sum / ADC_DMA_BUF_SIZE;           /* ȡƽ��ֵ */
            g_adc_dma_sta = 0;                                  /* ���DMA�ɼ����״̬��־ */
            adc_dma_enable(ADC_DMA_BUF_SIZE);                   /* ������һ��ADC DMA�ɼ� */
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
