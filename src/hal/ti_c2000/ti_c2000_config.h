/**
 * @file ti_c2000_config.h
 * @brief TI C2000平台編譯配置選擇
 * @author Cross-MCU Framework Team
 * @date 2024
 */

#ifndef TI_C2000_CONFIG_H
#define TI_C2000_CONFIG_H

/* ========================================================================== */
/*                             DriverLib整合配置                             */
/* ========================================================================== */

/**
 * @brief DriverLib使用配置
 * 0 = 使用簡化版本實現 (不依賴C2000Ware)
 * 1 = 使用DriverLib實現 (需要C2000Ware)
 */
#ifndef TI_C2000_USE_DRIVERLIB
    #define TI_C2000_USE_DRIVERLIB      0
#endif

/**
 * @brief DriverLib版本要求
 */
#if TI_C2000_USE_DRIVERLIB
    #define TI_C2000_DRIVERLIB_VERSION  "4.04.00.00"
#endif

/* ========================================================================== */
/*                             模組選擇配置                                   */
/* ========================================================================== */

#if TI_C2000_USE_DRIVERLIB
    #define TI_C2000_GPIO_USE_DRIVERLIB     1
    #define TI_C2000_UART_USE_DRIVERLIB     1
    #define TI_C2000_SPI_USE_DRIVERLIB      1
    #define TI_C2000_I2C_USE_DRIVERLIB      1
    #define TI_C2000_ADC_USE_DRIVERLIB      1
    #define TI_C2000_SYSTEM_USE_DRIVERLIB   1
#else
    #define TI_C2000_GPIO_USE_DRIVERLIB     0
    #define TI_C2000_UART_USE_DRIVERLIB     0
    #define TI_C2000_SPI_USE_DRIVERLIB      0
    #define TI_C2000_I2C_USE_DRIVERLIB      0
    #define TI_C2000_ADC_USE_DRIVERLIB      0
    #define TI_C2000_SYSTEM_USE_DRIVERLIB   0
#endif

/* ========================================================================== */
/*                             功能特性配置                                   */
/* ========================================================================== */

/**
 * @brief 除錯功能配置
 */
#define TI_C2000_DEBUG_ENABLED          1

/**
 * @brief 錯誤檢查級別
 * 0 = 基本檢查
 * 1 = 完整檢查
 */
#define TI_C2000_ERROR_CHECK_LEVEL      1

/**
 * @brief 中斷處理配置
 */
#define TI_C2000_IRQ_SUPPORT_ENABLED    1

/**
 * @brief DMA支援配置
 */
#define TI_C2000_DMA_SUPPORT_ENABLED    0

#endif /* TI_C2000_CONFIG_H */
