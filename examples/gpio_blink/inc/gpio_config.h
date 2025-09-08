/**
 * @file gpio_config.h
 * @brief GPIO配置標頭檔
 * @author Cross-MCU Framework Team
 * @date 2024
 */

#ifndef GPIO_CONFIG_H
#define GPIO_CONFIG_H

#include "hal.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================== */
/*                             平台特定GPIO引腳定義                           */
/* ========================================================================== */

#ifdef PLATFORM_TI_C2000
    // TI C2000平台的GPIO引腳定義
    #define LED_PIN         31      // GPIO31 作為LED輸出
    #define LED4_PIN        20      // GPIO20 作為LED4輸出
    #define LED5_PIN        21      // GPIO21 作為LED5輸出
    #define BUTTON_PIN      34      // GPIO34 作為按鈕輸入
    
#elif defined(PLATFORM_STM32)
    // STM32平台的GPIO引腳定義 (假設使用GPIOA的引腳)
    // 引腳編碼: 高8位為埠號，低8位為引腳號
    #define LED_PIN         ((0 << 8) | 5)    // GPIOA Pin 5 (PA5)
    #define BUTTON_PIN      ((2 << 8) | 13)   // GPIOC Pin 13 (PC13)
    
#else
    // 預設引腳定義
    #define LED_PIN         0
    #define BUTTON_PIN      1
#endif

/* ========================================================================== */
/*                             函式聲明                                        */
/* ========================================================================== */

/**
 * @brief 初始化GPIO配置
 * @return HAL_OK 成功，其他值表示失敗
 */
hal_status_t gpio_config_init(void);

/**
 * @brief 讀取按鈕狀態
 * @return 按鈕狀態 (HAL_GPIO_HIGH/HAL_GPIO_LOW)
 */
hal_gpio_state_t gpio_read_button(void);

/**
 * @brief 設置LED狀態
 * @param state LED狀態 (HAL_GPIO_HIGH/HAL_GPIO_LOW)
 */
void gpio_set_led(hal_gpio_state_t state);

/**
 * @brief 切換LED狀態
 */
void gpio_toggle_led(void);

#ifdef __cplusplus
}
#endif

#endif /* GPIO_CONFIG_H */
