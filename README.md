<p align="center">
  <img src="meta/artwork/LogoLight.png#gh-light-mode-only" height="96" />
  <img src="meta/artwork/LogoDark.png#gh-dark-mode-only" height="96" />
  <p align="center">The modern, multi-architecture, elegant, easy-to-understand kernel.</p>

</p>

## What is SipaaKernel?
SipaaKernel is a monolithic kernel made in C

It features an easy & elegant API.

> [!WARNING]
> SipaaKernel is only in his first developement stages, and isn't ready for production use.

## Objectives
SipaaKernel aims to be:

* Usable for everyday use
* Performant
* Easy
* Linux and NT-compatible (use the same syscalls, same APIs), making application porting easier.

## Q&A

### Will it have a desktop environment & apps?
Nope. SipaaKernel is just an operating system kernel: the userspace (CLI commands, libraries, apps) needs to be built manually. (writing one from scratch, or using existing software)

## Credits
* Limine Contributors : The Limine bootloader & his protocol. (meta/limine-*, src/kernel/entry/limine.h)
* Pritam Zope : i686 architecture implementation, PS2 drivers, string library
* Charles Nicholson : The printf implementation used in SipaaKernel. (nanoprintf)