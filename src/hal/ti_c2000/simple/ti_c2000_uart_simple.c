/**
 * @file ti_c2000_uart_simple.c
 * @brief TI C2000系列UART硬體抽象層簡化實現
 * @author Cross-MCU Framework Team
 * @date 2024
 */

#include "../include/hal_uart.h"
#include "ti_c2000_common.h"

#ifdef PLATFORM_TI_C2000

/* ========================================================================== */
/*                             UART介面實現                                   */
/* ========================================================================== */

hal_status_t hal_uart_init(hal_uart_id_t uart_id, const hal_uart_config_t* config)
{
    if (config == NULL) {
        return HAL_INVALID_PARAM;
    }
    
    // 簡化的UART初始化
    // 在實際實現中，這裡會配置UART寄存器
    
    return HAL_OK;
}

hal_status_t hal_uart_deinit(hal_uart_id_t uart_id)
{
    // 簡化的UART反初始化
    return HAL_OK;
}

hal_status_t hal_uart_transmit(hal_uart_id_t uart_id, const uint8_t* data, 
                               uint16_t size, uint32_t timeout)
{
    if (data == NULL || size == 0) {
        return HAL_INVALID_PARAM;
    }
    
    // 簡化的UART發送實現
    // 在實際實現中，這裡會操作UART寄存器發送資料
    uint16_t i;
    for (i = 0; i < size; i++) {
        // 模擬發送延時
        hal_delay_us(100);
    }
    
    return HAL_OK;
}

hal_status_t hal_uart_receive(hal_uart_id_t uart_id, uint8_t* data, 
                              uint16_t size, uint32_t timeout)
{
    if (data == NULL || size == 0) {
        return HAL_INVALID_PARAM;
    }
    
    // 簡化的UART接收實現
    // 在實際實現中，這裡會從UART寄存器讀取資料
    uint16_t i;
    for (i = 0; i < size; i++) {
        data[i] = 0; // 模擬接收到的資料
        hal_delay_us(100);
    }
    
    return HAL_OK;
}

hal_status_t hal_uart_putchar(hal_uart_id_t uart_id, uint8_t ch)
{
    return hal_uart_transmit(uart_id, &ch, 1, 1000);
}

hal_status_t hal_uart_getchar(hal_uart_id_t uart_id, uint8_t* ch, uint32_t timeout)
{
    if (ch == NULL) {
        return HAL_INVALID_PARAM;
    }
    
    // 簡化的字元接收實現
    *ch = 0;
    hal_delay_us(100);
    
    return HAL_OK;
}

bool hal_uart_is_busy(hal_uart_id_t uart_id)
{
    // 簡化實現，假設UART總是不忙
    return false;
}

bool hal_uart_data_available(hal_uart_id_t uart_id)
{
    // 簡化實現，假設總是有資料可用
    return true;
}

hal_status_t hal_uart_flush_rx(hal_uart_id_t uart_id)
{
    // 簡化的接收緩衝區清空實現
    return HAL_OK;
}

hal_status_t hal_uart_flush_tx(hal_uart_id_t uart_id)
{
    // 簡化的發送緩衝區清空實現
    return HAL_OK;
}

#endif /* PLATFORM_TI_C2000 */

