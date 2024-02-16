ARCH=i686
KERNEL_ELF = output/kernel-$(ARCH).elf
KERNEL_ELF_WIN = output\kernel-$(ARCH).elf
OUTPUT_ISO = output/sk-$(ARCH).iso
TEMP_ISO = .skb/iso-$(ARCH)
TEMP_ISO_WIN = .skb\iso-$(ARCH)
SK_BUILD_UNIX = sk-build/bin/Debug/net7.0/sk-build
SK_BUILD_WIN32 = sk-build/bin/Debug/net7.0/sk-build.exe
XORRISO_WIN32 = meta\xorriso-win\xorriso.exe

all:
	@$(SK_BUILD_UNIX) build -t kernel-i686-gnu

config:
	@$(SK_BUILD_UNIX) configure

wconfig:
	@$(SK_BUILD_WIN32) configure

iso: all
	@rm -rf $(TEMP_ISO)
	@mkdir -p $(TEMP_ISO)
	@cp meta/limine-cfg/limine-i686.cfg meta/limine-cfg/wallp.jpg meta/limine/limine-bios.sys meta/limine/limine-bios-cd.bin meta/limine/limine-uefi-cd.bin $(TEMP_ISO)/
	@mv $(TEMP_ISO)/limine-i686.cfg $(TEMP_ISO)/limine.cfg
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
	qemu-system-i386 -m 4g -enable-kvm -serial stdio -cdrom $(OUTPUT_ISO)

debug:
	make iso
	qemu-system-i386 -m 4g -enable-kvm -serial stdio -cdrom $(OUTPUT_ISO) -s -S

wrun:
	make wiso
	qemu-system-i386 -m 4g -accel whpx -serial stdio -cdrom $(OUTPUT_ISO)

wdebug:
	make wiso
	qemu-system-i386 -m 4g -accel whpx -serial stdio -cdrom $(OUTPUT_ISO) -s -S