# 架構設計文件

Cross-MCU Framework採用分層架構設計，提供統一的API介面，隱藏不同MCU平台的硬體差異。

## 整體架構

```
┌─────────────────────────────────────────────────────────┐
│                   應用程式層                              │
│                (Application Layer)                      │
├─────────────────────────────────────────────────────────┤
│                   中介軟體層                              │
│                (Middleware Layer)                       │
├─────────────────────────────────────────────────────────┤
│                   驅動程式層                              │
│                 (Driver Layer)                          │
├─────────────────────────────────────────────────────────┤
│                硬體抽象層 (HAL)                           │
│              Hardware Abstraction Layer                 │
├─────────────────┬───────────────────┬───────────────────┤
│   TI C2000      │     STM32G4       │   其他平台...      │
│   Platform      │     Platform      │   Other Platforms │
└─────────────────┴───────────────────┴───────────────────┘
```

## 設計原則

### 1. 分層設計
- **應用程式層**: 使用者應用邏輯，與硬體無關
- **中介軟體層**: 通用功能模組 (RTOS、通信協定等)
- **驅動程式層**: 特定設備驅動程式
- **硬體抽象層**: 統一的硬體介面

### 2. 介面統一
所有平台使用相同的API介面，確保程式碼可移植性。

### 3. 條件編譯
使用預處理器巨集處理平台特定代碼。

### 4. 配置靈活
支援編譯時和執行時配置。

## 硬體抽象層 (HAL)

HAL是框架的核心，提供統一的硬體介面。

### 模組結構

```
src/hal/
├── include/              # 公共標頭檔
│   ├── hal.h            # 主要HAL介面
│   ├── hal_common.h     # 通用定義和型別
│   ├── hal_gpio.h       # GPIO介面
│   ├── hal_uart.h       # UART介面
│   ├── hal_spi.h        # SPI介面
│   ├── hal_i2c.h        # I2C介面
│   └── hal_adc.h        # ADC介面
├── ti_c2000/            # TI C2000平台實現
│   ├── ti_c2000_common.h
│   ├── ti_c2000_gpio.c
│   ├── ti_c2000_uart.c
│   └── ti_c2000_system.c
└── stm32g4/             # STM32G4平台實現
    ├── stm32g4_common.h
    ├── stm32g4_gpio.c
    ├── stm32g4_uart.c
    └── stm32g4_system.c
```

### 平台識別

使用預處理器巨集識別目標平台:

```c
#if defined(F28P55X) || defined(F28P65X)
    #define PLATFORM_TI_C2000
#elif defined(STM32G4)
    #define PLATFORM_STM32
#endif
```

### 型別抽象

針對不同平台定義適當的資料型別:

```c
// GPIO引腳型別
#ifdef PLATFORM_TI_C2000
    typedef uint32_t hal_gpio_pin_t;
#elif defined(PLATFORM_STM32)
    typedef uint16_t hal_gpio_pin_t;
#endif

// UART識別碼型別
#ifdef PLATFORM_TI_C2000
    typedef uint32_t hal_uart_id_t;
#elif defined(PLATFORM_STM32)
    typedef void* hal_uart_id_t;
#endif
```

## 建置系統

### CMake配置

使用CMake作為建置系統，支援跨平台編譯:

```cmake
# 平台選擇
set(TARGET_PLATFORM "STM32G4" CACHE STRING "Target MCU Platform")

# 平台特定配置
if(TARGET_PLATFORM MATCHES "TI_C2000")
    include(cmake/ti_c2000.cmake)
elseif(TARGET_PLATFORM STREQUAL "STM32G4")
    include(cmake/stm32g4.cmake)
endif()
```

### 工具鏈配置

每個平台都有專用的工具鏈配置檔案:

- `cmake/ti_c2000.cmake`: TI C2000工具鏈配置
- `cmake/stm32g4.cmake`: ARM GCC工具鏈配置

## API設計

### 統一介面

所有HAL模組都遵循統一的介面設計模式:

```c
// 初始化函式
hal_status_t hal_module_init(hal_module_id_t id, const hal_module_config_t* config);

// 反初始化函式
hal_status_t hal_module_deinit(hal_module_id_t id);

// 功能函式
hal_status_t hal_module_function(hal_module_id_t id, ...);

// 狀態查詢函式
bool hal_module_is_ready(hal_module_id_t id);
```

### 錯誤處理

使用統一的錯誤狀態碼:

```c
typedef enum {
    HAL_OK = 0,
    HAL_ERROR,
    HAL_BUSY,
    HAL_TIMEOUT,
    HAL_INVALID_PARAM
} hal_status_t;
```

### 回呼函式

支援事件驅動的回呼機制:

```c
typedef void (*hal_callback_t)(void* context);

hal_status_t hal_module_register_callback(hal_module_id_t id, 
                                          hal_callback_t callback, 
                                          void* context);
```

## 記憶體管理

### 靜態配置

預設使用靜態記憶體配置，避免動態記憶體分配的問題。

### 緩衝區管理

提供統一的緩衝區管理介面:

```c
typedef struct {
    uint8_t* data;
    uint16_t size;
    uint16_t head;
    uint16_t tail;
} hal_buffer_t;
```

## 中斷處理

### 中斷抽象

提供統一的中斷處理介面:

```c
typedef void (*hal_irq_handler_t)(void);

hal_status_t hal_irq_register(uint32_t irq_num, hal_irq_handler_t handler);
hal_status_t hal_irq_enable(uint32_t irq_num);
hal_status_t hal_irq_disable(uint32_t irq_num);
```

### 平台特定實現

每個平台實現自己的中斷向量表和處理機制。

## 時鐘管理

### 時鐘抽象

提供統一的時鐘配置和查詢介面:

```c
uint32_t hal_get_system_clock(void);
uint32_t hal_get_peripheral_clock(hal_peripheral_t peripheral);
hal_status_t hal_set_peripheral_clock(hal_peripheral_t peripheral, uint32_t frequency);
```

## 電源管理

### 低功耗支援

提供統一的電源管理介面:

```c
hal_status_t hal_enter_sleep_mode(hal_sleep_mode_t mode);
hal_status_t hal_enter_stop_mode(void);
hal_status_t hal_enter_standby_mode(void);
```

## 除錯支援

### 除錯介面

提供統一的除錯輸出介面:

```c
void hal_debug_printf(const char* format, ...);
void hal_debug_assert(bool condition, const char* message);
```

## 效能考慮

### 內聯函式

關鍵路徑函式使用內聯實現以提高效能:

```c
static inline void hal_gpio_set_fast(hal_gpio_pin_t pin) {
    // 平台特定的快速實現
}
```

### 編譯時最佳化

使用條件編譯移除未使用的功能:

```c
#if defined(HAL_UART_ENABLED)
    // UART相關代碼
#endif
```

## 測試策略

### 單元測試

每個HAL模組都有對應的單元測試:

```
tests/
├── test_hal_gpio.c
├── test_hal_uart.c
└── test_hal_system.c
```

### 硬體在環測試

支援硬體在環 (HIL) 測試驗證實際硬體功能。

## 擴展性

### 新平台移植

添加新平台支援的步驟:

1. 建立平台特定目錄
2. 實現HAL介面函式
3. 添加CMake配置檔案
4. 更新建置腳本

### 新功能模組

添加新HAL模組的步驟:

1. 定義統一介面
2. 實現平台特定功能
3. 添加測試用例
4. 更新文件

這種架構設計確保了框架的可擴展性、可維護性和跨平台相容性。
