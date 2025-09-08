/**
 * @file gpio_config.c
 * @brief GPIO配置實現
 * @author Cross-MCU Framework Team
 * @date 2024
 */

#include "gpio_config.h"

hal_status_t gpio_config_init(void)
{
    hal_gpio_config_t led_config;
    hal_gpio_config_t button_config;
    
    // 配置LED引腳
    led_config.pin = LED_PIN;
    led_config.mode = HAL_GPIO_MODE_OUTPUT;
    led_config.pull = HAL_GPIO_NOPULL;
    led_config.initial_state = HAL_GPIO_LOW;
    
    hal_status_t status = hal_gpio_init(&led_config);
    if (status != HAL_OK) {
        return status;
    }
    
    // 配置按鈕引腳 (可選)
    button_config.pin = BUTTON_PIN;
    button_config.mode = HAL_GPIO_MODE_INPUT;
    button_config.pull = HAL_GPIO_PULLUP;  // 使用內部上拉電阻
    button_config.initial_state = HAL_GPIO_LOW;  // 輸入模式下此參數無效
    
    status = hal_gpio_init(&button_config);
    if (status != HAL_OK) {
        return status;
    }
    
    return HAL_OK;
}

hal_gpio_state_t gpio_read_button(void)
{
    return hal_gpio_read(BUTTON_PIN);
}

void gpio_set_led(hal_gpio_state_t state)
{
    hal_gpio_write(LED_PIN, state);
}

void gpio_toggle_led(void)
{
    hal_gpio_toggle(LED_PIN);
}
