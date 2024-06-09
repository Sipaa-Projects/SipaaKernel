MAKEFLAGS += --no-print-directory

KLANG=EN
ARCH=x86_64

INCLUDEDIR=src/include
BINDIR=bin-$(ARCH)
SRCDIR=src
OBJDIR=obj-$(ARCH)

SRCS := $(shell find $(SRCDIR) -name '*.c')
OBJS := $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o,$(SRCS))
ASMS := $(patsubst $(SRCDIR)/%.asm, $(OBJDIR)/%_asm.o,$(shell find src/ -name '*.asm'))
CC = $(ARCH)-elf-gcc
LD = $(ARCH)-elf-ld

CFLAGS := \
	-w \
	-std=gnu++11 \
	-D__SIPAAKERNEL_KLANG_$(KLANG) \
	-ffreestanding \
	-fno-stack-protector \
	-fpermissive \
	-fno-stack-check \
	-fno-lto \
	-fno-PIE \
	-fno-PIC \
	-Isrc/include \
	-g \

ifeq ($(ARCH),x86_64)
override CFLAGS += -m64 \
	-march=x86-64 \
	-mabi=sysv \
	-mno-80387 \
	-mno-mmx \
	-mno-sse \
	-mno-sse2 \
	-mno-red-zone \
	-mcmodel=kernel
endif

ASM_FLAGS := \
	-f elf64

LD_FLAGS := \
	-nostdlib \
	-z max-page-size=0x1000 \
	-T meta/ld/link-$(ARCH).ld

$(OBJDIR)/%_asm.o: $(SRCDIR)/%.asm
ifeq ($(ARCH),x86_64)
	@mkdir -p $(@D)
	@echo [NASM] $<
	@nasm $< $(ASM_FLAGS) -o $@
else
	@$(CC) $(CFLAGS) -c src/arch/asm-nonx86-placeholder.c -o $@
endif

kernel: $(OBJS) $(ASMS)
	@mkdir -p $(BINDIR)
	@echo [LD] kernel-$(ARCH).sk
	@$(LD) $(LD_FLAGS) $(OBJS) $(ASMS) -o $(BINDIR)/kernel-$(ARCH).sk -Map=$(BINDIR)/kernel-$(ARCH).sk.map

# Compile C files
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(@D)
	@echo [CC] $<
	@$(CC) $(CFLAGS) -c $< -o $@

# Download limine
meta/limine:
	@rm -rf meta/limine
	@git clone --branch v7.x-binary https://github.com/limine-bootloader/limine meta/limine/
	@make -C meta/limine

# Build ISO
iso: meta/limine kernel
	@rm -rf bin-$(ARCH)/iso_root
	@mkdir -p bin-$(ARCH)/iso_root
	@echo [TAR] ramdisk.tar
	@tar -C initrd -cvf bin/ramdisk.tar $(shell find ./initrd -printf '%P\n')
	@echo [CP] Copying kernel files to the ISO file root...
	@cp meta/limine.cfg meta/limine/limine-bios.sys meta/limine/limine-bios-cd.bin meta/limine/limine-uefi-cd.bin bin-$(ARCH)/iso_root/
	@cp bin-$(ARCH)/kernel-$(ARCH).sk bin-$(ARCH)/iso_root/kernel.sk
	@echo [XORRISO] Sk-$(ARCH).iso
	@xorriso -as mkisofs -b limine-bios-cd.bin \
		-no-emul-boot -boot-load-size 4 -boot-info-table \
		--efi-boot limine-uefi-cd.bin \
		-efi-boot-part --efi-boot-image --protective-msdos-label \
		bin-$(ARCH)/iso_root -o bin-$(ARCH)/Sk-$(ARCH).iso
	@echo [LIMINE-DEPLOY] bin-$(ARCH)/Sk-$(ARCH).iso
	@meta/limine/limine bios-install bin-$(ARCH)/Sk-$(ARCH).iso
	@rm -rf bin/iso_root

# Clean
clean:
	rm -rf $(OBJDIR)
	rm -rf $(BINDIR)
	rm -f *.o
	rm -f kernel/disk.img

# Run
run: iso
	qemu-system-x86_64 -accel kvm -m 1g -cdrom ./$(BINDIR)/Sk-$(ARCH).iso -vga std -boot d  -no-shutdown -smp 2 -drive file="disk.img",format=raw

run-uefi: iso
	qemu-system-x86_64 -accel kvm -m 1g -cdrom ./$(BINDIR)/Sk-$(ARCH).iso -vga std -bios /usr/share/edk2/x64/OVMF.fd -boot d  -no-shutdown -smp 2

run-vmware: iso
	qemu-system-x86_64 -accel kvm -m 1g -cdrom ./$(BINDIR)/Sk-$(ARCH).iso -vga vmware -boot d  -no-shutdown -smp 2

run-serial: iso
	qemu-system-x86_64 -accel kvm -m 1g -serial stdio -cdrom ./$(BINDIR)/Sk-$(ARCH).iso -display sdl -vga std -boot d  -no-shutdown -smp 2 -drive file="disk.img",format=raw

debug-int: iso
	qemu-system-x86_64 -m 1g -serial stdio -cdrom ./$(BINDIR)/Sk-$(ARCH).iso -d int -M smm=off -display sdl -boot d  -no-shutdown -smp 2

debug: iso
	qemu-system-x86_64 -m 1g -serial stdio -cdrom ./$(BINDIR)/Sk-$(ARCH).iso -s -S -display sdl -boot d  -no-shutdown -smp 2

debug-accel: iso
	qemu-system-x86_64 -m 1g -accel kvm -serial stdio -cdrom ./$(BINDIR)/Sk-$(ARCH).iso -s -S -display sdl -boot d  -no-shutdown -smp 2