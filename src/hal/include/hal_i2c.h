/**
 * @file hal_i2c.h
 * @brief I2C硬體抽象層介面
 * @author Cross-MCU Framework Team
 * @date 2024
 */

#ifndef HAL_I2C_H
#define HAL_I2C_H

#include "hal_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================== */
/*                             I2C介面函式                                    */
/* ========================================================================== */

/**
 * @brief 初始化I2C
 * @param i2c_id I2C識別碼
 * @param config I2C配置結構體指標
 * @return HAL_OK 成功，其他值表示失敗
 */
hal_status_t hal_i2c_init(hal_i2c_id_t i2c_id, const hal_i2c_config_t* config);

/**
 * @brief 反初始化I2C
 * @param i2c_id I2C識別碼
 * @return HAL_OK 成功，其他值表示失敗
 */
hal_status_t hal_i2c_deinit(hal_i2c_id_t i2c_id);

/**
 * @brief I2C主機發送資料
 * @param i2c_id I2C識別碼
 * @param device_addr 從機位址
 * @param data 發送資料緩衝區
 * @param size 資料長度
 * @param timeout 超時時間(ms)
 * @return HAL_OK 成功，其他值表示失敗
 */
hal_status_t hal_i2c_master_transmit(hal_i2c_id_t i2c_id, uint16_t device_addr, 
                                     const uint8_t* data, uint16_t size, uint32_t timeout);

/**
 * @brief I2C主機接收資料
 * @param i2c_id I2C識別碼
 * @param device_addr 從機位址
 * @param data 接收資料緩衝區
 * @param size 資料長度
 * @param timeout 超時時間(ms)
 * @return HAL_OK 成功，其他值表示失敗
 */
hal_status_t hal_i2c_master_receive(hal_i2c_id_t i2c_id, uint16_t device_addr, 
                                    uint8_t* data, uint16_t size, uint32_t timeout);

/**
 * @brief I2C主機寫入暫存器
 * @param i2c_id I2C識別碼
 * @param device_addr 從機位址
 * @param reg_addr 暫存器位址
 * @param data 寫入資料緩衝區
 * @param size 資料長度
 * @param timeout 超時時間(ms)
 * @return HAL_OK 成功，其他值表示失敗
 */
hal_status_t hal_i2c_mem_write(hal_i2c_id_t i2c_id, uint16_t device_addr, uint16_t reg_addr, 
                               const uint8_t* data, uint16_t size, uint32_t timeout);

/**
 * @brief I2C主機讀取暫存器
 * @param i2c_id I2C識別碼
 * @param device_addr 從機位址
 * @param reg_addr 暫存器位址
 * @param data 讀取資料緩衝區
 * @param size 資料長度
 * @param timeout 超時時間(ms)
 * @return HAL_OK 成功，其他值表示失敗
 */
hal_status_t hal_i2c_mem_read(hal_i2c_id_t i2c_id, uint16_t device_addr, uint16_t reg_addr, 
                              uint8_t* data, uint16_t size, uint32_t timeout);

/**
 * @brief 檢查I2C設備是否準備就緒
 * @param i2c_id I2C識別碼
 * @param device_addr 從機位址
 * @param trials 重試次數
 * @param timeout 超時時間(ms)
 * @return HAL_OK 設備就緒，其他值表示設備未準備或錯誤
 */
hal_status_t hal_i2c_is_device_ready(hal_i2c_id_t i2c_id, uint16_t device_addr, 
                                     uint32_t trials, uint32_t timeout);

/**
 * @brief 檢查I2C是否忙碌
 * @param i2c_id I2C識別碼
 * @return true 忙碌，false 空閒
 */
bool hal_i2c_is_busy(hal_i2c_id_t i2c_id);

/**
 * @brief 掃描I2C匯流排上的設備
 * @param i2c_id I2C識別碼
 * @param devices 找到的設備位址陣列
 * @param max_devices 最大設備數量
 * @return 找到的設備數量
 */
uint8_t hal_i2c_scan_devices(hal_i2c_id_t i2c_id, uint16_t* devices, uint8_t max_devices);

#ifdef __cplusplus
}
#endif

#endif /* HAL_I2C_H */
