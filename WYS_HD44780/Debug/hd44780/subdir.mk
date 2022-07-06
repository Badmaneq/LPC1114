################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../hd44780/hd44780.c 

OBJS += \
./hd44780/hd44780.o 

C_DEPS += \
./hd44780/hd44780.d 


# Each subdirectory must supply rules for building sources it contributes
hd44780/%.o: ../hd44780/%.c hd44780/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DDEBUG -D__CODE_RED -DCORE_M0 -D__USE_CMSIS=CMSIS_CORE_LPC11xx -D__LPC11XX__ -D__REDLIB__ -I"/home/badman/Programowanie/LPC1114_MCU/CMSIS_CORE_LPC11xx/inc" -I"/home/badman/Programowanie/LPC1114_MCU/WYS_HD44780/hd44780" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m0 -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


