#ifndef H_RAMFS
#define H_RAMFS

#include <stdint.h>


/* ## The Idea ##
The main purpose of this system is to select large/sections
of memory and represent them as files. For example, the 
video ram can be stored as "/system/video/25X80".
This is useful in preventing conflicts/overlapping of
section of memory different components the OS use.

FYI, the sections for dynamic memory itself is contained in
a separate file(s) as well. In dir "/system/dm/"

Possible layout:
/system
  /video
    /80X25
  /dm                   -Dynamic Memory
    /[hex]

*/

// In bytes
typedef uint32_t mem_addr;

// Use all memory after 16M
const mem_addr MEMORY_BEGIN = 0x1000000;

const mem_addr RAM_FS_BLOCK_LENGTH = 0xFF;

enum RAM_FILE_TYPE
{
	RAM_FILE_TYPE_NONE,
	RAM_FILE_TYPE_FREE,
	RAM_FILE_TYPE_FILE,
	RAM_FILE_TYPE_DIRECTORY
};

const unsigned int RAM_FILE_NAME_LENGTH = 239;

struct ram_dir;
struct ram_entry;

struct ram_entry
{
	ram_dir* dir;
	
	char  name[RAM_FILE_NAME_LENGTH];
	
	//union{
		
	// Region in which data is stored
	// (start=0, end=1)
	// Range is inclusive
	mem_addr range[2];
		
	//};
	uint8_t  type;
};

struct ram_dir
{
	ram_dir* root;
	ram_dir* up;
	ram_entry* ref; // entry referencing this dir
	
	ram_entry block[RAM_FS_BLOCK_LENGTH];
};

ram_dir*    clear_dir_block(ram_dir* dir);
ram_dir*    create_root_dir(mem_addr address, mem_addr start, mem_addr end);
ram_dir*    create_dir(ram_dir* dir, const char* name);
ram_dir*    find_dir(ram_dir* dir, const char* name);

mem_addr    get_file_size(ram_entry* file);
ram_entry*  create_file(ram_dir* dir, char* name, mem_addr size, int type);
ram_entry* make_file(ram_dir* dir, const char* name, mem_addr start, mem_addr end, int type, bool force = false);
ram_entry*  find_file(ram_dir* dir, const char* name);
ram_entry*  get_first_file(ram_dir* dir);
ram_entry*  get_next_file(ram_dir* dir, ram_entry* iter);
ram_entry*  get_entry_path(ram_dir* dir, const char* path);

#endif