# TI C2000 DriverLib 整合策略

## 🎯 整合目標

將TI C2000Ware DriverLib完全整合到Cross-MCU Framework的HAL層中，提供更完整和穩定的硬體抽象。

## 📊 現況分析

### 目前的實現狀況

| 模組 | 簡化版本 | DriverLib版本 | 狀態 |
|------|----------|---------------|------|
| GPIO | ✅ 已實現 | ✅ 已實現 | 🔄 需要整合 |
| UART | ✅ 已實現 | ✅ 已實現 | 🔄 需要整合 |
| System | ✅ 已實現 | ❌ 未實現 | 🔧 需要開發 |
| SPI | ❌ 未實現 | ❌ 未實現 | 📋 計劃中 |
| I2C | ❌ 未實現 | ❌ 未實現 | 📋 計劃中 |
| ADC | ❌ 未實現 | ❌ 未實現 | 📋 計劃中 |

## 🏗️ 整合策略

### 1. 分階段整合方法

#### 階段一：基礎整合 (立即執行)
- 統一DriverLib依賴管理
- 創建DriverLib包裝層
- 建立編譯時選擇機制

#### 階段二：核心模組整合
- GPIO模組完整DriverLib整合
- UART模組完整DriverLib整合
- System模組DriverLib實現

#### 階段三：擴展模組整合
- SPI模組DriverLib實現
- I2C模組DriverLib實現
- ADC模組DriverLib實現

### 2. 架構設計

```
src/hal/ti_c2000/
├── driverlib/                    # DriverLib包裝層
│   ├── ti_c2000_driverlib.h     # DriverLib統一標頭檔
│   ├── ti_c2000_gpio_dl.c       # GPIO DriverLib實現
│   ├── ti_c2000_uart_dl.c       # UART DriverLib實現
│   ├── ti_c2000_spi_dl.c        # SPI DriverLib實現
│   ├── ti_c2000_i2c_dl.c        # I2C DriverLib實現
│   └── ti_c2000_adc_dl.c        # ADC DriverLib實現
├── simple/                       # 簡化版本實現
│   ├── ti_c2000_gpio_simple.c   # 現有簡化實現
│   ├── ti_c2000_uart_simple.c   # 現有簡化實現
│   └── ti_c2000_system_simple.c # 現有簡化實現
├── ti_c2000_common.h            # 平台公共定義
└── ti_c2000_config.h            # 編譯配置選擇
```

### 3. 編譯時選擇機制

```c
// ti_c2000_config.h
#ifndef TI_C2000_CONFIG_H
#define TI_C2000_CONFIG_H

// DriverLib整合配置
#define TI_C2000_USE_DRIVERLIB      1    // 0=簡化版本, 1=DriverLib版本
#define TI_C2000_DRIVERLIB_VERSION  "4.04.00.00"

// 模組選擇配置
#if TI_C2000_USE_DRIVERLIB
    #define TI_C2000_GPIO_USE_DRIVERLIB     1
    #define TI_C2000_UART_USE_DRIVERLIB     1
    #define TI_C2000_SPI_USE_DRIVERLIB      1
    #define TI_C2000_I2C_USE_DRIVERLIB      1
    #define TI_C2000_ADC_USE_DRIVERLIB      1
#else
    #define TI_C2000_GPIO_USE_DRIVERLIB     0
    #define TI_C2000_UART_USE_DRIVERLIB     0
    #define TI_C2000_SPI_USE_DRIVERLIB      0
    #define TI_C2000_I2C_USE_DRIVERLIB      0
    #define TI_C2000_ADC_USE_DRIVERLIB      0
#endif

#endif
```

## 🔧 實施步驟

### 步驟1: 創建DriverLib包裝層

```c
// src/hal/ti_c2000/driverlib/ti_c2000_driverlib.h
#ifndef TI_C2000_DRIVERLIB_H
#define TI_C2000_DRIVERLIB_H

#include "../ti_c2000_config.h"

#if TI_C2000_USE_DRIVERLIB
    // 包含C2000Ware DriverLib標頭檔
    #include "device.h"
    #include "driverlib.h"
    
    // DriverLib版本檢查
    #if !defined(DRIVERLIB_VERSION_NUMBER)
        #error "DriverLib version not detected"
    #endif
    
    // 定義DriverLib包裝函數
    #define TI_GPIO_INIT            GPIO_setPinConfig
    #define TI_GPIO_SET_DIR         GPIO_setDirectionMode
    #define TI_GPIO_WRITE           GPIO_writePin
    #define TI_GPIO_READ            GPIO_readPin
    
    #define TI_UART_INIT            SCI_setConfig
    #define TI_UART_ENABLE          SCI_enableModule
    #define TI_UART_WRITE           SCI_writeCharBlockingFIFO
    #define TI_UART_READ            SCI_readCharBlockingFIFO
    
#else
    // 簡化版本的宏定義
    #define TI_GPIO_INIT(...)       ti_gpio_simple_init(__VA_ARGS__)
    #define TI_GPIO_SET_DIR(...)    ti_gpio_simple_set_dir(__VA_ARGS__)
    #define TI_GPIO_WRITE(...)      ti_gpio_simple_write(__VA_ARGS__)
    #define TI_GPIO_READ(...)       ti_gpio_simple_read(__VA_ARGS__)
#endif

#endif
```

### 步驟2: 更新Makefile配置

```makefile
# makefiles/ti_c2000_f28p55x.mk

# DriverLib配置
TI_C2000_USE_DRIVERLIB ?= 1

ifeq ($(TI_C2000_USE_DRIVERLIB),1)
    # DriverLib版本的源檔案
    PLATFORM_HAL_SOURCES := ti_c2000/driverlib/ti_c2000_gpio_dl.c \
                            ti_c2000/driverlib/ti_c2000_uart_dl.c \
                            ti_c2000/driverlib/ti_c2000_system_dl.c
    
    # DriverLib特定的編譯定義
    PLATFORM_DEFINES += --define=TI_C2000_USE_DRIVERLIB=1
    
    # 檢查C2000Ware路徑
    ifneq ($(wildcard $(C2000WARE_PATH)),)
        PLATFORM_INCLUDE_DIRS += -I$(DEVICE_SUPPORT_PATH)/common/include
        PLATFORM_INCLUDE_DIRS += -I$(DEVICE_SUPPORT_PATH)/headers/include
        PLATFORM_INCLUDE_DIRS += -I$(DRIVERLIB_PATH)
        PLATFORM_LIBS += -L$(DRIVERLIB_PATH) -ldriverlib.lib
    else
        $(warning C2000Ware not found, falling back to simple implementation)
        TI_C2000_USE_DRIVERLIB := 0
    endif
else
    # 簡化版本的源檔案
    PLATFORM_HAL_SOURCES := ti_c2000/simple/ti_c2000_gpio_simple.c \
                            ti_c2000/simple/ti_c2000_uart_simple.c \
                            ti_c2000/simple/ti_c2000_system_simple.c
    
    PLATFORM_DEFINES += --define=TI_C2000_USE_DRIVERLIB=0
endif
```

### 步驟3: 實現DriverLib版本的GPIO

```c
// src/hal/ti_c2000/driverlib/ti_c2000_gpio_dl.c
#include "../../include/hal_gpio.h"
#include "../ti_c2000_common.h"
#include "ti_c2000_driverlib.h"

#if TI_C2000_GPIO_USE_DRIVERLIB

hal_status_t hal_gpio_init(const hal_gpio_config_t* config)
{
    if (config == NULL) {
        return HAL_INVALID_PARAM;
    }
    
    hal_gpio_pin_t pin = config->pin;
    
    // 檢查引腳範圍
    if (pin > 168) {
        return HAL_INVALID_PARAM;
    }
    
    EALLOW;
    
    // 設置GPIO功能 (非複用功能)
    GPIO_setPinConfig(GPIO_0_GPIO0 + pin);
    
    // 設置方向
    if (config->mode == HAL_GPIO_MODE_OUTPUT) {
        GPIO_setDirectionMode(pin, GPIO_DIR_MODE_OUT);
        GPIO_writePin(pin, (config->initial_state == HAL_GPIO_HIGH) ? 1 : 0);
    } else {
        GPIO_setDirectionMode(pin, GPIO_DIR_MODE_IN);
    }
    
    // 設置上拉/下拉
    switch (config->pull) {
        case HAL_GPIO_PULLUP:
            GPIO_setPadConfig(pin, GPIO_PIN_TYPE_PULLUP);
            break;
        case HAL_GPIO_PULLDOWN:
            // C2000沒有內建下拉電阻
            break;
        case HAL_GPIO_NOPULL:
        default:
            GPIO_setPadConfig(pin, GPIO_PIN_TYPE_STD);
            break;
    }
    
    EDIS;
    
    return HAL_OK;
}

hal_status_t hal_gpio_write(hal_gpio_pin_t pin, hal_gpio_state_t state)
{
    if (pin > 168) {
        return HAL_INVALID_PARAM;
    }
    
    GPIO_writePin(pin, (state == HAL_GPIO_HIGH) ? 1 : 0);
    
    return HAL_OK;
}

hal_gpio_state_t hal_gpio_read(hal_gpio_pin_t pin)
{
    if (pin > 168) {
        return HAL_GPIO_LOW;
    }
    
    return (GPIO_readPin(pin) != 0) ? HAL_GPIO_HIGH : HAL_GPIO_LOW;
}

hal_status_t hal_gpio_toggle(hal_gpio_pin_t pin)
{
    if (pin > 168) {
        return HAL_INVALID_PARAM;
    }
    
    GPIO_togglePin(pin);
    
    return HAL_OK;
}

#endif // TI_C2000_GPIO_USE_DRIVERLIB
```

## 📋 實施計劃

### 立即可執行的任務

1. **創建配置系統**
   ```bash
   mkdir -p src/hal/ti_c2000/driverlib
   mkdir -p src/hal/ti_c2000/simple
   # 移動現有簡化實現到simple目錄
   ```

2. **更新建置系統**
   - 修改Makefile支援DriverLib/Simple選擇
   - 添加C2000Ware依賴檢查

3. **實現DriverLib版本**
   - GPIO DriverLib實現
   - UART DriverLib實現

### 中期目標

1. **擴展功能模組**
   - SPI DriverLib實現
   - I2C DriverLib實現
   - ADC DriverLib實現

2. **測試和驗證**
   - 創建DriverLib版本的測試用例
   - 性能比較測試

### 長期目標

1. **文檔和範例**
   - DriverLib整合文檔
   - 進階功能範例

2. **持續整合**
   - 自動化測試
   - 多版本DriverLib支援

## 🎯 優勢分析

### DriverLib版本優勢
- ✅ **穩定性**: TI官方維護，經過充分測試
- ✅ **完整性**: 支援所有硬體功能和配置選項
- ✅ **效能**: 優化的寄存器操作和中斷處理
- ✅ **維護性**: 自動跟隨TI更新

### 簡化版本優勢
- ✅ **輕量級**: 較小的代碼體積
- ✅ **獨立性**: 不依賴外部庫
- ✅ **可控性**: 完全掌控實現細節
- ✅ **學習性**: 適合理解硬體操作原理

## 🚀 建議的實施順序

1. **Phase 1** (1-2週): 建立配置框架和GPIO DriverLib整合
2. **Phase 2** (2-3週): UART DriverLib整合和測試
3. **Phase 3** (3-4週): SPI/I2C/ADC DriverLib實現
4. **Phase 4** (1週): 文檔和範例完善

這種整合策略既保持了現有代碼的相容性，又提供了使用TI官方DriverLib的選項，讓開發者可以根據專案需求選擇最適合的實現方式。
