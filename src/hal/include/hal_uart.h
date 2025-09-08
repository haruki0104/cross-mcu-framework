/**
 * @file hal_uart.h
 * @brief UART硬體抽象層介面
 * @author Cross-MCU Framework Team
 * @date 2024
 */

#ifndef HAL_UART_H
#define HAL_UART_H

#include "hal_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================== */
/*                             UART介面函式                                   */
/* ========================================================================== */

/**
 * @brief 初始化UART
 * @param uart_id UART識別碼
 * @param config UART配置結構體指標
 * @return HAL_OK 成功，其他值表示失敗
 */
hal_status_t hal_uart_init(hal_uart_id_t uart_id, const hal_uart_config_t* config);

/**
 * @brief 反初始化UART
 * @param uart_id UART識別碼
 * @return HAL_OK 成功，其他值表示失敗
 */
hal_status_t hal_uart_deinit(hal_uart_id_t uart_id);

/**
 * @brief 發送資料
 * @param uart_id UART識別碼
 * @param data 要發送的資料緩衝區
 * @param size 資料長度
 * @param timeout 超時時間(ms)
 * @return HAL_OK 成功，其他值表示失敗
 */
hal_status_t hal_uart_transmit(hal_uart_id_t uart_id, const uint8_t* data, 
                               uint16_t size, uint32_t timeout);

/**
 * @brief 接收資料
 * @param uart_id UART識別碼
 * @param data 接收資料緩衝區
 * @param size 要接收的資料長度
 * @param timeout 超時時間(ms)
 * @return HAL_OK 成功，其他值表示失敗
 */
hal_status_t hal_uart_receive(hal_uart_id_t uart_id, uint8_t* data, 
                              uint16_t size, uint32_t timeout);

/**
 * @brief 發送單個字元
 * @param uart_id UART識別碼
 * @param ch 要發送的字元
 * @return HAL_OK 成功，其他值表示失敗
 */
hal_status_t hal_uart_putchar(hal_uart_id_t uart_id, uint8_t ch);

/**
 * @brief 接收單個字元
 * @param uart_id UART識別碼
 * @param ch 接收字元的緩衝區
 * @param timeout 超時時間(ms)
 * @return HAL_OK 成功，其他值表示失敗
 */
hal_status_t hal_uart_getchar(hal_uart_id_t uart_id, uint8_t* ch, uint32_t timeout);

/**
 * @brief 檢查UART是否忙碌
 * @param uart_id UART識別碼
 * @return true 忙碌，false 空閒
 */
bool hal_uart_is_busy(hal_uart_id_t uart_id);

/**
 * @brief 檢查接收緩衝區是否有資料
 * @param uart_id UART識別碼
 * @return true 有資料，false 無資料
 */
bool hal_uart_data_available(hal_uart_id_t uart_id);

/**
 * @brief 清空接收緩衝區
 * @param uart_id UART識別碼
 * @return HAL_OK 成功，其他值表示失敗
 */
hal_status_t hal_uart_flush_rx(hal_uart_id_t uart_id);

/**
 * @brief 清空發送緩衝區
 * @param uart_id UART識別碼
 * @return HAL_OK 成功，其他值表示失敗
 */
hal_status_t hal_uart_flush_tx(hal_uart_id_t uart_id);

#ifdef __cplusplus
}
#endif

#endif /* HAL_UART_H */
