/**
 * @file ti_c2000_gpio_dl.c
 * @brief TI C2000系列GPIO硬體抽象層DriverLib實現
 * @author Cross-MCU Framework Team
 * @date 2024
 */

#include "../../include/hal_gpio.h"
#include "ti_c2000_driverlib.h"

#if TI_C2000_GPIO_USE_DRIVERLIB

/* ========================================================================== */
/*                             內部函式                                       */
/* ========================================================================== */

/**
 * @brief 將HAL GPIO模式轉換為DriverLib配置
 */
static uint32_t hal_gpio_mode_to_driverlib_config(hal_gpio_pin_t pin, hal_gpio_mode_t mode)
{
    // GPIO配置基礎值 (GPIO功能，非複用)
    uint32_t config_base = GPIO_0_GPIO0 + pin;
    
    switch (mode) {
        case HAL_GPIO_MODE_INPUT:
        case HAL_GPIO_MODE_OUTPUT:
            return config_base;  // 標準GPIO功能
            
        case HAL_GPIO_MODE_ALTERNATE:
            // 複用功能需要根據具體引腳和功能來配置
            // 這裡返回基礎配置，實際應用中需要更具體的配置
            return config_base;
            
        case HAL_GPIO_MODE_ANALOG:
            // C2000的模擬功能通常通過特定的複用功能實現
            return config_base;
            
        default:
            return config_base;
    }
}

/**
 * @brief 將HAL上拉/下拉配置轉換為DriverLib配置
 */
static uint32_t hal_gpio_pull_to_driverlib_type(hal_gpio_pull_t pull)
{
    switch (pull) {
        case HAL_GPIO_PULLUP:
            return GPIO_PIN_TYPE_PULLUP;
            
        case HAL_GPIO_PULLDOWN:
            // C2000系列沒有內建下拉電阻，使用標準配置
            return GPIO_PIN_TYPE_STD;
            
        case HAL_GPIO_NOPULL:
        default:
            return GPIO_PIN_TYPE_STD;
    }
}

/* ========================================================================== */
/*                             HAL GPIO介面實現                              */
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
    
    TI_DEBUG_ASSERT(config->mode <= HAL_GPIO_MODE_ANALOG);
    TI_DEBUG_ASSERT(config->pull <= HAL_GPIO_PULLDOWN);
    
    TI_EALLOW();
    
    // 1. 設置GPIO功能配置
    uint32_t gpio_config = hal_gpio_mode_to_driverlib_config(pin, config->mode);
    TI_GPIO_SET_CONFIG(pin, gpio_config);
    
    // 2. 設置方向
    if (config->mode == HAL_GPIO_MODE_OUTPUT) {
        TI_GPIO_SET_DIR_OUT(pin);
        
        // 設置初始狀態
        if (config->initial_state == HAL_GPIO_HIGH) {
            TI_GPIO_WRITE_HIGH(pin);
        } else {
            TI_GPIO_WRITE_LOW(pin);
        }
    } else {
        TI_GPIO_SET_DIR_IN(pin);
    }
    
    // 3. 設置上拉/下拉配置
    uint32_t pin_type = hal_gpio_pull_to_driverlib_type(config->pull);
    GPIO_setPadConfig(pin, pin_type);
    
    // 4. 設置輸入限定模式 (預設為同步輸入)
    if (config->mode == HAL_GPIO_MODE_INPUT) {
        TI_GPIO_SET_QUALIFICATION(pin, GPIO_QUAL_SYNC);
    }
    
    TI_EDIS();
    
    return HAL_OK;
}

hal_status_t hal_gpio_deinit(hal_gpio_pin_t pin)
{
    if (pin > 168) {
        return HAL_INVALID_PARAM;
    }
    
    TI_EALLOW();
    
    // 重設為輸入模式，標準配置
    TI_GPIO_SET_DIR_IN(pin);
    TI_GPIO_SET_PAD_STD(pin);
    TI_GPIO_SET_CONFIG(pin, GPIO_0_GPIO0 + pin);
    
    TI_EDIS();
    
    return HAL_OK;
}

hal_status_t hal_gpio_write(hal_gpio_pin_t pin, hal_gpio_state_t state)
{
    if (pin > 168) {
        return HAL_INVALID_PARAM;
    }
    
    if (state == HAL_GPIO_HIGH) {
        TI_GPIO_WRITE_HIGH(pin);
    } else {
        TI_GPIO_WRITE_LOW(pin);
    }
    
    return HAL_OK;
}

hal_gpio_state_t hal_gpio_read(hal_gpio_pin_t pin)
{
    if (pin > 168) {
        return HAL_GPIO_LOW;
    }
    
    uint32_t pin_value = TI_GPIO_READ(pin);
    return (pin_value != 0) ? HAL_GPIO_HIGH : HAL_GPIO_LOW;
}

hal_status_t hal_gpio_toggle(hal_gpio_pin_t pin)
{
    if (pin > 168) {
        return HAL_INVALID_PARAM;
    }
    
    TI_GPIO_TOGGLE(pin);
    
    return HAL_OK;
}

hal_status_t hal_gpio_set_mode(hal_gpio_pin_t pin, hal_gpio_mode_t mode)
{
    if (pin > 168) {
        return HAL_INVALID_PARAM;
    }
    
    TI_EALLOW();
    
    // 重新配置GPIO功能
    uint32_t gpio_config = hal_gpio_mode_to_driverlib_config(pin, mode);
    TI_GPIO_SET_CONFIG(pin, gpio_config);
    
    // 設置方向
    if (mode == HAL_GPIO_MODE_OUTPUT) {
        TI_GPIO_SET_DIR_OUT(pin);
    } else {
        TI_GPIO_SET_DIR_IN(pin);
    }
    
    TI_EDIS();
    
    return HAL_OK;
}

hal_status_t hal_gpio_set_pull(hal_gpio_pin_t pin, hal_gpio_pull_t pull)
{
    if (pin > 168) {
        return HAL_INVALID_PARAM;
    }
    
    TI_EALLOW();
    
    uint32_t pin_type = hal_gpio_pull_to_driverlib_type(pull);
    GPIO_setPadConfig(pin, pin_type);
    
    TI_EDIS();
    
    return HAL_OK;
}

/* ========================================================================== */
/*                             擴展功能 (DriverLib特有)                      */
/* ========================================================================== */

/**
 * @brief 設置GPIO中斷配置 (DriverLib版本特有功能)
 * @param pin GPIO引腳編號
 * @param int_type 中斷類型 (0=禁用, 1=上升沿, 2=下降沿, 3=雙沿)
 * @return HAL_OK 成功，其他值表示失敗
 */
hal_status_t hal_gpio_set_interrupt(hal_gpio_pin_t pin, uint32_t int_type)
{
    if (pin > 168) {
        return HAL_INVALID_PARAM;
    }
    
    // 只有部分GPIO支援中斷功能
    if (pin > 31) {
        return HAL_ERROR;  // GPIO32-168不支援中斷
    }
    
    TI_EALLOW();
    
    // 簡化的中斷配置，實際使用時需要更詳細的實現
    // 這裡只是示例代碼
    
    TI_EDIS();
    
    return HAL_OK;
}

/**
 * @brief 設置GPIO輸入限定模式 (DriverLib版本特有功能)
 * @param pin GPIO引腳編號
 * @param qual_mode 限定模式 (0=同步, 1=3採樣, 2=6採樣, 3=非同步)
 * @return HAL_OK 成功，其他值表示失敗
 */
hal_status_t hal_gpio_set_qualification(hal_gpio_pin_t pin, uint32_t qual_mode)
{
    if (pin > 168) {
        return HAL_INVALID_PARAM;
    }
    
    TI_EALLOW();
    
    TI_GPIO_SET_QUALIFICATION(pin, qual_mode);
    
    TI_EDIS();
    
    return HAL_OK;
}

/**
 * @brief 設置GPIO輸出驅動強度 (DriverLib版本特有功能)
 * @param pin GPIO引腳編號
 * @param drive_strength 驅動強度
 * @return HAL_OK 成功，其他值表示失敗
 */
hal_status_t hal_gpio_set_drive_strength(hal_gpio_pin_t pin, bool high_drive)
{
    if (pin > 168) {
        return HAL_INVALID_PARAM;
    }
    
    TI_EALLOW();
    
    if (high_drive) {
        // 設置高驅動強度 (如果硬體支援)
        // 注意：不是所有GPIO都支援高驅動強度
    }
    
    TI_EDIS();
    
    return HAL_OK;
}

#endif /* TI_C2000_GPIO_USE_DRIVERLIB */
