#include <arch/x86_64/gdt.h>
#include <arch/x86_64/idt.h>
#include <device/serial/serial.h>
#include <lib/libc/stdlib.h>
#include <lib/liblogging/liblogging.h>
#include <lib/libconsole/libconsole.h>
#include <limine/limine.h>
#include <stdint.h>
#include <video/video.h>
#include <video/bmp.h>

uint64_t kernel_stack[8192];

static volatile struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0};

static volatile struct limine_bootloader_info_request info_request = {
    .id = LIMINE_BOOTLOADER_INFO_REQUEST,
    .revision = 0};

void print_sysinfo()
{
    log(LOG_INFO, "Bootloader : ");
    printf(info_request.response->name);
    serial_putsf(info_request.response->name);
    print_char(' ');
    serial_putc(' ');
    printf(info_request.response->version);
    serial_putsf(info_request.response->version);
    printf(" (revision %llx)\n", info_request.response->revision);
    serial_putsf(" (revision %llx)\n", info_request.response->revision);
    log(LOG_INFO, "Framebuffers : \n");

    for (int i = 0; i < framebuffer_request.response->framebuffer_count; i++)
    {
        struct limine_framebuffer *f = framebuffer_request.response->framebuffers[i];
        printf("       Framebuffer #%d (%dx%dx%d)\n", i, f->width, f->height, f->bpp);
        serial_putsf("       Framebuffer #%d (%dx%dx%d)\n", i, f->width, f->height, f->bpp);
    }
}

void print_issucess(int returncode)
{
    if (returncode == 0)
    {
        serial_puts("Success\n");
        return;
    }
    serial_puts("Fail\n");
}

void _start(void)
{
    init_video(framebuffer_request.response->framebuffers[0]);
    init_serial();
    print_sysinfo();
    init_gdt(kernel_stack);
    init_idt();
    //while (1) { /**log(LOG_DEBUG, "hi\n");**/ }
}