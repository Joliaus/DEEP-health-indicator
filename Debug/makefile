################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include lib/middleware/fft/subdir.mk
-include lib/middleware/MIDI/subdir.mk
-include lib/middleware/subdir.mk
-include lib/hal/src/Legacy/subdir.mk
-include lib/hal/src/subdir.mk
-include lib/bsp/trace/subdir.mk
-include lib/bsp/tft_pcd8544/subdir.mk
-include lib/bsp/tft_ili9341/subdir.mk
-include lib/bsp/lcd2x16/subdir.mk
-include lib/bsp/ir/subdir.mk
-include lib/bsp/iks01a1/lsm6ds3/subdir.mk
-include lib/bsp/iks01a1/lsm6ds0/subdir.mk
-include lib/bsp/iks01a1/lps25hb/subdir.mk
-include lib/bsp/iks01a1/lps22hb/subdir.mk
-include lib/bsp/iks01a1/lis3mdl/subdir.mk
-include lib/bsp/iks01a1/hts221/subdir.mk
-include lib/bsp/iks01a1/subdir.mk
-include lib/bsp/hx711/subdir.mk
-include lib/bsp/epaper/Fonts/subdir.mk
-include lib/bsp/epaper/subdir.mk
-include lib/bsp/dmx/subdir.mk
-include lib/bsp/dac_mcp4921/subdir.mk
-include lib/bsp/bmp180/subdir.mk
-include lib/bsp/YX6300/subdir.mk
-include lib/bsp/VL53L0X/subdir.mk
-include lib/bsp/NFC03A1/lib_nfc/lib_pcd/subdir.mk
-include lib/bsp/NFC03A1/lib_nfc/lib_NDEF/subdir.mk
-include lib/bsp/NFC03A1/lib_nfc/common/subdir.mk
-include lib/bsp/NFC03A1/CR95HF/subdir.mk
-include lib/bsp/NFC03A1/subdir.mk
-include lib/bsp/MatrixLed/subdir.mk
-include lib/bsp/MatrixKeyboard/subdir.mk
-include lib/bsp/MPU6050/subdir.mk
-include lib/bsp/MLX90614/subdir.mk
-include lib/bsp/MCP23S17/subdir.mk
-include lib/bsp/MCP23017/subdir.mk
-include lib/bsp/MAX30102/subdir.mk
-include lib/bsp/HC-SR04/subdir.mk
-include lib/bsp/GPS/subdir.mk
-include lib/bsp/FingerprintReader/subdir.mk
-include lib/bsp/ESP8266/subdir.mk
-include lib/bsp/DriverLed_STP16CP05/subdir.mk
-include lib/bsp/DS18B20/subdir.mk
-include lib/bsp/DHT11/CapacitiveKeyboard/subdir.mk
-include lib/bsp/DHT11/subdir.mk
-include lib/bsp/BH1750FVI/subdir.mk
-include lib/bsp/APDS9960/subdir.mk
-include lib/bsp/subdir.mk
-include lib/subdir.mk
-include appli/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := projet-f103
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
projet-f103.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
projet-f103.list \

OBJCOPY_BIN += \
projet-f103.bin \


# All Target
all: main-build

# Main-build Target
main-build: projet-f103.elf secondary-outputs

# Tool invocations
projet-f103.elf: $(OBJS) $(USER_OBJS) ../lib/LinkerScript.ld makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-gcc -o "projet-f103.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m3 -T"../lib/LinkerScript.ld" -Wl,-Map="projet-f103.map" -Wl,--gc-sections -static -u_printf_float -specs=rdimon.specs  -mfloat-abi=soft -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

projet-f103.list: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "projet-f103.list"
	@echo 'Finished building: $@'
	@echo ' '

projet-f103.bin: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "projet-f103.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(SIZE_OUTPUT)$(OBJDUMP_LIST)$(EXECUTABLES)$(OBJS)$(S_DEPS)$(S_UPPER_DEPS)$(C_DEPS)$(OBJCOPY_BIN) projet-f103.elf
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_BIN)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents fail-specified-linker-script-missing warn-no-linker-script-specified

-include ../makefile.targets
