
#include <include/ramfs.h>
#include <include/library/string.h>

ram_dir* clear_directory_block(ram_dir* dir)
{
	for(uint32_t i = 0; i < RAM_FS_BLOCK_LENGTH; i++)
	{
		dir->block[i].type = RAM_FILE_TYPE_NONE;
	}
	return dir;
}

ram_dir* create_root_directory(mem_addr address, mem_addr start, mem_addr end)
{
	ram_dir* dir = (ram_dir*)address;
	
	dir->root = dir;
	dir->up   = nullptr;
	
	clear_directory_block(dir);
	
	const char* name = "free";
	
	strncpy((char*)dir->block[0].name, name, strlen(name) + 1);
	dir->block[0].range[0]    = start;
	dir->block[0].range[1]    = end;
	dir->block[0].type        = RAM_FILE_TYPE_FREE;
	
	return dir;
}

mem_addr get_file_size(ram_entry* file)
{
	return file->range[1] - file->range[0] + 1;
}

ram_entry* find_free_entry(ram_dir* dir, mem_addr size)
{
	for(uint32_t i = 0; i < RAM_FS_BLOCK_LENGTH; i++)
	{
		if(dir->block[i].type == RAM_FILE_TYPE_FREE && 
		   get_file_size(&dir->block[i]) >= size)
			return &dir->block[i];
	}
	return nullptr;
}

ram_entry* find_unused_entry(ram_dir* dir)
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
ram_entry* find_free_entry_region(ram_dir* dir, mem_addr start, mem_addr end)
{
	for(uint32_t i = 0; i < RAM_FS_BLOCK_LENGTH; i++)
	{
		if (dir->block[i].type == RAM_FILE_TYPE_FREE && 
		    dir->block[i].range[0] <= start &&
		    dir->block[i].range[1] >= end)
			return &dir->block[i];
	}
	return nullptr;
}
int cutout_entry_region(ram_dir* dir, ram_entry* file, mem_addr start, mem_addr end)
{
	
	if(file->range[0] == start)
	{
		file->range[0] = end + 1;
		
		// Region takes entire space
		if(file->range[1] == end)
		{
			file->type = RAM_FILE_TYPE_NONE;
		}
	}
	else if(file->range[1] == end)
	{
		file->range[0] = start - 1;
	}
	else
	{
		// Create a 2 new files. 1st file 
		//  containing the selected region.
		//  2nd containing the other part.
		//  This will leave a spot in between.
		
		// Contains last part
		ram_entry* f = find_unused_entry(dir);
		
		if(f == nullptr)
			return 1;
		
		f->type = file->type;
		f->range[0] = end + 1;
		f->range[1] = file->range[1];
		
		file->range[1] = start - 1;
	}
	
	return 0;
}

ram_entry* allocate_file(ram_dir* dir, mem_addr size)
{
	// Find file entry with the right amount of room
	// At the moment, root has all the free space
	ram_entry* nf = find_free_entry(dir->root, size);
	
	// No free space available
	if (nf == nullptr)
		return nullptr;
	
	// Find file entry that is not being used
	ram_entry* ef = find_unused_entry(dir);
	
	// Reached full capacity
	if(ef == nullptr)
		return nullptr;
	
	ef->range[0] = nf->range[0];
	ef->range[1] = nf->range[0] + size - 1;
	nf->range[0] += size;
	
	return ef;
}

ram_entry* allocate_file_region(ram_dir* dir, mem_addr start, mem_addr end)
{
	// Find file entry with the right amount of room
	// At the moment, root has all the free space
	ram_entry* nf = find_free_entry_region(dir->root, start, end);
	
	// No free space available
	if (nf == nullptr)
		return nullptr;
	
	// Find file entry that is not being used
	ram_entry* ef = find_unused_entry(dir);
	
	// Reached full capacity
	if(ef == nullptr)
		return nullptr;
	
	cutout_entry_region(dir->root, nf, start, end);
	
	ef->range[0] = start;
	ef->range[1] = end;
	
	return ef;
}


ram_entry* create_file(ram_dir* dir, const char* name, mem_addr size, int type)
{
	ram_entry* nf = allocate_file(dir, size);
	
	// Couldn't create file
	if (nf == nullptr)
		return nullptr;
	
	strncpy((char*)nf->name, name, strlen(name) + 1);
	nf->type        = type;
	nf->dir         = dir;
	
	return nf;
}

ram_entry* make_file(ram_dir* dir, const char* name, mem_addr start, mem_addr end, int type)
{
	// Check for free space
	ram_entry* nf = allocate_file_region(dir, start, end);
	
	strncpy((char*)nf->name, name, strlen(name) + 1);
	nf->type        = type;
	nf->range[0]    = start;
	nf->range[1]    = end;
	nf->dir         = dir;
	return nf;
}

ram_dir* create_directory(ram_dir* dir, const char* name)
{
	// Create the file representing the directory
	ram_entry* nf = create_file(dir, name, sizeof(ram_dir),RAM_FILE_TYPE_DIRECTORY);
	
	if (nf == nullptr)
		return nullptr;
	
	ram_dir* nd = (ram_dir*)nf->range[0];
	
	nd->root = dir->root;
	nd->up   = dir;
	
	clear_directory_block(nd);
	
	return nd;
}

ram_dir* find_directory(ram_dir* dir, const char* name)
{
	for(mem_addr i = 0; i < RAM_FS_BLOCK_LENGTH; i++)
	{
		if(dir->block[i].type == RAM_FILE_TYPE_DIRECTORY &&
		   !strcmp(dir->block[i].name, name))
		   return (ram_dir*)dir->block[i].range[0];
	}
	return nullptr;
}

ram_entry* find_file(ram_dir* dir, const char* name)
{
	for(mem_addr i = 0; i < RAM_FS_BLOCK_LENGTH; i++)
	{
		if(dir->block[i].type == RAM_FILE_TYPE_FILE &&
		   !strcmp(dir->block[i].name, name))
		   return &dir->block[i];
	}
	return nullptr;
}

// Find first valid entry
ram_entry* get_first_file(ram_dir* dir)
{
	ram_entry* f = nullptr;
	
	mem_addr i = 0;
	while (i < RAM_FS_BLOCK_LENGTH)
	{
		if(dir->block[i].type != RAM_FILE_TYPE_NONE)
		{
			f = &dir->block[i];
			break;
		}
		++i;
	}
	return f;
}

ram_entry* get_next_file(ram_dir* dir, ram_entry* iter)
{
	ram_entry* f = nullptr;
	
	// Calculate point to start
	mem_addr i = ((mem_addr)iter - (mem_addr)&dir->block[0]) / sizeof(ram_entry);
	
	while (i < RAM_FS_BLOCK_LENGTH)
	{
		if(dir->block[i].type != RAM_FILE_TYPE_NONE)
		{
			f = &dir->block[i];
			break;
		}
		++i;
	}
	return f;
}



