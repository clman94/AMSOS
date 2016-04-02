#ifndef DRIVERS_ATA_H
#define DRIVERS_ATA_H

#include <stdint.h>

void ata_lba24_write_block(uint32_t addr, uint8_t* block, uint8_t drive);
void ata_lba24_read_block(uint32_t addr, uint8_t* block, uint8_t drive);
void ata_lba48_write_block(uint64_t addr, uint8_t* block, uint8_t drive);
void ata_lba48_read_block(uint64_t addr, uint8_t* block, uint8_t drive);

#endif