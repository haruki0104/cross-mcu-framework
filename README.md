# Cross-MCU Framework

這是一個支持多種MCU平台的開發框架，允許開發者使用同一份程式碼在不同的MCU平台上運行。

## 🎯 專案特色

- **跨平台支援**: 統一的HAL介面，支援多種MCU平台
- **分層架構**: 清晰的分層設計，易於維護和擴展
- **模組化設計**: 獨立的功能模組，提高代碼可重用性
- **建置系統**: 完整的Makefile建置系統，支援多平台編譯

## 🔧 支援的MCU平台

### TI C2000系列
- ✅ F28P55x (120MHz)
- ✅ F28P65x (200MHz)

### STM32系列
- 🔄 STM32G4 (規劃中)

## 📁 專案架構

```
cross_mcu_framework/
├── src/
│   └── hal/                   # 硬體抽象層
│       ├── include/           # HAL公共標頭檔
│       ├── ti_c2000/          # TI C2000平台實現
│       └── stm32g4/           # STM32G4平台實現
├── examples/                  # 範例程式
│   ├── gpio_blink/           # GPIO LED閃爍範例
│   └── uart_echo/            # UART回音範例
├── makefiles/                 # Makefile配置檔案
├── build/                     # 建置輸出目錄
├── docs/                      # 文件
├── Makefile                   # 主要建置檔案
└── build.sh                   # 建置腳本
```

## 🚀 快速開始

### 系統要求

**TI C2000平台**:
- TI Code Composer Studio (CCS) 12.0或更高版本
- TI C2000 Compiler Tools
- TI C2000Ware軟體包

**STM32G4平台**:
- ARM GNU Toolchain (arm-none-eabi-gcc)
- STM32CubeG4 HAL庫

### 建置專案

```bash
# 使用建置腳本 (推薦)
./build.sh -p TI_C2000_F28P55X -c Debug

# 或直接使用Makefile
make TARGET_PLATFORM=TI_C2000_F28P55X BUILD_TYPE=Debug
```

### 範例程式

**GPIO LED閃爍**:
```bash
make gpio_blink TARGET_PLATFORM=TI_C2000_F28P55X BUILD_TYPE=Debug
```

**UART回音**:
```bash
make uart_echo TARGET_PLATFORM=TI_C2000_F28P55X BUILD_TYPE=Debug
```

## 📊 建置輸出

成功建置後會生成：
- `.out` 檔案：用於除錯和分析
- `.hex` 檔案：用於燒錄到開發板

## 🔌 硬體連接

### TI C2000平台
- LED: 連接到GPIO31
- 按鈕: 連接到GPIO34 (使用內部上拉)
- UART: SCIA (GPIO28-RX, GPIO29-TX)

## 📚 文件

- [快速開始指南](docs/getting_started.md)
- [架構文件](docs/architecture.md)
- [API參考](docs/api_reference.md)

## 🤝 貢獻

歡迎提交Issue和Pull Request來改進這個專案！

## 📄 授權

本專案採用 MIT 授權條款。

## 🔧 開發狀態

- [x] TI C2000 F28P55X 支援
- [x] TI C2000 F28P65X 支援
- [x] GPIO HAL實現
- [x] UART HAL實現
- [x] 建置系統
- [ ] STM32G4 完整支援
- [ ] SPI HAL實現
- [ ] I2C HAL實現
- [ ] ADC HAL實現

## 📞 聯繫

如有問題或建議，請提交Issue或聯繫開發團隊。