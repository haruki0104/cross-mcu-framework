# 快速開始指南

本指南將幫助您快速開始使用Cross-MCU Framework進行跨平台MCU開發。

## 系統要求

### 通用要求
- GNU Make 4.0或更高版本
- Git (用於版本控制)

### TI C2000平台
- TI Code Composer Studio (CCS) 12.0或更高版本
- TI C2000 Compiler Tools
- TI C2000Ware軟體包

### STM32G4平台
- ARM GNU Toolchain (arm-none-eabi-gcc)
- STM32CubeG4 HAL庫
- OpenOCD或ST-Link工具 (用於燒錄和除錯)

## 安裝和配置

### 1. 獲取源碼

```bash
git clone <repository-url>
cd cross_mcu_project
```

### 2. 安裝依賴

#### TI C2000平台
1. 下載並安裝Code Composer Studio
2. 確保環境變數設置正確
3. 下載C2000Ware軟體包

#### STM32G4平台
1. 安裝ARM GNU Toolchain:
   ```bash
   # Ubuntu/Debian
   sudo apt-get install gcc-arm-none-eabi
   
   # macOS
   brew install arm-none-eabi-gcc
   
   # Windows
   # 下載並安裝GNU Arm Embedded Toolchain
   ```

2. 下載STM32CubeG4軟體包並解壓到`third_party/STM32CubeG4`目錄

### 3. 建置專案

#### 使用建置腳本 (推薦):

```bash
# STM32G4平台 (Debug) - 預設平台
./build.sh -p STM32G4 -c Debug

# TI C2000 F28P55x平台 (Release)
./build.sh -p TI_C2000_F28P55X -c Release

# 只建置範例程式
./build.sh -p STM32G4 -c Debug -t examples

# 顯示詳細編譯資訊
./build.sh -p STM32G4 -v

# 清理建置檔案
./build.sh -t clean
```

#### 直接使用Makefile:

```bash
# 建置所有目標 (預設STM32G4 Debug)
make

# 指定平台和配置
make TARGET_PLATFORM=STM32G4 BUILD_TYPE=Release

# 只建置HAL函式庫
make hal TARGET_PLATFORM=STM32G4

# 建置特定範例
make gpio_blink TARGET_PLATFORM=STM32G4

# 並行編譯 (4個工作)
make -j4 TARGET_PLATFORM=STM32G4

# 顯示建置配置資訊
make info TARGET_PLATFORM=STM32G4

# 清理建置檔案
make clean
```

## 支援的平台

| 平台 | MCU型號 | 狀態 |
|------|---------|------|
| TI C2000 | F28P55x | ✅ 支援 |
| TI C2000 | F28P65x | ✅ 支援 |
| STM32G4 | STM32G474xx | ✅ 支援 |

## 範例程式

### GPIO LED閃爍
最基本的範例，演示如何使用GPIO控制LED閃爍。

```bash
# 建置GPIO範例
./build.sh -p STM32G4 -c Debug -t gpio_blink

# 或使用Makefile
make gpio_blink TARGET_PLATFORM=STM32G4 BUILD_TYPE=Debug

# 執行檔位於
build/STM32G4_Debug/bin/gpio_blink_example.elf
```

### UART回音
演示UART通信功能，接收資料並回傳。

```bash
# 建置UART範例
./build.sh -p STM32G4 -c Debug -t uart_echo

# 或使用Makefile
make uart_echo TARGET_PLATFORM=STM32G4 BUILD_TYPE=Debug

# 執行檔位於
build/STM32G4_Debug/bin/uart_echo_example.elf
```

## 硬體連接

### TI C2000平台
- LED: 連接到GPIO31
- 按鈕: 連接到GPIO34 (使用內部上拉)
- UART: SCIA (GPIO28-RX, GPIO29-TX)

### STM32G4平台
- LED: 連接到PA5
- 按鈕: 連接到PC13 (使用內部上拉)
- UART: USART2 (PA2-TX, PA3-RX)

## 燒錄程式

### TI C2000平台
使用Code Composer Studio或uniflash工具燒錄生成的.out檔案。

### STM32G4平台
使用OpenOCD、ST-Link或其他支援的工具燒錄生成的.elf或.hex檔案:

```bash
# 使用OpenOCD燒錄
openocd -f interface/stlink.cfg -f target/stm32g4x.cfg -c "program build/STM32G4_Debug/bin/gpio_blink_example.elf verify reset exit"

# 使用st-flash燒錄
st-flash write build/STM32G4_Debug/bin/gpio_blink_example.bin 0x08000000
```

## 除錯

### TI C2000平台
使用Code Composer Studio的內建除錯器。

### STM32G4平台
使用GDB配合OpenOCD或ST-Link:

```bash
# 啟動OpenOCD (終端1)
openocd -f interface/stlink.cfg -f target/stm32g4x.cfg

# 啟動GDB (終端2)
arm-none-eabi-gdb build/STM32G4_Debug/bin/gpio_blink_example.elf
(gdb) target remote localhost:3333
(gdb) monitor reset halt
(gdb) load
(gdb) continue
```

## 常見問題

### 1. 找不到編譯器
確保相應的工具鏈已正確安裝並添加到系統PATH中。

### 2. 缺少HAL庫
確保STM32CubeG4或C2000Ware已正確安裝在指定路徑。

### 3. 燒錄失敗
檢查硬體連接和除錯器配置。

## 下一步

- 閱讀[架構文件](architecture.md)了解框架設計
- 查看[API參考](api_reference.md)了解詳細介面
- 學習[移植指南](porting_guide.md)添加新平台支援

如有問題，請查看[常見問題](faq.md)或提交Issue。
