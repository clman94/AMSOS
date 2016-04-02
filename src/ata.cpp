

#include <amsos/drivers/ata.h>
#include <amsos/ports.h>

inline void ata_wait()
{
	while (!(port_in_b(0x1F7) & 0x08)) {}
}

void ata_do_read(uint8_t* block)
{
	for(int i = 0; i < 256; i++)
	{
		((uint16_t*)(block))[i] = port_in_w(0x1F0);;
	}
}

void ata_do_write(uint8_t* block)
{
	for(int i = 0; i < 256; i++)
	{
		port_out_w(0x1f0, ((uint16_t*)(block))[i]);
	}
}

inline void ata_lba24_set_options(uint32_t addr, uint8_t drive)
{
	port_out_b(0x1F1, 0x00);
	port_out_b(0x1F2, 0x01);
	port_out_b(0x1F3, (uint8_t)addr);
	port_out_b(0x1F4, (uint8_t)(addr >> 8));
	port_out_b(0x1F5, (uint8_t)(addr >> 16));
	port_out_b(0x1F6, 0xE0 | (drive << 4) | ((addr >> 24) & 0x0F));
}

inline void ata_lba48_set_options(uint64_t addr, uint8_t drive)
{
	port_out_b(0x1F1, 0x00);
	port_out_b(0x1F2, 0x01);
	port_out_b(0x1F3, (uint8_t)(addr >> 24));
	port_out_b(0x1F3, (uint8_t)addr);
	port_out_b(0x1F4, (uint8_t)(addr >> 32));
	port_out_b(0x1F4, (uint8_t)(addr >> 8));
	port_out_b(0x1F5, (uint8_t)(addr >> 40));
	port_out_b(0x1F5, (uint8_t)(addr >> 16));
	port_out_b(0x1F6, 0x40 | (drive << 4));
}

void ata_lba24_read_block(uint32_t addr, uint8_t* block, uint8_t drive)
{
	ata_lba24_set_options(addr, drive);

	// Set to read
	port_out_b(0x1F7, 0x20);
	
	// Wait for the hard drive to be ready
	ata_wait();
	
	// Get data, 2 bytes at a time
	ata_do_read(block);
}

void ata_lba48_read_block(uint64_t addr, uint8_t* block, uint8_t drive)
{
	ata_lba48_set_options(addr, drive);

	// Set to read
	port_out_b(0x1F7, 0x24);
	
	// Wait for the hard drive to be ready
	ata_wait();
	
	// Get data, 2 bytes at a time
	ata_do_read(block);
}

void ata_lba24_write_block(uint32_t addr, uint8_t* block, uint8_t drive)
{
	ata_lba24_set_options(addr, drive);
	
	// Set to write
	port_out_b(0x1F7, 0x30);
	
	// Wait for the hard drive to be ready
	ata_wait();
	
	// Set data, 2 bytes at a time
	ata_do_write(block);
}

void ata_lba48_write_block(uint64_t addr, uint8_t* block, uint8_t drive)
{
	ata_lba48_set_options(addr, drive);
	
	// Set to write
	port_out_b(0x1F7, 0x34);
	
	// Wait for the hard drive to be ready
	ata_wait();
	
	// Set data, 2 bytes at a time
	ata_do_write(block);
}
