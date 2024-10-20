#include "./BSP/ADC/adc.h"
#include "./SYSTEM/delay/delay.h"

ADC_HandleTypeDef g_adc_handle;

void adc_init(void)
{
    g_adc_handle.Instance = ADC_ADCX;
    g_adc_handle.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    g_adc_handle.Init.ScanConvMode = ADC_SCAN_DISABLE;
    g_adc_handle.Init.ContinuousConvMode = DISABLE;
    g_adc_handle.Init.NbrOfConversion = 1;
    g_adc_handle.Init.DiscontinuousConvMode = DISABLE;
    g_adc_handle.Init.NbrOfDiscConversion = 0;
    g_adc_handle.Init.ExternalTrigConv = ADC_SOFTWARE_START;
    HAL_ADC_Init(&g_adc_handle);
    
    HAL_ADCEx_Calibration_Start(&g_adc_handle);
}

void HAL_ADC_MspInit(ADC_HandleTypeDef *hadc)
{
    if (hadc->Instance == ADC_ADCX)
    {
        GPIO_InitTypeDef gpio_init_struct;
        RCC_PeriphCLKInitTypeDef adc_clk_init = {0};
        
        ADC_ADCX_CHY_CLK_ENABLE();
        ADC_ADCX_CHY_GPIO_CLK_ENABLE();
        
        adc_clk_init.PeriphClockSelection = RCC_PERIPHCLK_ADC;
        adc_clk_init.AdcClockSelection = RCC_ADCPCLK2_DIV6;
        HAL_RCCEx_PeriphCLKConfig(&adc_clk_init);
        
        gpio_init_struct.Pin = ADC_ADCX_CHY_GPIO_PIN;
        gpio_init_struct.Mode = GPIO_MODE_ANALOG;
        HAL_GPIO_Init(ADC_ADCX_CHY_GPIO_PORT,&gpio_init_struct);
    }
}

void adc_channel_set(ADC_HandleTypeDef *adc_handle,uint32_t ch,uint32_t rank,uint32_t stime)
{
    ADC_ChannelConfTypeDef adc_ch_conf;
    
    adc_ch_conf.Channel = ch;
    adc_ch_conf.Rank = rank;
    adc_ch_conf.SamplingTime = stime;
    HAL_ADC_ConfigChannel(adc_handle,&adc_ch_conf);
}

uint32_t adc_get_result(uint32_t ch)
{
    adc_channel_set(&g_adc_handle,ch,ADC_REGULAR_RANK_1,ADC_SAMPLETIME_239CYCLES_5);
    
    HAL_ADC_Start(&g_adc_handle);
    HAL_ADC_PollForConversion(&g_adc_handle,10);
    return (uint16_t)HAL_ADC_GetValue(&g_adc_handle);
}

uint32_t adc_get_result_average(uint32_t ch,uint8_t times)
{
    uint32_t temp_val = 0;
    uint8_t t;
    
    for(t = 0;t < times;t++)
    {
        temp_val += adc_get_result(ch);
        delay_ms(5);
    }
    return temp_val / times;
}

DMA_HandleTypeDef g_dma_adc_handle;
ADC_HandleTypeDef g_adc_dma_handle;
uint8_t g_adc_dma_sta = 0;

void adc_dma_init(uint32_t mar)
{
    GPIO_InitTypeDef gpio_init_struct;
    RCC_PeriphCLKInitTypeDef adc_clk_init = {0};
    ADC_ChannelConfTypeDef adc_ch_conf;
    
    ADC_ADCX_CHY_CLK_ENABLE();
    ADC_ADCX_CHY_GPIO_CLK_ENABLE();
    
    if((uint32_t)ADC_ADCX_DMACx > (uint32_t)DMA1_Channel7)
    {
        __HAL_RCC_DMA2_CLK_ENABLE();
    }
    else
    {
        __HAL_RCC_DMA1_CLK_ENABLE();
    }
    
    adc_clk_init.PeriphClockSelection = RCC_PERIPHCLK_ADC;
    adc_clk_init.AdcClockSelection = RCC_ADCPCLK2_DIV6;
    HAL_RCCEx_PeriphCLKConfig(&adc_clk_init);
    
    gpio_init_struct.Pin = ADC_ADCX_CHY_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_ANALOG;
    HAL_GPIO_Init(ADC_ADCX_CHY_GPIO_PORT,&gpio_init_struct);
    
    g_dma_adc_handle.Instance = ADC_ADCX_DMACx;
    g_dma_adc_handle.Init.Direction = DMA_PERIPH_TO_MEMORY;
    g_dma_adc_handle.Init.PeriphInc = DMA_PINC_DISABLE;
    g_dma_adc_handle.Init.MemInc = DMA_MINC_ENABLE;
    g_dma_adc_handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
    g_dma_adc_handle.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
    g_dma_adc_handle.Init.Mode = DMA_NORMAL;
    g_dma_adc_handle.Init.Priority = DMA_PRIORITY_MEDIUM;
    
    HAL_DMA_Init(&g_dma_adc_handle);
    
    __HAL_LINKDMA(&g_adc_dma_handle,DMA_Handle,g_dma_adc_handle);
    
    g_adc_dma_handle.Instance = ADC_ADCX;
    g_adc_dma_handle.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    g_adc_dma_handle.Init.ScanConvMode = ADC_SCAN_DISABLE;
    g_adc_dma_handle.Init.ContinuousConvMode = ENABLE;
    g_adc_dma_handle.Init.NbrOfConversion = 1;
    g_adc_dma_handle.Init.DiscontinuousConvMode = DISABLE;
    g_adc_dma_handle.Init.NbrOfDiscConversion = 0;
    g_adc_dma_handle.Init.ExternalTrigConv = ADC_SOFTWARE_START;
    HAL_ADC_Init(&g_adc_dma_handle);
    
    HAL_ADCEx_Calibration_Start(&g_adc_dma_handle);
    
    adc_ch_conf.Channel = ADC_ADCX_CHY;
    adc_ch_conf.Rank = ADC_REGULAR_RANK_1;
    adc_ch_conf.SamplingTime = ADC_SAMPLETIME_239CYCLES_5;
    HAL_ADC_ConfigChannel(&g_adc_dma_handle,&adc_ch_conf);
    
    HAL_NVIC_SetPriority(ADC_ADCX_DMACx_IRQn,3,3);
    HAL_NVIC_EnableIRQ(ADC_ADCX_DMACx_IRQn);
    
    HAL_DMA_Start_IT(&g_dma_adc_handle,(uint32_t)&ADC1->DR,mar,0);
    HAL_ADC_Start_DMA(&g_adc_dma_handle,&mar,0);
}

void adc_dma_enable(uint16_t cndtr)
{
    ADC_ADCX->CR2 &= ~(1 << 0);
    ADC_ADCX_DMACx->CCR &= ~(1 << 0);
    while(ADC_ADCX_DMACx->CCR & (1 << 0));
    ADC_ADCX_DMACx->CNDTR = cndtr;
    ADC_ADCX_DMACx->CCR |= 1 << 0;
    
    ADC_ADCX->CR2 |= 1<<0;
    ADC_ADCX->CR2 |= 1<<22;
}

void ADC_ADCX_DMACx_IRQHandler(void)
{
    if(ADC_ADCX_DMACx_IS_TC())
    {
        g_adc_dma_sta = 1;
        ADC_ADCX_DMACx_CLR_TC();
    }
}
