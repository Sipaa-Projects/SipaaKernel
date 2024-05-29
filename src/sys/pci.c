/**
 * @file  src/sys/pci.c
 * @brief PCI configuration and scanning. (taken from ToaruOS)
 *
 * Functions for dealing with PCI devices through configuration mode #1
 * (CPU port I/O methods), including scanning and modifying device
 * configuration bytes.
 *
 * This used to have methods for dealing with ISA bridge IRQ remapping,
 * but it has been removed for the moment.
 *
 * TODO: Implement MSI configuration?
 * TODO: SKDevs: PCI should be available on AArch64, but this implementation is only for x86_64...
 *
 * @copyright Copyright (C) 2011-2021 K. Lange
 */

#ifdef __x86_64__

#include <stdint.h>
#include <sipaa/libc/string.h>
#include <sipaa/pci.h>
#include <sipaa/logger.h>
#include <sipaa/pmm.h>
#include <sipaa/x86_64/io.h>

static uintptr_t pcie_addr(uint32_t device, int field) {
	return (Pci_ExtractBus(device) << 20) | (Pci_ExtractSlot(device) << 15) | (Pci_ExtractFunc(device) << 12) | (field);
}

uintptr_t pcie_ecam_phys = 0x3f000000;

/**
 * @brief Write to a PCI device configuration space field.
 */
void Pci_WriteField(uint32_t device, int field, int size, uint32_t value) {
	outl(PCI_ADDRESS_PORT, Pci_GetAddress(device, field));
	outl(PCI_VALUE_PORT, value);
}

/**
 * @brief Read from a PCI device configuration space field.
 */
uint32_t Pci_ReadField(uint32_t device, int field, int size) {
	outl(PCI_ADDRESS_PORT, Pci_GetAddress(device, field));

	if (size == 4) {
		uint32_t t = inl(PCI_VALUE_PORT);
		return t;
	} else if (size == 2) {
		uint16_t t = ins(PCI_VALUE_PORT + (field & 2));
		return t;
	} else if (size == 1) {
		uint8_t t = inb(PCI_VALUE_PORT + (field & 3));
		return t;
	}
	return 0xFFFF;
}

/**
 * @brief Obtain the device type from the class and subclass fields.
 */
uint16_t Pci_FindType(uint32_t dev) {
	return (Pci_ReadField(dev, PCI_CLASS, 1) << 8) | Pci_ReadField(dev, PCI_SUBCLASS, 1);
}

static void Pci_ScanHit(pci_func_t f, uint32_t dev, void * extra) {
	int dev_vend = (int)Pci_ReadField(dev, PCI_VENDOR_ID, 2);
	int dev_dvid = (int)Pci_ReadField(dev, PCI_DEVICE_ID, 2);

	f(dev, dev_vend, dev_dvid, extra);
}

void Pci_ScanFunc(pci_func_t f, int type, int bus, int slot, int func, void * extra) {
	uint32_t dev = Pci_BoxDevice(bus, slot, func);
	if (type == -1 || type == Pci_FindType(dev)) {
		Pci_ScanHit(f, dev, extra);
	}
	if (Pci_FindType(dev) == PCI_TYPE_BRIDGE) {
		Pci_ScanBus(f, type, Pci_ReadField(dev, PCI_SECONDARY_BUS, 1), extra);
	}
}

void Pci_ScanSlot(pci_func_t f, int type, int bus, int slot, void * extra) {
	uint32_t dev = Pci_BoxDevice(bus, slot, 0);
	if (Pci_ReadField(dev, PCI_VENDOR_ID, 2) == PCI_NONE) {
		return;
	}
	Pci_ScanFunc(f, type, bus, slot, 0, extra);
	if (!Pci_ReadField(dev, PCI_HEADER_TYPE, 1)) {
		return;
	}
	for (int func = 1; func < 8; func++) {
		uint32_t dev = Pci_BoxDevice(bus, slot, func);
		if (Pci_ReadField(dev, PCI_VENDOR_ID, 2) != PCI_NONE) {
			Pci_ScanFunc(f, type, bus, slot, func, extra);
		}
	}
}

void Pci_ScanBus(pci_func_t f, int type, int bus, void * extra) {
	for (int slot = 0; slot < 32; ++slot) {
		Pci_ScanSlot(f, type, bus, slot, extra);
	}
}

/**
 * @brief Scan PCI buses for devices, calling the given function for each device.
 *
 * Used by drivers to implement device discovery, runs a callback function for ever
 * device found. A device consists of a bus, slot, and function. Also performs
 * recursive scans of bridges.
 */
void Pci_Scan(pci_func_t f, int type, void * extra) {
	if ((Pci_ReadField(0, PCI_HEADER_TYPE, 1) & 0x80) == 0) {
		Pci_ScanBus(f,type,0,extra);
		return;
	}

	int hit = 0;
	for (int func = 0; func < 8; ++func) {
		uint32_t dev = Pci_BoxDevice(0, 0, func);
		if (Pci_ReadField(dev, PCI_VENDOR_ID, 2) != PCI_NONE) {
			hit = 1;
			Pci_ScanBus(f, type, func, extra);
		} else {
			break;
		}
	}

	if (!hit) {
		for (int bus = 0; bus < 256; ++bus) {
			for (int slot = 0; slot < 32; ++slot) {
				Pci_ScanSlot(f,type,bus,slot,extra);
			}
		}
	}
}

int Pci_GetInterrupt(uint32_t device) {
	return Pci_ReadField(device, PCI_INTERRUPT_LINE, 1);
}

// Check if a PCI device exists
uint32_t Pci_ProbeCache_Vid = 0;
uint32_t Pci_ProbeCache_Did = 0;

void Pci_Exists_Callback(uint32_t device, uint16_t vendor_id, uint16_t device_id, void * extra)
{
	if (vendor_id = Pci_ProbeCache_Vid && device_id == Pci_ProbeCache_Did)
	{
		int *extraint = (int *)extra;
		*extraint = 1;
	}
}

int Pci_Exists(uint32_t vendorId, uint32_t deviceId)
{
	int *found = (int *)Pmm_Allocate(1);
	*found = 0;

	Pci_ProbeCache_Did = deviceId;
	Pci_ProbeCache_Vid = vendorId;

	Pci_Scan(&Pci_Exists_Callback, -1, found);

	return *found;
}

#endif