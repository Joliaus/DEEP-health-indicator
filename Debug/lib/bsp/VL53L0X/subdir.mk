################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lib/bsp/VL53L0X/X-NUCLEO-53L0A1.c \
../lib/bsp/VL53L0X/vl53_x-nucleo-53l0A1_demo.c \
../lib/bsp/VL53L0X/vl53l0_demo.c \
../lib/bsp/VL53L0X/vl53l0a1-f103msp.c \
../lib/bsp/VL53L0X/vl53l0x_api.c \
../lib/bsp/VL53L0X/vl53l0x_api_calibration.c \
../lib/bsp/VL53L0X/vl53l0x_api_core.c \
../lib/bsp/VL53L0X/vl53l0x_api_ranging.c \
../lib/bsp/VL53L0X/vl53l0x_api_strings.c \
../lib/bsp/VL53L0X/vl53l0x_platform.c \
../lib/bsp/VL53L0X/vl53l0x_platform_log.c 

OBJS += \
./lib/bsp/VL53L0X/X-NUCLEO-53L0A1.o \
./lib/bsp/VL53L0X/vl53_x-nucleo-53l0A1_demo.o \
./lib/bsp/VL53L0X/vl53l0_demo.o \
./lib/bsp/VL53L0X/vl53l0a1-f103msp.o \
./lib/bsp/VL53L0X/vl53l0x_api.o \
./lib/bsp/VL53L0X/vl53l0x_api_calibration.o \
./lib/bsp/VL53L0X/vl53l0x_api_core.o \
./lib/bsp/VL53L0X/vl53l0x_api_ranging.o \
./lib/bsp/VL53L0X/vl53l0x_api_strings.o \
./lib/bsp/VL53L0X/vl53l0x_platform.o \
./lib/bsp/VL53L0X/vl53l0x_platform_log.o 

C_DEPS += \
./lib/bsp/VL53L0X/X-NUCLEO-53L0A1.d \
./lib/bsp/VL53L0X/vl53_x-nucleo-53l0A1_demo.d \
./lib/bsp/VL53L0X/vl53l0_demo.d \
./lib/bsp/VL53L0X/vl53l0a1-f103msp.d \
./lib/bsp/VL53L0X/vl53l0x_api.d \
./lib/bsp/VL53L0X/vl53l0x_api_calibration.d \
./lib/bsp/VL53L0X/vl53l0x_api_core.d \
./lib/bsp/VL53L0X/vl53l0x_api_ranging.d \
./lib/bsp/VL53L0X/vl53l0x_api_strings.d \
./lib/bsp/VL53L0X/vl53l0x_platform.d \
./lib/bsp/VL53L0X/vl53l0x_platform_log.d 


# Each subdirectory must supply rules for building sources it contributes
lib/bsp/VL53L0X/%.o: ../lib/bsp/VL53L0X/%.c lib/bsp/VL53L0X/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32F1 -DNUCLEO_F103RB -DSTM32F103RBTx -DSTM32 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../lib/bsp -I../lib//middleware/MIDI/ -I../lib/bsp/VL53L0X -I../lib/bsp/MPU6050 -I../lib/bsp/iks01a1 -I../lib/bsp/iks01a1/Common -I../lib/bsp/iks01a1/hts221 -I../lib/bsp/iks01a1/lis3mdl -I../lib/bsp/iks01a1/lps22hb -I../lib/bsp/iks01a1/lps25hb -I../lib/bsp/iks01a1/lsm6ds0 -I../lib/bsp/iks01a1/lsm6ds3 -I../lib/bsp/tft_ili9341 -I../lib/bsp/tft_pcd8544 -I../lib/bsp/MCP23S17 -I../lib/hal/inc -I../lib/bsp/Common -I../lib/bsp/lcd2x16 -I../lib/bsp/MLX90614 -I../lib/bsp/MatrixKeyboard -I../lib/bsp/MatrixLed -I../lib/CMSIS/core -I../lib/CMSIS/device -I../lib/middleware/FatFs -I../lib/middleware/FatFs/src -I../lib/middleware/FatFs/src/drivers -I../appli -O0 -ffunction-sections -fdata-sections -Wall -Wextra -Wconversion -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@"  -mfloat-abi=soft -mthumb -o "$@"

