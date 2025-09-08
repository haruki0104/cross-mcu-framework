/**
 * @file main.c
 * @brief GPIO LED閃爍範例應用程式
 * @author Cross-MCU Framework Team
 * @date 2024
 */

#include "hal.h"
#include "gpio_config.h"

/**
 * @brief 主函式
 */
int main(void)
{
    hal_status_t status;
    
    // 初始化HAL系統
    status = hal_init();
    if (status != HAL_OK) {
        // 初始化失敗，進入無限循環
        while (1) {
            // 錯誤處理
        }
    }
    
    // 初始化GPIO配置
    status = gpio_config_init();
    if (status != HAL_OK) {
        // GPIO配置失敗
        while (1) {
            // 錯誤處理
        }
    }
    
    // 主循環
    while (1) {
        // 點亮LED
        hal_gpio_write(LED_PIN, HAL_GPIO_HIGH);
        hal_delay_ms(500);  // 延時500ms
        
        // 熄滅LED
        hal_gpio_write(LED_PIN, HAL_GPIO_LOW);
        hal_delay_ms(500);  // 延時500ms
        
        // 也可以使用切換函式
        // hal_gpio_toggle(LED_PIN);
        // hal_delay_ms(1000);
    }
    
    return 0;
}
