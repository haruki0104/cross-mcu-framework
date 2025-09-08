/**
 * @file ti_c2000_driverlib.h
 * @brief TI C2000 DriverLib包裝層統一標頭檔
 * @author Cross-MCU Framework Team
 * @date 2024
 */

#ifndef TI_C2000_DRIVERLIB_H
#define TI_C2000_DRIVERLIB_H

#include "../ti_c2000_config.h"
#include "../ti_c2000_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================== */
/*                             DriverLib包含檔案                             */
/* ========================================================================== */

#if TI_C2000_USE_DRIVERLIB

    // 檢查是否有C2000Ware環境
    #ifdef MCU_F28P55X
        #include "device.h"
        #include "driverlib.h"
    #elif defined(MCU_F28P65X)
        #include "device.h" 
        #include "driverlib.h"
    #else
        #warning "Unknown MCU type for DriverLib"
    #endif
    
    // DriverLib版本檢查
    #if defined(DRIVERLIB_VERSION_NUMBER)
        #if DRIVERLIB_VERSION_NUMBER < 0x04040000
            #warning "DriverLib version may be too old, recommended 4.04.00.00 or newer"
        #endif
    #else
        #warning "DriverLib version not detected"
    #endif

#endif

/* ========================================================================== */
/*                             GPIO包裝宏定義                               */
/* ========================================================================== */

#if TI_C2000_GPIO_USE_DRIVERLIB

    // GPIO基本操作包裝
    #define TI_GPIO_SET_CONFIG(pin, config)    GPIO_setPinConfig(config)
    #define TI_GPIO_SET_DIR_OUT(pin)           GPIO_setDirectionMode(pin, GPIO_DIR_MODE_OUT)
    #define TI_GPIO_SET_DIR_IN(pin)            GPIO_setDirectionMode(pin, GPIO_DIR_MODE_IN)
    #define TI_GPIO_WRITE_HIGH(pin)            GPIO_writePin(pin, 1)
    #define TI_GPIO_WRITE_LOW(pin)             GPIO_writePin(pin, 0)
    #define TI_GPIO_READ(pin)                  GPIO_readPin(pin)
    #define TI_GPIO_TOGGLE(pin)                GPIO_togglePin(pin)
    
    // GPIO配置包裝
    #define TI_GPIO_SET_PAD_STD(pin)           GPIO_setPadConfig(pin, GPIO_PIN_TYPE_STD)
    #define TI_GPIO_SET_PAD_PULLUP(pin)        GPIO_setPadConfig(pin, GPIO_PIN_TYPE_PULLUP)
    #define TI_GPIO_SET_QUALIFICATION(pin, q)  GPIO_setQualificationMode(pin, q)

#else

    // 簡化版本前向聲明
    hal_status_t ti_gpio_simple_init(const hal_gpio_config_t* config);
    hal_status_t ti_gpio_simple_write(hal_gpio_pin_t pin, hal_gpio_state_t state);
    hal_gpio_state_t ti_gpio_simple_read(hal_gpio_pin_t pin);
    hal_status_t ti_gpio_simple_toggle(hal_gpio_pin_t pin);

#endif

/* ========================================================================== */
/*                             UART包裝宏定義                               */
/* ========================================================================== */

#if TI_C2000_UART_USE_DRIVERLIB

    // UART基本操作包裝
    #define TI_UART_SET_CONFIG(base, lspclk, baud, config) \
            SCI_setConfig(base, lspclk, baud, config)
    #define TI_UART_ENABLE(base)               SCI_enableModule(base)
    #define TI_UART_DISABLE(base)              SCI_disableModule(base)
    #define TI_UART_RESET(base)                SCI_performSoftwareReset(base)
    
    // UART資料傳輸包裝
    #define TI_UART_WRITE_CHAR(base, data)     SCI_writeCharBlockingFIFO(base, data)
    #define TI_UART_READ_CHAR(base)            SCI_readCharBlockingFIFO(base)
    #define TI_UART_IS_TX_READY(base)          SCI_isTransmitterEmpty(base)
    #define TI_UART_IS_RX_READY(base)          SCI_isDataAvailableNonFIFO(base)
    
    // UART FIFO操作包裝
    #define TI_UART_ENABLE_FIFO(base)          SCI_enableFIFO(base)
    #define TI_UART_DISABLE_FIFO(base)         SCI_disableFIFO(base)

#else

    // 簡化版本前向聲明
    hal_status_t ti_uart_simple_init(hal_uart_id_t uart_id, const hal_uart_config_t* config);
    hal_status_t ti_uart_simple_transmit(hal_uart_id_t uart_id, const uint8_t* data, 
                                        uint16_t size, uint32_t timeout);
    hal_status_t ti_uart_simple_receive(hal_uart_id_t uart_id, uint8_t* data, 
                                       uint16_t size, uint32_t timeout);

#endif

/* ========================================================================== */
/*                             系統控制包裝宏定義                            */
/* ========================================================================== */

#if TI_C2000_SYSTEM_USE_DRIVERLIB

    // 系統控制包裝
    #define TI_SYSCTL_ENABLE_PERIPH(periph)   SysCtl_enablePeripheral(periph)
    #define TI_SYSCTL_DISABLE_PERIPH(periph)  SysCtl_disablePeripheral(periph)
    #define TI_SYSCTL_RESET_DEVICE()          SysCtl_resetDevice()
    
    // 時鐘控制包裝
    #define TI_SYSCTL_GET_DEVICE_CLK()        SysCtl_getDeviceParameterValue(SYSCTL_DEVICE_SYSCLK)

#endif

/* ========================================================================== */
/*                             通用工具宏                                    */
/* ========================================================================== */

/**
 * @brief EALLOW/EDIS包裝宏
 */
#if TI_C2000_USE_DRIVERLIB
    #define TI_EALLOW()    EALLOW
    #define TI_EDIS()      EDIS
#else
    #define TI_EALLOW()    __asm(" EALLOW")
    #define TI_EDIS()      __asm(" EDIS")
#endif

/**
 * @brief 中斷控制包裝宏
 */
#if TI_C2000_USE_DRIVERLIB
    #define TI_ENABLE_GLOBAL_INT()     Interrupt_enableGlobal()
    #define TI_DISABLE_GLOBAL_INT()    Interrupt_disableGlobal()
#else
    #define TI_ENABLE_GLOBAL_INT()     __asm(" CLRC INTM")
    #define TI_DISABLE_GLOBAL_INT()    __asm(" SETC INTM")
#endif

/**
 * @brief 延時操作包裝宏
 */
#if TI_C2000_USE_DRIVERLIB && defined(DEVICE_DELAY_US)
    #define TI_DELAY_US(us)            DEVICE_DELAY_US(us)
#else
    #define TI_DELAY_US(us)            ti_delay_us_simple(us)
    void ti_delay_us_simple(uint32_t us);
#endif

/* ========================================================================== */
/*                             除錯支援宏                                    */
/* ========================================================================== */

#if TI_C2000_DEBUG_ENABLED
    #define TI_DEBUG_ASSERT(condition) \
        do { \
            if (!(condition)) { \
                __asm(" ESTOP0"); \
            } \
        } while(0)
#else
    #define TI_DEBUG_ASSERT(condition) ((void)0)
#endif

#ifdef __cplusplus
}
#endif

#endif /* TI_C2000_DRIVERLIB_H */
