################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/gpio.c \
../Src/keypad.c \
../Src/main.c \
../Src/sevenseg.c \
../Src/syscalls.c \
../Src/sysmem.c 

OBJS += \
./Src/gpio.o \
./Src/keypad.o \
./Src/main.o \
./Src/sevenseg.o \
./Src/syscalls.o \
./Src/sysmem.o 

C_DEPS += \
./Src/gpio.d \
./Src/keypad.d \
./Src/main.d \
./Src/sevenseg.d \
./Src/syscalls.d \
./Src/sysmem.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o Src/%.su Src/%.cyclo: ../Src/%.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32L4 -DSTM32 -DNUCLEO_L476RG -DSTM32L476RGTx -c -I../Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Src

clean-Src:
	-$(RM) ./Src/gpio.cyclo ./Src/gpio.d ./Src/gpio.o ./Src/gpio.su ./Src/keypad.cyclo ./Src/keypad.d ./Src/keypad.o ./Src/keypad.su ./Src/main.cyclo ./Src/main.d ./Src/main.o ./Src/main.su ./Src/sevenseg.cyclo ./Src/sevenseg.d ./Src/sevenseg.o ./Src/sevenseg.su ./Src/syscalls.cyclo ./Src/syscalls.d ./Src/syscalls.o ./Src/syscalls.su ./Src/sysmem.cyclo ./Src/sysmem.d ./Src/sysmem.o ./Src/sysmem.su

.PHONY: clean-Src

