# API參考文件

本文件詳細說明Cross-MCU Framework的所有API介面。

## 目錄

- [通用定義](#通用定義)
- [系統API](#系統api)
- [GPIO API](#gpio-api)
- [UART API](#uart-api)
- [SPI API](#spi-api)
- [I2C API](#i2c-api)
- [ADC API](#adc-api)

## 通用定義

### 狀態碼

```c
typedef enum {
    HAL_OK = 0,           // 操作成功
    HAL_ERROR,            // 一般錯誤
    HAL_BUSY,             // 設備忙碌
    HAL_TIMEOUT,          // 操作超時
    HAL_INVALID_PARAM     // 無效參數
} hal_status_t;
```

### 布林型別

```c
#include <stdbool.h>
// 使用標準C的bool, true, false
```

## 系統API

### hal_init()

**功能**: 初始化HAL系統

```c
hal_status_t hal_init(void);
```

**返回值**:
- `HAL_OK`: 初始化成功
- `HAL_ERROR`: 初始化失敗

**說明**: 必須在使用任何其他HAL函式前呼叫此函式。

### hal_deinit()

**功能**: 反初始化HAL系統

```c
hal_status_t hal_deinit(void);
```

**返回值**:
- `HAL_OK`: 反初始化成功
- `HAL_ERROR`: 反初始化失敗

### hal_get_system_clock()

**功能**: 獲取系統時鐘頻率

```c
uint32_t hal_get_system_clock(void);
```

**返回值**: 系統時鐘頻率 (Hz)

### hal_delay_ms()

**功能**: 毫秒級延時

```c
void hal_delay_ms(uint32_t ms);
```

**參數**:
- `ms`: 延時時間 (毫秒)

### hal_delay_us()

**功能**: 微秒級延時

```c
void hal_delay_us(uint32_t us);
```

**參數**:
- `us`: 延時時間 (微秒)

### hal_get_tick()

**功能**: 獲取系統tick計數

```c
uint32_t hal_get_tick(void);
```

**返回值**: 系統tick計數值 (通常為毫秒)

### hal_system_reset()

**功能**: 系統復位

```c
void hal_system_reset(void);
```

**說明**: 此函式不會返回，系統將復位重啟。

## GPIO API

### 資料型別

```c
// GPIO引腳狀態
typedef enum {
    HAL_GPIO_LOW = 0,
    HAL_GPIO_HIGH = 1
} hal_gpio_state_t;

// GPIO引腳模式
typedef enum {
    HAL_GPIO_MODE_INPUT = 0,
    HAL_GPIO_MODE_OUTPUT,
    HAL_GPIO_MODE_ALTERNATE,
    HAL_GPIO_MODE_ANALOG
} hal_gpio_mode_t;

// GPIO上拉/下拉配置
typedef enum {
    HAL_GPIO_NOPULL = 0,
    HAL_GPIO_PULLUP,
    HAL_GPIO_PULLDOWN
} hal_gpio_pull_t;

// GPIO配置結構體
typedef struct {
    hal_gpio_pin_t pin;
    hal_gpio_mode_t mode;
    hal_gpio_pull_t pull;
    hal_gpio_state_t initial_state;
} hal_gpio_config_t;
```

### hal_gpio_init()

**功能**: 初始化GPIO引腳

```c
hal_status_t hal_gpio_init(const hal_gpio_config_t* config);
```

**參數**:
- `config`: GPIO配置結構體指標

**返回值**:
- `HAL_OK`: 初始化成功
- `HAL_INVALID_PARAM`: 無效參數
- `HAL_ERROR`: 初始化失敗

**範例**:
```c
hal_gpio_config_t led_config = {
    .pin = LED_PIN,
    .mode = HAL_GPIO_MODE_OUTPUT,
    .pull = HAL_GPIO_NOPULL,
    .initial_state = HAL_GPIO_LOW
};
hal_gpio_init(&led_config);
```

### hal_gpio_deinit()

**功能**: 反初始化GPIO引腳

```c
hal_status_t hal_gpio_deinit(hal_gpio_pin_t pin);
```

### hal_gpio_write()

**功能**: 設置GPIO引腳輸出狀態

```c
hal_status_t hal_gpio_write(hal_gpio_pin_t pin, hal_gpio_state_t state);
```

**參數**:
- `pin`: GPIO引腳識別碼
- `state`: 輸出狀態

### hal_gpio_read()

**功能**: 讀取GPIO引腳狀態

```c
hal_gpio_state_t hal_gpio_read(hal_gpio_pin_t pin);
```

**參數**:
- `pin`: GPIO引腳識別碼

**返回值**: 引腳狀態

### hal_gpio_toggle()

**功能**: 切換GPIO引腳輸出狀態

```c
hal_status_t hal_gpio_toggle(hal_gpio_pin_t pin);
```

## UART API

### 資料型別

```c
// UART波特率
typedef enum {
    HAL_UART_BAUDRATE_9600 = 9600,
    HAL_UART_BAUDRATE_19200 = 19200,
    HAL_UART_BAUDRATE_38400 = 38400,
    HAL_UART_BAUDRATE_57600 = 57600,
    HAL_UART_BAUDRATE_115200 = 115200
} hal_uart_baudrate_t;

// UART資料位數
typedef enum {
    HAL_UART_DATABITS_7 = 7,
    HAL_UART_DATABITS_8 = 8,
    HAL_UART_DATABITS_9 = 9
} hal_uart_databits_t;

// UART停止位
typedef enum {
    HAL_UART_STOPBITS_1 = 0,
    HAL_UART_STOPBITS_2
} hal_uart_stopbits_t;

// UART奇偶校驗
typedef enum {
    HAL_UART_PARITY_NONE = 0,
    HAL_UART_PARITY_EVEN,
    HAL_UART_PARITY_ODD
} hal_uart_parity_t;

// UART配置結構體
typedef struct {
    hal_uart_baudrate_t baudrate;
    hal_uart_databits_t databits;
    hal_uart_stopbits_t stopbits;
    hal_uart_parity_t parity;
} hal_uart_config_t;
```

### hal_uart_init()

**功能**: 初始化UART

```c
hal_status_t hal_uart_init(hal_uart_id_t uart_id, const hal_uart_config_t* config);
```

**參數**:
- `uart_id`: UART識別碼
- `config`: UART配置結構體指標

**範例**:
```c
hal_uart_config_t uart_config = {
    .baudrate = HAL_UART_BAUDRATE_115200,
    .databits = HAL_UART_DATABITS_8,
    .stopbits = HAL_UART_STOPBITS_1,
    .parity = HAL_UART_PARITY_NONE
};
hal_uart_init(CONSOLE_UART, &uart_config);
```

### hal_uart_transmit()

**功能**: 發送資料

```c
hal_status_t hal_uart_transmit(hal_uart_id_t uart_id, const uint8_t* data, 
                               uint16_t size, uint32_t timeout);
```

**參數**:
- `uart_id`: UART識別碼
- `data`: 發送資料緩衝區
- `size`: 資料長度
- `timeout`: 超時時間 (毫秒)

### hal_uart_receive()

**功能**: 接收資料

```c
hal_status_t hal_uart_receive(hal_uart_id_t uart_id, uint8_t* data, 
                              uint16_t size, uint32_t timeout);
```

### hal_uart_putchar()

**功能**: 發送單個字元

```c
hal_status_t hal_uart_putchar(hal_uart_id_t uart_id, uint8_t ch);
```

### hal_uart_getchar()

**功能**: 接收單個字元

```c
hal_status_t hal_uart_getchar(hal_uart_id_t uart_id, uint8_t* ch, uint32_t timeout);
```

### hal_uart_data_available()

**功能**: 檢查是否有資料可讀

```c
bool hal_uart_data_available(hal_uart_id_t uart_id);
```

**返回值**:
- `true`: 有資料可讀
- `false`: 無資料

## SPI API

### 資料型別

```c
// SPI模式
typedef enum {
    HAL_SPI_MODE_MASTER = 0,
    HAL_SPI_MODE_SLAVE
} hal_spi_mode_t;

// SPI時鐘極性
typedef enum {
    HAL_SPI_CPOL_LOW = 0,
    HAL_SPI_CPOL_HIGH
} hal_spi_cpol_t;

// SPI時鐘相位
typedef enum {
    HAL_SPI_CPHA_1EDGE = 0,
    HAL_SPI_CPHA_2EDGE
} hal_spi_cpha_t;

// SPI配置結構體
typedef struct {
    hal_spi_mode_t mode;
    hal_spi_cpol_t cpol;
    hal_spi_cpha_t cpha;
    uint32_t frequency;
} hal_spi_config_t;
```

### hal_spi_init()

**功能**: 初始化SPI

```c
hal_status_t hal_spi_init(hal_spi_id_t spi_id, const hal_spi_config_t* config);
```

### hal_spi_transmit_receive()

**功能**: SPI傳輸資料

```c
hal_status_t hal_spi_transmit_receive(hal_spi_id_t spi_id, const uint8_t* tx_data, 
                                      uint8_t* rx_data, uint16_t size, uint32_t timeout);
```

### hal_spi_transmit()

**功能**: SPI只發送資料

```c
hal_status_t hal_spi_transmit(hal_spi_id_t spi_id, const uint8_t* data, 
                              uint16_t size, uint32_t timeout);
```

### hal_spi_receive()

**功能**: SPI只接收資料

```c
hal_status_t hal_spi_receive(hal_spi_id_t spi_id, uint8_t* data, 
                             uint16_t size, uint32_t timeout);
```

## I2C API

### 資料型別

```c
// I2C配置結構體
typedef struct {
    uint32_t frequency;
    bool master_mode;
} hal_i2c_config_t;
```

### hal_i2c_init()

**功能**: 初始化I2C

```c
hal_status_t hal_i2c_init(hal_i2c_id_t i2c_id, const hal_i2c_config_t* config);
```

### hal_i2c_master_transmit()

**功能**: I2C主機發送資料

```c
hal_status_t hal_i2c_master_transmit(hal_i2c_id_t i2c_id, uint16_t device_addr, 
                                     const uint8_t* data, uint16_t size, uint32_t timeout);
```

### hal_i2c_master_receive()

**功能**: I2C主機接收資料

```c
hal_status_t hal_i2c_master_receive(hal_i2c_id_t i2c_id, uint16_t device_addr, 
                                    uint8_t* data, uint16_t size, uint32_t timeout);
```

### hal_i2c_mem_write()

**功能**: I2C寫入暫存器

```c
hal_status_t hal_i2c_mem_write(hal_i2c_id_t i2c_id, uint16_t device_addr, uint16_t reg_addr, 
                               const uint8_t* data, uint16_t size, uint32_t timeout);
```

### hal_i2c_mem_read()

**功能**: I2C讀取暫存器

```c
hal_status_t hal_i2c_mem_read(hal_i2c_id_t i2c_id, uint16_t device_addr, uint16_t reg_addr, 
                              uint8_t* data, uint16_t size, uint32_t timeout);
```

## ADC API

### 資料型別

```c
// ADC配置結構體
typedef struct {
    uint32_t resolution;
    uint32_t sampling_time;
} hal_adc_config_t;
```

### hal_adc_init()

**功能**: 初始化ADC

```c
hal_status_t hal_adc_init(hal_adc_id_t adc_id, const hal_adc_config_t* config);
```

### hal_adc_read_single()

**功能**: 讀取ADC單次轉換結果

```c
hal_status_t hal_adc_read_single(hal_adc_id_t adc_id, uint32_t channel, 
                                 uint32_t* value, uint32_t timeout);
```

### hal_adc_to_voltage()

**功能**: 將ADC原始值轉換為電壓值

```c
uint32_t hal_adc_to_voltage(uint32_t adc_value, uint32_t resolution, uint32_t vref_mv);
```

**參數**:
- `adc_value`: ADC原始值
- `resolution`: ADC解析度位數
- `vref_mv`: 參考電壓 (毫伏)

**返回值**: 電壓值 (毫伏)

## 使用範例

### 完整的GPIO控制範例

```c
#include "hal.h"

int main(void) {
    // 初始化HAL系統
    if (hal_init() != HAL_OK) {
        return -1;
    }
    
    // 配置LED引腳
    hal_gpio_config_t led_config = {
        .pin = LED_PIN,
        .mode = HAL_GPIO_MODE_OUTPUT,
        .pull = HAL_GPIO_NOPULL,
        .initial_state = HAL_GPIO_LOW
    };
    
    if (hal_gpio_init(&led_config) != HAL_OK) {
        return -1;
    }
    
    // 主循環
    while (1) {
        hal_gpio_write(LED_PIN, HAL_GPIO_HIGH);
        hal_delay_ms(500);
        hal_gpio_write(LED_PIN, HAL_GPIO_LOW);
        hal_delay_ms(500);
    }
    
    return 0;
}
```

### UART通信範例

```c
#include "hal.h"
#include <string.h>

int main(void) {
    hal_init();
    
    // 配置UART
    hal_uart_config_t uart_config = {
        .baudrate = HAL_UART_BAUDRATE_115200,
        .databits = HAL_UART_DATABITS_8,
        .stopbits = HAL_UART_STOPBITS_1,
        .parity = HAL_UART_PARITY_NONE
    };
    
    hal_uart_init(CONSOLE_UART, &uart_config);
    
    const char* message = "Hello, Cross-MCU Framework!\r\n";
    hal_uart_transmit(CONSOLE_UART, (uint8_t*)message, strlen(message), 1000);
    
    return 0;
}
```

這些API提供了完整的硬體抽象，讓您可以編寫跨平台的MCU應用程式。
