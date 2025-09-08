# CMake到Makefile遷移說明

本文件記錄了Cross-MCU Framework從CMake建置系統遷移到Makefile建置系統的變更。

## 遷移概要

### 變更內容
- ✅ 專案從 `/tmp/cross_mcu_project` 遷移到 `/home/patrick/Workspace/projects/cross_mcu_framework`
- ✅ 建置系統從 CMake 改為 GNU Make
- ✅ 移除所有CMake相關檔案 (`CMakeLists.txt`, `cmake/` 目錄)
- ✅ 建立新的Makefile建置系統
- ✅ 更新建置腳本和文件

### 新建立的檔案
- `Makefile` - 主要建置檔案
- `makefiles/ti_c2000_f28p55x.mk` - TI C2000 F28P55x平台配置
- `makefiles/ti_c2000_f28p65x.mk` - TI C2000 F28P65x平台配置  
- `makefiles/stm32g4.mk` - STM32G4平台配置
- `examples/gpio_blink/Makefile` - GPIO範例Makefile
- `examples/uart_echo/Makefile` - UART範例Makefile

### 更新的檔案
- `build.sh` - 更新為使用Makefile
- `README.md` - 更新建置說明
- `docs/getting_started.md` - 更新快速開始指南

## 建置系統比較

### CMake版本 (舊)
```bash
mkdir build
cd build
cmake .. -DTARGET_PLATFORM=STM32G4
make -j4
```

### Makefile版本 (新)
```bash
# 使用建置腳本
./build.sh -p STM32G4 -c Debug

# 直接使用Make
make TARGET_PLATFORM=STM32G4 BUILD_TYPE=Debug -j4
```

## 優勢

### Makefile系統的優勢
1. **更簡單**: 不需要額外安裝CMake
2. **更直觀**: 建置邏輯更容易理解和修改
3. **更快速**: 減少配置步驟，直接編譯
4. **更靈活**: 平台特定配置更容易管理
5. **更標準**: Make是Unix/Linux標準建置工具

### 功能對比
| 功能 | CMake版本 | Makefile版本 |
|------|-----------|--------------|
| 跨平台建置 | ✅ | ✅ |
| 平台自動檢測 | ✅ | ✅ |
| 工具鏈配置 | ✅ | ✅ |
| 並行編譯 | ✅ | ✅ |
| Debug/Release | ✅ | ✅ |
| 清理功能 | ✅ | ✅ |
| 安裝功能 | ✅ | ✅ |
| 配置複雜度 | 高 | 中 |
| 學習曲線 | 陡峭 | 平緩 |

## 使用方法

### 基本建置
```bash
# 建置所有目標 (預設STM32G4 Debug)
make

# 指定平台
make TARGET_PLATFORM=TI_C2000_F28P55X

# 指定配置
make BUILD_TYPE=Release

# 組合使用
make TARGET_PLATFORM=STM32G4 BUILD_TYPE=Release -j4
```

### 建置腳本
```bash
# 基本使用
./build.sh

# 指定平台和配置
./build.sh -p STM32G4 -c Release

# 只建置特定目標
./build.sh -t gpio_blink

# 顯示詳細資訊
./build.sh -v

# 清理
./build.sh -t clean
```

### 可用目標
- `all` - 建置HAL函式庫和所有範例 (預設)
- `hal` - 只建置HAL函式庫
- `examples` - 建置所有範例程式
- `gpio_blink` - 建置GPIO閃爍範例
- `uart_echo` - 建置UART回音範例
- `clean` - 清理建置檔案
- `distclean` - 深度清理
- `install` - 安裝函式庫和標頭檔
- `info` - 顯示建置配置資訊
- `help` - 顯示幫助資訊

## 平台支援

### 支援的平台
1. **TI_C2000_F28P55X** - TI C2000 F28P55x系列
2. **TI_C2000_F28P65X** - TI C2000 F28P65x系列  
3. **STM32G4** - STM32G4系列 (預設)

### 工具鏈要求
- **TI C2000**: Code Composer Studio (CCS) 或使用GCC語法檢查
- **STM32G4**: ARM GCC工具鏈 或使用預設GCC語法檢查

## 目錄結構

```
cross_mcu_framework/
├── Makefile                   # 主要建置檔案
├── build.sh                   # 建置腳本
├── makefiles/                 # 平台特定Makefile
│   ├── ti_c2000_f28p55x.mk
│   ├── ti_c2000_f28p65x.mk
│   └── stm32g4.mk
├── src/hal/                   # HAL源碼
├── examples/                  # 範例程式 (每個都有自己的Makefile)
├── docs/                      # 文件
└── build/                     # 建置輸出目錄
```

## 注意事項

1. **工具鏈**: 如果沒有安裝對應的工具鏈，系統會自動回退到GCC進行語法檢查
2. **HAL庫**: STM32平台需要下載STM32CubeG4軟體包到 `third_party/STM32CubeG4`
3. **並行編譯**: 使用 `-j` 參數可以加速編譯過程
4. **清理**: 使用 `make clean` 或 `./build.sh -t clean` 清理建置檔案

## 遷移完成

✅ 專案已成功遷移到Makefile建置系統，所有功能正常運作。
✅ 建置腳本和文件已更新以反映新的建置系統。
✅ 保持了原有的跨平台支援和功能特性。

現在您可以在新的位置 `/home/patrick/Workspace/projects/cross_mcu_framework` 使用Makefile建置系統進行開發！
