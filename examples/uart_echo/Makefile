# UART回音範例Makefile
# 作者: Cross-MCU Framework Team

# 平台設定引入
PLATFORM ?= ti_c2000
ifeq ($(PLATFORM),ti_c2000)
include ../../makefiles/ti_c2000_f28p55x.mk
else ifeq ($(PLATFORM),stm32g4)
include ../../makefiles/stm32g4.mk
endif

PROJECT_NAME := uart_echo_example
EXAMPLE_DIR := $(CURDIR)
SRC_DIR := $(EXAMPLE_DIR)/src
INC_DIR := $(EXAMPLE_DIR)
OBJ_DIR := $(EXAMPLE_DIR)/obj
BIN_DIR := $(EXAMPLE_DIR)/bin
CMD_DIR := $(EXAMPLE_DIR)/cmd

$(info 使用範例目錄: $(EXAMPLE_DIR), INC_DIR: $(INC_DIR))

C_SOURCES := $(wildcard $(EXAMPLE_DIR)/*.c) $(wildcard $(SRC_DIR)/*.c)
ASM_SOURCES := $(wildcard $(SRC_DIR)/*.asm)

COBJECTS := $(patsubst $(EXAMPLE_DIR)/%.c,$(OBJ_DIR)/%.obj,$(filter $(EXAMPLE_DIR)/%.c,$(C_SOURCES))) \
            $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.obj,$(filter $(SRC_DIR)/%.c,$(C_SOURCES)))
ASMOBJECTS := $(patsubst $(SRC_DIR)/%.asm,$(OBJ_DIR)/%.obj,$(ASM_SOURCES))
OBJECTS := $(COBJECTS) $(ASMOBJECTS)

LINKCMD := $(wildcard $(CMD_DIR)/*flash*.cmd)
TARGET := $(BIN_DIR)/$(PROJECT_NAME)$(EXECUTABLE_SUFFIX)

HAL_INC_DIR := $(EXAMPLE_DIR)/../../src/hal/include
HAL_LIB := $(EXAMPLE_DIR)/../../build/$(PLATFORM)_Debug/lib/libcross_mcu_hal.a

# TI C2000 使用 --include_path 語法
ifeq ($(PLATFORM),ti_c2000)
INCLUDE_DIRS := --include_path="$(INC_DIR)" --include_path="$(HAL_INC_DIR)"
else
INCLUDE_DIRS := -I$(INC_DIR) -I$(HAL_INC_DIR) $(PLATFORM_INCLUDE_DIRS)
endif

HAL_BUILD_DIR := $(EXAMPLE_DIR)/../../src/hal

all: hal $(BIN_DIR) $(OBJ_DIR) $(TARGET)

hal:
	$(MAKE) -C $(HAL_BUILD_DIR) PLATFORM=$(PLATFORM)

$(BIN_DIR):
	@mkdir -p $(BIN_DIR)

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.obj: $(EXAMPLE_DIR)/%.c
	@echo "編譯 $<"
	$(CC) $(CFLAGS) $(INCLUDE_DIRS) $(PLATFORM_DEFINES) -c $< --output_file=$@

$(OBJ_DIR)/%.obj: $(SRC_DIR)/%.c
	@echo "編譯 $<"
	$(CC) $(CFLAGS) $(INCLUDE_DIRS) $(PLATFORM_DEFINES) -c $< --output_file=$@

$(OBJ_DIR)/%.obj: $(SRC_DIR)/%.asm
	@echo "編譜 $< (ASM)"
	$(CC) $(CFLAGS) $(INCLUDE_DIRS) $(PLATFORM_DEFINES) -c $< --output_file=$@

$(TARGET): $(OBJECTS) $(HAL_LIB) $(LINKCMD)
	@echo "連結 $(PROJECT_NAME)"
	$(CC) -z $(OBJECTS) $(HAL_LIB) $(LDFLAGS) $(LINKCMD) --library=/opt/ti/ccs/ti-cgt-c2000_22.6.2.LTS/lib/rts2800_fpu32_eabi.lib --output_file=$@
	$(call PLATFORM_POST_BUILD,$@)

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

.PHONY: all clean hal
