# Cross-MCU Framework

一個支持多種MCU平台的跨平台開發框架，允許開發者使用同一份程式碼在不同的MCU平台上運行。

## 🚀 特性

- **跨平台支援**: 支援TI C2000和STM32系列MCU
- **硬體抽象層**: 統一的HAL介面，屏蔽硬體差異
- **模組化設計**: 獨立的功能模組，提高代碼重用性
- **DriverLib整合**: 支援TI官方DriverLib和簡化實現兩種模式
- **靈活建置**: 支援多種建置配置和目標平台

## 📋 支援的平台

| 平台 | MCU型號 | 狀態 | DriverLib支援 |
|------|---------|------|---------------|
| TI C2000 | F28P55x | ✅ 支援 | ✅ 完整支援 |
| TI C2000 | F28P65x | ✅ 支援 | ✅ 完整支援 |
| STM32G4 | STM32G474xx | 🚧 開發中 | N/A |

## 🏗️ 專案架構

```
cross_mcu_framework/
├── src/
│   └── hal/                      # 硬體抽象層
│       ├── include/              # HAL公共標頭檔
│       ├── ti_c2000/             # TI C2000平台實現
│       │   ├── simple/           # 簡化實現 (不依賴DriverLib)
│       │   ├── driverlib/        # DriverLib實現
│       │   └── ti_c2000_config.h # 平台配置
│       └── stm32g4/              # STM32G4平台實現
├── examples/                     # 範例程式
│   ├── gpio_blink/               # GPIO LED閃爍範例
│   └── uart_echo/                # UART回音範例
├── makefiles/                    # 平台特定Makefile
├── docs/                         # 文件
├── build/                        # 建置輸出目錄
└── tools/                        # 工具腳本
```

## 🛠️ 環境需求

### 通用要求
- GNU Make 4.0+
- Git

### TI C2000平台
- TI Code Composer Studio 12.0+
- TI C2000 Compiler Tools
- TI C2000Ware軟體包 (可選，用於DriverLib)

### STM32G4平台
- ARM GNU Toolchain (arm-none-eabi-gcc)
- STM32CubeG4 HAL庫

## 🚀 快速開始

### 1. 克隆專案

```bash
git clone https://github.com/your-username/cross-mcu-framework.git
cd cross-mcu-framework
```

### 2. 建置專案

使用建置腳本（推薦）：

```bash
# TI C2000 F28P55x平台 (Debug)
./build.sh -p TI_C2000_F28P55X -c Debug

# 使用DriverLib版本
./build.sh -p TI_C2000_F28P55X -c Debug -t all -D TI_C2000_USE_DRIVERLIB=1
```

使用Makefile：

```bash
# 建置所有目標
make TARGET_PLATFORM=TI_C2000_F28P55X BUILD_TYPE=Debug

# 只建置GPIO範例
make gpio_blink TARGET_PLATFORM=TI_C2000_F28P55X
```


### 3. 燒錄程式

生成的文件位於 `build/TARGET_PLATFORM_BUILD_TYPE/bin/`：

- `.out` 文件：用於Code Composer Studio除錯
- `.hex` 文件：用於燒錄工具

#### TI C2000 F28P55x MCU 燒錄教學

本專案提供 `program.sh` 腳本，搭配 `ti_c2000_f28p55x.ccxml` 設定檔，可自動將編譯產生的檔案燒錄至開發板。

**需求：**
- TI Code Composer Studio (CCS) 或 Debug Server Scripting Lite (dslite)
- `ti_c2000_f28p55x.ccxml` 設定檔（已隨專案提供）
- 欲燒錄的 `.hex` 或 `.out` 檔案

**使用方式：**
1. 確認 `dslite.sh` 已安裝（預設路徑為 `/opt/ti/ccs_base/DebugServer/bin/dslite.sh`），如有不同請編輯 `program.sh` 內的 `DSLite_PATH` 變數。
2. 執行腳本：
	```bash
	./program.sh build/TI_C2000_F28P55X_Debug/bin/gpio_blink_example.out
	```
3. 腳本會自動燒錄並驗證檔案。

如遇錯誤請檢查檔案路徑與工具安裝情形。

## 📖 DriverLib整合

本框架支援兩種實現模式：

### 簡化模式 (預設)
- 不依賴TI C2000Ware
- 基本功能實現
- 編譯快速，依賴少

### DriverLib模式
- 使用TI官方DriverLib
- 完整功能支援
- 需要安裝C2000Ware

切換到DriverLib模式：

```bash
# 方法1: 建置時指定
make TARGET_PLATFORM=TI_C2000_F28P55X TI_C2000_USE_DRIVERLIB=1

# 方法2: 修改配置文件
# 編輯 src/hal/ti_c2000/ti_c2000_config.h
#define TI_C2000_USE_DRIVERLIB  1
```

## 📚 範例程式

### GPIO LED閃爍
```bash
make gpio_blink TARGET_PLATFORM=TI_C2000_F28P55X
```

演示基本的GPIO控制，LED以500ms間隔閃爍。

### UART回音
```bash
make uart_echo TARGET_PLATFORM=TI_C2000_F28P55X
```

演示UART通信，接收資料並回傳。

## 🔧 硬體連接

### TI C2000 F28P55x LaunchPad
- **LED**: GPIO31 (板載LED)
- **按鈕**: GPIO34 (板載按鈕)
- **UART**: SCIA (GPIO28-RX, GPIO29-TX)

### STM32G4 Nucleo
- **LED**: PA5 (板載LED)
- **按鈕**: PC13 (板載按鈕)
- **UART**: USART2 (PA2-TX, PA3-RX)

## 📋 API參考

### HAL GPIO
```c
hal_status_t hal_gpio_init(const hal_gpio_config_t* config);
hal_status_t hal_gpio_write(hal_gpio_pin_t pin, hal_gpio_state_t state);
hal_gpio_state_t hal_gpio_read(hal_gpio_pin_t pin);
hal_status_t hal_gpio_toggle(hal_gpio_pin_t pin);
```

### HAL UART
```c
hal_status_t hal_uart_init(hal_uart_id_t uart_id, const hal_uart_config_t* config);
hal_status_t hal_uart_transmit(hal_uart_id_t uart_id, const uint8_t* data, uint16_t size, uint32_t timeout);
hal_status_t hal_uart_receive(hal_uart_id_t uart_id, uint8_t* data, uint16_t size, uint32_t timeout);
```

詳細API文件請參考 [docs/api_reference.md](docs/api_reference.md)

## 🧪 測試

```bash
# 驗證建置結果
./verify_build.sh

# 執行所有測試
make test TARGET_PLATFORM=TI_C2000_F28P55X
```

## 🤝 貢獻

歡迎提交Pull Request和Issue！

1. Fork專案
2. 創建功能分支 (`git checkout -b feature/amazing-feature`)
3. 提交更改 (`git commit -m 'Add amazing feature'`)
4. 推送分支 (`git push origin feature/amazing-feature`)
5. 開啟Pull Request

## 📄 授權

本專案採用MIT授權 - 詳見 [LICENSE](LICENSE) 文件

## 🙏 致謝

- Texas Instruments for C2000Ware and DriverLib
- STMicroelectronics for STM32Cube HAL
- 所有貢獻者

## 📞 聯繫

如有問題或建議，請：
- 提交Issue
- 聯繫維護者: [your-email@example.com]

---

**注意**: 本框架仍在積極開發中，API可能會有變更。建議在生產環境使用前進行充分測試。
