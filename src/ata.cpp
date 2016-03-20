#include <include/ports.h>

#include <stdint.h>

inline void ata_wait(){
	while (!(port_in_b(0x1F7) & 0x08)) {}
}

void ata_do_read(unsigned char* block){
	for(int i = 0; i < 256; i++){
		unsigned short data = port_in_w(0x1F0);
		block[i*2] = (unsigned char)data;
		block[i*2 + 1] = (unsigned char)(data >> 8);
	}
}

void ata_do_write(unsigned char* block){
	for(int i = 0; i < 256; i++){
		unsigned short data = block[i*2] | (block[i*2 + 1] << 8);
		port_out_w(0x1f0, data);
	}
}

inline void ata_lba24_set_options(unsigned int addr,unsigned char drive){
	port_out_b(0x1F1, 0x00);
	port_out_b(0x1F2, 0x01);
	port_out_b(0x1F3, (unsigned char)addr);
	port_out_b(0x1F4, (unsigned char)(addr >> 8));
	port_out_b(0x1F5, (unsigned char)(addr >> 16));
	port_out_b(0x1F6, 0xE0 | (drive << 4) | ((addr >> 24) & 0x0F));
}

inline void ata_lba48_set_options(unsigned long long int addr,unsigned char drive){
	port_out_b(0x1F1, 0x00);
	port_out_b(0x1F2, 0x01);
	port_out_b(0x1F3, (unsigned char)(addr >> 24));
	port_out_b(0x1F3, (unsigned char)addr);
	port_out_b(0x1F4, (unsigned char)(addr >> 32));
	port_out_b(0x1F4, (unsigned char)(addr >> 8));
	port_out_b(0x1F5, (unsigned char)(addr >> 40));
	port_out_b(0x1F5, (unsigned char)(addr >> 16));
	port_out_b(0x1F6, 0x40 | (drive << 4));
}

extern
void ata_lba24_read_block(unsigned int addr, unsigned char* block, unsigned char drive){
	
	ata_lba24_set_options(addr, drive);

	// Set to read
	port_out_b(0x1F7, 0x20);
	
	// Wait for the hard drive to be ready
	ata_wait();
	
	// Get data, 2 bytes at a time
	ata_do_read(block);
}

extern
void ata_lba48_read_block(unsigned long long int addr, unsigned char* block, unsigned char drive){
	
	ata_lba48_set_options(addr, drive);

	// Set to read
	port_out_b(0x1F7, 0x24);
	
	// Wait for the hard drive to be ready
	ata_wait();
	
	// Get data, 2 bytes at a time
	ata_do_read(block);
}

extern
void ata_lba24_write_block(unsigned int addr, unsigned char* block, unsigned char drive){
	
	ata_lba24_set_options(addr, drive);
	
	// Set to write
	port_out_b(0x1F7, 0x30);
	
	// Wait for the hard drive to be ready
	ata_wait();
	
	// Set data, 2 bytes at a time
	ata_do_write(block);
}

extern
void ata_lba48_write_block(unsigned long long int addr, unsigned char* block, unsigned char drive){
	
	ata_lba48_set_options(addr, drive);
	
	// Set to write
	port_out_b(0x1F7, 0x34);
	
	// Wait for the hard drive to be ready
	ata_wait();
	
	// Set data, 2 bytes at a time
	ata_do_write(block);
}
