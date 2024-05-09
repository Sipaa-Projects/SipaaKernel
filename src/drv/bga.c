/// @brief SipaaKernel's BGA driver.
#include <sipaa/bga.h>
#include <sipaa/i686/io.h>

void BochsGA_WriteReg(unsigned short IndexValue, unsigned short DataValue)
{
    outw(VBE_DISPI_IOPORT_INDEX, IndexValue);
    outw(VBE_DISPI_IOPORT_DATA, DataValue);
}
 
unsigned short BochsGA_ReadReg(unsigned short IndexValue)
{
    outw(VBE_DISPI_IOPORT_INDEX, IndexValue);
    return inw(VBE_DISPI_IOPORT_DATA);
}
 
bool BochsGA_IsAvailable(unsigned short version)
{
    return (BochsGA_ReadReg(VBE_DISPI_INDEX_ID) >= version);
}
 
void BochsGA_SetVideoMode(unsigned int Width, unsigned int Height, unsigned int BitDepth, int UseLinearFrameBuffer, int ClearVideoMemory)
{
    BochsGA_WriteReg(VBE_DISPI_INDEX_ENABLE, VBE_DISPI_DISABLED);
    BochsGA_WriteReg(VBE_DISPI_INDEX_XRES, Width);
    BochsGA_WriteReg(VBE_DISPI_INDEX_YRES, Height);
    BochsGA_WriteReg(VBE_DISPI_INDEX_BPP, BitDepth);
    BochsGA_WriteReg(VBE_DISPI_INDEX_ENABLE, VBE_DISPI_ENABLED |
        (UseLinearFrameBuffer ? VBE_DISPI_LFB_ENABLED : 0) |
        (ClearVideoMemory ? 0 : VBE_DISPI_NOCLEARMEM));
}
 
void BochsGA_SetBank(unsigned short BankNumber)
{
    BochsGA_WriteReg(VBE_DISPI_INDEX_BANK, BankNumber);
}