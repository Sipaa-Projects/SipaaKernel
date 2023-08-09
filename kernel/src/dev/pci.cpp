#include "pci.h"

using namespace Sk::Arch;

namespace Sk {
namespace Dev {

uint32_t Pci::ReadConfig(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset)
{
    uint32_t address = (1 << 31) | (bus << 16) | (slot << 11) | (func << 8) | (offset & 0xFC);
    
    // Write address to CONFIG_ADDRESS register
    Io::OutL(PCI_CONFIG_ADDRESS, address);
    
    // Read data from CONFIG_DATA register
    return Io::InL(PCI_CONFIG_DATA);
}

void Pci::GetDevice(uint16_t vendor_id, uint16_t device_id, struct PCIDevice *device_info)
{
    int st = 0;

    for (uint8_t bus = 0; bus < 256; ++bus) {
        for (uint8_t slot = 0; slot < 32; ++slot) {
            for (uint8_t func = 0; func < 8; ++func) {
                uint32_t header_type = (ReadConfig(bus, slot, func, 0x0C) >> 16) & 0xFF;
                
                if (header_type == 0xFF) {
                    // No device present, skip
                    st++;
                    if (st >= 100)
                    {
                        break;
                    }
                    continue;
                }
                
                uint16_t current_vendor_id = ReadConfig(bus, slot, func, 0x00) & 0xFFFF;
                uint16_t current_device_id = (ReadConfig(bus, slot, func, 0x00) >> 16) & 0xFFFF;
                
                if (current_vendor_id == vendor_id && current_device_id == device_id) {
                    device_info->vendor_id = current_vendor_id;
                    device_info->device_id = current_device_id;
                    device_info->class_code = (ReadConfig(bus, slot, func, 0x08) >> 24) & 0xFF;
                    device_info->subclass = (ReadConfig(bus, slot, func, 0x08) >> 16) & 0xFF;
                    device_info->prog_if = (ReadConfig(bus, slot, func, 0x08) >> 8) & 0xFF;
                    return;
                }
            }

            if (st >= 100)
            {
                break;
            }
        }

        if (st >= 100)
        {
            break;
        }
    }
    
    // If device is not found, populate the struct with default values
    device_info->vendor_id = 0xFFFF;
    device_info->device_id = 0xFFFF;
    device_info->class_code = 0xFF;
    device_info->subclass = 0xFF;
    device_info->prog_if = 0xFF;
}

}
}