#include <sipaa/i686/tss.h>
#include <sipaa/i686/gdt.h>

TSS g_tss;

extern uint32_t get_eip();

static void set_tss_entry(int index, uint16_t ss0, uint32_t esp0) {
    uint32_t eip = get_eip();
    uint32_t base = (uint32_t)&g_tss;
    uint32_t limit = base + sizeof(g_tss);

    gdt_set_entry(index, base, limit, 0xE9, 0x00);
    memset(&g_tss, 0x0, sizeof(TSS));

    g_tss.ss0 = ss0;
    g_tss.esp0 = esp0;
    g_tss.eip = eip;
    g_tss.cs = 0x0b;
    g_tss.ss = 0x13;
    g_tss.ds = 0x13;
    g_tss.es = 0x13;
    g_tss.fs = 0x13;
    g_tss.gs = 0x13;
    g_tss.iomap_base = sizeof(TSS);
}

void tss_init() {
    set_tss_entry(5, 0x10, 0x0100000);
    load_tss();
}

void tss_set_stack(uint32_t esp0) {
    g_tss.esp0 = esp0;
}