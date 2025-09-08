/**
 * @file hal_adc.h
 * @brief ADC硬體抽象層介面
 * @author Cross-MCU Framework Team
 * @date 2024
 */

#ifndef HAL_ADC_H
#define HAL_ADC_H

#include "hal_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================== */
/*                             ADC介面函式                                    */
/* ========================================================================== */

/**
 * @brief 初始化ADC
 * @param adc_id ADC識別碼
 * @param config ADC配置結構體指標
 * @return HAL_OK 成功，其他值表示失敗
 */
hal_status_t hal_adc_init(hal_adc_id_t adc_id, const hal_adc_config_t* config);

/**
 * @brief 反初始化ADC
 * @param adc_id ADC識別碼
 * @return HAL_OK 成功，其他值表示失敗
 */
hal_status_t hal_adc_deinit(hal_adc_id_t adc_id);

/**
 * @brief 配置ADC通道
 * @param adc_id ADC識別碼
 * @param channel ADC通道號
 * @param sampling_time 取樣時間
 * @return HAL_OK 成功，其他值表示失敗
 */
hal_status_t hal_adc_config_channel(hal_adc_id_t adc_id, uint32_t channel, uint32_t sampling_time);

/**
 * @brief 開始ADC轉換
 * @param adc_id ADC識別碼
 * @return HAL_OK 成功，其他值表示失敗
 */
hal_status_t hal_adc_start_conversion(hal_adc_id_t adc_id);

/**
 * @brief 停止ADC轉換
 * @param adc_id ADC識別碼
 * @return HAL_OK 成功，其他值表示失敗
 */
hal_status_t hal_adc_stop_conversion(hal_adc_id_t adc_id);

/**
 * @brief 讀取ADC單次轉換結果
 * @param adc_id ADC識別碼
 * @param channel ADC通道號
 * @param value 轉換結果緩衝區
 * @param timeout 超時時間(ms)
 * @return HAL_OK 成功，其他值表示失敗
 */
hal_status_t hal_adc_read_single(hal_adc_id_t adc_id, uint32_t channel, 
                                 uint32_t* value, uint32_t timeout);

/**
 * @brief 讀取ADC多通道轉換結果
 * @param adc_id ADC識別碼
 * @param channels 通道陣列
 * @param values 轉換結果陣列
 * @param num_channels 通道數量
 * @param timeout 超時時間(ms)
 * @return HAL_OK 成功，其他值表示失敗
 */
hal_status_t hal_adc_read_multiple(hal_adc_id_t adc_id, const uint32_t* channels, 
                                   uint32_t* values, uint8_t num_channels, uint32_t timeout);

/**
 * @brief 檢查ADC轉換是否完成
 * @param adc_id ADC識別碼
 * @return true 轉換完成，false 轉換進行中
 */
bool hal_adc_is_conversion_complete(hal_adc_id_t adc_id);

/**
 * @brief 獲取ADC轉換結果
 * @param adc_id ADC識別碼
 * @return ADC轉換值
 */
uint32_t hal_adc_get_value(hal_adc_id_t adc_id);

/**
 * @brief 將ADC原始值轉換為電壓值
 * @param adc_value ADC原始值
 * @param resolution ADC解析度位數
 * @param vref_mv 參考電壓(mV)
 * @return 電壓值(mV)
 */
uint32_t hal_adc_to_voltage(uint32_t adc_value, uint32_t resolution, uint32_t vref_mv);

/**
 * @brief 校準ADC
 * @param adc_id ADC識別碼
 * @return HAL_OK 成功，其他值表示失敗
 */
hal_status_t hal_adc_calibrate(hal_adc_id_t adc_id);

#ifdef __cplusplus
}
#endif

#endif /* HAL_ADC_H */
