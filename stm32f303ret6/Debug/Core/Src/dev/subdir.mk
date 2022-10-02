################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/dev/dev_adc.c \
../Core/Src/dev/dev_hash.c \
../Core/Src/dev/dev_opamp2_custom_gain.c 

OBJS += \
./Core/Src/dev/dev_adc.o \
./Core/Src/dev/dev_hash.o \
./Core/Src/dev/dev_opamp2_custom_gain.o 

C_DEPS += \
./Core/Src/dev/dev_adc.d \
./Core/Src/dev/dev_hash.d \
./Core/Src/dev/dev_opamp2_custom_gain.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/dev/%.o Core/Src/dev/%.su: ../Core/Src/dev/%.c Core/Src/dev/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F303xE -c -I../Core/Inc -I../Drivers/STM32F3xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F3xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F3xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-dev

clean-Core-2f-Src-2f-dev:
	-$(RM) ./Core/Src/dev/dev_adc.d ./Core/Src/dev/dev_adc.o ./Core/Src/dev/dev_adc.su ./Core/Src/dev/dev_hash.d ./Core/Src/dev/dev_hash.o ./Core/Src/dev/dev_hash.su ./Core/Src/dev/dev_opamp2_custom_gain.d ./Core/Src/dev/dev_opamp2_custom_gain.o ./Core/Src/dev/dev_opamp2_custom_gain.su

.PHONY: clean-Core-2f-Src-2f-dev

