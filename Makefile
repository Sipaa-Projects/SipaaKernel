SRCS := $(shell find kernel/ -name '*.cpp')  # Modifier l'extension des fichiers sources
OBJS := $(SRCS:.cpp=.o)  # Modifier l'extension des fichiers objets
CC = g++  # Utiliser le compilateur C++
LD = ld

CFLAGS :=                  \
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
	-mcmodel=kernel                   \

ASM_FLAGS :=                \
	-f elf64

LD_FLAGS :=                 \
	-nostdlib \
	-static \
	-m elf_x86_64 \
	-z max-page-size=0x1000 \
	-T kernel/link.ld

.SUFFIXE: .cpp  # Modifier l'extension des fichiers sources
%.o: %.cpp  # Modifier l'extension des fichiers sources
	@echo [CC] $<
	@$(CC) $(CFLAGS) -c $< -o $@

kernel.elf: $(OBJS)
	@echo [NASM] kernel/src/arch/gdt.asm
	@nasm kernel/src/arch/gdt.asm -felf64 -o gdt.o
	@echo [LD] kernel.elf
	@$(LD) $(LD_FLAGS) $(OBJS) gdt.o -o $@

iso:
	@rm -rf iso_root
	@mkdir -p iso_root
	@echo [CP] Copying kernel files to the ISO file root...
	@cp kernel.elf \
		limine.cfg wallpaper.bmp limine/limine.sys limine/limine-cd.bin limine/limine-cd-efi.bin iso_root/
	@echo [XORRISO] sipaakernel.iso
	@xorriso -as mkisofs -b limine-cd.bin \
		-no-emul-boot -boot-load-size 4 -boot-info-table \
		--efi-boot limine-cd-efi.bin \
		-efi-boot-part --efi-boot-image --protective-msdos-label \
		iso_root -o sipaakernel.iso
	@echo [LIMINE-DEPLOY] sipaakernel.iso
	@limine/limine-deploy sipaakernel.iso
	@rm -rf iso_root

clean:
	rm -f $(OBJS)
	rm -f kernel.elf
	rm sipaakernel.iso
	rm *.o

run:
	@make iso
	qemu-system-x86_64 -m 4g -enable-kvm -serial stdio -cdrom ./sipaakernel.iso -display sdl -vga vmware

debug-int:
	@make iso
	qemu-system-x86_64 -m 4g -serial stdio -cdrom ./sipaakernel.iso -d int -M smm=off -display sdl

debug:
	@make iso
	qemu-system-x86_64 -m 4g -enable-kvm -serial stdio -cdrom ./sipaakernel.iso -s -S -display sdl
