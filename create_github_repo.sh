#!/bin/bash

# GitHub Repository 創建腳本
# 用於將Cross-MCU Framework上傳到GitHub

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

# 專案資訊
REPO_NAME="cross-mcu-framework"
DESCRIPTION="跨MCU平台開發框架，支援TI C2000和STM32系列"

log_info "Cross-MCU Framework GitHub Repository 創建腳本"

# 檢查是否已經有remote origin
if git remote get-url origin >/dev/null 2>&1; then
    CURRENT_ORIGIN=$(git remote get-url origin)
    log_info "當前遠程倉庫: $CURRENT_ORIGIN"
    
    read -p "是否要更新到新的GitHub倉庫？(y/N): " -n 1 -r
    echo
    if [[ ! $REPLY =~ ^[Yy]$ ]]; then
        log_info "保持當前遠程倉庫設置"
        exit 0
    fi
fi

# 檢查是否安裝了GitHub CLI
if command -v gh >/dev/null 2>&1; then
    log_success "找到GitHub CLI"
    
    # 檢查是否已經登錄
    if gh auth status >/dev/null 2>&1; then
        log_success "GitHub CLI已認證"
        
        log_info "創建GitHub倉庫..."
        
        # 創建倉庫
        gh repo create "$REPO_NAME" \
            --description "$DESCRIPTION" \
            --public \
            --source=. \
            --remote=origin \
            --push
        
        if [ $? -eq 0 ]; then
            log_success "GitHub倉庫創建成功！"
            log_info "倉庫URL: https://github.com/$(gh api user --jq .login)/$REPO_NAME"
        else
            log_error "GitHub倉庫創建失敗"
            exit 1
        fi
        
    else
        log_warning "GitHub CLI未認證"
        log_info "請運行: gh auth login"
        exit 1
    fi
    
else
    log_warning "未找到GitHub CLI"
    log_info "手動創建GitHub倉庫的步驟："
    echo "1. 前往 https://github.com/new"
    echo "2. 倉庫名稱: $REPO_NAME"
    echo "3. 描述: $DESCRIPTION"
    echo "4. 設置為Public"
    echo "5. 不要初始化README、.gitignore或LICENSE（我們已經有了）"
    echo "6. 創建倉庫後，運行以下命令："
    echo ""
    echo "   git remote add origin https://github.com/YOUR_USERNAME/$REPO_NAME.git"
    echo "   git branch -M main"
    echo "   git push -u origin main"
    echo ""
    
    read -p "是否要繼續手動設置？(y/N): " -n 1 -r
    echo
    if [[ $REPLY =~ ^[Yy]$ ]]; then
        read -p "請輸入您的GitHub用戶名: " GITHUB_USERNAME
        
        if [ -n "$GITHUB_USERNAME" ]; then
            GITHUB_URL="https://github.com/$GITHUB_USERNAME/$REPO_NAME.git"
            
            log_info "設置遠程倉庫..."
            git remote remove origin 2>/dev/null || true
            git remote add origin "$GITHUB_URL"
            git branch -M main
            
            log_info "推送到GitHub..."
            git push -u origin main
            
            if [ $? -eq 0 ]; then
                log_success "代碼已成功推送到GitHub！"
                log_info "倉庫URL: https://github.com/$GITHUB_USERNAME/$REPO_NAME"
            else
                log_error "推送失敗，請檢查倉庫是否已創建並且有正確的權限"
            fi
        else
            log_error "用戶名不能為空"
            exit 1
        fi
    fi
fi

log_info "後續步驟建議："
echo "1. 在GitHub上設置倉庫描述和標籤"
echo "2. 創建Issues模板"
echo "3. 設置GitHub Actions CI/CD"
echo "4. 添加貢獻者指南"
echo "5. 設置GitHub Pages（如果需要文檔站點）"

log_success "GitHub倉庫設置完成！"
