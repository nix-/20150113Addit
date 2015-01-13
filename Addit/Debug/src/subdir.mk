################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/Addit.c \
../src/cr_startup_lpc175x_6x.c \
../src/crp.c 

OBJS += \
./src/Addit.o \
./src/cr_startup_lpc175x_6x.o \
./src/crp.o 

C_DEPS += \
./src/Addit.d \
./src/cr_startup_lpc175x_6x.d \
./src/crp.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DDEBUG -DUSB_DEVICE_ONLY -D__LPC175X_6X__ -D__CODE_RED -DCORE_M3 -D__LPC17XX__ -D__REDLIB__ -I"C:\Projects\20150113Addit\Addit\inc" -I"C:\Projects\20150113Addit\Addit\sys" -I../../software/CMSIS/CMSIS/Include -I../../software/lpc_core/lpc_ip -I../../software/lpc_core/lpc_chip/chip_common -I../../software/lpc_core/lpc_chip/chip_17xx_40xx -I../../software/lpc_core/lpc_board/board_common -I../../software/lpc_core/lpc_board/boards_17xx_40xx/nxp_xpresso_1769 -I../../software/LPCUSBLib/Drivers/USB -O0 -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m3 -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


