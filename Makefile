MAKEFLAGS += --no-print-directory

SRCS := $(shell find kernel/ -name '*.cpp')
OBJS := $(patsubst kernel/%.cpp,kernel/obj/%.o,$(SRCS))
ASMS := $(patsubst kernel/%.asm,kernel/obj/asm/%.o,$(shell find kernel/ -name '*.asm'))
CC = g++
LD = ld

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
	-m64 \
	-Ikernel/src/ \
	-march=x86-64 \
	-mabi=sysv \
	-g \
	-mno-80387 \
	-mno-mmx \
	-mno-sse \
	-mno-sse2 \
	-mno-red-zone \
	-mcmodel=kernel

ASM_FLAGS := \
	-f elf64

LD_FLAGS := \
	-nostdlib \
	-static \
	-m elf_x86_64 \
	-z max-page-size=0x1000 \
	-T kernel/link.ld

FAT_IMAGE := kernel/disk.img

kernel.elf: $(OBJS) $(ASMS)
	@make disk_img
	@mkdir -p kernel/bin
	@echo [LD] kernel.elf
	@$(LD) $(LD_FLAGS) $(OBJS) $(ASMS) -o kernel/bin/kernel.elf

kernel/obj/%.o: kernel/%.cpp
	@mkdir -p $(@D)
	@echo [CC] $<
	@$(CC) $(CFLAGS) -c $< -o $@

kernel/obj/asm/%.o: kernel/%.asm
	@mkdir -p $(@D)
	@echo [NASM] $<
	@nasm $< $(ASM_FLAGS) -o $@

disk_img:
	@echo [DD] Creating disk image...
	@dd if=/dev/zero of=kernel/disk.img bs=1M count=64 status=none
	@echo [MKFS.FAT] Formatting disk image to FAT32...
	@mformat -i $(FAT_IMAGE) -F ::
	@echo [CP] Copying files to the disk image...
	@mcopy -i $(FAT_IMAGE) ./disk/* ::/
	@echo [DONE] FAT32 disk image created: $(FAT_IMAGE)

iso:
	@mkdir -p kernel/bin/iso_root
	@echo [CP] Copying kernel files to the ISO file root...
	@cp kernel/bin/kernel.elf \
		kernel/config/limine.cfg kernel/res/wallpaper.bmp limine/limine.sys limine/limine-cd.bin limine/limine-cd-efi.bin kernel/bin/iso_root/
	@echo [XORRISO] kernel/bin/sipaakernel.iso
	@xorriso -as mkisofs -b limine-cd.bin \
		-no-emul-boot -boot-load-size 4 -boot-info-table \
		--efi-boot limine-cd-efi.bin \
		-efi-boot-part --efi-boot-image --protective-msdos-label \
		kernel/bin/iso_root -o kernel/bin/sipaakernel.iso
	@echo [LIMINE-DEPLOY] kernel/bin/sipaakernel.iso
	@limine/limine-deploy kernel/bin/sipaakernel.iso

clean:
	rm -rf kernel/obj
	rm -rf kernel/bin
	rm -f *.o
	rm -f kernel/disk.img
	rm -f skpt/skpt

run:
	@make iso
	qemu-system-x86_64 -m 1g -enable-kvm -serial stdio -cdrom ./kernel/bin/sipaakernel.iso -display sdl -vga vmware -hda kernel/disk.img -boot d

run-uefi:
	@make iso
	qemu-system-x86_64 -m 1g -enable-kvm -serial stdio -cdrom ./kernel/bin/sipaakernel.iso -display sdl -vga vmware -bios /usr/share/qemu/OVMF.fd -hda kernel/disk.img -boot d

run-gtk:
	@make iso
	qemu-system-x86_64 -m 1g -enable-kvm -serial stdio -cdrom ./kernel/bin/sipaakernel.iso -vga vmware -boot d

run-gtk-uefi:
	@make iso
	qemu-system-x86_64 -m 1g -enable-kvm -serial stdio -cdrom ./kernel/bin/sipaakernel.iso -vga vmware -bios /usr/share/qemu/OVMF.fd -hda kernel/disk.img -boot d

debug-int:
	@make iso
	qemu-system-x86_64 -m 1g -serial stdio -cdrom ./kernel/bin/sipaakernel.iso -d int -M smm=off -display sdl -hda kernel/disk.img -boot d

debug:
	@make iso
	qemu-system-x86_64 -m 1g -enable-kvm -serial stdio -cdrom ./kernel/bin/sipaakernel.iso -s -S -display sdl -hda kernel/disk.img -boot d

sipaakpt:
	@echo [GCC] skpt/main.c to skpt/skpt
	@gcc ./skpt/main.c -o ./skpt/skpt -lcjson
	@echo [CP] skpt/skpt to /usr/bin/skpt
	@cp ./skpt/skpt /usr/bin/skpt
	@echo [CHMOD] Allowing user to run skpt...
	@chmod +x /usr/bin/skpt
