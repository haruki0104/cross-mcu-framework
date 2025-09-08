# TI C2000 F28P65x平台Makefile配置
# 作者: Cross-MCU Framework Team
# 日期: 2024

# MCU特定定義
MCU_PART := F28P65x
MCU_DEFINE := F28P65X
CPU_FREQ := 200000000

# TI C2000工具鏈路徑 (需要根據實際安裝路徑調整)
ifeq ($(OS),Windows_NT)
    TI_CCS_PATH ?= C:/ti/ccs1200/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS
else
    # TI_CCS_PATH ?= /opt/ti/ccs1200/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS
    TI_CCS_PATH ?= /opt/ti/ccs/ti-cgt-c2000_22.6.2.LTS
endif

# 檢查工具鏈是否存在
ifeq ($(wildcard $(TI_CCS_PATH)/bin/cl2000),)
    $(warning TI C2000 compiler not found at $(TI_CCS_PATH))
    $(warning Please install Code Composer Studio or set TI_CCS_PATH variable)
    $(warning Using default gcc for syntax checking only)
    CC := gcc
    AR := ar
else
    # TI C2000工具鏈
    CC := $(TI_CCS_PATH)/bin/cl2000
    AR := $(TI_CCS_PATH)/bin/ar2000
    LINKER := $(TI_CCS_PATH)/bin/cl2000
    HEX := $(TI_CCS_PATH)/bin/hex2000
endif

# 平台特定定義
PLATFORM_DEFINES := --define=PLATFORM_TI_C2000 \
                    --define=MCU_$(MCU_DEFINE) \
                    --define=CPU_FREQ=$(CPU_FREQ) \
                    --define=_FLASH

# 平台特定包含目錄
PLATFORM_INCLUDE_DIRS := -I$(HAL_DIR)/ti_c2000

# TI C2000Ware路徑配置
C2000WARE_PATH := /opt/ti/c2000/C2000Ware_6_00_00_00
DEVICE_SUPPORT_PATH := $(C2000WARE_PATH)/device_support/f28p65x
DRIVERLIB_PATH := $(C2000WARE_PATH)/driverlib/f28p65x/driverlib

# 檢查並添加TI C2000Ware包含路徑
ifneq ($(wildcard $(DEVICE_SUPPORT_PATH)),)
    PLATFORM_INCLUDE_DIRS += -I$(DEVICE_SUPPORT_PATH)/common/include
    PLATFORM_INCLUDE_DIRS += -I$(DEVICE_SUPPORT_PATH)/headers/include
endif

# 檢查並添加DriverLib路徑
ifneq ($(wildcard $(DRIVERLIB_PATH)),)
    PLATFORM_INCLUDE_DIRS += -I$(DRIVERLIB_PATH)
    PLATFORM_LIBS += -L$(DRIVERLIB_PATH) -ldriverlib.lib
endif

# TI C2000特定編譯選項
ifeq ($(CC),$(TI_CCS_PATH)/bin/cl2000)
    # TI編譯器選項
    CFLAGS := -v28 -ml -mt --cla_support=cla2 --float_support=fpu32 --tmu_support=tmu1 \
              --abi=eabi \
              --cla_support=cla2 \
              --float_support=fpu32 \
              --tmu_support=tmu0 \
              --vcu_support=vcu2 \
              --include_path=$(TI_CCS_PATH)/include \
              --diag_suppress=10063 \
              --diag_warning=225 \
              --display_error_number \
              --gen_func_subsections=on \
              --printf_support=minimal
    
    # Debug/Release特定選項
    ifeq ($(BUILD_TYPE),Debug)
        CFLAGS += --opt_level=0 --debug
    else
        CFLAGS += --opt_level=2 --opt_for_speed=3
    endif
    
    # 連結選項
    LDFLAGS := -z -m"launchxl_ex1_f28p65x_demo.map" \
               -i"$(TI_CCS_PATH)/lib" \
               -i"$(TI_CCS_PATH)/include" --reread_libs \
               --entry_point=code_start \
               --stack_size=0x200 \
               --heap_size=0x100 \
               --warn_sections \
               --display_error_number \
               --diag_wrap=off \
               --entry_point=code_start --rom_model
    
    # 歸檔選項
    ARFLAGS := rq
    
    # 輸出檔案副檔名
    EXECUTABLE_SUFFIX := .out
    
    # TI編譯器特定的編譯命令
    COMPILE_CMD = $(CC) $(CFLAGS) $(INCLUDE_DIRS) $(PLATFORM_INCLUDE_DIRS) $(PLATFORM_DEFINES) -c $< --output_file=$@
    
else
    # 使用GCC進行語法檢查
    CFLAGS := -Wall -Wextra -std=c11
    ifeq ($(BUILD_TYPE),Debug)
        CFLAGS += -g -O0
    else
        CFLAGS += -O2 -DNDEBUG
    endif
    LDFLAGS := 
    ARFLAGS := rcs
    EXECUTABLE_SUFFIX := .elf
    
    # 標準編譯器的編譯命令
    COMPILE_CMD = $(CC) $(CFLAGS) $(INCLUDE_DIRS) $(PLATFORM_INCLUDE_DIRS) $(PLATFORM_DEFINES) -c $< -o $@
endif

# 平台特定HAL源檔案
PLATFORM_HAL_SOURCES := ti_c2000/ti_c2000_gpio_simple.c \
                        ti_c2000/ti_c2000_system_simple.c

# 根據MCU型號選擇連結描述檔 (如果使用TI編譯器)
ifeq ($(CC),$(TI_CCS_PATH)/bin/cl2000)
    ifeq ($(MCU_PART),F28P65x)
        LINKER_SCRIPT := makefiles/linker_scripts/F28P65x_flash.cmd
    else ifeq ($(MCU_PART),F28P55x)
        LINKER_SCRIPT := makefiles/linker_scripts/F28P55x_flash.cmd
    else
        LINKER_SCRIPT := makefiles/linker_scripts/default_flash.cmd
    endif
    ifneq ($(wildcard $(LINKER_SCRIPT)),)
        LDFLAGS += $(LINKER_SCRIPT)
    endif
endif

# 平台特定的後處理規則
define PLATFORM_POST_BUILD
	@if [ "$(CC)" = "$(TI_CCS_PATH)/bin/cl2000" ] && [ -f "$(HEX)" ]; then \
		echo "生成HEX檔案: $(basename $1).hex"; \
		$(HEX) $1 -o $(basename $1).hex -order MS -romwidth 16; \
	fi
endef
