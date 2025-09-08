# Cross-MCU Framework Makefile
# 作者: Cross-MCU Framework Team
# 日期: 2024

# 預設目標平台
TARGET_PLATFORM ?= TI_C2000_F28P55X

# 建置配置
BUILD_TYPE ?= Debug

# 專案資訊
PROJECT_NAME := cross_mcu_framework
VERSION := 1.0.0

# 目錄定義
SRC_DIR := src
HAL_DIR := $(SRC_DIR)/hal
EXAMPLES_DIR := examples
BUILD_DIR := build/$(TARGET_PLATFORM)_$(BUILD_TYPE)
OBJ_DIR := $(BUILD_DIR)/obj
BIN_DIR := $(BUILD_DIR)/bin
LIB_DIR := $(BUILD_DIR)/lib

# 包含目錄
INCLUDE_DIRS := -I$(HAL_DIR)/include

# 平台特定配置
ifeq ($(TARGET_PLATFORM),TI_C2000_F28P55X)
    include makefiles/ti_c2000_f28p55x.mk
else ifeq ($(TARGET_PLATFORM),TI_C2000_F28P65X)
    include makefiles/ti_c2000_f28p65x.mk
else ifeq ($(TARGET_PLATFORM),STM32G4)
    include makefiles/stm32g4.mk
else
    $(error Unsupported platform: $(TARGET_PLATFORM))
endif

# HAL源檔案 (平台無關)
HAL_COMMON_SOURCES := 

# HAL源檔案 (平台特定)
HAL_PLATFORM_SOURCES := $(addprefix $(HAL_DIR)/,$(PLATFORM_HAL_SOURCES))

# 所有HAL源檔案
HAL_SOURCES := $(HAL_COMMON_SOURCES) $(HAL_PLATFORM_SOURCES)

# HAL目標檔案
HAL_OBJECTS := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.obj,$(HAL_SOURCES))

# HAL函式庫
HAL_LIB := $(LIB_DIR)/libcross_mcu_hal.a

# 範例程式
EXAMPLES := gpio_blink uart_echo

# 預設目標
.PHONY: all
all: hal examples

# 建立目錄
$(OBJ_DIR) $(BIN_DIR) $(LIB_DIR):
	@mkdir -p $@

# 編譯HAL目標檔案
$(OBJ_DIR)/%.obj: $(SRC_DIR)/%.c | $(OBJ_DIR)
	@mkdir -p $(dir $@)
	@echo "編譯 $<"
	$(COMPILE_CMD)

# 建立HAL函式庫
$(HAL_LIB): $(HAL_OBJECTS) | $(LIB_DIR)
	@echo "建立HAL函式庫 $@"
	$(AR) $(ARFLAGS) $@ $^

# HAL目標
.PHONY: hal
hal: $(HAL_LIB)

# 範例程式目標
.PHONY: examples
examples: $(EXAMPLES)

# GPIO閃爍範例
.PHONY: gpio_blink
gpio_blink: $(HAL_LIB) | $(BIN_DIR)
	@echo "建置GPIO閃爍範例"
	$(MAKE) -C $(EXAMPLES_DIR)/gpio_blink \
		TARGET_PLATFORM=$(TARGET_PLATFORM) \
		BUILD_TYPE=$(BUILD_TYPE) \
		HAL_LIB=../../$(HAL_LIB) \
		INCLUDE_DIRS="-I../../$(HAL_DIR)/include $(patsubst -I%,-I../../%,$(PLATFORM_INCLUDE_DIRS))" \
		PLATFORM_DEFINES="$(PLATFORM_DEFINES)" \
		CC="$(CC)" \
		CFLAGS="$(CFLAGS)" \
		LDFLAGS="$(LDFLAGS)" \
		BIN_DIR=../../$(BIN_DIR)

# UART回音範例
.PHONY: uart_echo
uart_echo: $(HAL_LIB) | $(BIN_DIR)
	@echo "建置UART回音範例"
	$(MAKE) -C $(EXAMPLES_DIR)/uart_echo \
		TARGET_PLATFORM=$(TARGET_PLATFORM) \
		BUILD_TYPE=$(BUILD_TYPE) \
		HAL_LIB=../../$(HAL_LIB) \
		INCLUDE_DIRS="-I../../$(HAL_DIR)/include $(patsubst -I%,-I../../%,$(PLATFORM_INCLUDE_DIRS))" \
		PLATFORM_DEFINES="$(PLATFORM_DEFINES)" \
		CC="$(CC)" \
		CFLAGS="$(CFLAGS)" \
		LDFLAGS="$(LDFLAGS)" \
		BIN_DIR=../../$(BIN_DIR)

# 清理目標
.PHONY: clean
clean:
	@echo "清理建置檔案"
	cd $(EXAMPLES_DIR)/uart_echo && make clean
	cd $(EXAMPLES_DIR)/gpio_blink && make clean
	cd $(HAL_DIR) && make clean
	# rm -rf build/

# 深度清理
.PHONY: distclean
distclean: clean
	@echo "深度清理"
	rm -rf build/

# 安裝目標
.PHONY: install
install: all
	@echo "安裝Cross-MCU Framework"
	mkdir -p $(DESTDIR)/usr/local/include/cross_mcu
	mkdir -p $(DESTDIR)/usr/local/lib
	cp -r $(HAL_DIR)/include/* $(DESTDIR)/usr/local/include/cross_mcu/
	cp $(HAL_LIB) $(DESTDIR)/usr/local/lib/

# 顯示幫助資訊
.PHONY: help
help:
	@echo "Cross-MCU Framework Makefile"
	@echo ""
	@echo "可用目標:"
	@echo "  all          - 建置HAL函式庫和所有範例 (預設)"
	@echo "  hal          - 只建置HAL函式庫"
	@echo "  examples     - 建置所有範例程式"
	@echo "  gpio_blink   - 建置GPIO閃爍範例"
	@echo "  uart_echo    - 建置UART回音範例"
	@echo "  clean        - 清理建置檔案"
	@echo "  distclean    - 深度清理"
	@echo "  install      - 安裝函式庫和標頭檔"
	@echo "  help         - 顯示此幫助資訊"
	@echo ""
	@echo "變數:"
	@echo "  TARGET_PLATFORM - 目標平台 (TI_C2000_F28P55X, TI_C2000_F28P65X, STM32G4)"
	@echo "  BUILD_TYPE      - 建置類型 (Debug, Release)"
	@echo ""
	@echo "範例:"
	@echo "  make TARGET_PLATFORM=STM32G4 BUILD_TYPE=Release"
	@echo "  make gpio_blink TARGET_PLATFORM=TI_C2000_F28P55X"

# 顯示配置資訊
.PHONY: info
info:
	@echo "建置配置資訊:"
	@echo "  專案名稱: $(PROJECT_NAME)"
	@echo "  版本: $(VERSION)"
	@echo "  目標平台: $(TARGET_PLATFORM)"
	@echo "  建置類型: $(BUILD_TYPE)"
	@echo "  編譯器: $(CC)"
	@echo "  建置目錄: $(BUILD_DIR)"
	@echo "  HAL源檔案: $(words $(HAL_SOURCES)) 個檔案"
	@echo "  包含目錄: $(INCLUDE_DIRS) $(PLATFORM_INCLUDE_DIRS)"
	@echo "  平台定義: $(PLATFORM_DEFINES)"
