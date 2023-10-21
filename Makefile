MAKEFLAGS += --no-print-directory

ARCH=x86_64

INCLUDEDIR=kernel/inc
BINDIR=kernel/bin-$(ARCH)
SRCDIR=kernel/src
RESDIR=kernel/res
OBJDIR=kernel/obj-$(ARCH)

SRCS := $(shell find $(SRCDIR) -name '*.c')
OBJS := $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o,$(SRCS))
x86_64_ASMS := $(patsubst $(SRCDIR)/%.asm, $(OBJDIR)/asm/%.o,$(shell find kernel/ -name '*.asm'))

CC = $(ARCH)-elf-gcc
LD = $(ARCH)-elf-ld

CFLAGS := \
	-w \
	-Dlimine \
	-std=gnu++11 \
	-ffreestanding \
	-fno-stack-protector \
	-fpermissive \
	-fno-stack-check \
	-fno-lto \
	-fno-PIE \
	-fno-PIC \
	-Ikernel/src/ \
	-g

ifeq ($(ARCH),x86_64)
CFLAGS += \
	-m64 \
	-mabi=sysv \
	-march=x86-64 \
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
	-T kernel/lnk/$(ARCH).ld

FAT_IMAGE := kernel/disk.img

ifeq ($(ARCH), x86_64)
$(OBJDIR)/asm/%.o: $(SRCDIR)/%.asm
	@mkdir -p $(@D)
	@echo [NASM] $<
	@nasm $< $(ASM_FLAGS) -o $@

kernel: $(OBJS) $(x86_64_ASMS)
	@mkdir -p $(BINDIR)
	@echo [LD] kernel.elf
	@$(LD) $(LD_FLAGS) $(OBJS) $(x86_64_ASMS) -o $(BINDIR)/kernel.sk
else
kernel: $(OBJS)
	@mkdir -p $(BINDIR)
	@echo [LD] kernel.elf
	@$(LD) $(LD_FLAGS) $(OBJS) -o $(BINDIR)/kernel.sk
endif

# Compile C files
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(@D)
	@echo [CC] $<
	@$(CC) $(CFLAGS) -c $< -o $@

# Build ISO
iso: kernel
	@mkdir -p  $(BINDIR)/iso_root
	@echo [CP] Copying kernel files to the ISO file root...
	@cp -r $(BINDIR)/kernel.sk \
		boot/limine.cfg boot/wlp.jpg boot/limine-bios.sys boot/limine-bios-cd.bin boot/limine-uefi-cd.bin $(BINDIR)/iso_root/
	@echo [XORRISO] $(BINDIR)/sipaakernel.iso
	@xorriso -as mkisofs -b limine-bios-cd.bin \
		-no-emul-boot -boot-load-size 4 -boot-info-table \
		--efi-boot limine-uefi-cd.bin \
		-efi-boot-part --efi-boot-image --protective-msdos-label \
		 $(BINDIR)/iso_root -o $(BINDIR)/sipaakernel.iso
	@echo [LIMINE-DEPLOY] $(BINDIR)/sipaakernel.iso
	@boot/limine bios-install $(BINDIR)/sipaakernel.iso

# Clean
clean:
	rm -rf $(OBJDIR)
	rm -rf $(BINDIR)
	rm -f *.o
	rm -f kernel/disk.img

# Run
run: iso
	qemu-system-x86_64 -m 1g -serial stdio -cdrom ./$(BINDIR)/sipaakernel.iso -display sdl -vga vmware -boot d

run-uefi: iso
	qemu-system-x86_64 -m 1g -serial stdio -cdrom ./$(BINDIR)/sipaakernel.iso -display sdl -vga vmware -bios ./assets/OVMF-x86_64.fd -boot d

run-kvm: iso
	qemu-system-x86_64 -m 1g -accel kvm -serial stdio -cdrom ./$(BINDIR)/sipaakernel.iso -display sdl -vga vmware -boot d

run-kvm-uefi: iso
	qemu-system-x86_64 -m 1g -accel kvm -serial stdio -cdrom ./$(BINDIR)/sipaakernel.iso -display sdl -vga vmware -bios ./assets/OVMF-x86_64.fd -boot d -smp 2

run-gtk: iso
	qemu-system-x86_64 -m 1g -accel kvm -serial stdio -cdrom ./$(BINDIR)/sipaakernel.iso -vga vmware -boot d

run-gtk-uefi: iso
	qemu-system-x86_64 -m 1g -accel kvm -serial stdio -cdrom ./$(BINDIR)/sipaakernel.iso -vga vmware -bios ./assets/OVMF-x86_64.fd -boot d

run-macos: iso
	qemu-system-x86_64 -m 1g -accel hvf -serial stdio -cdrom ./$(BINDIR)/sipaakernel.iso -vga vmware -boot d

run-macos-uefi: iso
	qemu-system-x86_64 -m 1g -accel hvf -serial stdio -cdrom ./$(BINDIR)/sipaakernel.iso -vga vmware -bios ./assets/OVMF-x86_64.fd -boot d

debug-int: iso
	qemu-system-x86_64 -m 1g -serial stdio -cdrom ./$(BINDIR)/sipaakernel.iso -d int -M smm=off -display sdl -boot d

debug: iso
	qemu-system-x86_64 -m 1g -serial stdio -cdrom ./$(BINDIR)/sipaakernel.iso -s -S -display sdl -boot d

debug-kvm: iso
	qemu-system-x86_64 -m 1g -accel kvm -serial stdio -cdrom ./$(BINDIR)/sipaakernel.iso -s -S -display sdl -boot d