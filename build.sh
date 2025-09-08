#!/bin/bash

# Cross-MCU Framework 建置腳本 (Makefile版本)
# 作者: Cross-MCU Framework Team
# 日期: 2024

set -e  # 遇到錯誤時停止執行

# 顏色定義
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# 函式: 顯示使用說明
show_usage() {
    echo "Cross-MCU Framework 建置腳本 (Makefile版本)"
    echo ""
    echo "使用方法: $0 [選項]"
    echo ""
    echo "選項:"
    echo "  -p, --platform PLATFORM    目標平台 (TI_C2000_F28P55X, TI_C2000_F28P65X, STM32G4)"
    echo "  -c, --config CONFIG         建置配置 (Debug, Release)"
    echo "  -t, --target TARGET         建置目標 (all, hal, examples, gpio_blink, uart_echo, clean)"
    echo "  -j, --jobs JOBS             並行編譯工作數"
    echo "  -v, --verbose               顯示詳細編譯資訊"
    echo "  -h, --help                  顯示此說明"
    echo ""
    echo "範例:"
    echo "  $0 -p TI_C2000_F28P55X -c Debug"
    echo "  $0 -p STM32G4 -c Release -t examples"
    echo "  $0 --platform STM32G4 --config Debug --target all"
    echo "  $0 -p STM32G4 -t clean  # 清理建置檔案"
}

# 函式: 顯示訊息
log_info() {
    echo -e "${BLUE}[INFO]${NC} $1"
}

log_success() {
    echo -e "${GREEN}[SUCCESS]${NC} $1"
}

log_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

log_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

# 預設參數
PLATFORM="STM32G4"  # 預設改為STM32G4，因為更容易獲得工具鏈
CONFIG="Debug"
TARGET="all"
JOBS=4
VERBOSE=""

# 解析命令列參數
while [[ $# -gt 0 ]]; do
    case $1 in
        -p|--platform)
            PLATFORM="$2"
            shift 2
            ;;
        -c|--config)
            CONFIG="$2"
            shift 2
            ;;
        -t|--target)
            TARGET="$2"
            shift 2
            ;;
        -j|--jobs)
            JOBS="$2"
            shift 2
            ;;
        -v|--verbose)
            VERBOSE="V=1"
            shift
            ;;
        -h|--help)
            show_usage
            exit 0
            ;;
        *)
            log_error "未知選項: $1"
            show_usage
            exit 1
            ;;
    esac
done

# 檢查平台是否支援
SUPPORTED_PLATFORMS=("TI_C2000_F28P55X" "TI_C2000_F28P65X" "STM32G4")
if [[ ! " ${SUPPORTED_PLATFORMS[@]} " =~ " ${PLATFORM} " ]]; then
    log_error "不支援的平台: $PLATFORM"
    log_info "支援的平台: ${SUPPORTED_PLATFORMS[*]}"
    exit 1
fi

# 檢查配置是否有效
SUPPORTED_CONFIGS=("Debug" "Release")
if [[ ! " ${SUPPORTED_CONFIGS[@]} " =~ " ${CONFIG} " ]]; then
    log_error "無效的配置: $CONFIG"
    log_info "支援的配置: ${SUPPORTED_CONFIGS[*]}"
    exit 1
fi

# 檢查目標是否有效
SUPPORTED_TARGETS=("all" "hal" "examples" "gpio_blink" "uart_echo" "clean" "distclean" "info")
if [[ ! " ${SUPPORTED_TARGETS[@]} " =~ " ${TARGET} " ]]; then
    log_error "無效的目標: $TARGET"
    log_info "支援的目標: ${SUPPORTED_TARGETS[*]}"
    exit 1
fi

# 設置建置目錄
BUILD_DIR="build/${PLATFORM}_${CONFIG}"

log_info "開始建置 Cross-MCU Framework"
log_info "平台: $PLATFORM"
log_info "配置: $CONFIG"
log_info "目標: $TARGET"
log_info "並行工作數: $JOBS"
log_info "建置目錄: $BUILD_DIR"

# 檢查工具鏈
case $PLATFORM in
    TI_C2000_*)
        log_info "檢查TI C2000工具鏈..."
        if command -v cl2000 >/dev/null 2>&1; then
            log_success "找到TI C2000編譯器"
        else
            log_warning "未找到TI C2000編譯器，將使用GCC進行語法檢查"
        fi
        ;;
    STM32G4)
        log_info "檢查ARM GCC工具鏈..."
        if command -v arm-none-eabi-gcc >/dev/null 2>&1; then
            log_success "找到ARM GCC工具鏈"
            arm-none-eabi-gcc --version | head -n 1
        else
            log_warning "未找到ARM GCC工具鏈，將使用預設GCC進行語法檢查"
            log_info "請安裝: sudo apt-get install gcc-arm-none-eabi"
        fi
        ;;
esac

# 執行Make建置
log_info "執行Make建置..."

MAKE_ARGS="TARGET_PLATFORM=$PLATFORM BUILD_TYPE=$CONFIG -j$JOBS $VERBOSE"

if [[ "$TARGET" == "clean" || "$TARGET" == "distclean" ]]; then
    log_info "清理建置檔案..."
    make $TARGET $VERBOSE
else
    # 顯示建置配置資訊
    if [[ "$TARGET" == "info" ]]; then
        make info $MAKE_ARGS
        exit 0
    fi
    
    # 執行建置
    make $TARGET $MAKE_ARGS
fi

if [[ $? -ne 0 ]]; then
    log_error "建置失敗"
    exit 1
fi

log_success "建置完成！"

# 顯示建置結果
if [[ "$TARGET" != "clean" && "$TARGET" != "distclean" ]]; then
    log_info "建置輸出位於: $BUILD_DIR"
    
    # 顯示生成的檔案
    if [[ -d "$BUILD_DIR/bin" ]]; then
        log_info "生成的可執行檔案:"
        ls -la "$BUILD_DIR/bin/" 2>/dev/null || true
    fi
    
    if [[ -d "$BUILD_DIR/lib" ]]; then
        log_info "生成的函式庫檔案:"
        ls -la "$BUILD_DIR/lib/" 2>/dev/null || true
    fi
    
    # 平台特定的後處理資訊
    case $PLATFORM in
        TI_C2000_*)
            log_info "TI C2000平台建置完成"
            if [[ -d "$BUILD_DIR/bin" ]]; then
                log_info "OUT和HEX檔案已生成，可用於Code Composer Studio燒錄"
            fi
            ;;
        STM32G4)
            log_info "STM32G4平台建置完成"
            if [[ -d "$BUILD_DIR/bin" ]]; then
                log_info "ELF, BIN, HEX檔案已生成，可用於ST-Link或OpenOCD燒錄"
                log_info "燒錄命令範例:"
                log_info "  st-flash write $BUILD_DIR/bin/gpio_blink_example.bin 0x08000000"
                log_info "  openocd -f interface/stlink.cfg -f target/stm32g4x.cfg -c \"program $BUILD_DIR/bin/gpio_blink_example.elf verify reset exit\""
            fi
            ;;
    esac
fi

log_success "所有任務完成！"

# 顯示後續步驟建議
if [[ "$TARGET" == "all" || "$TARGET" == "examples" ]]; then
    echo ""
    log_info "後續步驟:"
    echo "  1. 檢查硬體連接 (參考 docs/getting_started.md)"
    echo "  2. 燒錄程式到目標MCU"
    echo "  3. 執行和測試應用程式"
    echo ""
    log_info "更多資訊請查看:"
    echo "  - docs/getting_started.md  (快速開始指南)"
    echo "  - docs/api_reference.md    (API參考)"
    echo "  - docs/architecture.md     (架構文件)"
fi