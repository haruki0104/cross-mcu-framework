/**
 * @file ti_c2000_uart.c
 * @brief TI C2000系列UART硬體抽象層實現
 * @author Cross-MCU Framework Team
 * @date 2024
 */

#include "../include/hal_uart.h"
#include "ti_c2000_common.h"

#ifdef PLATFORM_TI_C2000

/* ========================================================================== */
/*                             內部變數                                        */
/* ========================================================================== */

// UART基址對應表
static const uint32_t uart_bases[] = {
    SCIA_BASE,  // UART A
    SCIB_BASE,  // UART B
#ifdef SCIC_BASE
    SCIC_BASE   // UART C (如果存在)
#endif
};

/* ========================================================================== */
/*                             內部函式聲明                                    */
/* ========================================================================== */

static hal_status_t ti_uart_config_gpio(hal_uart_id_t uart_id);
static uint32_t ti_uart_get_base(hal_uart_id_t uart_id);

/* ========================================================================== */
/*                             UART介面實現                                   */
/* ========================================================================== */

hal_status_t hal_uart_init(hal_uart_id_t uart_id, const hal_uart_config_t* config)
{
    if (config == NULL || uart_id >= (sizeof(uart_bases)/sizeof(uart_bases[0]))) {
        return HAL_INVALID_PARAM;
    }
    
    uint32_t uart_base = ti_uart_get_base(uart_id);
    if (uart_base == 0) {
        return HAL_INVALID_PARAM;
    }
    
    // 配置GPIO引腳
    hal_status_t status = ti_uart_config_gpio(uart_id);
    if (status != HAL_OK) {
        return status;
    }
    
    // 使能SCI模組時鐘
    switch (uart_id) {
        case TI_UART_A:
            SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_SCIA);
            break;
        case TI_UART_B:
            SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_SCIB);
            break;
#ifdef TI_UART_C
        case TI_UART_C:
            SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_SCIC);
            break;
#endif
    }
    
    // 配置UART參數
    SCI_disableModule(uart_base);
    
    // 設置波特率
    SCI_setBaud(uart_base, DEVICE_LSPCLK_FREQ, config->baudrate);
    
    // 設置資料格式
    uint16_t config_reg = 0;
    
    // 資料位數 (SCI使用字元長度 = 資料位數 - 1)
    switch (config->databits) {
        case HAL_UART_DATABITS_7:
            config_reg |= SCI_CONFIG_WLEN_7;
            break;
        case HAL_UART_DATABITS_8:
            config_reg |= SCI_CONFIG_WLEN_8;
            break;
        default:
            config_reg |= SCI_CONFIG_WLEN_8;
            break;
    }
    
    // 停止位
    if (config->stopbits == HAL_UART_STOPBITS_2) {
        config_reg |= SCI_CONFIG_STOP_TWO;
    } else {
        config_reg |= SCI_CONFIG_STOP_ONE;
    }
    
    // 奇偶校驗
    switch (config->parity) {
        case HAL_UART_PARITY_EVEN:
            config_reg |= SCI_CONFIG_PAR_EVEN;
            break;
        case HAL_UART_PARITY_ODD:
            config_reg |= SCI_CONFIG_PAR_ODD;
            break;
        case HAL_UART_PARITY_NONE:
        default:
            config_reg |= SCI_CONFIG_PAR_NONE;
            break;
    }
    
    SCI_setConfig(uart_base, DEVICE_LSPCLK_FREQ, config->baudrate, config_reg);
    
    // 使能FIFO
    SCI_enableFIFO(uart_base);
    SCI_enableModule(uart_base);
    SCI_performSoftwareReset(uart_base);
    
    return HAL_OK;
}

hal_status_t hal_uart_deinit(hal_uart_id_t uart_id)
{
    if (uart_id >= (sizeof(uart_bases)/sizeof(uart_bases[0]))) {
        return HAL_INVALID_PARAM;
    }
    
    uint32_t uart_base = ti_uart_get_base(uart_id);
    if (uart_base == 0) {
        return HAL_INVALID_PARAM;
    }
    
    // 禁用SCI模組
    SCI_disableModule(uart_base);
    
    // 禁用SCI模組時鐘
    switch (uart_id) {
        case TI_UART_A:
            SysCtl_disablePeripheral(SYSCTL_PERIPH_CLK_SCIA);
            break;
        case TI_UART_B:
            SysCtl_disablePeripheral(SYSCTL_PERIPH_CLK_SCIB);
            break;
#ifdef TI_UART_C
        case TI_UART_C:
            SysCtl_disablePeripheral(SYSCTL_PERIPH_CLK_SCIC);
            break;
#endif
    }
    
    return HAL_OK;
}

hal_status_t hal_uart_transmit(hal_uart_id_t uart_id, const uint8_t* data, 
                               uint16_t size, uint32_t timeout)
{
    if (data == NULL || size == 0 || uart_id >= (sizeof(uart_bases)/sizeof(uart_bases[0]))) {
        return HAL_INVALID_PARAM;
    }
    
    uint32_t uart_base = ti_uart_get_base(uart_id);
    if (uart_base == 0) {
        return HAL_INVALID_PARAM;
    }
    
    uint32_t start_tick = hal_get_tick();
    
    uint16_t i;
    for (i = 0; i < size; i++) {
        // 等待發送緩衝區空
        while (!SCI_isTransmitterEmpty(uart_base)) {
            if (timeout != 0 && (hal_get_tick() - start_tick) > timeout) {
                return HAL_TIMEOUT;
            }
        }
        
        // 發送資料
        SCI_writeCharBlockingFIFO(uart_base, data[i]);
    }
    
    return HAL_OK;
}

hal_status_t hal_uart_receive(hal_uart_id_t uart_id, uint8_t* data, 
                              uint16_t size, uint32_t timeout)
{
    if (data == NULL || size == 0 || uart_id >= (sizeof(uart_bases)/sizeof(uart_bases[0]))) {
        return HAL_INVALID_PARAM;
    }
    
    uint32_t uart_base = ti_uart_get_base(uart_id);
    if (uart_base == 0) {
        return HAL_INVALID_PARAM;
    }
    
    uint32_t start_tick = hal_get_tick();
    
    uint16_t i;
    for (i = 0; i < size; i++) {
        // 等待接收資料
        while (!SCI_isDataAvailableNonFIFO(uart_base)) {
            if (timeout != 0 && (hal_get_tick() - start_tick) > timeout) {
                return HAL_TIMEOUT;
            }
        }
        
        // 讀取資料
        data[i] = SCI_readCharBlockingFIFO(uart_base);
    }
    
    return HAL_OK;
}

hal_status_t hal_uart_putchar(hal_uart_id_t uart_id, uint8_t ch)
{
    return hal_uart_transmit(uart_id, &ch, 1, 1000);
}

hal_status_t hal_uart_getchar(hal_uart_id_t uart_id, uint8_t* ch, uint32_t timeout)
{
    return hal_uart_receive(uart_id, ch, 1, timeout);
}

bool hal_uart_is_busy(hal_uart_id_t uart_id)
{
    if (uart_id >= (sizeof(uart_bases)/sizeof(uart_bases[0]))) {
        return false;
    }
    
    uint32_t uart_base = ti_uart_get_base(uart_id);
    if (uart_base == 0) {
        return false;
    }
    
    // 檢查發送和接收是否忙碌
    return (!SCI_isTransmitterEmpty(uart_base));
}

bool hal_uart_data_available(hal_uart_id_t uart_id)
{
    if (uart_id >= (sizeof(uart_bases)/sizeof(uart_bases[0]))) {
        return false;
    }
    
    uint32_t uart_base = ti_uart_get_base(uart_id);
    if (uart_base == 0) {
        return false;
    }
    
    return SCI_isDataAvailableNonFIFO(uart_base);
}

hal_status_t hal_uart_flush_rx(hal_uart_id_t uart_id)
{
    if (uart_id >= (sizeof(uart_bases)/sizeof(uart_bases[0]))) {
        return HAL_INVALID_PARAM;
    }
    
    uint32_t uart_base = ti_uart_get_base(uart_id);
    if (uart_base == 0) {
        return HAL_INVALID_PARAM;
    }
    
    // 讀取所有待處理的接收資料
    while (SCI_isDataAvailableNonFIFO(uart_base)) {
        volatile uint16_t dummy = SCI_readCharBlockingFIFO(uart_base);
        (void)dummy;  // 避免編譯器警告
    }
    
    return HAL_OK;
}

hal_status_t hal_uart_flush_tx(hal_uart_id_t uart_id)
{
    if (uart_id >= (sizeof(uart_bases)/sizeof(uart_bases[0]))) {
        return HAL_INVALID_PARAM;
    }
    
    uint32_t uart_base = ti_uart_get_base(uart_id);
    if (uart_base == 0) {
        return HAL_INVALID_PARAM;
    }
    
    // 等待發送完成
    while (!SCI_isTransmitterEmpty(uart_base)) {
        // 等待發送緩衝區空
    }
    
    return HAL_OK;
}

/* ========================================================================== */
/*                             內部函式實現                                    */
/* ========================================================================== */

static hal_status_t ti_uart_config_gpio(hal_uart_id_t uart_id)
{
    EALLOW;
    
    switch (uart_id) {
        case TI_UART_A:
            // 配置SCIA的GPIO引腳 (GPIO28-RX, GPIO29-TX)
            GPIO_setPinConfig(GPIO_28_SCIA_RX);
            GPIO_setPinConfig(GPIO_29_SCIA_TX);
            break;
            
        case TI_UART_B:
            // 配置SCIB的GPIO引腳 (GPIO15-RX, GPIO14-TX)
            GPIO_setPinConfig(GPIO_15_SCIB_RX);
            GPIO_setPinConfig(GPIO_14_SCIB_TX);
            break;
            
#ifdef TI_UART_C
        case TI_UART_C:
            // 配置SCIC的GPIO引腳 (根據具體MCU型號而定)
            // 這裡需要根據實際硬體連接配置
            break;
#endif
    }
    
    EDIS;
    
    return HAL_OK;
}

static uint32_t ti_uart_get_base(hal_uart_id_t uart_id)
{
    if (uart_id >= (sizeof(uart_bases)/sizeof(uart_bases[0]))) {
        return 0;
    }
    
    return uart_bases[uart_id];
}

#endif /* PLATFORM_TI_C2000 */
