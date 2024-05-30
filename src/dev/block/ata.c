#include <sipaa/dev/block/ata.h>
#include <sipaa/x86_64/idt.h>

uint16_t Ata_Base = 0;
uint8_t Ata_Type = 0;
char Ata_Name[40];

void Ata_Wait() {
  for (uint8_t i = 0; i < 4; i++)
    inb(Ata_Base + 7); // 400ns delay
}

uint8_t Ata_Poll() {
  Ata_Wait();
  
  uint8_t status = 0;
  for (;;) {
    status = inb(Ata_Base + 7);
    if (!(status & 0x80)) break;
    if (status & 0x08) break;
    else if (status & 0x01) return ATA_DISK_ERR;
  }

  return ATA_OKAY;
}

uint8_t Ata_Identify(uint16_t ata, uint8_t type) {
  Ata_Base = ata;
  Ata_Type = type;

  // Select disk
  outb(ata + 6, type); // master or slave
  for (uint16_t i = 0x1F2; i != 0x1F5; i++)
    outb(i, 0);
  outb(ata + 7, Ata_Identify);
  
  uint8_t status = inb(ata + 7);
  if (status == 0) return ATA_DISK_NOT_IDENTIFIED;

  if (Ata_Poll() != ATA_OKAY)
    return ATA_DISK_ERR;

  uint8_t buf[512];
  Ata_Read(0, buf, 1);

  for (uint8_t i = 0; i < 40; i += 2) {
    Ata_Name[i] = buf[54 + i + 1];
    Ata_Name[i + 1] = buf[54 + i];
  }
  
  Ata_Wait();
  
  return ATA_OKAY;
}

uint8_t Ata_Read(uint32_t lba, uint8_t* buffer, uint32_t sector_count) {
  outb(Ata_Base + 6, (Ata_Type == ATA_MASTER ? 0xE0 : 0xF0) | ((lba >> 24) & 0x0F)); // Set master/slave
  outb(Ata_Base + 1, ATA_WAIT); // Send wait
  outb(Ata_Base + 2, sector_count); // Sector count
  outb(Ata_Base + 3, (uint8_t)lba); // Start sending LBA
  outb(Ata_Base + 4, (uint8_t)(lba >> 8));
  outb(Ata_Base + 5, (uint8_t)(lba >> 16)); // 24-bit LBA addressing
  outb(Ata_Base + 7, ATA_READ);

  uint16_t val = 0;
  uint32_t i = 0;

  for (; i < sector_count * 512; i += 2) {
    if (Ata_Poll() != ATA_OKAY)
      return ATA_DISK_ERR;
    val = inw(Ata_Base);
    buffer[i] = val & 0x00ff;
    if (i + 1 < sector_count * 512)
      buffer[i + 1] = (val >> 8) & 0x00ff;
  }

  Ata_Wait();

  return ATA_OKAY;
}

uint8_t Ata_Write(uint32_t lba, uint8_t* buffer, uint32_t sector_count) {
  outb(Ata_Base + 6, (Ata_Type == ATA_MASTER ? 0xE0 : 0xF0) | ((lba >> 24) & 0x0F));
  outb(Ata_Base + 1, ATA_WAIT);
  outb(Ata_Base + 2, sector_count);
  outb(Ata_Base + 3, (uint8_t)lba);
  outb(Ata_Base + 4, (uint8_t)(lba >> 8));
  outb(Ata_Base + 5, (uint8_t)(lba >> 16));
  outb(Ata_Base + 7, ATA_WRITE);

  uint16_t val = 0;
  uint32_t i = 0;

  for (; i < sector_count * 512; i += 2) {
    if (Ata_Poll() != ATA_OKAY)
      return ATA_DISK_ERR;
    val = buffer[i];
    if (i + 1 < sector_count * 512)
      val |= ((uint16_t)buffer[i + 1] << 8);
    outw(Ata_Base, val);
  }
  
  Ata_Wait();
  
  return ATA_OKAY;
}

uint8_t Ata_Initialize() {
  uint8_t ata_status = Ata_Identify(ATA_PRIMARY, ATA_MASTER);
  return ata_status;
}