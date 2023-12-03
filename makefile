KERNEL_ELF = output/kernel-x86_64.elf
OUTPUT_ISO = output/sk-x86_64.iso
TEMP_ISO = .skb/iso
SK_BUILD = ./sk-build.sh

all:
	@$(SK_BUILD) build

config:
	@$(SK_BUILD) config

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

clean:
	$(SK_BUILD) clean
	rm $(OUTPUT_ISO)

run:
	make iso
	qemu-system-x86_64 -m 4g -enable-kvm -serial stdio -cdrom $(OUTPUT_ISO)

debug:
	make iso
	qemu-system-x86_64 -m 4g -enable-kvm -serial stdio -cdrom $(OUTPUT_ISO) -s -S