#!/bin/bash

# Cross-MCU Framework DriverLib建置腳本
# 用於測試和使用DriverLib整合功能

set -e

# 顏色定義
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

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
PLATFORM="TI_C2000_F28P55X"
CONFIG="Debug"
TARGET="all"
USE_DRIVERLIB=1

log_info "Cross-MCU Framework DriverLib建置腳本"
log_info "平台: $PLATFORM"
log_info "配置: $CONFIG"
log_info "使用DriverLib: $USE_DRIVERLIB"

# 檢查C2000Ware環境
C2000WARE_PATH="/opt/ti/c2000/C2000Ware_6_00_00_00"
if [ -d "$C2000WARE_PATH" ]; then
    log_success "找到C2000Ware: $C2000WARE_PATH"
else
    log_warning "未找到C2000Ware，將回退到簡化實現"
    USE_DRIVERLIB=0
fi

# 檢查TI編譯器
TI_CCS_PATH="/opt/ti/ccs/ti-cgt-c2000_22.6.2.LTS"
if [ -f "$TI_CCS_PATH/bin/cl2000" ]; then
    log_success "找到TI C2000編譯器: $TI_CCS_PATH"
else
    log_error "未找到TI C2000編譯器"
    log_info "請安裝Code Composer Studio或設置正確的路徑"
    exit 1
fi

# 執行建置
log_info "開始建置..."

if [ "$USE_DRIVERLIB" = "1" ]; then
    log_info "使用DriverLib實現建置"
    make clean
    make TARGET_PLATFORM=$PLATFORM BUILD_TYPE=$CONFIG TI_C2000_USE_DRIVERLIB=1 -j4
else
    log_info "使用簡化實現建置"
    make clean  
    make TARGET_PLATFORM=$PLATFORM BUILD_TYPE=$CONFIG TI_C2000_USE_DRIVERLIB=0 -j4
fi

if [ $? -eq 0 ]; then
    log_success "建置完成！"
    
    # 顯示建置結果
    BUILD_DIR="build/${PLATFORM}_${CONFIG}"
    if [ -d "$BUILD_DIR/bin" ]; then
        log_info "生成的可執行文件:"
        ls -la "$BUILD_DIR/bin/"
    fi
    
    # 驗證建置結果
    if [ -f "verify_build.sh" ]; then
        log_info "執行建置驗證..."
        ./verify_build.sh
    fi
    
else
    log_error "建置失敗！"
    exit 1
fi

log_success "DriverLib建置腳本執行完成！"