#ifndef VMM_H
#define VMM_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <sipaa/skgbi.h>

// Define a structure to represent a page table entry
typedef struct
{
    uint64_t present : 1;
    uint64_t read_write : 1;
    uint64_t user_supervisor : 1;
    uint64_t write_through : 1;
    uint64_t cache_disabled : 1;
    uint64_t accessed : 1;
    uint64_t dirty : 1;
    uint64_t reserved : 1;
    uint64_t available : 3;
    uint64_t frame : 40; // Physical address of the frame
    uint64_t reserved2 : 12;
} page_table_entry;

// Define a structure to represent a page table
typedef struct
{
    page_table_entry entries[512];
} page_table;

// Define a structure to represent a page directory
typedef struct
{
    page_table *tables[512];
    uint64_t physical_addresses[512];
} page_directory;

// Functions to allocate and initialize page tables and directories
page_table *create_page_table();
page_directory *create_page_directory();

// Function to map a virtual address to a physical address
void map_page(page_directory *dir, uintptr_t virtual_addr, uintptr_t physical_addr, bool user, bool writable);

// Functions to enable and disable paging
void enable_paging(page_directory *dir);
void disable_paging();
#endif