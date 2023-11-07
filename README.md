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
* [X] : Done
* [-] : Buggy / Broken / Incomplete
* [ ] : To do

### x86_64 architecture (src/kernel/sk-x86_64)
* [X] IDT
* [X] GDT
* [X] PIC
* [X] TSS
* [ ] PCI
* [X] IO port operations

### AArch64
To do.

### Kernel
* [X] Logging (graphical & serial)
* [X] Kernel Panic
* [ ] ELF Loader

### Devices
* [ ] PS2 Mouse
* [-] PS2 Keyboard (only on x86_64)
* [ ] USB Mouse
* [ ] USB Keyboard
* [ ] Disks (Ramdisk, VFS)
* [-] Graphics (no proper API, no GPU acceleration)
* [-] Serial (only on x86_64)
* [ ] SoundBlaster 16
* [ ] PC speaker
* [ ] Intel HDA

### Memory Management
* [X] BMO (Basic Memory Operations) (memcpy, memset...)
* [X] PMM (Physical Memory Manager)
* [ ] VMM (Virtual Memory Manager)

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

To install them on Arch Linux:
```bash
$ pacman -S llvm qemu-full clang lld
```

### Step 3 : Build & Boot SipaaKernel
Now, you just need to use `ck B` to build & run SipaaKernel in QEMU with KVM (if available)
You can also use `ck bi` to build SipaaKernel & it's ISO file without running it in QEMU.

## Q&A

### Why chosing C?
We choosed C to have an easy & elegant API for making the best OSes.

### Will it have a desktop environment & apps?
Nope, but you can make an OS with the SipaaKernel APIs.

## Credits
* Cute Engineering : Cutekit (the build system used to compile SipaaKernel)
* Limine Contributors : The Limine bootloader & his protocol. (boot/*, kernel/src/boot/limine.h)