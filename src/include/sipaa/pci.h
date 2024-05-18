/// @brief Credit to K. Lange for the ToaruOS PCI.

#pragma once

#include <stdint.h>

#define PCI_VENDOR_ID            0x00 // 2
#define PCI_DEVICE_ID            0x02 // 2
#define PCI_COMMAND              0x04 // 2
#define PCI_STATUS               0x06 // 2
#define PCI_REVISION_ID          0x08 // 1

#define PCI_PROG_IF              0x09 // 1
#define PCI_SUBCLASS             0x0a // 1
#define PCI_CLASS                0x0b // 1
#define PCI_CACHE_LINE_SIZE      0x0c // 1
#define PCI_LATENCY_TIMER        0x0d // 1
#define PCI_HEADER_TYPE          0x0e // 1
#define PCI_BIST                 0x0f // 1
#define PCI_BAR0                 0x10 // 4
#define PCI_BAR1                 0x14 // 4
#define PCI_BAR2                 0x18 // 4
#define PCI_BAR3                 0x1C // 4
#define PCI_BAR4                 0x20 // 4
#define PCI_BAR5                 0x24 // 4

#define PCI_INTERRUPT_LINE       0x3C // 1
#define PCI_INTERRUPT_PIN        0x3D

#define PCI_SECONDARY_BUS        0x19 // 1

#define PCI_HEADER_TYPE_DEVICE  0
#define PCI_HEADER_TYPE_BRIDGE  1
#define PCI_HEADER_TYPE_CARDBUS 2

#define PCI_TYPE_BRIDGE 0x0604
#define PCI_TYPE_SATA   0x0106

#define PCI_ADDRESS_PORT 0xCF8
#define PCI_VALUE_PORT   0xCFC

#define PCI_NONE 0xFFFF

typedef void (*pci_func_t)(uint32_t device, uint16_t vendor_id, uint16_t device_id, void * extra);

static inline int Pci_ExtractBus(uint32_t device) {
	return (uint8_t)((device >> 16));
}
static inline int Pci_ExtractSlot(uint32_t device) {
	return (uint8_t)((device >> 8));
}
static inline int Pci_ExtractFunc(uint32_t device) {
	return (uint8_t)(device);
}

static inline uint32_t Pci_GetAddress(uint32_t device, int field) {
	return 0x80000000 | (Pci_ExtractBus(device) << 16) | (Pci_ExtractSlot(device) << 11) | (Pci_ExtractFunc(device) << 8) | ((field) & 0xFC);
}

static inline uint32_t Pci_BoxDevice(int bus, int slot, int func) {
	return (uint32_t)((bus << 16) | (slot << 8) | func);
}

uint32_t Pci_ReadField(uint32_t device, int field, int size);
void Pci_WriteField(uint32_t device, int field, int size, uint32_t value);
uint16_t Pci_FindType(uint32_t dev);
void Pci_ScanFunc(pci_func_t f, int type, int bus, int slot, int func, void * extra);
void Pci_ScanSlot(pci_func_t f, int type, int bus, int slot, void * extra);
void Pci_ScanBus(pci_func_t f, int type, int bus, void * extra);
void Pci_Scan(pci_func_t f, int type, void * extra);
void Pci_Remap(void);
int Pci_GetInterrupt(uint32_t device);
int Pci_Exists(uint32_t vendorId, uint32_t deviceId);