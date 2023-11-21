#include "hal.h"
#include "io.h"

#define PCI_CONFIG_ADDRESS 0xCF8
#define PCI_CONFIG_DATA 0xCFC

uint32_t pci_read(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset)
{
    uint32_t address;
    uint32_t lbus = (uint32_t)bus;
    uint32_t lslot = (uint32_t)slot;
    uint32_t lfunc = (uint32_t)func;

    address = (uint32_t)((lbus << 16) | (lslot << 11) |
                        (lfunc << 8) | (offset & 0xfc) | ((uint32_t)0x80000000));

    outl(PCI_CONFIG_ADDRESS, address);
    return inl(PCI_CONFIG_DATA);
}
