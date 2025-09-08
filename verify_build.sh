#!/bin/bash

# Cross-MCU Framework 建置驗證腳本
# 用於在沒有硬體的情況下驗證生成的文件

set -e

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

log_info() {
    echo -e "${BLUE}[INFO]${NC} $1"
}

log_success() {
    echo -e "${GREEN}[PASS]${NC} $1"
}

log_warning() {
    echo -e "${YELLOW}[WARN]${NC} $1"
}

log_error() {
    echo -e "${RED}[FAIL]${NC} $1"
}

# 檢查建置輸出目錄
BUILD_DIR="build/TI_C2000_F28P55X_Debug"
BIN_DIR="$BUILD_DIR/bin"
LIB_DIR="$BUILD_DIR/lib"

log_info "開始驗證 TI C2000 F28P55X 建置結果..."

# 1. 檢查目錄結構
log_info "檢查目錄結構..."
if [ -d "$BUILD_DIR" ]; then
    log_success "建置目錄存在: $BUILD_DIR"
else
    log_error "建置目錄不存在: $BUILD_DIR"
    exit 1
fi

# 2. 檢查可執行文件
log_info "檢查可執行文件..."
EXECUTABLES=("gpio_blink_example.out" "uart_echo_example.out")
for exe in "${EXECUTABLES[@]}"; do
    if [ -f "$BIN_DIR/$exe" ]; then
        log_success "可執行文件存在: $exe"
        
        # 檢查文件格式
        FILE_INFO=$(file "$BIN_DIR/$exe")
        if [[ "$FILE_INFO" == *"TI TMS320C2000"* ]]; then
            log_success "文件格式正確: TI C2000 ELF"
        else
            log_warning "文件格式可能不正確: $FILE_INFO"
        fi
        
        # 檢查文件大小
        FILE_SIZE=$(stat -c%s "$BIN_DIR/$exe")
        if [ "$FILE_SIZE" -gt 1000 ]; then
            log_success "文件大小合理: ${FILE_SIZE} bytes"
        else
            log_warning "文件大小可能太小: ${FILE_SIZE} bytes"
        fi
    else
        log_error "可執行文件不存在: $exe"
    fi
done

# 3. 檢查HEX文件
log_info "檢查HEX文件..."
HEX_FILES=("gpio_blink_example.hex" "uart_echo_example.hex")
for hex in "${HEX_FILES[@]}"; do
    if [ -f "$BIN_DIR/$hex" ]; then
        log_success "HEX文件存在: $hex"
        
        # 檢查HEX文件格式
        FIRST_LINE=$(head -n 1 "$BIN_DIR/$hex")
        if [[ "$FIRST_LINE" == "%"* ]] || [[ "$FIRST_LINE" == ":"* ]]; then
            log_success "HEX文件格式正確"
        else
            log_warning "HEX文件格式可能不正確"
        fi
    else
        log_error "HEX文件不存在: $hex"
    fi
done

# 4. 檢查HAL函式庫
log_info "檢查HAL函式庫..."
HAL_LIB="$LIB_DIR/libcross_mcu_hal.a"
if [ -f "$HAL_LIB" ]; then
    log_success "HAL函式庫存在"
    
    # 檢查函式庫內容
    if command -v /opt/ti/ccs/ti-cgt-c2000_22.6.2.LTS/bin/ar2000 >/dev/null 2>&1; then
        SYMBOLS=$(/opt/ti/ccs/ti-cgt-c2000_22.6.2.LTS/bin/ar2000 t "$HAL_LIB" | wc -l)
        if [ "$SYMBOLS" -gt 0 ]; then
            log_success "HAL函式庫包含 $SYMBOLS 個目標文件"
        else
            log_warning "HAL函式庫可能為空"
        fi
    fi
else
    log_error "HAL函式庫不存在"
fi

# 5. 使用TI工具鏈進行深度驗證
if command -v /opt/ti/ccs/ti-cgt-c2000_22.6.2.LTS/bin/ofd2000 >/dev/null 2>&1; then
    log_info "使用TI工具鏈進行深度驗證..."
    
    for exe in "${EXECUTABLES[@]}"; do
        if [ -f "$BIN_DIR/$exe" ]; then
            log_info "分析 $exe..."
            
            # 檢查入口點
            ENTRY_POINT=$(/opt/ti/ccs/ti-cgt-c2000_22.6.2.LTS/bin/ofd2000 "$BIN_DIR/$exe" | grep "Entry Point" | awk '{print $3}')
            if [ "$ENTRY_POINT" = "0x00080000" ]; then
                log_success "入口點正確: $ENTRY_POINT (Flash啟動位址)"
            else
                log_warning "入口點: $ENTRY_POINT (可能不是標準Flash位址)"
            fi
            
            # 檢查關鍵符號
            if command -v /opt/ti/ccs/ti-cgt-c2000_22.6.2.LTS/bin/nm2000 >/dev/null 2>&1; then
                SYMBOLS=$(/opt/ti/ccs/ti-cgt-c2000_22.6.2.LTS/bin/nm2000 "$BIN_DIR/$exe")
                
                if echo "$SYMBOLS" | grep -q "code_start"; then
                    log_success "找到啟動符號: code_start"
                else
                    log_error "缺少啟動符號: code_start"
                fi
                
                if echo "$SYMBOLS" | grep -q "main"; then
                    log_success "找到主函數符號: main"
                else
                    log_error "缺少主函數符號: main"
                fi
                
                if echo "$SYMBOLS" | grep -q "_c_int00"; then
                    log_success "找到C運行時初始化符號: _c_int00"
                else
                    log_warning "缺少C運行時初始化符號: _c_int00"
                fi
            fi
        fi
    done
else
    log_warning "TI工具鏈不可用，跳過深度驗證"
fi

# 6. 檢查連結腳本兼容性
log_info "檢查連結腳本..."
LINKER_SCRIPTS=$(find examples/ -name "*.cmd" -type f)
for script in $LINKER_SCRIPTS; do
    if [ -f "$script" ]; then
        log_success "找到連結腳本: $script"
        
        # 檢查是否包含必要的section
        if grep -q "codestart" "$script"; then
            log_success "連結腳本包含codestart section"
        else
            log_warning "連結腳本可能缺少codestart section"
        fi
    fi
done

# 7. 生成驗證報告
log_info "生成驗證報告..."
REPORT_FILE="build_verification_report.txt"
{
    echo "=== Cross-MCU Framework 建置驗證報告 ==="
    echo "驗證時間: $(date)"
    echo "平台: TI C2000 F28P55X"
    echo "建置配置: Debug"
    echo ""
    echo "=== 生成的文件 ==="
    ls -la "$BIN_DIR/" 2>/dev/null || echo "無文件"
    echo ""
    echo "=== HAL函式庫 ==="
    ls -la "$LIB_DIR/" 2>/dev/null || echo "無函式庫"
    echo ""
    echo "=== 文件格式驗證 ==="
    for exe in "${EXECUTABLES[@]}"; do
        if [ -f "$BIN_DIR/$exe" ]; then
            echo "$exe: $(file "$BIN_DIR/$exe")"
        fi
    done
} > "$REPORT_FILE"

log_success "驗證報告已生成: $REPORT_FILE"

log_info "=== 建議的後續步驟 ==="
echo "1. 將 .out 文件導入 Code Composer Studio 進行進一步驗證"
echo "2. 使用 TI 的 uniflash 工具檢查 .hex 文件的燒錄兼容性"
echo "3. 如果有 LaunchPad 開發板，可以嘗試實際燒錄測試"
echo "4. 使用 CCS 的模擬器進行功能測試（如果可用）"

log_success "驗證完成！"

