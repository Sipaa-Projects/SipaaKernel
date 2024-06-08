<p align="center">
  <img src="meta/artwork/LogoLight.png#gh-light-mode-only" height="96" />
  <img src="meta/artwork/LogoDark.png#gh-dark-mode-only" height="96" />
  <p align="center">The kernel from a modern era.</p>

</p>

## What is SipaaKernel?
SipaaKernel is a monolithic kernel made in C

It features an easy & elegant API.

> [!WARNING]
> SipaaKernel is only in his first developement stages, and isn't ready for production use.

## Objectives
SipaaKernel aims to:

* Have POSIX compilancy
* Be usable for everyday use
* Be performant
* Be easy
* Be Linux and NT-compatible (use the same syscalls, same APIs), making application porting easier.

## Q&A

### Will it have a desktop environment & apps?
Nope. SipaaKernel is just an operating system kernel: the userspace (CLI commands, libraries, apps) needs to be built manually. (writing one from scratch, or using existing software)

## Credits
### Kernel components
* Charles Nicolson: The `printf` implementation used in SipaaKernel (don't expect me to write one without breaking everything :() )
* Mintsuki & Limine contributors: The Limine bootloader, the Limine boot protocol and finally, Flanterm.

### Special Thanks
* Kokolor: My friend, which did put me in OSDev. Also helped me with multiple things
