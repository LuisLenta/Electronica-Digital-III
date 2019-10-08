################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/FreeRTOS/portable/MemMang/heap_4.c 

OBJS += \
./src/FreeRTOS/portable/MemMang/heap_4.o 

C_DEPS += \
./src/FreeRTOS/portable/MemMang/heap_4.d 


# Each subdirectory must supply rules for building sources it contributes
src/FreeRTOS/portable/MemMang/%.o: ../src/FreeRTOS/portable/MemMang/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DDEBUG -D__CODE_RED -DCORE_M3 -D__USE_CMSIS=CMSISv2p00_LPC17xx -D__LPC17XX__ -D__REDLIB__ -I"C:\Users\Lucas\Documents\MCUXpressoIDE_11.0.0_2516\workspace\tp4\src" -I"C:\Users\Lucas\Documents\MCUXpressoIDE_11.0.0_2516\workspace\CMSISv2p00_LPC17xx\inc" -I"C:\Users\Lucas\Documents\MCUXpressoIDE_11.0.0_2516\workspace\tp4\src\FreeRTOS\include" -I"C:\Users\Lucas\Documents\MCUXpressoIDE_11.0.0_2516\workspace\tp4\src\TraceRecorder\include" -I"C:\Users\Lucas\Documents\MCUXpressoIDE_11.0.0_2516\workspace\tp4\src\FreeRTOS\portable\GCC\ARM_CM3" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m3 -mthumb -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


