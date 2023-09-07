# Building SipaaKernel

## Linux
### 1 : Get the required packages
Before building the kernel, you may install this packages on your system :

`build-essential nasm xorriso qemu-system-x86 mtools make git` (*Debian based systems*)

`qemu-full nasm xorriso mtools make git` (*Arch Linux based systems*)

### 1.1 : Get the required toolchain(s)
To build SipaaKernel, you need a toolchain.

You can build it by [yourself](https://wiki.osdev.org/GCC_Cross-Compiler), or you can install a pre-built copy using the Homebrew package manager.

Do do that :
* Install Homebrew, instructions can be found [here](https://brew.sh)
* Add `PATH="$PATH:/home/linuxbrew/.linuxbrew/bin"` to your `~/.bashrc` file.

Now, if you wanna install the x86_64 toolchain, run `brew install x86_64-elf-gcc`, if you wanna install the aarch64 toolchain, run `brew install aarch64-elf-gcc`, or you can install the two toolchains

### 2 : Clone the repo
Just easy as paste `git clone https://github.com/SipaaOS-Project/SipaaKernel` in your terminal

### 2.1 : Build Limine command
If you don't see any 'limine' executable in the 'limine' directory, you must build it.
To do that, go in that 'limine' directory, and run make.

### 3 : Build it
Go in the SipaaKernel directory by typing `cd SipaaKernel` in your terminal.

Then, run `make`. The kernel source code will be compiled to a `kernel.elf` file (i see you trying to execute it on your host...)

But a `kernel.elf` file is not enougth for us... So we will run `make iso` to build the ISO file. (or `make run` to build the ISO, then run it on QEMU)

### 4 : Run it
#### 4.1 : QEMU
You have many choices :
* `run` : Run with classical options
* `run-uefi` : Run with UEFI (needs a copy of OVMF in `/usr/share/qemu`)
* `run-gtk` : Same as `run`, but with GTK as display
* `run-gtk-uefi` : Same as `run-uefi`, but with GTK as display
* `debug-int` : Debug interrupts. Can be used when IDT problems are present.
* `debug` : Debug mode. Start a GDB server at `127.0.0.1:1234` and halt the CPU for you to setup a GDB environment
#### 4.2 : Real x86_64 device
You can flash the SipaaKernel ISO for x86_64 (located in ./kernel/bin/x86_64/sipaakernel.iso) on a USB flash drive, then boot it on real hardware.

## Windows
You can't build SipaaKernel on native Windows, unless you install the Linux subsystem.

## macOS
Same build instructions as Linux, but you need to run it using `make run-macos` or `make run-macos-uefi`

## SerenityOS
Unchecked. You can try, but it's not guaranteed to work.
