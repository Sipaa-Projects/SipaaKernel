#pragma once

#include <stdint.h>

#define PCI_CONFIG_ADDRESS 0xCF8
#define PCI_CONFIG_DATA 0xCFC

typedef struct pci_dev
{
    uint16_t bus;
    uint8_t slot;
    uint8_t func;
    uint16_t vendor;
    uint16_t dev;
};

void pci_getdev(struct pci_dev *pcid, uint16_t vendor, uint16_t dev);
void pci_enum();