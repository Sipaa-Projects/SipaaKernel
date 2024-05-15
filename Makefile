MAKEFLAGS += --no-print-directory

INCLUDEDIR=src/include
BINDIR=bin
SRCDIR=src
OBJDIR=obj

SRCS := $(shell find $(SRCDIR) -name '*.c')
OBJS := $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o,$(SRCS))
ASMS := $(patsubst $(SRCDIR)/%.asm, $(OBJDIR)/%_asm.o,$(shell find src/ -name '*.asm'))
CC = x86_64-sipaa-gcc
LD = x86_64-sipaa-ld

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
	-m64 \
	-march=x86-64 \
	-mabi=sysv \
	-mno-80387 \
	-mno-mmx \
	-mno-sse \
	-mno-sse2 \
	-mno-red-zone \
	-mcmodel=kernel \

ASM_FLAGS := \
	-f elf64

LD_FLAGS := \
	-nostdlib \
	-z max-page-size=0x1000 \
	-T meta/ld/link-x86_64.ld

$(OBJDIR)/%_asm.o: $(SRCDIR)/%.asm
	@mkdir -p $(@D)
	@echo [NASM] $<
	@nasm $< $(ASM_FLAGS) -o $@

kernel: $(OBJS) $(ASMS)
	@mkdir -p $(BINDIR)
	@echo [LD] kernel.sk
	@$(LD) $(LD_FLAGS) $(OBJS) $(ASMS) -o $(BINDIR)/kernel.sk

# Compile C files
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(@D)
	@echo [CC] $<
	@$(CC) $(CFLAGS) -c $< -o $@

# Build ISO
iso: kernel
	@rm -rf bin/iso_root
	@mkdir -p bin/iso_root
	@echo [TAR] ramdisk.tar
	@tar -C initrd -cvf bin/ramdisk.tar $(shell find ./initrd -printf '%P\n')
	@echo [CP] Copying kernel files to the ISO file root...
	@cp bin/kernel.sk \
		meta/limine.cfg meta/limine/limine-bios.sys meta/limine/limine-bios-cd.bin meta/limine/limine-uefi-cd.bin meta/wallp.bmp bin/ramdisk.tar bin/iso_root/
	@echo [XORRISO] sipaakernel.iso
	@xorriso -as mkisofs -b limine-bios-cd.bin \
		-no-emul-boot -boot-load-size 4 -boot-info-table \
		--efi-boot limine-uefi-cd.bin \
		-efi-boot-part --efi-boot-image --protective-msdos-label \
		bin/iso_root -o bin/sipaakernel.iso
	@echo [LIMINE-DEPLOY] bin/sipaakernel.iso
	@meta/limine/limine bios-install bin/sipaakernel.iso
	@rm -rf bin/iso_root

# Clean
clean:
	rm -rf $(OBJDIR)
	rm -rf $(BINDIR)
	rm -f *.o
	rm -f kernel/disk.img

# Run
run: iso
	qemu-system-x86_64 -m 1g -serial stdio -cdrom ./$(BINDIR)/sipaakernel.iso -display sdl -vga std -boot d -no-reboot -no-shutdown

run-uefi: iso
	qemu-system-x86_64 -m 1g -serial stdio -cdrom ./$(BINDIR)/sipaakernel.iso -display sdl -vga std -bios /usr/share/edk2/x64/OVMF.fd -boot d -no-reboot -no-shutdown

run-kvm: iso
	qemu-system-x86_64 -m 1g -accel kvm -serial stdio -cdrom ./$(BINDIR)/sipaakernel.iso -display sdl -vga std -boot d -no-reboot -no-shutdown

run-kvm-uefi: iso
	qemu-system-x86_64 -m 1g -accel kvm -serial stdio -cdrom ./$(BINDIR)/sipaakernel.iso -display sdl -vga std -bios /usr/share/edk2/x64/OVMF.fd -boot d -smp 2 -no-reboot -no-shutdown

run-gtk: iso
	qemu-system-x86_64 -m 1g -accel kvm -serial stdio -cdrom ./$(BINDIR)/sipaakernel.iso -vga std -boot d -no-reboot -no-shutdown

run-gtk-uefi: iso
	qemu-system-x86_64 -m 1g -accel kvm -serial stdio -cdrom ./$(BINDIR)/sipaakernel.iso -vga std -bios /usr/share/edk2/x64/OVMF.fd -boot d -no-reboot -no-shutdown

run-macos: iso
	qemu-system-x86_64 -m 1g -accel hvf -serial stdio -cdrom ./$(BINDIR)/sipaakernel.iso -vga std -boot d -no-reboot -no-shutdown

run-macos-uefi: iso
	qemu-system-x86_64 -m 1g -accel hvf -serial stdio -cdrom ./$(BINDIR)/sipaakernel.iso -vga std -bios /usr/share/edk2/x64/OVMF.fd -boot d -no-reboot -no-shutdown

debug-int: iso
	qemu-system-x86_64 -m 1g -serial stdio -cdrom ./$(BINDIR)/sipaakernel.iso -d int -M smm=off -display sdl -boot d -no-reboot -no-shutdown

debug: iso
	qemu-system-x86_64 -m 1g -serial stdio -cdrom ./$(BINDIR)/sipaakernel.iso -s -S -display sdl -boot d -no-reboot -no-shutdown

debug-kvm: iso
	qemu-system-x86_64 -m 1g -accel kvm -serial stdio -cdrom ./$(BINDIR)/sipaakernel.iso -s -S -display sdl -boot d -no-reboot -no-shutdown