################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/cr_startup_lpc175x_6x.c \
../src/crp.c \
../src/sanchez_tp1_open.c \
../src/sysinit.c 

OBJS += \
./src/cr_startup_lpc175x_6x.o \
./src/crp.o \
./src/sanchez_tp1_open.o \
./src/sysinit.o 

C_DEPS += \
./src/cr_startup_lpc175x_6x.d \
./src/crp.d \
./src/sanchez_tp1_open.d \
./src/sysinit.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -I"C:\Users\Lucas\Documents\MCUXpressoIDE_11.0.0_2516\workspace\lpc_board_nxp_lpcxpresso_1769" -I"C:\Users\Lucas\Documents\MCUXpressoIDE_11.0.0_2516\workspace\lpc_chip_175x_6x" -include"C:\Users\Lucas\Documents\MCUXpressoIDE_11.0.0_2516\workspace\lpc_chip_175x_6x" -include"C:\Users\Lucas\Documents\MCUXpressoIDE_11.0.0_2516\workspace\lpc_board_nxp_lpcxpresso_1769\inc\board.h" -O0 -g3 -Wall -c -fmessage-length=0 -mcpu=cortex-m3 -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


