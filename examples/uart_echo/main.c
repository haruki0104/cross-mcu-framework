/**
 * @file main.c
 * @brief UART回音範例應用程式
 * @author Cross-MCU Framework Team
 * @date 2024
 */

#include "hal.h"
#include "uart_config.h"
#include <string.h>

#define BUFFER_SIZE     256
#define WELCOME_MSG     "Cross-MCU UART Echo Example\r\n"
#define PROMPT_MSG      "Enter text (press Enter to echo): "

/**
 * @brief 主函式
 */
int main(void)
{
    hal_status_t status;
    uint8_t rx_buffer[BUFFER_SIZE];
    uint8_t tx_buffer[BUFFER_SIZE];
    uint16_t rx_index = 0;
    uint8_t received_char;
    
    // 初始化HAL系統
    status = hal_init();
    if (status != HAL_OK) {
        // 初始化失敗，進入無限循環
        while (1) {
            // 錯誤處理
        }
    }
    
    // 初始化UART配置
    status = uart_config_init();
    if (status != HAL_OK) {
        // UART配置失敗
        while (1) {
            // 錯誤處理
        }
    }
    
    // 發送歡迎訊息
    hal_uart_transmit(CONSOLE_UART, (uint8_t*)WELCOME_MSG, strlen(WELCOME_MSG), 1000);
    hal_uart_transmit(CONSOLE_UART, (uint8_t*)PROMPT_MSG, strlen(PROMPT_MSG), 1000);
    
    // 主循環
    while (1) {
        // 檢查是否有資料可讀
        if (hal_uart_data_available(CONSOLE_UART)) {
            // 接收一個字元
            status = hal_uart_getchar(CONSOLE_UART, &received_char, 10);
            if (status == HAL_OK) {
                // 回音字元
                hal_uart_putchar(CONSOLE_UART, received_char);
                
                // 檢查是否為回車或換行
                if (received_char == '\r' || received_char == '\n') {
                    // 發送換行
                    hal_uart_transmit(CONSOLE_UART, (uint8_t*)"\r\n", 2, 100);
                    
                    if (rx_index > 0) {
                        // 準備回音訊息
                        strcpy((char*)tx_buffer, "Echo: ");
                        strncat((char*)tx_buffer, (char*)rx_buffer, rx_index);
                        strcat((char*)tx_buffer, "\r\n");
                        
                        // 發送回音
                        hal_uart_transmit(CONSOLE_UART, tx_buffer, strlen((char*)tx_buffer), 1000);
                        
                        // 清空接收緩衝區
                        memset(rx_buffer, 0, sizeof(rx_buffer));
                        rx_index = 0;
                    }
                    
                    // 發送新的提示
                    hal_uart_transmit(CONSOLE_UART, (uint8_t*)PROMPT_MSG, strlen(PROMPT_MSG), 1000);
                    
                } else if (received_char == '\b' || received_char == 127) {
                    // 處理退格鍵
                    if (rx_index > 0) {
                        rx_index--;
                        rx_buffer[rx_index] = '\0';
                        // 發送退格序列
                        hal_uart_transmit(CONSOLE_UART, (uint8_t*)" \b", 2, 100);
                    }
                    
                } else {
                    // 一般字元，加入緩衝區
                    if (rx_index < (BUFFER_SIZE - 1)) {
                        rx_buffer[rx_index] = received_char;
                        rx_index++;
                        rx_buffer[rx_index] = '\0';
                    }
                }
            }
        }
        
        // 小延時以避免過度占用CPU
        hal_delay_ms(1);
    }
    
    return 0;
}
