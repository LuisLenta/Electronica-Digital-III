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
all: sanchez_tp1_open.axf

# Tool invocations
sanchez_tp1_open.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU Linker'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -T sanchez_tp1_open.ld -o "sanchez_tp1_open.axf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(EXECUTABLES)$(OBJS)$(C_DEPS) sanchez_tp1_open.axf
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	-arm-none-eabi-size "sanchez_tp1_open.axf"; # arm-none-eabi-objcopy -v -O binary "sanchez_tp1_open.axf" "sanchez_tp1_open.bin" ; # checksum -p LPC1769 -d "sanchez_tp1_open.bin";
	-@echo ' '

.PHONY: all clean dependents post-build

-include ../makefile.targets
