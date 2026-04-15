# 1. Define the Toolchain
CC = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy

# 2. Define the Processor Architecture
MACH = cortex-m4
CFLAGS = -c -mcpu=$(MACH) -mthumb -std=gnu11 -Wall -O0
LDFLAGS = -nostdlib -T stm32_ls.ld -Wl,-Map=project.map

# 3. Find all your source files based on your directory structure
SRC = startup.c \
	  main.c \
      mcal/rcc.c \
      mcal/gpio.c \
      mcal/exti.c \
      mcal/nvic.c \
      hal/keypad.c \
      hal/seven_segment.c \
      hal/led.c \
      app/state_machine.c

# 4. Convert .c filenames to .o filenames
OBJ = $(SRC:.c=.o)

# 5. The Output File Name
TARGET = stm32-sec6

# --- Build Rules ---

# Default target when you just type 'make'
all: $(TARGET).hex

# Convert ELF to HEX for Proteus
$(TARGET).hex: $(TARGET).elf
	$(OBJCOPY) -O ihex $< $@
	@echo "Build Successful! Ready for Proteus."

# Link all Object files into an ELF
$(TARGET).elf: $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $^

# Compile every C file into an Object file
%.o: %.c
	$(CC) $(CFLAGS) -o $@ $<

# Type 'make clean' to delete old build files
clean:
	rm -rf *.o mcal/*.o hal/*.o app/*.o *.elf *.hex *.map