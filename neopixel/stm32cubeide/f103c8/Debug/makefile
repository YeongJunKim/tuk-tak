################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Startup/subdir.mk
-include Src/subdir.mk
-include Drivers/STM32F1xx_HAL_Driver/Src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
f103c8.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
f103c8.list \

OBJCOPY_HEX += \
f103c8.hex \

OBJCOPY_BIN += \
f103c8.bin \


# All Target
all: f103c8.elf secondary-outputs

# Tool invocations
f103c8.elf: $(OBJS) $(USER_OBJS) C:\Users\TUK-TAK\Desktop\workspace\tuk-tak\neopixel\stm32cubeide\f103c8\STM32F103C8TX_FLASH.ld
	arm-none-eabi-gcc -o "f103c8.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m3 -T"C:\Users\TUK-TAK\Desktop\workspace\tuk-tak\neopixel\stm32cubeide\f103c8\STM32F103C8TX_FLASH.ld" --specs=nosys.specs -Wl,-Map="f103c8.map" -Wl,--gc-sections -static --specs=nano.specs -mfloat-abi=soft -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

f103c8.list: $(EXECUTABLES)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "f103c8.list"
	@echo 'Finished building: $@'
	@echo ' '

f103c8.hex: $(EXECUTABLES)
	arm-none-eabi-objcopy  -O ihex $(EXECUTABLES) "f103c8.hex"
	@echo 'Finished building: $@'
	@echo ' '

f103c8.bin: $(EXECUTABLES)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "f103c8.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) *
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_HEX) $(OBJCOPY_BIN)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
