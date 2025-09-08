/**
 * @file stm32g4_common.h
 * @brief STM32G4系列MCU公共定義
 * @author Cross-MCU Framework Team
 * @date 2024
 */

#ifndef STM32G4_COMMON_H
#define STM32G4_COMMON_H

#include "../include/hal_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================== */
/*                             STM32G4平台檢查                                */
/* ========================================================================== */

#ifndef PLATFORM_STM32
#error "This file is only for STM32 platform"
#endif

/* ========================================================================== */
/*                             STM32G4特定包含檔                              */
/* ========================================================================== */

#ifdef MCU_STM32G4
    #include "stm32g4xx_hal.h"
    #include "stm32g4xx_ll_gpio.h"
    #include "stm32g4xx_ll_usart.h"
    #include "stm32g4xx_ll_spi.h"
    #include "stm32g4xx_ll_i2c.h"
    #include "stm32g4xx_ll_adc.h"
    #include "stm32g4xx_ll_rcc.h"
    #include "stm32g4xx_ll_system.h"
    #include "stm32g4xx_ll_utils.h"
#endif

/* ========================================================================== */
/*                             系統時鐘定義                                    */
/* ========================================================================== */

#define STM32G4_HSI_FREQ        16000000U   // 內部高速振盪器頻率
#define STM32G4_HSE_FREQ        8000000U    // 外部高速振盪器頻率 (可調整)
#define STM32G4_LSI_FREQ        32000U      // 內部低速振盪器頻率
#define STM32G4_LSE_FREQ        32768U      // 外部低速振盪器頻率

#define STM32G4_MAX_SYSCLK      170000000U  // 最大系統時鐘頻率
#define STM32G4_MAX_HCLK        170000000U  // 最大AHB時鐘頻率
#define STM32G4_MAX_PCLK1       170000000U  // 最大APB1時鐘頻率
#define STM32G4_MAX_PCLK2       170000000U  // 最大APB2時鐘頻率

/* ========================================================================== */
/*                             GPIO埠定義                                     */
/* ========================================================================== */

#define STM32_GPIOA             ((GPIO_TypeDef*)GPIOA_BASE)
#define STM32_GPIOB             ((GPIO_TypeDef*)GPIOB_BASE)
#define STM32_GPIOC             ((GPIO_TypeDef*)GPIOC_BASE)
#define STM32_GPIOD             ((GPIO_TypeDef*)GPIOD_BASE)
#define STM32_GPIOE             ((GPIO_TypeDef*)GPIOE_BASE)
#define STM32_GPIOF             ((GPIO_TypeDef*)GPIOF_BASE)
#define STM32_GPIOG             ((GPIO_TypeDef*)GPIOG_BASE)

/* ========================================================================== */
/*                             週邊介面定義                                    */
/* ========================================================================== */

// UART/USART定義
#define STM32_USART1            USART1
#define STM32_USART2            USART2
#define STM32_USART3            USART3
#define STM32_UART4             UART4
#define STM32_UART5             UART5

// SPI定義
#define STM32_SPI1              SPI1
#define STM32_SPI2              SPI2
#define STM32_SPI3              SPI3
#define STM32_SPI4              SPI4

// I2C定義
#define STM32_I2C1              I2C1
#define STM32_I2C2              I2C2
#define STM32_I2C3              I2C3
#define STM32_I2C4              I2C4

// ADC定義
#define STM32_ADC1              ADC1
#define STM32_ADC2              ADC2
#define STM32_ADC3              ADC3
#define STM32_ADC4              ADC4
#define STM32_ADC5              ADC5

/* ========================================================================== */
/*                             工具巨集                                        */
/* ========================================================================== */

#define STM32_GPIO_PIN(port, pin)   ((uint16_t)(1U << (pin)))
#define STM32_GPIO_PORT(port)       (GPIO##port)
#define STM32_GPIO_PORT_CLOCK(port) (RCC_AHB2ENR_GPIO##port##EN)

/* ========================================================================== */
/*                             函式聲明                                        */
/* ========================================================================== */

/**
 * @brief 初始化STM32G4系統時鐘
 * @return HAL_OK 成功，其他值表示失敗
 */
hal_status_t stm32g4_init_system_clock(void);

/**
 * @brief 初始化STM32G4週邊時鐘
 * @return HAL_OK 成功，其他值表示失敗
 */
hal_status_t stm32g4_init_peripheral_clocks(void);

/**
 * @brief 獲取GPIO埠指標
 * @param port_letter GPIO埠字母 ('A', 'B', 'C', etc.)
 * @return GPIO埠指標
 */
GPIO_TypeDef* stm32g4_get_gpio_port(char port_letter);

/**
 * @brief 使能GPIO埠時鐘
 * @param port_letter GPIO埠字母 ('A', 'B', 'C', etc.)
 * @return HAL_OK 成功，其他值表示失敗
 */
hal_status_t stm32g4_enable_gpio_clock(char port_letter);

/**
 * @brief 將HAL狀態轉換為STM32狀態
 * @param stm32_status STM32 HAL狀態
 * @return HAL狀態
 */
hal_status_t stm32g4_convert_status(HAL_StatusTypeDef stm32_status);

#ifdef __cplusplus
}
#endif

#endif /* STM32G4_COMMON_H */
