#include <sipaa/x86_64/vmm.h>
#include <sipaa/bootsrv.h>
#include <sipaa/logger.h>

uint64_t *Vmm_KernelPML4;

void *Vmm_VirtToPhys(ptr) { return (void*)(((uint64_t)ptr) + BootSrv_GetHHDMOffset()); }
void *Vmm_PhysToVirt(ptr) { return (void*)(((uint64_t)ptr) - BootSrv_GetHHDMOffset()); }

void vmm_init()
{
    Log(LT_INFO, "Vmm", "Setting kernel PML4...\n");
    Vmm_KernelPML4 = (uint64_t *)Vmm_VirtToPhys(Pmm_AllocatePage());
    memset(Vmm_KernelPML4, 0, PAGE_SIZE);

    Log(LT_INFO, "Vmm", "Mapping kernel ELF sections...\n");
    uint64_t phys_base = BootSrv_GetKernelPhysicalBase();
    uint64_t virt_base = BootSrv_GetKernelVirtualBase();

    uint64_t text_start = ALIGN_DOWN((uint64_t)TextStartLD, PAGE_SIZE);
    uint64_t text_end = ALIGN_UP((uint64_t)TextEndLD, PAGE_SIZE);
    uint64_t rodata_start = ALIGN_DOWN((uint64_t)RoDataStartLD, PAGE_SIZE);
    uint64_t rodata_end = ALIGN_UP((uint64_t)RoDataEndLD, PAGE_SIZE);
    uint64_t data_start = ALIGN_DOWN((uint64_t)DataStartLD, PAGE_SIZE);
    uint64_t data_end = ALIGN_UP((uint64_t)DataEndLD, PAGE_SIZE);

    for (uint64_t text = text_start; text < text_end; text += PAGE_SIZE)
        vmm_map(Vmm_KernelPML4, text, text - virt_base + phys_base, PTE_PRESENT | PTE_USER);
    for (uint64_t rodata = rodata_start; rodata < rodata_end; rodata += PAGE_SIZE)
        vmm_map(Vmm_KernelPML4, rodata, rodata - virt_base + phys_base, PTE_PRESENT | PTE_NX | PTE_USER);
    for (uint64_t data = data_start; data < data_end; data += PAGE_SIZE)
        vmm_map(Vmm_KernelPML4, data, data - virt_base + phys_base, PTE_PRESENT | PTE_WRITABLE | PTE_NX | PTE_USER);
    for (uint64_t gb4 = 0; gb4 < 0x100000000; gb4 += PAGE_SIZE)
    {
        vmm_map(Vmm_KernelPML4, gb4, gb4, PTE_PRESENT | PTE_WRITABLE | PTE_USER);
        vmm_map(Vmm_KernelPML4, (uint64_t)Vmm_VirtToPhys(gb4), gb4, PTE_PRESENT | PTE_WRITABLE | PTE_USER);
    }

    Log(LT_INFO, "Vmm", "Switching to the new PML4...\n");
    vmm_switch_pml4(Vmm_KernelPML4);
}

uint64_t *vmm_new_pml4()
{
    uint64_t *pml4 = (uint64_t *)Vmm_VirtToPhys(Pmm_AllocatePage());
    memset(pml4, 0, PAGE_SIZE);
    for (size_t i = 256; i < 512; i++)
        pml4[i] = Vmm_KernelPML4[i];
        
    return pml4;
}

void vmm_switch_pml4(uint64_t *pml4)
{
    __asm__ volatile("mov %0, %%cr3" ::"r"((uint64_t)Vmm_PhysToVirt(pml4)) : "memory");
}

void vmm_map(uint64_t *pml4, uint64_t vaddr, uint64_t paddr, uint64_t flags)
{
    uint64_t pml1_entry = (vaddr >> 12) & 0x1ff;
    uint64_t pml2_entry = (vaddr >> 21) & 0x1ff;
    uint64_t pml3_entry = (vaddr >> 30) & 0x1ff;
    uint64_t pml4_entry = (vaddr >> 39) & 0x1ff;

    uint64_t *pml3 = NULL;
    if (pml4[pml4_entry] & PTE_PRESENT)
    {
        pml3 = Vmm_VirtToPhys(PTE_GET_ADDR(pml4[pml4_entry]));
    }
    else
    {
        pml3 = (uint64_t *)Vmm_VirtToPhys(Pmm_AllocatePage());
        memset(pml3, 0, PAGE_SIZE);
        pml4[pml4_entry] = (uint64_t)Vmm_PhysToVirt(pml3) | PTE_PRESENT | PTE_WRITABLE;
    }

    uint64_t *pml2 = NULL;
    if (pml3[pml3_entry] & PTE_PRESENT)
    {
        pml2 = Vmm_VirtToPhys(PTE_GET_ADDR(pml3[pml3_entry]));
    }
    else
    {
        pml2 = (uint64_t *)Vmm_VirtToPhys(Pmm_AllocatePage());
        memset(pml2, 0, PAGE_SIZE);
        pml3[pml3_entry] = (uint64_t)Vmm_PhysToVirt(pml2) | PTE_PRESENT | PTE_WRITABLE;
    }

    uint64_t *pml1 = NULL;
    if (pml2[pml2_entry] & PTE_PRESENT)
    {
        pml1 = Vmm_VirtToPhys(PTE_GET_ADDR(pml2[pml2_entry]));
    }
    else
    {
        pml1 = (uint64_t *)Vmm_VirtToPhys(Pmm_AllocatePage());
        memset(pml1, 0, PAGE_SIZE);
        pml2[pml2_entry] = (uint64_t)Vmm_PhysToVirt(pml1) | PTE_PRESENT | PTE_WRITABLE;
    }

    pml1[pml1_entry] = paddr | flags;
}

void vmm_unmap(uint64_t *pml4, uint64_t vaddr)
{
    uint64_t pml1_entry = (vaddr >> 12) & 0x1ff;
    uint64_t pml2_entry = (vaddr >> 21) & 0x1ff;
    uint64_t pml3_entry = (vaddr >> 30) & 0x1ff;
    uint64_t pml4_entry = (vaddr >> 39) & 0x1ff;

    uint64_t *pml3 = Vmm_VirtToPhys(PTE_GET_ADDR(pml4[pml4_entry]));
    if (pml3 == NULL)
        return;

    uint64_t *pml2 = Vmm_VirtToPhys(PTE_GET_ADDR(pml3[pml3_entry]));
    if (pml2 == NULL)
        return;

    uint64_t *pml1 = Vmm_VirtToPhys(PTE_GET_ADDR(pml2[pml2_entry]));
    if (pml1 == NULL)
        return;

    pml1[pml1_entry] = 0;
    __asm__ volatile("invlpg (%0)" ::"b"(vaddr) : "memory");
}

void print_kernel_size()
{
    uint64_t text_size = ALIGN_UP((uint64_t)TextEndLD, PAGE_SIZE) - ALIGN_DOWN((uint64_t)TextStartLD, PAGE_SIZE);
    uint64_t rodata_size = ALIGN_UP((uint64_t)RoDataEndLD, PAGE_SIZE) - ALIGN_DOWN((uint64_t)RoDataStartLD, PAGE_SIZE);
    uint64_t data_size = ALIGN_UP((uint64_t)DataEndLD, PAGE_SIZE) - ALIGN_DOWN((uint64_t)DataStartLD, PAGE_SIZE);

    uint64_t kernel_size = text_size + rodata_size + data_size;
    uint64_t kernel_size_kb = kernel_size / 1024;

    Log(LT_INFO, "Vmm", "Kernel size: %llu KB\n", kernel_size_kb);
}