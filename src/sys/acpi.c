// For now, this only sets 3 values...

#include <sipaa/acpi.h>
#include <sipaa/bootsrv.h>
#include <sipaa/libc/string.h>
#include <sipaa/x86_64/vmm.h>

bool Acpi_Here = false;
bool Acpi_UseXSDP = false;
void* Acpi_RSDPAddress;

void Acpi_Initialize()
{
    struct limine_rsdp_response *rsdpa = BootSrv_GetAcpiRSDP();

    AcpiRsdpT *rsdp = (AcpiRsdpT *)rsdpa->address;

    if (CompareStrings(rsdp->Signature, "RSD PTR "))
        return 0;

    Acpi_Here = true;

    if (rsdp->Revision != 0) {
        // Use XSDT
        Acpi_UseXSDP = true;
        AcpiXsdpT *xsdp = (AcpiXsdpT *)rsdpa->address;
        Acpi_RSDPAddress = PHYSICAL_TO_VIRTUAL((void*)xsdp->XsdtAddress);
    }

    Acpi_RSDPAddress = PHYSICAL_TO_VIRTUAL((void*)rsdp->RsdtAddress);
}