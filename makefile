KERNEL_ELF = output/kernel-x86_64.elf
KERNEL_ELF_WIN = output\kernel-x86_64.elf
OUTPUT_ISO = output/sk-x86_64.iso
TEMP_ISO = .skb/iso-x86_64
TEMP_ISO_WIN = .skb\iso-x86_64
SK_BUILD_UNIX = sk-build/bin/Debug/net7.0/sk-build
SK_BUILD_WIN32 = sk-build/bin/Debug/net7.0/sk-build.exe
XORRISO_WIN32 = meta\xorriso-win\xorriso.exe

all:
	@$(SK_BUILD_UNIX) build

wall:
	@$(SK_BUILD_WIN32) build -t Llvm

config:
	@$(SK_BUILD_UNIX) configure

wconfig:
	@$(SK_BUILD_WIN32) configure

iso: all
	@rm -rf $(TEMP_ISO)
	@mkdir -p $(TEMP_ISO)
	@cp meta/limine-cfg/limine.cfg meta/limine-cfg/wallp.jpg meta/limine/limine-bios.sys meta/limine/limine-bios-cd.bin meta/limine/limine-uefi-cd.bin $(TEMP_ISO)/
	@cp $(KERNEL_ELF) $(TEMP_ISO)/kernel.elf
	@xorriso -as mkisofs -b limine-bios-cd.bin \
		-no-emul-boot -boot-load-size 4 -boot-info-table \
		--efi-boot limine-uefi-cd.bin \
		-efi-boot-part --efi-boot-image --protective-msdos-label \
		$(TEMP_ISO) -o $(OUTPUT_ISO)
	@meta/limine/limine bios-install $(OUTPUT_ISO)

wiso: wall
	@-del $(TEMP_ISO_WIN) /q
	@-rmdir $(TEMP_ISO_WIN)
	@mkdir $(TEMP_ISO_WIN)
	@copy meta\limine-cfg\limine.cfg $(TEMP_ISO_WIN)
	@copy meta\limine-cfg\wallp.jpg $(TEMP_ISO_WIN)
	@copy meta\limine\limine-bios.sys $(TEMP_ISO_WIN)
	@copy meta\limine\limine-bios-cd.bin $(TEMP_ISO_WIN)
	@copy meta\limine\limine-uefi-cd.bin $(TEMP_ISO_WIN)
	@copy $(KERNEL_ELF_WIN) $(TEMP_ISO_WIN)\kernel.elf
	@$(XORRISO_WIN32) -as mkisofs -b limine-bios-cd.bin \
		-no-emul-boot -boot-load-size 4 -boot-info-table \
		--efi-boot limine-uefi-cd.bin \
		-efi-boot-part --efi-boot-image --protective-msdos-label \
		$(TEMP_ISO_WIN) -o $(OUTPUT_ISO)
	@meta\limine\limine.exe bios-install $(OUTPUT_ISO)

clean:
	$(SK_BUILD_UNIX) clean

wclean:
	$(SK_BUILD_WIN32) clean

run:
	make iso
	qemu-system-x86_64 -m 4g -enable-kvm -serial stdio -cdrom $(OUTPUT_ISO) -vga vmware

debug:
	make iso
	qemu-system-x86_64 -m 4g -enable-kvm -serial stdio -cdrom $(OUTPUT_ISO) -s -S

wrun:
	make wiso
	qemu-system-x86_64 -m 4g -accel whpx -serial stdio -cdrom $(OUTPUT_ISO)

wdebug:
	make wiso
	qemu-system-x86_64 -m 4g -accel whpx -serial stdio -cdrom $(OUTPUT_ISO) -s -S