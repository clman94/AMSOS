#ifndef H_RAM
#define H_RAM

#include <stdint.h>

// In bytes
typedef uint32_t mem_addr;

// Use all memory after 16M
const unsigned int MEMORY_BEGIN = 0x1000000;

const unsigned int RAM_FS_BLOCK_LENGTH = 0xFF;

enum RAM_FILE_TYPE
{
	RAM_FILE_TYPE_NONE,
	RAM_FILE_TYPE_FREE,
	RAM_FILE_TYPE_DATA,
	RAM_FILE_TYPE_EXECUTE,
	RAM_FILE_TYPE_FILLER,
	RAM_FILE_TYPE_DIRECTORY,
	RAM_FILE_TYPE_LIST
};

const unsigned int RAM_FILE_NAME_LENGTH = 238;

struct ram_file_entry
{
	uint8_t  name_length;
	char  name[RAM_FILE_NAME_LENGTH];
	
	//union{
		
	// Region in which data is stored
	// (start=0, end=1)
	// Range is inclusive
	mem_addr range[2];
		
	//};
	uint8_t  type;
};

struct ram_directory
{	
	ram_directory* root;
	ram_directory* up;

	ram_file_entry block[RAM_FS_BLOCK_LENGTH];
};

#endif