/**
 * @file stm32g4_gpio.c
 * @brief STM32G4系列GPIO硬體抽象層實現
 * @author Cross-MCU Framework Team
 * @date 2024
 */

#include "../include/hal_gpio.h"
#include "stm32g4_common.h"

#ifdef PLATFORM_STM32

/* ========================================================================== */
/*                             內部函式聲明                                    */
/* ========================================================================== */

static GPIO_TypeDef* stm32_get_gpio_port_from_pin(hal_gpio_pin_t pin);
static uint32_t stm32_get_gpio_pin_number(hal_gpio_pin_t pin);
static uint32_t stm32_convert_gpio_mode(hal_gpio_mode_t mode);
static uint32_t stm32_convert_gpio_pull(hal_gpio_pull_t pull);

/* ========================================================================== */
/*                             GPIO介面實現                                   */
/* ========================================================================== */

hal_status_t hal_gpio_init(const hal_gpio_config_t* config)
{
    if (config == NULL) {
        return HAL_INVALID_PARAM;
    }
    
    GPIO_TypeDef* gpio_port = stm32_get_gpio_port_from_pin(config->pin);
    if (gpio_port == NULL) {
        return HAL_INVALID_PARAM;
    }
    
    uint32_t pin_number = stm32_get_gpio_pin_number(config->pin);
    
    // 使能GPIO埠時鐘
    if (gpio_port == GPIOA) {
        __HAL_RCC_GPIOA_CLK_ENABLE();
    } else if (gpio_port == GPIOB) {
        __HAL_RCC_GPIOB_CLK_ENABLE();
    } else if (gpio_port == GPIOC) {
        __HAL_RCC_GPIOC_CLK_ENABLE();
    } else if (gpio_port == GPIOD) {
        __HAL_RCC_GPIOD_CLK_ENABLE();
    } else if (gpio_port == GPIOE) {
        __HAL_RCC_GPIOE_CLK_ENABLE();
    } else if (gpio_port == GPIOF) {
        __HAL_RCC_GPIOF_CLK_ENABLE();
    } else if (gpio_port == GPIOG) {
        __HAL_RCC_GPIOG_CLK_ENABLE();
    }
    
    // 配置GPIO
    GPIO_InitTypeDef gpio_init;
    gpio_init.Pin = (1U << pin_number);
    gpio_init.Mode = stm32_convert_gpio_mode(config->mode);
    gpio_init.Pull = stm32_convert_gpio_pull(config->pull);
    gpio_init.Speed = GPIO_SPEED_FREQ_HIGH;
    
    HAL_GPIO_Init(gpio_port, &gpio_init);
    
    // 設置初始狀態
    if (config->mode == HAL_GPIO_MODE_OUTPUT) {
        hal_gpio_write(config->pin, config->initial_state);
    }
    
    return HAL_OK;
}

hal_status_t hal_gpio_deinit(hal_gpio_pin_t pin)
{
    GPIO_TypeDef* gpio_port = stm32_get_gpio_port_from_pin(pin);
    if (gpio_port == NULL) {
        return HAL_INVALID_PARAM;
    }
    
    uint32_t pin_number = stm32_get_gpio_pin_number(pin);
    
    HAL_GPIO_DeInit(gpio_port, (1U << pin_number));
    
    return HAL_OK;
}

hal_status_t hal_gpio_write(hal_gpio_pin_t pin, hal_gpio_state_t state)
{
    GPIO_TypeDef* gpio_port = stm32_get_gpio_port_from_pin(pin);
    if (gpio_port == NULL) {
        return HAL_INVALID_PARAM;
    }
    
    uint32_t pin_number = stm32_get_gpio_pin_number(pin);
    GPIO_PinState pin_state = (state == HAL_GPIO_HIGH) ? GPIO_PIN_SET : GPIO_PIN_RESET;
    
    HAL_GPIO_WritePin(gpio_port, (1U << pin_number), pin_state);
    
    return HAL_OK;
}

hal_gpio_state_t hal_gpio_read(hal_gpio_pin_t pin)
{
    GPIO_TypeDef* gpio_port = stm32_get_gpio_port_from_pin(pin);
    if (gpio_port == NULL) {
        return HAL_GPIO_LOW;
    }
    
    uint32_t pin_number = stm32_get_gpio_pin_number(pin);
    GPIO_PinState pin_state = HAL_GPIO_ReadPin(gpio_port, (1U << pin_number));
    
    return (pin_state == GPIO_PIN_SET) ? HAL_GPIO_HIGH : HAL_GPIO_LOW;
}

hal_status_t hal_gpio_toggle(hal_gpio_pin_t pin)
{
    GPIO_TypeDef* gpio_port = stm32_get_gpio_port_from_pin(pin);
    if (gpio_port == NULL) {
        return HAL_INVALID_PARAM;
    }
    
    uint32_t pin_number = stm32_get_gpio_pin_number(pin);
    
    HAL_GPIO_TogglePin(gpio_port, (1U << pin_number));
    
    return HAL_OK;
}

hal_status_t hal_gpio_set_mode(hal_gpio_pin_t pin, hal_gpio_mode_t mode)
{
    GPIO_TypeDef* gpio_port = stm32_get_gpio_port_from_pin(pin);
    if (gpio_port == NULL) {
        return HAL_INVALID_PARAM;
    }
    
    uint32_t pin_number = stm32_get_gpio_pin_number(pin);
    
    // 重新配置GPIO模式
    GPIO_InitTypeDef gpio_init;
    gpio_init.Pin = (1U << pin_number);
    gpio_init.Mode = stm32_convert_gpio_mode(mode);
    gpio_init.Pull = GPIO_NOPULL;  // 保持原有上拉/下拉設置
    gpio_init.Speed = GPIO_SPEED_FREQ_HIGH;
    
    HAL_GPIO_Init(gpio_port, &gpio_init);
    
    return HAL_OK;
}

hal_status_t hal_gpio_set_pull(hal_gpio_pin_t pin, hal_gpio_pull_t pull)
{
    GPIO_TypeDef* gpio_port = stm32_get_gpio_port_from_pin(pin);
    if (gpio_port == NULL) {
        return HAL_INVALID_PARAM;
    }
    
    uint32_t pin_number = stm32_get_gpio_pin_number(pin);
    
    // 重新配置GPIO上拉/下拉
    GPIO_InitTypeDef gpio_init;
    gpio_init.Pin = (1U << pin_number);
    gpio_init.Mode = GPIO_MODE_INPUT;  // 假設為輸入模式
    gpio_init.Pull = stm32_convert_gpio_pull(pull);
    gpio_init.Speed = GPIO_SPEED_FREQ_HIGH;
    
    HAL_GPIO_Init(gpio_port, &gpio_init);
    
    return HAL_OK;
}

/* ========================================================================== */
/*                             內部函式實現                                    */
/* ========================================================================== */

static GPIO_TypeDef* stm32_get_gpio_port_from_pin(hal_gpio_pin_t pin)
{
    // 假設引腳編碼格式: 高8位為埠號，低8位為引腳號
    uint8_t port = (pin >> 8) & 0xFF;
    
    switch (port) {
        case 0: return GPIOA;
        case 1: return GPIOB;
        case 2: return GPIOC;
        case 3: return GPIOD;
        case 4: return GPIOE;
        case 5: return GPIOF;
        case 6: return GPIOG;
        default: return NULL;
    }
}

static uint32_t stm32_get_gpio_pin_number(hal_gpio_pin_t pin)
{
    // 假設引腳編碼格式: 高8位為埠號，低8位為引腳號
    return pin & 0xFF;
}

static uint32_t stm32_convert_gpio_mode(hal_gpio_mode_t mode)
{
    switch (mode) {
        case HAL_GPIO_MODE_INPUT:
            return GPIO_MODE_INPUT;
        case HAL_GPIO_MODE_OUTPUT:
            return GPIO_MODE_OUTPUT_PP;
        case HAL_GPIO_MODE_ALTERNATE:
            return GPIO_MODE_AF_PP;
        case HAL_GPIO_MODE_ANALOG:
            return GPIO_MODE_ANALOG;
        default:
            return GPIO_MODE_INPUT;
    }
}

static uint32_t stm32_convert_gpio_pull(hal_gpio_pull_t pull)
{
    switch (pull) {
        case HAL_GPIO_NOPULL:
            return GPIO_NOPULL;
        case HAL_GPIO_PULLUP:
            return GPIO_PULLUP;
        case HAL_GPIO_PULLDOWN:
            return GPIO_PULLDOWN;
        default:
            return GPIO_NOPULL;
    }
}

#endif /* PLATFORM_STM32 */
