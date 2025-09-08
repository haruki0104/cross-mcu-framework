/**
 * @file hal_gpio.h
 * @brief GPIO硬體抽象層介面
 * @author Cross-MCU Framework Team
 * @date 2024
 */

#ifndef HAL_GPIO_H
#define HAL_GPIO_H

#include "hal_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================== */
/*                             GPIO介面函式                                   */
/* ========================================================================== */

/**
 * @brief 初始化GPIO引腳
 * @param config GPIO配置結構體指標
 * @return HAL_OK 成功，其他值表示失敗
 */
hal_status_t hal_gpio_init(const hal_gpio_config_t* config);

/**
 * @brief 反初始化GPIO引腳
 * @param pin GPIO引腳識別碼
 * @return HAL_OK 成功，其他值表示失敗
 */
hal_status_t hal_gpio_deinit(hal_gpio_pin_t pin);

/**
 * @brief 設置GPIO引腳輸出狀態
 * @param pin GPIO引腳識別碼
 * @param state 輸出狀態 (HIGH/LOW)
 * @return HAL_OK 成功，其他值表示失敗
 */
hal_status_t hal_gpio_write(hal_gpio_pin_t pin, hal_gpio_state_t state);

/**
 * @brief 讀取GPIO引腳狀態
 * @param pin GPIO引腳識別碼
 * @return 引腳狀態 (HIGH/LOW)
 */
hal_gpio_state_t hal_gpio_read(hal_gpio_pin_t pin);

/**
 * @brief 切換GPIO引腳輸出狀態
 * @param pin GPIO引腳識別碼
 * @return HAL_OK 成功，其他值表示失敗
 */
hal_status_t hal_gpio_toggle(hal_gpio_pin_t pin);

/**
 * @brief 配置GPIO引腳模式
 * @param pin GPIO引腳識別碼
 * @param mode GPIO模式
 * @return HAL_OK 成功，其他值表示失敗
 */
hal_status_t hal_gpio_set_mode(hal_gpio_pin_t pin, hal_gpio_mode_t mode);

/**
 * @brief 配置GPIO引腳上拉/下拉
 * @param pin GPIO引腳識別碼
 * @param pull 上拉/下拉配置
 * @return HAL_OK 成功，其他值表示失敗
 */
hal_status_t hal_gpio_set_pull(hal_gpio_pin_t pin, hal_gpio_pull_t pull);

#ifdef __cplusplus
}
#endif

#endif /* HAL_GPIO_H */
