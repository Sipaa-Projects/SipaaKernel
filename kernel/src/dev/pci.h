#pragma once

#include <stdint.h>

#include <arch/io.h>
#include <logging/logger.h>

#define PCI_CONFIG_ADDRESS  0xCF8
#define PCI_CONFIG_DATA     0xCFC

namespace Sk {
namespace Dev {

struct PCIDevice {
    uint16_t vendor_id;
    uint16_t device_id;
    uint8_t class_code;
    uint8_t subclass;
    uint8_t prog_if;
};

class Pci {
public:
    static uint32_t ReadConfig(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset);
    static void GetDevice(uint16_t vendor_id, uint16_t device_id, struct PCIDevice *devinfo);
};

}
}