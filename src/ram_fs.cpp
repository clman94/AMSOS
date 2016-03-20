
#include <include/ram.h>
#include <include/library/string.h>

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
	return file->range[1] - file->range[0] + 1;
}

ram_file_entry* find_free_entry(ram_directory* dir, mem_addr size)
{
	for(uint32_t i = 0; i < RAM_FS_BLOCK_LENGTH; i++)
	{
		if(dir->block[i].type == RAM_FILE_TYPE_FREE && 
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

// Find a entry with free memory of 
//  a selected region
ram_file_entry* find_free_entry_region(ram_directory* dir, mem_addr start, mem_addr end)
{
	for(uint32_t i = 0; i < RAM_FS_BLOCK_LENGTH; i++)
	{
		if(dir->block[i].type == RAM_FILE_TYPE_FREE && 
		   dir->block[i].range[0] <= start &&
		   dir->block[i].range[1] >= end)
			return &dir->block[i];
	}
	return nullptr;
}
int cutout_entry_region(ram_directory* dir, ram_file_entry* file, mem_addr start, mem_addr end)
{
	
	if(file->range[0] == start){
		file->range[0] = end + 1;
		
		// Region takes entire space
		if(file->range[1] == end){
			file->type = RAM_FILE_TYPE_NONE;
		}
	}
	else if(file->range[1] == end){
		file->range[0] = start - 1;
	}
	else{
		
		// Create a 2 new files. 1st file 
		//  containing the selected region.
		//  2nd containing the other half.
		ram_file_entry* f = find_unused_entry(dir);
		
		if(f == nullptr)
			return 1;
		
		f->type = file->type;
		f->range[0] = end + 1;
		f->range[1] = file->range[1];
		
		file->range[1] = start - 1;
	}
	
	return 0;
}

ram_file_entry* allocate_file(ram_directory* dir, mem_addr size)
{
	// Find file entry with the right amount of room
	// At the moment, root has all the free space
	ram_file_entry* nf = find_free_entry(dir->root, size);
	
	// No free space available
	if (nf == nullptr) {
		return nullptr;
	}
	
	// Find file entry that is not being used
	ram_file_entry* ef = find_unused_entry(dir);
	
	// Reached full capacity
	if(ef == nullptr)
		return nullptr;
	
	ef->range[0] = nf->range[0];
	ef->range[1] = nf->range[0] + size - 1;
	nf->range[0] += size;
	
	return ef;
}

ram_file_entry* allocate_file_region(ram_directory* dir, mem_addr start, mem_addr end)
{
	// Find file entry with the right amount of room
	// At the moment, root has all the free space
	ram_file_entry* nf = find_free_entry_region(dir->root, start, end);
	
	// No free space available
	if (nf == nullptr) {
		return nullptr;
	}
	
	// Find file entry that is not being used
	ram_file_entry* ef = find_unused_entry(dir);
	
	// Reached full capacity
	if(ef == nullptr)
		return nullptr;
	
	cutout_entry_region(dir->root, nf, start, end);
	
	ef->range[0] = start;
	ef->range[1] = end;
	
	return ef;
}


ram_file_entry* create_file(ram_directory* dir, char* name, mem_addr size, int type)
{
	ram_file_entry* nf = allocate_file(dir, size);
	
	// Couldn't create file
	if (nf == nullptr)
		return nullptr;
	
	nf->name_length = strlen(name);
	strncpy((char*)nf->name, name, nf->name_length);
	nf->type        = type;
	
	return nf;
}

ram_file_entry* make_file(ram_directory* dir, char* name, mem_addr start, mem_addr end, int type)
{
	
	ram_file_entry* nf = allocate_file_region(dir, start, end);
	
	nf->name_length = strlen(name);
	strncpy((char*)nf->name, name, nf->name_length);
	nf->type        = type;
	nf->range[0]    = start;
	nf->range[1]    = end;
	
	return nf;
}

ram_directory* create_directory(ram_directory* dir, char* name)
{
	ram_file_entry* nf = create_file(dir, name, sizeof(ram_directory),RAM_FILE_TYPE_DIRECTORY);
	
	ram_directory* nd = (ram_directory*)nf->range[0];
	
	if (nd == nullptr)
		return nullptr;
	
	nd->root = dir->root;
	nd->up   = dir;
	
	clear_directory_block(nd);
	
	return nd;
}
