/**
 * @file stm32g4_system.c
 * @brief STM32G4系列系統硬體抽象層實現
 * @author Cross-MCU Framework Team
 * @date 2024
 */

#include "../include/hal.h"
#include "stm32g4_common.h"

#ifdef PLATFORM_STM32

/* ========================================================================== */
/*                             系統初始化實現                                  */
/* ========================================================================== */

hal_status_t hal_init(void)
{
    HAL_StatusTypeDef status;
    
    // 初始化HAL函式庫
    status = HAL_Init();
    if (status != HAL_OK) {
        return stm32g4_convert_status(status);
    }
    
    // 初始化系統時鐘
    hal_status_t clock_status = stm32g4_init_system_clock();
    if (clock_status != HAL_OK) {
        return clock_status;
    }
    
    // 初始化週邊時鐘
    clock_status = stm32g4_init_peripheral_clocks();
    if (clock_status != HAL_OK) {
        return clock_status;
    }
    
    // 初始化SysTick (1ms)
    status = HAL_InitTick(TICK_INT_PRIORITY);
    if (status != HAL_OK) {
        return stm32g4_convert_status(status);
    }
    
    return HAL_OK;
}

hal_status_t hal_deinit(void)
{
    HAL_StatusTypeDef status = HAL_DeInit();
    return stm32g4_convert_status(status);
}

uint32_t hal_get_system_clock(void)
{
    return HAL_RCC_GetSysClockFreq();
}

void hal_delay_ms(uint32_t ms)
{
    HAL_Delay(ms);
}

void hal_delay_us(uint32_t us)
{
    // STM32G4的微秒延時實現
    uint32_t start = DWT->CYCCNT;
    uint32_t cycles = (SystemCoreClock / 1000000U) * us;
    
    while ((DWT->CYCCNT - start) < cycles) {
        // 等待
    }
}

uint32_t hal_get_tick(void)
{
    return HAL_GetTick();
}

void hal_system_reset(void)
{
    HAL_NVIC_SystemReset();
}

/* ========================================================================== */
/*                             STM32G4特定函式實現                            */
/* ========================================================================== */

hal_status_t stm32g4_init_system_clock(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
    
    // 配置電源調節器輸出電壓以達到最大性能
    HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1_BOOST);
    
    // 初始化RCC振盪器
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
    RCC_OscInitStruct.PLL.PLLM = 4;
    RCC_OscInitStruct.PLL.PLLN = 85;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
    RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
    
    HAL_StatusTypeDef status = HAL_RCC_OscConfig(&RCC_OscInitStruct);
    if (status != HAL_OK) {
        return stm32g4_convert_status(status);
    }
    
    // 初始化CPU、AHB和APB匯流排時鐘
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                                | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
    
    status = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4);
    if (status != HAL_OK) {
        return stm32g4_convert_status(status);
    }
    
    return HAL_OK;
}

hal_status_t stm32g4_init_peripheral_clocks(void)
{
    // 使能DWT (用於微秒延時)
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
    DWT->CYCCNT = 0;
    
    // 其他週邊時鐘根據需要使能
    // GPIO時鐘在各自的初始化函式中使能
    
    return HAL_OK;
}

GPIO_TypeDef* stm32g4_get_gpio_port(char port_letter)
{
    switch (port_letter) {
        case 'A': return GPIOA;
        case 'B': return GPIOB;
        case 'C': return GPIOC;
        case 'D': return GPIOD;
        case 'E': return GPIOE;
        case 'F': return GPIOF;
        case 'G': return GPIOG;
        default: return NULL;
    }
}

hal_status_t stm32g4_enable_gpio_clock(char port_letter)
{
    switch (port_letter) {
        case 'A':
            __HAL_RCC_GPIOA_CLK_ENABLE();
            break;
        case 'B':
            __HAL_RCC_GPIOB_CLK_ENABLE();
            break;
        case 'C':
            __HAL_RCC_GPIOC_CLK_ENABLE();
            break;
        case 'D':
            __HAL_RCC_GPIOD_CLK_ENABLE();
            break;
        case 'E':
            __HAL_RCC_GPIOE_CLK_ENABLE();
            break;
        case 'F':
            __HAL_RCC_GPIOF_CLK_ENABLE();
            break;
        case 'G':
            __HAL_RCC_GPIOG_CLK_ENABLE();
            break;
        default:
            return HAL_INVALID_PARAM;
    }
    
    return HAL_OK;
}

hal_status_t stm32g4_convert_status(HAL_StatusTypeDef stm32_status)
{
    switch (stm32_status) {
        case HAL_OK:
            return HAL_OK;
        case HAL_ERROR:
            return HAL_ERROR;
        case HAL_BUSY:
            return HAL_BUSY;
        case HAL_TIMEOUT:
            return HAL_TIMEOUT;
        default:
            return HAL_ERROR;
    }
}

#endif /* PLATFORM_STM32 */
