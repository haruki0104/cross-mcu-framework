/**
 * @file ti_c2000_system.c
 * @brief TI C2000系列系統硬體抽象層實現
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
    // 初始化系統時鐘
    ti_c2000_init_system_clock();
    
    // 禁用看門狗
    ti_c2000_disable_watchdog();
    
    // 初始化週邊時鐘
    ti_c2000_init_peripheral_clocks();
    
    // 初始化PIE中斷控制器
    ti_c2000_init_pie();
    
    // 使能全域中斷
    ti_c2000_enable_global_interrupts();
    
    return HAL_OK;
}

hal_status_t hal_deinit(void)
{
    // 禁用全域中斷
    ti_c2000_disable_global_interrupts();
    
    return HAL_OK;
}

uint32_t hal_get_system_clock(void)
{
    return DEVICE_SYSCLK_FREQ;
}

void hal_delay_ms(uint32_t ms)
{
    DEVICE_DELAY_US(ms * 1000);
}

void hal_delay_us(uint32_t us)
{
    DEVICE_DELAY_US(us);
}

uint32_t hal_get_tick(void)
{
    return system_tick_counter;
}

void hal_system_reset(void)
{
    SysCtl_resetDevice();
    
    // 無限循環等待復位
    while (1) {
        __asm(" NOP");
    }
}

/* ========================================================================== */
/*                             TI C2000特定函式實現                           */
/* ========================================================================== */

void ti_c2000_init_system_clock(void)
{
    // 使用DriverLib進行系統時鐘初始化
    // 這會根據device.h中的定義自動配置適當的時鐘
    
    // 初始化系統控制
    SysCtl_deviceCal();
    
    // 禁用看門狗
    SysCtl_disableWatchdog();
    
#ifdef MCU_F28P55X
    // F28P55x特定的時鐘配置 (120MHz)
    // 使用內部振盪器配置PLL
    SysCtl_setClock(DEVICE_SETCLOCK_CFG);
    
#elif defined(MCU_F28P65X)
    // F28P65x特定的時鐘配置 (200MHz)  
    // 使用內部振盪器配置PLL
    SysCtl_setClock(DEVICE_SETCLOCK_CFG);
#endif
}

void ti_c2000_init_peripheral_clocks(void)
{
    // 使能必要的週邊時鐘
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_TIMER0);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_TIMER1);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_TIMER2);
    
    // GPIO時鐘通常在系統啟動時已經使能
    // 其他週邊時鐘根據需要在各自的初始化函式中使能
}

void ti_c2000_disable_watchdog(void)
{
    // 使用DriverLib禁用看門狗
    SysCtl_disableWatchdog();
}

void ti_c2000_init_pie(void)
{
    // 禁用CPU中斷
    DINT;
    
    // 初始化PIE控制器
    Interrupt_initModule();
    
    // 初始化PIE向量表
    Interrupt_initVectorTable();
    
    // 清除所有中斷旗標
    Interrupt_clearAllFlags();
}

void ti_c2000_enable_global_interrupts(void)
{
    // 使能全域中斷和即時中斷
    EINT;   // 使能全域中斷
    ERTM;   // 使能即時中斷
}

void ti_c2000_disable_global_interrupts(void)
{
    // 禁用全域中斷
    DINT;
}

/* ========================================================================== */
/*                             中斷服務程式                                    */
/* ========================================================================== */

// CPU Timer0中斷服務程式 (用於系統tick)
__interrupt void cpu_timer0_isr(void)
{
    system_tick_counter++;
    
    // 確認中斷
    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP1);
}

#endif /* PLATFORM_TI_C2000 */