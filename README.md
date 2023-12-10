<p align="center">
  <img src="meta/artwork/LogoLight.png#gh-light-mode-only" height="96" />
  <img src="meta/artwork/LogoDark.png#gh-dark-mode-only" height="96" />
  <p align="center">The modern, multi-architecture, elegant, easy-to-understand kernel.</p>

</p>

## What is SipaaKernel?
SipaaKernel is a monolithic kernel made in C

It features an easy & elegant API.

## Features & Todo
### What does it mean?
* ✅ : Done
* ⚠️ : Buggy / Broken / Incomplete
* ❌ : To do

### x86_64 architecture (src/kernel/sk-x86_64)
* ✅ IDT
* ✅ GDT
* ✅ PIC
* ✅ TSS
* ❌ PCI
* ✅ IO port operations

### AArch64 (src/kernel/sk-aarch64)
SipaaKernel can be built & runned on AArch64, but not anything specific to this architecture is implemented.

### Kernel (src/kernel/sk-core)
* ✅ Logging (graphical & serial)
* ✅ Kernel Panic
* ❌ ELF Loader
* ❌ Kernel Extensions
* ✅ BMO (Basic Memory Operations) (memcpy, memset...)
* ✅ PMM (Physical Memory Manager)
* ❌ VMM (Virtual Memory Manager)
* ❌ initrd

### Kernel (in User Mode)
* ❌ Tasking
* ❌ User Space
* ❌ Syscalls

### Devices (src/kernel/sk-dev)
* ❌ PS2 Mouse
* ⚠️ PS2 Keyboard (no proper API, only on x86_64)
* ❌ USB Mouse
* ❌ USB Keyboard
* ❌ Disks (Ramdisk, VFS)
* ⚠️ Graphics (no proper API, no GPU acceleration)
* ⚠️ Serial (only on x86_64)
* ❌ SoundBlaster 16
* ❌ PC speaker
* ❌ Intel HDA

## How to build SipaaKernel?
### Step 1 : Install the dependencies
SipaaKernel needs the GCC ELF toolchain, NASM to be built. If you are too lazy to build the toolchains, you can use the prebuilt one of Homebrew Formulae. QEMU & Xorriso are required if you want to build the ISO & run SipaaKernel.
All (or most) these packages should be on available on your distro's package repo. SipaaKernel also contains an underlying tool to compile it called SK-Build. It needs the .NET 7 SDK to be built.

#### On Arch Linux
```bash
$ pacman -S qemu-full nasm make dotnet-sdk-7.0 xorriso
$ brew install x86_64-elf-gcc aarch64-elf-gcc
```

### Step 2 : Build & Boot SipaaKernel
Now, you just need to use `make run` to build, make the ISO & run SipaaKernel in QEMU with KVM.
You can also use `make iso` to build SipaaKernel & it's ISO file without running it in QEMU.

## Q&A

### Why chosing CuteKit?
We choosed CuteKit for having a good kernel directory structure

### Will it have a desktop environment & apps?
Nope, but you can make an OS with the SipaaKernel APIs.

## Credits
* Cute Engineering : Cutekit (the build system used to compile SipaaKernel)
* Limine Contributors : The Limine bootloader & his protocol. (boot/*, kernel/src/boot/limine.h)