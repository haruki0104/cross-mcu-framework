/**
 * @file uart_config.c
 * @brief UART配置實現
 * @author Cross-MCU Framework Team
 * @date 2024
 */

#include "uart_config.h"
#include <stddef.h>

hal_status_t uart_config_init(void)
{
    hal_uart_config_t uart_config;
    
    // 配置UART參數
    uart_config.baudrate = HAL_UART_BAUDRATE_115200;
    uart_config.databits = HAL_UART_DATABITS_8;
    uart_config.stopbits = HAL_UART_STOPBITS_1;
    uart_config.parity = HAL_UART_PARITY_NONE;
    
    // 初始化UART
    hal_status_t status = hal_uart_init(CONSOLE_UART, &uart_config);
    if (status != HAL_OK) {
        return status;
    }
    
    return HAL_OK;
}

hal_status_t uart_send_string(const char* str)
{
    if (str == NULL) {
        return HAL_INVALID_PARAM;
    }
    
    uint16_t length = 0;
    
    // 計算字串長度
    while (str[length] != '\0') {
        length++;
    }
    
    return hal_uart_transmit(CONSOLE_UART, (const uint8_t*)str, length, 1000);
}

hal_status_t uart_send_line(const char* str)
{
    hal_status_t status = uart_send_string(str);
    if (status != HAL_OK) {
        return status;
    }
    
    return hal_uart_transmit(CONSOLE_UART, (const uint8_t*)"\r\n", 2, 100);
}

hal_status_t uart_receive_line(char* buffer, uint16_t buffer_size, uint32_t timeout)
{
    if (buffer == NULL || buffer_size == 0) {
        return HAL_INVALID_PARAM;
    }
    
    uint16_t index = 0;
    uint8_t received_char;
    uint32_t start_tick = hal_get_tick();
    
    while (index < (buffer_size - 1)) {
        // 檢查超時
        if (timeout != 0 && (hal_get_tick() - start_tick) > timeout) {
            return HAL_TIMEOUT;
        }
        
        // 嘗試接收字元
        hal_status_t status = hal_uart_getchar(CONSOLE_UART, &received_char, 10);
        if (status == HAL_OK) {
            // 檢查是否為行結束符
            if (received_char == '\r' || received_char == '\n') {
                buffer[index] = '\0';
                return HAL_OK;
            }
            
            // 檢查是否為退格鍵
            if (received_char == '\b' || received_char == 127) {
                if (index > 0) {
                    index--;
                }
                continue;
            }
            
            // 一般字元
            buffer[index] = received_char;
            index++;
        }
        
        // 小延時
        hal_delay_ms(1);
    }
    
    // 緩衝區滿
    buffer[buffer_size - 1] = '\0';
    return HAL_OK;
}

bool uart_data_ready(void)
{
    return hal_uart_data_available(CONSOLE_UART);
}
