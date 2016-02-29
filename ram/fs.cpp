
#include "ram.h"
#include "../library/string.h"

ram_directory* clear_directory_block(ram_directory* dir)
{
	for(uint32_t i = 0; i < RAM_FS_BLOCK_LENGTH; i++)
	{
		dir->block[i].name_length = 0;
		dir->block[i].type        = RAM_FILE_TYPE_NONE;
	}
	
	return dir;
}

ram_directory* create_root_directory(mem_addr address, mem_addr start, mem_addr end)
{
	ram_directory* dir = (ram_directory*)address;
	
	dir->root = dir;
	dir->up   = nullptr;
	
	clear_directory_block(dir);
	
	dir->block[0].name_length = 4;
	strncpy((char*)dir->block[0].name, "free", 4);
	dir->block[0].range[0]    = start;
	dir->block[0].range[1]    = end;
	dir->block[0].type        = RAM_FILE_TYPE_FREE;
	
	return 0;
}

mem_addr get_file_size(ram_file_entry* file)
{
	return file->range[1] - file->range[0];
}

ram_file_entry* find_free_entry(ram_directory* dir, mem_addr size)
{
	for(uint32_t i = 0; i < RAM_FS_BLOCK_LENGTH; i++)
	{
		if(dir->block[i].type != RAM_FILE_TYPE_NONE && 
		   get_file_size(&dir->block[i]) >= size)
			return &dir->block[i];
	}
	return nullptr;
}

ram_file_entry* find_unused_entry(ram_directory* dir)
{
	for(uint32_t i = 0; i < RAM_FS_BLOCK_LENGTH; i++)
	{
		if(dir->block[i].type == RAM_FILE_TYPE_NONE)
			return &dir->block[i];
	}
	return nullptr;
}

ram_file_entry* allocate_file(ram_directory* dir, mem_addr size)
{
	// Find file entry with the right amount of room
	// At the moment, root has all the free space
	ram_file_entry* nf = find_free_entry(dir->root, size);
	
	// No free space available
	if(nf == nullptr){
		return nullptr;
	}
	
	// Find file entry that is not being used
	ram_file_entry* ef = find_unused_entry(dir);
	
	// Reached full capacity
	if(ef == nullptr)
		return nullptr;
	
	// Setup ranges
	ef->range[0] = nf->range[0];
	ef->range[1] = nf->range[0] + size - 1;
	
	// Occupy the space
	if(get_file_size(nf) == get_file_size(ef)) {
		nf->type = RAM_FILE_TYPE_NONE;
	} else {
		nf->range[0] += size;
	}
	
	return ef;
}

ram_file_entry* create_file(ram_directory* dir, char* name, mem_addr size, int type)
{
	ram_file_entry* nf = allocate_file(dir, size);
	
	// Couldn't create file
	if(nf == nullptr)
		return nullptr;
	
	nf->name_length = strlen(name);
	strncpy((char*)nf->name, name, nf->name_length);
	nf->type        = type;
	
	return nf;
}

ram_file_entry* make_file(ram_directory* dir, char* name, void* address, int type){
	ram_file_entry* nf = find_unused_entry(dir);
	
	// Reached full capacity
	if(nf == nullptr)
		return nullptr;
	
	nf->name_length = strlen(name);
	strncpy((char*)nf->name, name, nf->name_length);
	nf->address     = address;
	nf->type        = type;
	return nf;
}



ram_directory* create_directory(ram_directory* dir, char* name){
	ram_file_entry* nf = create_file(dir, name, sizeof(ram_directory),RAM_FILE_TYPE_DIRECTORY);
	
	ram_directory* nd = (ram_directory*)nf->range[0];
	
	if(nd == nullptr)
		return nullptr;
	
	nd->root = dir->root;
	nd->up   = dir;
	
	clear_directory_block(nd);
	
	return nd;
}
