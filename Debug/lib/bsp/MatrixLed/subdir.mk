################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../lib/bsp/MatrixLed/WS2812S_asm.s 

C_SRCS += \
../lib/bsp/MatrixLed/WS2812B.c \
../lib/bsp/MatrixLed/WS2812S.c \
../lib/bsp/MatrixLed/matrix_led_32_32.c 

OBJS += \
./lib/bsp/MatrixLed/WS2812B.o \
./lib/bsp/MatrixLed/WS2812S.o \
./lib/bsp/MatrixLed/WS2812S_asm.o \
./lib/bsp/MatrixLed/matrix_led_32_32.o 

S_DEPS += \
./lib/bsp/MatrixLed/WS2812S_asm.d 

C_DEPS += \
./lib/bsp/MatrixLed/WS2812B.d \
./lib/bsp/MatrixLed/WS2812S.d \
./lib/bsp/MatrixLed/matrix_led_32_32.d 


# Each subdirectory must supply rules for building sources it contributes
lib/bsp/MatrixLed/%.o: ../lib/bsp/MatrixLed/%.c lib/bsp/MatrixLed/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32F1 -DNUCLEO_F103RB -DSTM32F103RBTx -DSTM32 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../lib/bsp -I../lib//middleware/MIDI/ -I../lib/bsp/VL53L0X -I../lib/bsp/MPU6050 -I../lib/bsp/iks01a1 -I../lib/bsp/iks01a1/Common -I../lib/bsp/iks01a1/hts221 -I../lib/bsp/iks01a1/lis3mdl -I../lib/bsp/iks01a1/lps22hb -I../lib/bsp/iks01a1/lps25hb -I../lib/bsp/iks01a1/lsm6ds0 -I../lib/bsp/iks01a1/lsm6ds3 -I../lib/bsp/tft_ili9341 -I../lib/bsp/tft_pcd8544 -I../lib/bsp/MCP23S17 -I../lib/hal/inc -I../lib/bsp/Common -I../lib/bsp/lcd2x16 -I../lib/bsp/MLX90614 -I../lib/bsp/MatrixKeyboard -I../lib/bsp/MatrixLed -I../lib/CMSIS/core -I../lib/CMSIS/device -I../lib/middleware/FatFs -I../lib/middleware/FatFs/src -I../lib/middleware/FatFs/src/drivers -I../appli -O0 -ffunction-sections -fdata-sections -Wall -Wextra -Wconversion -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@"  -mfloat-abi=soft -mthumb -o "$@"
lib/bsp/MatrixLed/%.o: ../lib/bsp/MatrixLed/%.s lib/bsp/MatrixLed/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m3 -g3 -c -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@"  -mfloat-abi=soft -mthumb -o "$@" "$<"

