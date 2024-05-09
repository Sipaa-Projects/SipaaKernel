MAKEFLAGS += --no-print-directory

INCLUDEDIR=src/include
BINDIR=bin
SRCDIR=src
OBJDIR=obj

SRCS := $(shell find $(SRCDIR) -name '*.c')
OBJS := $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o,$(SRCS))
ASMS := $(patsubst $(SRCDIR)/%.asm, $(OBJDIR)/%_asm.o,$(shell find src/ -name '*.asm'))
CC = i686-sipaa-gcc
LD = i686-sipaa-ld

CFLAGS := \
	-w \
	-std=gnu++11 \
	-ffreestanding \
	-fno-stack-protector \
	-fpermissive \
	-fno-stack-check \
	-fno-lto \
	-fno-PIE \
	-fno-PIC \
	-Isrc/include \
	-g \
	-m32 \

ASM_FLAGS := \
	-f elf32

LD_FLAGS := \
	-nostdlib \
	-z max-page-size=0x1000 \
	-T meta/ld/link-i686.ld

$(OBJDIR)/%_asm.o: $(SRCDIR)/%.asm
	@mkdir -p $(@D)
	@echo [NASM] $<
	@nasm $< $(ASM_FLAGS) -o $@

kernel: $(OBJS) $(ASMS)
	@mkdir -p $(BINDIR)
	@echo [LD] kernel.elf
	@$(LD) $(LD_FLAGS) $(OBJS) $(ASMS) -o $(BINDIR)/kernel.sk

# Compile C files
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(@D)
	@echo [CC] $<
	@$(CC) $(CFLAGS) -c $< -o $@

# Build ISO
iso: kernel
	@mkdir -p $(BINDIR)/iso_root/boot/grub
	@echo [CP] Copying kernel files to the ISO file root...
	@cp -r $(BINDIR)/kernel.sk $(BINDIR)/iso_root/boot
	@cp -r meta/grub.cfg $(BINDIR)/iso_root/boot/grub
	@echo [GRUB-MKRESCUE] $(BINDIR)/sipaakernel.iso
	@grub-mkrescue -o $(BINDIR)/sipaakernel.iso $(BINDIR)/iso_root

# Clean
clean:
	rm -rf $(OBJDIR)
	rm -rf $(BINDIR)
	rm -f *.o
	rm -f kernel/disk.img

# Run
run: iso
	qemu-system-i386 -m 1g -serial stdio -cdrom ./$(BINDIR)/sipaakernel.iso -display sdl -device bochs-display -boot d

run-uefi: iso
	qemu-system-i386 -m 1g -serial stdio -cdrom ./$(BINDIR)/sipaakernel.iso -display sdl -device bochs-display -bios ./assets/OVMF-x86_64.fd -boot d

run-kvm: iso
	qemu-system-i386 -m 1g -accel kvm -serial stdio -cdrom ./$(BINDIR)/sipaakernel.iso -display sdl -device bochs-display -boot d

run-kvm-uefi: iso
	qemu-system-i386 -m 1g -accel kvm -serial stdio -cdrom ./$(BINDIR)/sipaakernel.iso -display sdl -device bochs-display -bios ./assets/OVMF-x86_64.fd -boot d -smp 2

run-gtk: iso
	qemu-system-i386 -m 1g -accel kvm -serial stdio -cdrom ./$(BINDIR)/sipaakernel.iso -device bochs-display -boot d

run-gtk-uefi: iso
	qemu-system-i386 -m 1g -accel kvm -serial stdio -cdrom ./$(BINDIR)/sipaakernel.iso -device bochs-display -bios ./assets/OVMF-x86_64.fd -boot d

run-macos: iso
	qemu-system-i386 -m 1g -accel hvf -serial stdio -cdrom ./$(BINDIR)/sipaakernel.iso -device bochs-display -boot d

run-macos-uefi: iso
	qemu-system-i386 -m 1g -accel hvf -serial stdio -cdrom ./$(BINDIR)/sipaakernel.iso -device bochs-display -bios ./assets/OVMF-x86_64.fd -boot d

debug-int: iso
	qemu-system-i386 -m 1g -serial stdio -cdrom ./$(BINDIR)/sipaakernel.iso -d int -M smm=off -display sdl -boot d

debug: iso
	qemu-system-i386 -m 1g -serial stdio -cdrom ./$(BINDIR)/sipaakernel.iso -s -S -display sdl -boot d

debug-kvm: iso
	qemu-system-i386 -m 1g -accel kvm -serial stdio -cdrom ./$(BINDIR)/sipaakernel.iso -s -S -display sdl -boot d