<p align="center">
  <img src="meta/artwork/LogoLight.png#gh-light-mode-only" height="96" />
  <img src="meta/artwork/LogoDark.png#gh-dark-mode-only" height="96" />
  <p align="center">The modern, elegant, easy-to-understand kernel.</p>

</p>

## What is SipaaKernel?
SipaaKernel is a monolithic kernel made in C++ for the x86-64 architecture.

It features an easy & elegant API.

## Features
* Boot :
  * UEFI/BIOS support
  * Powered by Limine

* Kernel :
  * x86-64 architecture & long mode
  * IDT/PIC
    * Keyboard support
  * GDT/TSS
  * Video
  * Serial Console
  * Video Console
  * Logging
  * VMM & Paging
  * Really basic PCI

## Q&A

### Why chosing C++?
We choosed C++ to have an easy & elegant API for making the best OSes.

### It will be compatible for ARM?
Maybe, if i get an ARM device.

### Will it have a desktop environment & apps?
Nope, but you can make an OS with the SipaaKernel APIs.