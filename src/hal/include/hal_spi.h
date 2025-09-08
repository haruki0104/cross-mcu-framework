/**
 * @file hal_spi.h
 * @brief SPI硬體抽象層介面
 * @author Cross-MCU Framework Team
 * @date 2024
 */

#ifndef HAL_SPI_H
#define HAL_SPI_H

#include "hal_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================== */
/*                             SPI介面函式                                    */
/* ========================================================================== */

/**
 * @brief 初始化SPI
 * @param spi_id SPI識別碼
 * @param config SPI配置結構體指標
 * @return HAL_OK 成功，其他值表示失敗
 */
hal_status_t hal_spi_init(hal_spi_id_t spi_id, const hal_spi_config_t* config);

/**
 * @brief 反初始化SPI
 * @param spi_id SPI識別碼
 * @return HAL_OK 成功，其他值表示失敗
 */
hal_status_t hal_spi_deinit(hal_spi_id_t spi_id);

/**
 * @brief SPI傳輸資料
 * @param spi_id SPI識別碼
 * @param tx_data 發送資料緩衝區
 * @param rx_data 接收資料緩衝區
 * @param size 資料長度
 * @param timeout 超時時間(ms)
 * @return HAL_OK 成功，其他值表示失敗
 */
hal_status_t hal_spi_transmit_receive(hal_spi_id_t spi_id, const uint8_t* tx_data, 
                                      uint8_t* rx_data, uint16_t size, uint32_t timeout);

/**
 * @brief SPI只發送資料
 * @param spi_id SPI識別碼
 * @param data 發送資料緩衝區
 * @param size 資料長度
 * @param timeout 超時時間(ms)
 * @return HAL_OK 成功，其他值表示失敗
 */
hal_status_t hal_spi_transmit(hal_spi_id_t spi_id, const uint8_t* data, 
                              uint16_t size, uint32_t timeout);

/**
 * @brief SPI只接收資料
 * @param spi_id SPI識別碼
 * @param data 接收資料緩衝區
 * @param size 資料長度
 * @param timeout 超時時間(ms)
 * @return HAL_OK 成功，其他值表示失敗
 */
hal_status_t hal_spi_receive(hal_spi_id_t spi_id, uint8_t* data, 
                             uint16_t size, uint32_t timeout);

/**
 * @brief 檢查SPI是否忙碌
 * @param spi_id SPI識別碼
 * @return true 忙碌，false 空閒
 */
bool hal_spi_is_busy(hal_spi_id_t spi_id);

/**
 * @brief 設置SPI片選信號
 * @param spi_id SPI識別碼
 * @param cs_pin 片選引腳
 * @param state 片選狀態 (LOW=選中, HIGH=未選中)
 * @return HAL_OK 成功，其他值表示失敗
 */
hal_status_t hal_spi_set_cs(hal_spi_id_t spi_id, hal_gpio_pin_t cs_pin, hal_gpio_state_t state);

#ifdef __cplusplus
}
#endif

#endif /* HAL_SPI_H */
