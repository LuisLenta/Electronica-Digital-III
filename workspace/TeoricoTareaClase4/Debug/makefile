################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: TeoricoTareaClase4.axf

# Tool invocations
TeoricoTareaClase4.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU Linker'
	arm-none-eabi-gcc -nostdlib -L"C:\Users\Luis Lenta\Documents\MCUXpressoIDE_11.0.0_2516\workspace\CMSISv2p00_LPC17xx\Debug\Drivers" -L"C:\Users\Luis Lenta\Documents\MCUXpressoIDE_11.0.0_2516\workspace\CMSISv2p00_LPC17xx\Debug" -Xlinker -Map="TeoricoTareaClase4.map" -Xlinker --cref -Xlinker --gc-sections -Xlinker -print-memory-usage -mcpu=cortex-m3 -mthumb -T "TeoricoTareaClase4_Debug.ld" -o "TeoricoTareaClase4.axf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(EXECUTABLES)$(OBJS)$(C_DEPS) TeoricoTareaClase4.axf
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	-arm-none-eabi-size "TeoricoTareaClase4.axf"; # arm-none-eabi-objcopy -v -O binary "TeoricoTareaClase4.axf" "TeoricoTareaClase4.bin" ; # checksum -p LPC1769 -d "TeoricoTareaClase4.bin";
	-@echo ' '

.PHONY: all clean dependents post-build

-include ../makefile.targets
