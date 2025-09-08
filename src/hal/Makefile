# Cross-MCU HAL build Makefile
# 根據 MCU 平台選擇 toolchain 與 SDK 路徑

# 平台設定引入
PLATFORM ?= ti_c2000

ifeq ($(PLATFORM),ti_c2000)
include ../../makefiles/ti_c2000_f28p55x.mk
endif

ifeq ($(PLATFORM),stm32g4)
include ../../makefiles/stm32g4.mk
endif

OUT_DIR := ../../build/$(PLATFORM)_Debug/lib
LIB_NAME := libcross_mcu_hal.a

SRC := $(PLATFORM_HAL_SOURCES)
OBJ := $(SRC:.c=.obj)

all: $(OUT_DIR)/$(LIB_NAME)

$(OUT_DIR):
	@mkdir -p $(OUT_DIR)

%.obj: %.c
	$(CC) $(CFLAGS) $(INCLUDE_DIRS) $(PLATFORM_INCLUDE_DIRS) $(PLATFORM_DEFINES) -c $< --output_file=$@

$(OUT_DIR)/$(LIB_NAME): $(OBJ) | $(OUT_DIR)
	@echo "產生 $(LIB_NAME) for $(PLATFORM)"
	$(AR) $(ARFLAGS) $@ $(OBJ)

clean:
	rm -f *.o ti_c2000/*.o stm32g4/*.o $(OUT_DIR)/$(LIB_NAME)

.PHONY: all clean
