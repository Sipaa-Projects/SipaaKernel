#pragma once

#include <stdbool.h>
#include <stdint.h>

extern bool Acpi_UseXSDP;

struct AcpiXsdp {
 char Signature[8];
 uint8_t Checksum;
 char OEMID[6];
 uint8_t Revision;
 uint32_t RsdtAddress;      // deprecated since version 2.0
 
 uint32_t Length;
 uint64_t XsdtAddress;
 uint8_t ExtendedChecksum;
 uint8_t reserved[3];
} __attribute__ ((packed));

typedef struct AcpiXsdp AcpiXsdpT;

struct AcpiRsdp {
 char Signature[8];
 uint8_t Checksum;
 char OEMID[6];
 uint8_t Revision;
 uint32_t RsdtAddress;      // deprecated since version 2.0
} __attribute__ ((packed));

typedef struct AcpiRsdp AcpiRsdpT;