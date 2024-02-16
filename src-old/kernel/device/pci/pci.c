// SKB_NO_AARCH64 / SKB_NO_RISCV64

#include "pci.h"
#include <arch/x86/io.h>
#include <logger/logger.h>

uint32_t pci_read(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset)
{
    uint32_t address = (1 << 31) | (bus << 16) | (slot << 11) | (func << 8) | (offset & 0xFC);
    
    // Write address to CONFIG_ADDRESS register
    outl(PCI_CONFIG_ADDRESS, address);
    
    // Read data from CONFIG_DATA register
    return inl(PCI_CONFIG_DATA);
}

void pci_getdev(struct pci_dev *pcid, uint16_t vendor, uint16_t dev)
{
        uint16_t bus;
        uint8_t slot;
        uint8_t func;

        for (bus = 0; bus < 256; bus++)
        {
                for (slot = 0; slot < 32; slot++)
                {
                        for (func = 0; func < 8; func++)
                        {
                                uint32_t res = pci_read(bus, slot, func, 0);
                                uint16_t vendorID = res & 0xFFFF;
                                uint16_t deviceID = res >> 16;
                                if (vendorID == vendor && deviceID == dev)
                                {
                                        pcid->bus = bus;
                                        pcid->dev = deviceID;
                                        pcid->vendor = vendorID;
                                        pcid->func = func;
                                        pcid->slot = slot;
                                        return pcid; 
                                }
                        }
                }
        }
        pcid->bus = bus;
        pcid->dev = 0xFFFFF;
        pcid->vendor = 0xFFFFF;
        pcid->func = func;
        pcid->slot = slot;
        return pcid; 
}

void pci_enum()
{
        uint16_t bus;
        uint8_t slot;
        uint8_t func;

        for (bus = 0; bus < 256; bus++)
        {
                for (slot = 0; slot < 32; slot++)
                {
                        for (func = 0; func < 8; func++)
                        {
                                uint32_t res = pci_read(bus, slot, func, 0);
                                uint16_t vendorID = res & 0xFFFF;
                                uint16_t deviceID = res >> 16;
                                if (vendorID == 0xFFFF)
                                        continue;

                                char buffer[16];

                                log(LT_INFO, "pci", "PCI Device Found:\n");
                                log(LT_INFO, "pci", "  Bus: %u\n", bus);
                                log(LT_INFO, "pci", "  Slot: %u\n", slot);
                                log(LT_INFO, "pci", "  Function: %u\n", func);
                                log(LT_INFO, "pci", "  Vendor ID: %u\n", vendorID);
                                log(LT_INFO, "pci", "  Device ID: %u\n", deviceID);
                        }
                }
        }
}