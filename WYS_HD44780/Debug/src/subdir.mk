################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/WYS_HD44780.c \
../src/cr_startup_lpc11xx.c \
../src/crp.c \
../src/delay.c 

OBJS += \
./src/WYS_HD44780.o \
./src/cr_startup_lpc11xx.o \
./src/crp.o \
./src/delay.o 

C_DEPS += \
./src/WYS_HD44780.d \
./src/cr_startup_lpc11xx.d \
./src/crp.d \
./src/delay.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c src/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DDEBUG -D__CODE_RED -DCORE_M0 -D__USE_CMSIS=CMSIS_CORE_LPC11xx -D__LPC11XX__ -D__REDLIB__ -I"/home/badman/Programowanie/LPC1114_MCU/CMSIS_CORE_LPC11xx/inc" -I"/home/badman/Programowanie/LPC1114_MCU/WYS_HD44780/hd44780" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m0 -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


