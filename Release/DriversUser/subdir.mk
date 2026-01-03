################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../DriversUser/motors.c \
../DriversUser/mpu6050.c 

C_DEPS += \
./DriversUser/motors.d \
./DriversUser/mpu6050.d 

OBJS += \
./DriversUser/motors.o \
./DriversUser/mpu6050.o 


# Each subdirectory must supply rules for building sources it contributes
DriversUser/%.o DriversUser/%.su DriversUser/%.cyclo: ../DriversUser/%.c DriversUser/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32F303xC -c -I../Core/Inc -I"/home/axel/STM32CubeIDE/workspace_1.18.1/self_balancing_robot/Utils" -I../Drivers/STM32F3xx_HAL_Driver/Inc -I../Drivers/STM32F3xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F3xx/Include -I../Drivers/CMSIS/Include -I"/home/axel/STM32CubeIDE/workspace_1.18.1/self_balancing_robot/App" -I"/home/axel/STM32CubeIDE/workspace_1.18.1/self_balancing_robot/DriversUser" -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-DriversUser

clean-DriversUser:
	-$(RM) ./DriversUser/motors.cyclo ./DriversUser/motors.d ./DriversUser/motors.o ./DriversUser/motors.su ./DriversUser/mpu6050.cyclo ./DriversUser/mpu6050.d ./DriversUser/mpu6050.o ./DriversUser/mpu6050.su

.PHONY: clean-DriversUser

