################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Utils/debug_huart.c 

C_DEPS += \
./Utils/debug_huart.d 

OBJS += \
./Utils/debug_huart.o 


# Each subdirectory must supply rules for building sources it contributes
Utils/%.o Utils/%.su Utils/%.cyclo: ../Utils/%.c Utils/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F303xC -c -I../Core/Inc -I"/home/axel/STM32CubeIDE/workspace_1.18.1/self_balancing_robot/Utils" -I"/home/axel/STM32CubeIDE/workspace_1.18.1/self_balancing_robot/DriversUser" -I"/home/axel/STM32CubeIDE/workspace_1.18.1/self_balancing_robot/App" -I../Drivers/STM32F3xx_HAL_Driver/Inc -I../Drivers/STM32F3xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F3xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Utils

clean-Utils:
	-$(RM) ./Utils/debug_huart.cyclo ./Utils/debug_huart.d ./Utils/debug_huart.o ./Utils/debug_huart.su

.PHONY: clean-Utils

