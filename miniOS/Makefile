# ----------------------------
# Cross Compiler / Tools
# ----------------------------
TARGET = i686-elf
AS     = $(TARGET)-as
CC     = $(TARGET)-gcc
LD     = $(TARGET)-ld
CFLAGS = -m32 -ffreestanding -O2 -Wall -Wextra -fno-leading-underscore


NASM   = nasm
NASMFLAGS = -f elf32

# ----------------------------
# Directories and files
# ----------------------------
BUILD_DIR = build
ISO_DIR   = iso
ISO_FILE  = miniOS.iso

BOOT_SRC    = src/boot/boot.s
KERNEL_SRCS = \
	src/kernel/kernel.c \
	src/kernel/screen.c \
	src/kernel/keyboard.c \
	src/kernel/idt.c \
	src/kernel/pic.c

ASM_SRCS    = src/kernel/isr.asm

LD_SCRIPT   = src/kernel/linker.ld

BOOT_OBJ    = $(BUILD_DIR)/boot.o
KERNEL_OBJS = \
	$(BUILD_DIR)/kernel.o \
	$(BUILD_DIR)/screen.o \
	$(BUILD_DIR)/keyboard.o \
	$(BUILD_DIR)/idt.o \
	$(BUILD_DIR)/pic.o

ASM_OBJS    = $(BUILD_DIR)/isr.o

KERNEL_BIN  = $(BUILD_DIR)/kernel.bin

# ----------------------------
# Default target
# ----------------------------
all: $(KERNEL_BIN)

# ----------------------------
# Create build directory
# ----------------------------
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# ----------------------------
# Compile bootloader
# ----------------------------
$(BOOT_OBJ): $(BOOT_SRC) | $(BUILD_DIR)
	$(AS) $< -o $@

# ----------------------------
# Compile C kernel modules
# ----------------------------
$(BUILD_DIR)/%.o: src/kernel/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# ----------------------------
# Assemble ASM modules
# ----------------------------
$(BUILD_DIR)/%.o: src/kernel/%.asm | $(BUILD_DIR)
	$(NASM) $(NASMFLAGS) $< -o $@

# ----------------------------
# Link everything into kernel.bin
# Ensure keyboard.o is before isr.o
# ----------------------------
$(KERNEL_BIN): $(BOOT_OBJ) $(KERNEL_OBJS) $(ASM_OBJS)
	$(LD) -T $(LD_SCRIPT) -o $@ $(BOOT_OBJ) \
		$(BUILD_DIR)/kernel.o \
		$(BUILD_DIR)/screen.o \
		$(BUILD_DIR)/keyboard.o \
		$(BUILD_DIR)/idt.o \
		$(BUILD_DIR)/pic.o \
		$(BUILD_DIR)/isr.o

# ----------------------------
# Clean build files
# ----------------------------
clean:
	rm -rf $(BUILD_DIR)/* $(ISO_DIR)/* $(ISO_FILE)


# ----------------------------
# Copy kernel to ISO and rebuild ISO
# ----------------------------
iso: $(KERNEL_BIN)
	mkdir -p $(ISO_DIR)/boot/grub
	cp $(KERNEL_BIN) $(ISO_DIR)/boot/
	@echo -e 'set timeout=0\nset default=0\nmenuentry "miniOS" { multiboot /boot/kernel.bin }' > $(ISO_DIR)/boot/grub/grub.cfg
	grub-mkrescue -o $(ISO_FILE) $(ISO_DIR)

# ----------------------------
# Run OS in QEMU
# ----------------------------
run: iso
	qemu-system-i386 -cdrom $(ISO_FILE) -boot d -m 512
