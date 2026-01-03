################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../App/app.c 

C_DEPS += \
./App/app.d 

OBJS += \
./App/app.o 


# Each subdirectory must supply rules for building sources it contributes
App/%.o App/%.su App/%.cyclo: ../App/%.c App/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32F303xC -c -I../Core/Inc -I"/home/axel/STM32CubeIDE/workspace_1.18.1/self_balancing_robot/Utils" -I../Drivers/STM32F3xx_HAL_Driver/Inc -I../Drivers/STM32F3xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F3xx/Include -I../Drivers/CMSIS/Include -I"/home/axel/STM32CubeIDE/workspace_1.18.1/self_balancing_robot/App" -I"/home/axel/STM32CubeIDE/workspace_1.18.1/self_balancing_robot/DriversUser" -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-App

clean-App:
	-$(RM) ./App/app.cyclo ./App/app.d ./App/app.o ./App/app.su

.PHONY: clean-App

