/*
  WARNING: the VMM implementation below is very very very very very very very very very very very 
  very very very very very very very very very very very very very very very very very very very 
  very very very very very very very very very very very very very very very very very very very very 
  very very very very very very very very very very very very very very very very very very very 
  very very very very very very very very very very very very very very very very very very v
  very very very very very very very very very very very very very very very very very very very very very very very very very very very very 
  very very very very very very very very very very very very very very very very very very very very very 
  very very very very very very very beta
*/

#include <sipaa/i686/vmm.h>
#include <sipaa/pmm.h>

page_table *create_page_table()
{
    // Allocate a page for the page table
    page_table *table = (page_table *)pmm_alloc(PAGE_SIZE);

    // Clear the page table
    for (int i = 0; i < PAGE_TABLE_ENTRIES; ++i)
    {
        table->entries[i].frame = 0;
        // Set other flags as needed
    }

    return table;
}

page_directory *create_page_directory()
{
    // Allocate a page for the page directory
    page_directory *dir = (page_directory *)pmm_alloc(PAGE_SIZE);

    // Clear the page directory
    for (int i = 0; i < PAGE_TABLE_ENTRIES; ++i)
    {
        dir->tables[i] = NULL;
        dir->physical_addresses[i] = 0;
    }

    return dir;
}

void map_page(page_directory *dir, uintptr_t virtual_addr, uintptr_t physical_addr, bool user, bool writable)
{
    
    // Calculate indices for page directory and page table
    uint64_t pd_index = PAGE_DIRECTORY_INDEX(virtual_addr);
    uint64_t pt_index = PAGE_TABLE_INDEX(virtual_addr);

    // Check if the page table exists, create it if not
    if (dir->tables[pd_index] == NULL)
    {
        dir->tables[pd_index] = create_page_table();
        dir->physical_addresses[pd_index] = (uint64_t)dir->tables[pd_index] | PAGE_PRESENT | PAGE_RW | (user ? PAGE_USER : 0);
    }

    // Set the page table entry for the virtual address
    dir->tables[pd_index]->entries[pt_index].frame = physical_addr >> 12;
    dir->tables[pd_index]->entries[pt_index].present = 1;
    dir->tables[pd_index]->entries[pt_index].user_supervisor = user ? 1 : 0;
    dir->tables[pd_index]->entries[pt_index].read_write = writable ? 1 : 0;

    // Invalidate TLB
    asm volatile ("invlpg (%0)" ::"r"(virtual_addr) : "memory");
}

void enable_paging(page_directory *dir)
{
    // Load the page directory base address into CR3
    asm volatile ("mov %0, %%cr3" :: "r"((uint64_t)dir->physical_addresses));

    // Enable paging by setting the paging bit (bit 31) in CR0
    asm volatile ("mov %%cr0, %%eax; or $0x80000000, %%eax; mov %%eax, %%cr0" ::: "%eax");
}

void disable_paging()
{
    // Disable paging by clearing the paging bit (bit 31) in CR0
    asm volatile ("mov %%cr0, %%eax; and $0x7FFFFFFF, %%eax; mov %%eax, %%cr0" ::: "%eax");
}