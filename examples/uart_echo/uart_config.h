/**
 * @file uart_config.h
 * @brief UART配置標頭檔
 * @author Cross-MCU Framework Team
 * @date 2024
 */

#ifndef UART_CONFIG_H
#define UART_CONFIG_H

#include "hal.h"
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================== */
/*                             平台特定UART定義                               */
/* ========================================================================== */

#ifdef PLATFORM_TI_C2000
    // TI C2000平台的UART定義
    #define CONSOLE_UART    0   // UART A
    
#elif defined(PLATFORM_STM32)
    // STM32平台的UART定義
    #define CONSOLE_UART    ((void*)USART2)  // USART2作為控制台
    
#else
    // 預設UART定義
    #define CONSOLE_UART    0
#endif

/* ========================================================================== */
/*                             函式聲明                                        */
/* ========================================================================== */

/**
 * @brief 初始化UART配置
 * @return HAL_OK 成功，其他值表示失敗
 */
hal_status_t uart_config_init(void);

/**
 * @brief 發送字串
 * @param str 要發送的字串
 * @return HAL_OK 成功，其他值表示失敗
 */
hal_status_t uart_send_string(const char* str);

/**
 * @brief 發送字串並添加換行
 * @param str 要發送的字串
 * @return HAL_OK 成功，其他值表示失敗
 */
hal_status_t uart_send_line(const char* str);

/**
 * @brief 接收一行資料
 * @param buffer 接收緩衝區
 * @param buffer_size 緩衝區大小
 * @param timeout 超時時間(ms)，0表示無超時
 * @return HAL_OK 成功，其他值表示失敗
 */
hal_status_t uart_receive_line(char* buffer, uint16_t buffer_size, uint32_t timeout);

/**
 * @brief 檢查是否有資料可讀
 * @return true 有資料，false 無資料
 */
bool uart_data_ready(void);

#ifdef __cplusplus
}
#endif

#endif /* UART_CONFIG_H */
