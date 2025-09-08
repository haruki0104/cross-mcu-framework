/**
 * @file hal_common.h
 * @brief 硬體抽象層公共定義和介面
 * @author Cross-MCU Framework Team
 * @date 2024
 */

#ifndef HAL_COMMON_H
#define HAL_COMMON_H

// 支援不同編譯器的標準類型定義
#ifdef __TI_COMPILER_VERSION__
    // TI編譯器
    #include <stdint.h>
    #include <stdbool.h>
    
    // TI C2000系列沒有原生8位類型，需要手動定義
    #if defined(__TMS320C2000__) && !defined(_UINT8_T_DECLARED)
        typedef unsigned int uint8_t;   // C2000使用16位int來模擬8位
        typedef signed int   int8_t;
        #define _UINT8_T_DECLARED
    #endif
#else
    // 標準編譯器
    #include <stdint.h>
    #include <stdbool.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================== */
/*                             平台識別巨集                                    */
/* ========================================================================== */

// TI C2000系列
#if defined(F28P55X) || defined(F28P65X)
    #define PLATFORM_TI_C2000
    #ifdef F28P55X
        #define MCU_F28P55X
    #elif defined(F28P65X)
        #define MCU_F28P65X
    #endif
#endif

// STM32系列
#if defined(STM32G4)
    #define PLATFORM_STM32
    #define MCU_STM32G4
#endif

/* ========================================================================== */
/*                             通用資料型別                                    */
/* ========================================================================== */

/** HAL返回狀態 */
typedef enum {
    HAL_OK = 0,
    HAL_ERROR,
    HAL_BUSY,
    HAL_TIMEOUT,
    HAL_INVALID_PARAM
} hal_status_t;

/** GPIO引腳狀態 */
typedef enum {
    HAL_GPIO_LOW = 0,
    HAL_GPIO_HIGH = 1
} hal_gpio_state_t;

/** GPIO引腳模式 */
typedef enum {
    HAL_GPIO_MODE_INPUT = 0,
    HAL_GPIO_MODE_OUTPUT,
    HAL_GPIO_MODE_ALTERNATE,
    HAL_GPIO_MODE_ANALOG
} hal_gpio_mode_t;

/** GPIO上拉/下拉配置 */
typedef enum {
    HAL_GPIO_NOPULL = 0,
    HAL_GPIO_PULLUP,
    HAL_GPIO_PULLDOWN
} hal_gpio_pull_t;

/** UART波特率 */
typedef enum {
    HAL_UART_BAUDRATE_9600 = 9600,
    HAL_UART_BAUDRATE_19200 = 19200,
    HAL_UART_BAUDRATE_38400 = 38400,
    HAL_UART_BAUDRATE_57600 = 57600,
    HAL_UART_BAUDRATE_115200 = 115200
} hal_uart_baudrate_t;

/** UART資料位數 */
typedef enum {
    HAL_UART_DATABITS_7 = 7,
    HAL_UART_DATABITS_8 = 8,
    HAL_UART_DATABITS_9 = 9
} hal_uart_databits_t;

/** UART停止位 */
typedef enum {
    HAL_UART_STOPBITS_1 = 0,
    HAL_UART_STOPBITS_2
} hal_uart_stopbits_t;

/** UART奇偶校驗 */
typedef enum {
    HAL_UART_PARITY_NONE = 0,
    HAL_UART_PARITY_EVEN,
    HAL_UART_PARITY_ODD
} hal_uart_parity_t;

/** SPI模式 */
typedef enum {
    HAL_SPI_MODE_MASTER = 0,
    HAL_SPI_MODE_SLAVE
} hal_spi_mode_t;

/** SPI時鐘極性 */
typedef enum {
    HAL_SPI_CPOL_LOW = 0,
    HAL_SPI_CPOL_HIGH
} hal_spi_cpol_t;

/** SPI時鐘相位 */
typedef enum {
    HAL_SPI_CPHA_1EDGE = 0,
    HAL_SPI_CPHA_2EDGE
} hal_spi_cpha_t;

/* ========================================================================== */
/*                             平台特定型別                                    */
/* ========================================================================== */

// 根據平台定義GPIO引腳型別
#ifdef PLATFORM_TI_C2000
    typedef uint32_t hal_gpio_pin_t;
#elif defined(PLATFORM_STM32)
    typedef uint16_t hal_gpio_pin_t;
#else
    typedef uint32_t hal_gpio_pin_t;  // 預設型別
#endif

// 根據平台定義週邊介面識別碼
#ifdef PLATFORM_TI_C2000
    typedef uint32_t hal_uart_id_t;
    typedef uint32_t hal_spi_id_t;
    typedef uint32_t hal_i2c_id_t;
    typedef uint32_t hal_adc_id_t;
#elif defined(PLATFORM_STM32)
    typedef void* hal_uart_id_t;  // STM32使用指標指向週邊結構
    typedef void* hal_spi_id_t;
    typedef void* hal_i2c_id_t;
    typedef void* hal_adc_id_t;
#else
    typedef uint32_t hal_uart_id_t;
    typedef uint32_t hal_spi_id_t;
    typedef uint32_t hal_i2c_id_t;
    typedef uint32_t hal_adc_id_t;
#endif

/* ========================================================================== */
/*                             配置結構體                                      */
/* ========================================================================== */

/** GPIO配置結構體 */
typedef struct {
    hal_gpio_pin_t pin;
    hal_gpio_mode_t mode;
    hal_gpio_pull_t pull;
    hal_gpio_state_t initial_state;
} hal_gpio_config_t;

/** UART配置結構體 */
typedef struct {
    hal_uart_baudrate_t baudrate;
    hal_uart_databits_t databits;
    hal_uart_stopbits_t stopbits;
    hal_uart_parity_t parity;
} hal_uart_config_t;

/** SPI配置結構體 */
typedef struct {
    hal_spi_mode_t mode;
    hal_spi_cpol_t cpol;
    hal_spi_cpha_t cpha;
    uint32_t frequency;
} hal_spi_config_t;

/** I2C配置結構體 */
typedef struct {
    uint32_t frequency;
    bool master_mode;
} hal_i2c_config_t;

/** ADC配置結構體 */
typedef struct {
    uint32_t resolution;
    uint32_t sampling_time;
} hal_adc_config_t;

#ifdef __cplusplus
}
#endif

#endif /* HAL_COMMON_H */
