################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lib/middleware/MIDI/midi.c \
../lib/middleware/MIDI/usb_device.c \
../lib/middleware/MIDI/usbd_conf.c \
../lib/middleware/MIDI/usbd_core.c \
../lib/middleware/MIDI/usbd_ctlreq.c \
../lib/middleware/MIDI/usbd_desc.c \
../lib/middleware/MIDI/usbd_ioreq.c \
../lib/middleware/MIDI/usbd_midi.c \
../lib/middleware/MIDI/usbd_midi_if.c 

OBJS += \
./lib/middleware/MIDI/midi.o \
./lib/middleware/MIDI/usb_device.o \
./lib/middleware/MIDI/usbd_conf.o \
./lib/middleware/MIDI/usbd_core.o \
./lib/middleware/MIDI/usbd_ctlreq.o \
./lib/middleware/MIDI/usbd_desc.o \
./lib/middleware/MIDI/usbd_ioreq.o \
./lib/middleware/MIDI/usbd_midi.o \
./lib/middleware/MIDI/usbd_midi_if.o 

C_DEPS += \
./lib/middleware/MIDI/midi.d \
./lib/middleware/MIDI/usb_device.d \
./lib/middleware/MIDI/usbd_conf.d \
./lib/middleware/MIDI/usbd_core.d \
./lib/middleware/MIDI/usbd_ctlreq.d \
./lib/middleware/MIDI/usbd_desc.d \
./lib/middleware/MIDI/usbd_ioreq.d \
./lib/middleware/MIDI/usbd_midi.d \
./lib/middleware/MIDI/usbd_midi_if.d 


# Each subdirectory must supply rules for building sources it contributes
lib/middleware/MIDI/%.o: ../lib/middleware/MIDI/%.c lib/middleware/MIDI/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32F1 -DNUCLEO_F103RB -DSTM32F103RBTx -DSTM32 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../lib/bsp -I../lib//middleware/MIDI/ -I../lib/bsp/VL53L0X -I../lib/bsp/MPU6050 -I../lib/bsp/iks01a1 -I../lib/bsp/iks01a1/Common -I../lib/bsp/iks01a1/hts221 -I../lib/bsp/iks01a1/lis3mdl -I../lib/bsp/iks01a1/lps22hb -I../lib/bsp/iks01a1/lps25hb -I../lib/bsp/iks01a1/lsm6ds0 -I../lib/bsp/iks01a1/lsm6ds3 -I../lib/bsp/tft_ili9341 -I../lib/bsp/tft_pcd8544 -I../lib/bsp/MCP23S17 -I../lib/hal/inc -I../lib/bsp/Common -I../lib/bsp/lcd2x16 -I../lib/bsp/MLX90614 -I../lib/bsp/MatrixKeyboard -I../lib/bsp/MatrixLed -I../lib/CMSIS/core -I../lib/CMSIS/device -I../lib/middleware/FatFs -I../lib/middleware/FatFs/src -I../lib/middleware/FatFs/src/drivers -I../appli -O0 -ffunction-sections -fdata-sections -Wall -Wextra -Wconversion -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@"  -mfloat-abi=soft -mthumb -o "$@"

