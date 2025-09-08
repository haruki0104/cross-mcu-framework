# STM32G4平台Makefile配置
# 作者: Cross-MCU Framework Team
# 日期: 2024

# MCU特定定義
MCU_FAMILY := STM32G4xx
MCU_PART := STM32G474xx
CPU_FREQ := 170000000

# ARM GCC工具鏈
CC := arm-none-eabi-gcc
AR := arm-none-eabi-ar
OBJCOPY := arm-none-eabi-objcopy
OBJDUMP := arm-none-eabi-objdump
SIZE := arm-none-eabi-size

# 檢查工具鏈是否存在
ifeq ($(shell which $(CC)),)
    $(warning ARM GCC toolchain not found)
    $(warning Please install arm-none-eabi-gcc)
    $(warning Using default gcc for syntax checking only)
    CC := gcc
    AR := ar
    OBJCOPY := objcopy
    SIZE := size
endif

# ARM Cortex-M4特定選項
CPU_OPTIONS := -mcpu=cortex-m4 \
               -mthumb \
               -mfpu=fpv4-sp-d16 \
               -mfloat-abi=hard

# 平台特定定義
PLATFORM_DEFINES := -DPLATFORM_STM32 \
                    -DMCU_STM32G4 \
                    -DSTM32G474xx \
                    -DUSE_HAL_DRIVER \
                    -DCPU_FREQ=$(CPU_FREQ) \
                    -DARM_MATH_CM4 \
                    -D__FPU_PRESENT=1

# 平台特定包含目錄
PLATFORM_INCLUDE_DIRS := -I$(HAL_DIR)/stm32g4

# STM32 HAL庫路徑
STM32_HAL_PATH := third_party/STM32CubeG4
ifneq ($(wildcard $(STM32_HAL_PATH)),)
    PLATFORM_INCLUDE_DIRS += -I$(STM32_HAL_PATH)/Drivers/STM32G4xx_HAL_Driver/Inc \
                             -I$(STM32_HAL_PATH)/Drivers/STM32G4xx_HAL_Driver/Inc/Legacy \
                             -I$(STM32_HAL_PATH)/Drivers/CMSIS/Device/ST/STM32G4xx/Include \
                             -I$(STM32_HAL_PATH)/Drivers/CMSIS/Include
    
    # STM32 HAL源檔案
    STM32_HAL_SOURCES := $(wildcard $(STM32_HAL_PATH)/Drivers/STM32G4xx_HAL_Driver/Src/*.c)
    # 排除模板檔案
    STM32_HAL_SOURCES := $(filter-out %_template.c,$(STM32_HAL_SOURCES))
else
    $(warning STM32 HAL library not found at $(STM32_HAL_PATH))
    $(warning Please download STM32CubeG4 package)
endif

# STM32G4特定編譯選項
ifeq ($(CC),arm-none-eabi-gcc)
    CFLAGS := $(CPU_OPTIONS) \
              -Wall \
              -Wextra \
              -Wpedantic \
              -fdata-sections \
              -ffunction-sections \
              -fstack-usage \
              -MMD \
              -MP \
              -std=c11
    
    # Debug/Release特定選項
    ifeq ($(BUILD_TYPE),Debug)
        CFLAGS += -Og -g3 -ggdb -DDEBUG
    else
        CFLAGS += -O3 -g0 -DNDEBUG
    endif
    
    # 連結選項
    LDFLAGS := $(CPU_OPTIONS) \
               -specs=nano.specs \
               -Wl,-Map=$(BUILD_DIR)/output.map,--cref \
               -Wl,--gc-sections \
               -static \
               -Wl,--start-group \
               -lc \
               -lm \
               -Wl,--end-group
    
    # 連結描述檔
    LINKER_SCRIPT := makefiles/linker_scripts/STM32G474RETx_FLASH.ld
    ifneq ($(wildcard $(LINKER_SCRIPT)),)
        LDFLAGS += -T$(LINKER_SCRIPT)
    endif
    
    # 歸檔選項
    ARFLAGS := rcs
    
    # 輸出檔案副檔名
    EXECUTABLE_SUFFIX := .elf
    
    # ARM GCC編譯命令
    COMPILE_CMD = $(CC) $(CFLAGS) $(INCLUDE_DIRS) $(PLATFORM_INCLUDE_DIRS) $(PLATFORM_DEFINES) -c $< -o $@
    
else
    # 使用預設GCC進行語法檢查
    CFLAGS := -Wall -Wextra -std=c11
    ifeq ($(BUILD_TYPE),Debug)
        CFLAGS += -g -O0 -DDEBUG
    else
        CFLAGS += -O2 -DNDEBUG
    endif
    LDFLAGS := 
    ARFLAGS := rcs
    EXECUTABLE_SUFFIX := .elf
    
    # 預設GCC編譯命令
    COMPILE_CMD = $(CC) $(CFLAGS) $(INCLUDE_DIRS) $(PLATFORM_INCLUDE_DIRS) $(PLATFORM_DEFINES) -c $< -o $@
endif

# 平台特定HAL源檔案
PLATFORM_HAL_SOURCES := stm32g4/stm32g4_gpio.c \
                        stm32g4/stm32g4_system.c

# 如果有STM32 HAL源檔案，添加到編譯列表
ifneq ($(STM32_HAL_SOURCES),)
    # 將STM32 HAL源檔案轉換為相對於src目錄的路徑
    PLATFORM_HAL_SOURCES += $(patsubst %,../%,$(STM32_HAL_SOURCES))
endif

# 平台特定的後處理規則
define PLATFORM_POST_BUILD
	@if [ "$(CC)" = "arm-none-eabi-gcc" ]; then \
		echo "生成二進制檔案: $(basename $1).bin"; \
		$(OBJCOPY) -O binary $1 $(basename $1).bin; \
		echo "生成HEX檔案: $(basename $1).hex"; \
		$(OBJCOPY) -O ihex $1 $(basename $1).hex; \
		echo "顯示大小資訊:"; \
		$(SIZE) --format=berkeley $1; \
	fi
endef
