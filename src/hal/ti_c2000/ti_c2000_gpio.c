/**
 * @file ti_c2000_gpio.c
 * @brief TI C2000系列GPIO硬體抽象層實現
 * @author Cross-MCU Framework Team
 * @date 2024
 */

#include "../include/hal_gpio.h"
#include "ti_c2000_common.h"

#ifdef PLATFORM_TI_C2000

/* ========================================================================== */
/*                             GPIO介面實現                                   */
/* ========================================================================== */

hal_status_t hal_gpio_init(const hal_gpio_config_t* config)
{
    if (config == NULL) {
        return HAL_INVALID_PARAM;
    }
    
    hal_gpio_pin_t pin = config->pin;
    
    // 檢查引腳範圍 (F28P55x/F28P65x最多支援到GPIO168)
    if (pin > 168) {
        return HAL_INVALID_PARAM;
    }
    
    // 使能全域中斷保護
    EALLOW;
    
    // 配置GPIO引腳為GPIO功能 (MUX = 0)
    // 使用基本的GPIO配置，避免複雜的API調用
    
    // 配置GPIO方向
    if (config->mode == HAL_GPIO_MODE_OUTPUT) {
        GPIO_setDirectionMode(pin, GPIO_DIR_MODE_OUT);
    } else {
        GPIO_setDirectionMode(pin, GPIO_DIR_MODE_IN);
    }
    
    // 配置上拉/下拉
    if (config->pull == HAL_GPIO_PULLUP) {
        GPIO_setPadConfig(pin, GPIO_PIN_TYPE_PULLUP);
    } else {
        GPIO_setPadConfig(pin, GPIO_PIN_TYPE_STD);
    }
    
    // 配置輸入同步模式 (對於輸入引腳)
    if (config->mode == HAL_GPIO_MODE_INPUT) {
        GPIO_setQualificationMode(pin, GPIO_QUAL_ASYNC);
    }
    
    // 設置初始狀態 (對於輸出引腳)
    if (config->mode == HAL_GPIO_MODE_OUTPUT) {
        if (config->initial_state == HAL_GPIO_HIGH) {
            GPIO_writePin(pin, 1);
        } else {
            GPIO_writePin(pin, 0);
        }
    }
    
    EDIS;
    
    return HAL_OK;
}

hal_status_t hal_gpio_deinit(hal_gpio_pin_t pin)
{
    if (pin > 168) {
        return HAL_INVALID_PARAM;
    }
    
    // TI C2000的GPIO不需要特別的反初始化
    // 可以選擇將引腳重設為預設狀態
    EALLOW;
    GPIO_setDirectionMode(pin, GPIO_DIR_MODE_IN);
    GPIO_setPadConfig(pin, GPIO_PIN_TYPE_STD);
    EDIS;
    
    return HAL_OK;
}

hal_status_t hal_gpio_write(hal_gpio_pin_t pin, hal_gpio_state_t state)
{
    if (pin > 168) {
        return HAL_INVALID_PARAM;
    }
    
    if (state == HAL_GPIO_HIGH) {
        GPIO_writePin(pin, 1);
    } else {
        GPIO_writePin(pin, 0);
    }
    
    return HAL_OK;
}

hal_gpio_state_t hal_gpio_read(hal_gpio_pin_t pin)
{
    if (pin > 168) {
        return HAL_GPIO_LOW;
    }
    
    uint32_t pin_state = GPIO_readPin(pin);
    
    return (pin_state != 0) ? HAL_GPIO_HIGH : HAL_GPIO_LOW;
}

hal_status_t hal_gpio_toggle(hal_gpio_pin_t pin)
{
    if (pin > 168) {
        return HAL_INVALID_PARAM;
    }
    
    GPIO_togglePin(pin);
    
    return HAL_OK;
}

hal_status_t hal_gpio_set_mode(hal_gpio_pin_t pin, hal_gpio_mode_t mode)
{
    if (pin > 168) {
        return HAL_INVALID_PARAM;
    }
    
    EALLOW;
    
    if (mode == HAL_GPIO_MODE_OUTPUT) {
        GPIO_setDirectionMode(pin, GPIO_DIR_MODE_OUT);
    } else {
        GPIO_setDirectionMode(pin, GPIO_DIR_MODE_IN);
        GPIO_setQualificationMode(pin, GPIO_QUAL_ASYNC);
    }
    
    EDIS;
    
    return HAL_OK;
}

hal_status_t hal_gpio_set_pull(hal_gpio_pin_t pin, hal_gpio_pull_t pull)
{
    if (pin > 168) {
        return HAL_INVALID_PARAM;
    }
    
    EALLOW;
    
    if (pull == HAL_GPIO_PULLUP) {
        GPIO_setPadConfig(pin, GPIO_PIN_TYPE_PULLUP);
    } else {
        GPIO_setPadConfig(pin, GPIO_PIN_TYPE_STD);
    }
    
    EDIS;
    
    return HAL_OK;
}

#endif /* PLATFORM_TI_C2000 */