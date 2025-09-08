/**
 * @file ti_c2000_system_simple.c
 * @brief TI C2000系列系統硬體抽象層簡化實現
 * @author Cross-MCU Framework Team
 * @date 2024
 */

#include "../include/hal.h"
#include "ti_c2000_common.h"

#ifdef PLATFORM_TI_C2000

/* ========================================================================== */
/*                             內部變數                                        */
/* ========================================================================== */

static volatile uint32_t system_tick_counter = 0;

/* ========================================================================== */
/*                             系統初始化實現                                  */
/* ========================================================================== */

hal_status_t hal_init(void)
{
    // 基本的系統初始化
    ti_c2000_init_system_clock();
    ti_c2000_disable_watchdog();
    ti_c2000_init_peripheral_clocks();
    
    return HAL_OK;
}

hal_status_t hal_deinit(void)
{
    return HAL_OK;
}

uint32_t hal_get_system_clock(void)
{
    return CPU_FREQ;
}

void hal_delay_ms(uint32_t ms)
{
    // 簡單的延時實現
    volatile uint32_t i, j;
    for (i = 0; i < ms; i++) {
        for (j = 0; j < (CPU_FREQ / 10000); j++) {
            __asm(" NOP");
        }
    }
}

void hal_delay_us(uint32_t us)
{
    // 簡單的微秒延時實現
    volatile uint32_t i;
    for (i = 0; i < (us * CPU_FREQ / 1000000); i++) {
        __asm(" NOP");
    }
}

uint32_t hal_get_tick(void)
{
    return system_tick_counter;
}

void hal_system_reset(void)
{
    // 基本的系統復位
    __asm(" ESTOP0");
    
    while (1) {
        __asm(" NOP");
    }
}

/* ========================================================================== */
/*                             TI C2000特定函式實現                           */
/* ========================================================================== */

void ti_c2000_init_system_clock(void)
{
    // 基本的時鐘初始化
    // 這裡使用最基本的配置
}

void ti_c2000_init_peripheral_clocks(void)
{
    // 基本的週邊時鐘初始化
}

void ti_c2000_disable_watchdog(void)
{
    // 基本的看門狗禁用
}

void ti_c2000_init_pie(void)
{
    // 基本的PIE初始化
}

void ti_c2000_enable_global_interrupts(void)
{
    // 使能全域中斷
    __asm(" CLRC INTM");
}

void ti_c2000_disable_global_interrupts(void)
{
    // 禁用全域中斷  
    __asm(" SETC INTM");
}

#endif /* PLATFORM_TI_C2000 */
