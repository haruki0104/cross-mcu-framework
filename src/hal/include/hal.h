/**
 * @file hal.h
 * @brief 硬體抽象層主要標頭檔
 * @author Cross-MCU Framework Team
 * @date 2024
 */

#ifndef HAL_H
#define HAL_H

// 包含所有HAL模組
#include "hal_common.h"
#include "hal_gpio.h"
#include "hal_uart.h"
#include "hal_spi.h"
#include "hal_i2c.h"
#include "hal_adc.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================== */
/*                             系統初始化函式                                  */
/* ========================================================================== */

/**
 * @brief 初始化HAL系統
 * @return HAL_OK 成功，其他值表示失敗
 */
hal_status_t hal_init(void);

/**
 * @brief 反初始化HAL系統
 * @return HAL_OK 成功，其他值表示失敗
 */
hal_status_t hal_deinit(void);

/**
 * @brief 獲取系統時鐘頻率
 * @return 系統時鐘頻率(Hz)
 */
uint32_t hal_get_system_clock(void);

/**
 * @brief 延時函式(毫秒)
 * @param ms 延時時間(毫秒)
 */
void hal_delay_ms(uint32_t ms);

/**
 * @brief 延時函式(微秒)
 * @param us 延時時間(微秒)
 */
void hal_delay_us(uint32_t us);

/**
 * @brief 獲取系統tick計數
 * @return 系統tick計數值
 */
uint32_t hal_get_tick(void);

/**
 * @brief 系統復位
 */
void hal_system_reset(void);

#ifdef __cplusplus
}
#endif

#endif /* HAL_H */
