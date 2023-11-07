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
* ✅ BMO (Basic Memory Operations) (memcpy, memset...)
* ✅ PMM (Physical Memory Manager)
* ❌ VMM (Virtual Memory Manager)

### Devices (src/kernel/sk-dev)
* ❌ PS2 Mouse
* ⚠️ PS2 Keyboard (only on x86_64)
* ❌ USB Mouse
* ❌ USB Keyboard
* ❌ Disks (Ramdisk, VFS)
* ⚠️ Graphics (no proper API, no GPU acceleration)
* ⚠️ Serial (only on x86_64)
* ❌ SoundBlaster 16
* ❌ PC speaker
* ❌ Intel HDA

## How to build SipaaKernel?
### Step 1 : Install CuteKit on your PC
SipaaKernel uses [CuteKit](https://github.com/cute-engineering/cutekit) as build system/package manager. I (RaphMar2021) made a script to install CuteKit automatically.

Use `./install-ck.sh install` to install CuteKit. 

Note: it won't add automatically into the PATH. To add it, open `~/.bashrc` (or any other shell startup script)
and put that in:
`PATH=$PATH:~/.local/share/venvs/cutekit/bin`

### Step 2 : Install the dependencies
SipaaKernel needs the LLVM toolchain, NASM to be built. QEMU & Xorriso are required if you want to build the ISO & run SipaaKernel.
All (or most) these packages should be on available on your distro's package repo.

#### On Arch Linux
```bash
$ pacman -S llvm qemu-full clang lld
```

#### On Windows (without WSL)
* [LLVM Toolchain (official)](https://github.com/llvm/llvm-project/releases/tag/llvmorg-16.0.6)
* [NASM (official)](https://nasm.us/)
* [QEMU (official mirror for Win64)](https://qemu.weilnetz.de/w64/)
* [Xorriso (unofficial, but no viruses)](https://github.com/PeyTy/xorriso-exe-for-windows)

All the software binaries shown before should be in your PATH variable. To add/modify environment variables on Windows, open `C:\Windows\System32\systempropertiesadvanced.exe`, then click on the Environment Variables button.

### Step 3 : Build & Boot SipaaKernel
Now, you just need to use `ck B` to build & run SipaaKernel in QEMU with KVM (if available)
You can also use `ck bi` to build SipaaKernel & it's ISO file without running it in QEMU.

## Q&A

### Why chosing CuteKit?
We choosed CuteKit for having a good kernel directory structure

### Will it have a desktop environment & apps?
Nope, but you can make an OS with the SipaaKernel APIs.

## Credits
* Cute Engineering : Cutekit (the build system used to compile SipaaKernel)
* Limine Contributors : The Limine bootloader & his protocol. (boot/*, kernel/src/boot/limine.h)