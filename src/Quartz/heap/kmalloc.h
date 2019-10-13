#ifndef QZ_KMALLOC_H
#define QZ_KMALLOC_H

#include "MemoryManager.h"

void* kmalloc(size_t size) 
{
	return MemoryManager::Get()->malloc(size);
}

void kfree(void* ptr)
{
	return MemoryManager::Get()->free(ptr);
}

#endif // !QZ_KMALLOC_H
