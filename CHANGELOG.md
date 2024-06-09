# Changelogs

## Latest version (0.1.5, Flowing River)
### arch
* idt: Add handlers for more IRQs and syscalls
* vmm: New variable "vmm_current_address_space"
* pit: Implement PIT

### log
* logger: Disable serial output by default

### memory
* heap: Implement heap memory.

### process
* process: Implement process scheduler

### srv
* bootsrv: Add support for getting ACPI (R/X)SDP.

### sys
* acpi: Add ACPI RSDP detection

### syscall
* syscall: Start implementation
* syscall: Add print syscall

## 0.1.0 (Flowing River)
### drv
* drv: Port ConIO driver from older SipaaKernel versions.
* framebuffer: Split the initialization process into 2 functions
* framebuffer: Fixed a bug on the red mask
* framebuffer: Added function Fbuf_SetupBasicCapabilities
* vmsvgaii: Now displays the capabilities of the SVGAII adapter.

### fs
* fs: Add HelloFS to demonstate the usage of the VFS
* vfs: Now uses KLibC types.

### klibc
* klibc: Started implementation

### sys
* sys: Add uptime support

### srv
* bootsrv: Added support for getting SMP infos for a CPU
* bootsrv: Added support for getting bootloader infos

### log
* logger: Added 3 new log types: Success, Fatal & Debug
* logger: Only show debug logs when compiled with SK_SHOWDBGLINES defined.
* logger: Uncommented ConIO code

### memory
* pmm: Added Log calls in Pmm_Initialize

### arch
* idt: Call Log when general_interrupt_handler is called (except if the interrupt is from the PIT)
* idt: Call Log when a PS/2 keyboard interrupt is received
* vmm: Fixed INT 13 when switching to kernel PML4 (now works)

### trdparty
* trdparty: Add Flanterm