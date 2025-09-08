/**
 * @file ti_c2000_common.h
 * @brief TI C2000系列MCU公共定義
 * @author Cross-MCU Framework Team
 * @date 2024
 */

#ifndef TI_C2000_COMMON_H
#define TI_C2000_COMMON_H

// 包含HAL公共標頭檔 (它會處理標準類型定義)
#include "../include/hal_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================== */
/*                             TI C2000平台檢查                               */
/* ========================================================================== */

#ifndef PLATFORM_TI_C2000
#error "This file is only for TI C2000 platform"
#endif

/* ========================================================================== */
/*                             TI C2000特定包含檔                             */
/* ========================================================================== */

#ifdef MCU_F28P55X
    #include "device.h"
    #include "driverlib.h"
    #define CPU_FREQ_MHZ    120
    #define LSPCLK_FREQ_MHZ 60
#elif defined(MCU_F28P65X)
    #include "device.h"
    #include "driverlib.h"
    #define CPU_FREQ_MHZ    200
    #define LSPCLK_FREQ_MHZ 100
#endif

/* ========================================================================== */
/*                             GPIO引腳定義                                   */
/* ========================================================================== */

// GPIO引腳編號定義 (0-168)
#define TI_GPIO_PIN(port, pin)  ((port << 5) | (pin))

// 常用GPIO引腳定義
#define TI_GPIO_0       0
#define TI_GPIO_1       1
#define TI_GPIO_2       2
#define TI_GPIO_3       3
#define TI_GPIO_4       4
#define TI_GPIO_5       5
#define TI_GPIO_31      31
#define TI_GPIO_34      34
#define TI_GPIO_35      35

/* ========================================================================== */
/*                             UART模組定義                                   */
/* ========================================================================== */

#define TI_UART_A       0
#define TI_UART_B       1
#define TI_UART_C       2

/* ========================================================================== */
/*                             SPI模組定義                                    */
/* ========================================================================== */

#define TI_SPI_A        0
#define TI_SPI_B        1
#define TI_SPI_C        2

/* ========================================================================== */
/*                             I2C模組定義                                    */
/* ========================================================================== */

#define TI_I2C_A        0
#define TI_I2C_B        1

/* ========================================================================== */
/*                             ADC模組定義                                    */
/* ========================================================================== */

#define TI_ADC_A        0
#define TI_ADC_B        1
#define TI_ADC_C        2
#define TI_ADC_D        3

/* ========================================================================== */
/*                             工具函式                                        */
/* ========================================================================== */

/**
 * @brief 初始化TI C2000系統時鐘
 */
void ti_c2000_init_system_clock(void);

/**
 * @brief 初始化TI C2000週邊時鐘
 */
void ti_c2000_init_peripheral_clocks(void);

/**
 * @brief 禁用看門狗
 */
void ti_c2000_disable_watchdog(void);

/**
 * @brief 初始化PIE中斷控制器
 */
void ti_c2000_init_pie(void);

/**
 * @brief 使能全域中斷
 */
void ti_c2000_enable_global_interrupts(void);

/**
 * @brief 禁用全域中斷
 */
void ti_c2000_disable_global_interrupts(void);

#ifdef __cplusplus
}
#endif

#endif /* TI_C2000_COMMON_H */
