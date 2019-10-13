#include "MemoryManager.h"

MemoryManager* MemoryManager::m_current_manager = nullptr;

MemoryManager::MemoryManager(size_t start_address, size_t size)
	: m_memory_start((void*) start_address), m_size(size)
{
	if (m_current_manager != nullptr)
	{
		// ERROR
		return;
	}

	m_current_manager = this;

	// Create a free chunk using all available space
	m_first_chunk = (ChunkHeader*)start_address;
	m_first_chunk->allocated = false;
	m_first_chunk->size = m_size;
	m_first_chunk->previous = nullptr;
	m_first_chunk->next = nullptr;

	m_last_chunk = m_first_chunk;
}

MemoryManager::~MemoryManager()
{
	if (m_current_manager == this)
		m_current_manager = nullptr;
}

void* MemoryManager::malloc(size_t size)
{
	ChunkHeader* current_header = m_first_chunk;

	// First fit search
	// Look for the first memory chunk that is large enough
	while (current_header != nullptr)
	{
		if (current_header->allocated)
			continue;
		
		size_t chunk_size = current_header->size;
		if (chunk_size < size)
			continue;
		
		void* allocated_chunk = current_header + sizeof(ChunkHeader);
		current_header->allocated = true;

		// 10 is arbitrary => need to change to more relevant method
		// If the chunk is "a lot" bigger than the requested size
		// Split it to make another free chunk
		if (chunk_size > size + sizeof(ChunkHeader) * 10)
		{
			// Create a new chunk after the current one with all unused space
			ChunkHeader* new_header = (ChunkHeader*)((size_t)allocated_chunk + size);
			new_header->allocated = false;
			new_header->size = current_header->size - size - (2 * sizeof(ChunkHeader));
			new_header->previous = current_header;
			new_header->next = current_header->next;

			// Update current chunk to use only necessary space
			current_header->next = new_header;
			current_header->size = size;
		}

		return allocated_chunk;
	}

	return nullptr;
}

void MemoryManager::free(void* ptr)
{
	if (ptr < m_first_chunk || ptr > m_last_chunk)
	{
		// Error;
		return;
	}

	// Get the header corresponding to ptr's chunk
	ChunkHeader* header = getHeader(ptr);

	if (header->allocated)
	{
		header->allocated = false;

		if (header->previous != nullptr && !header->previous->allocated)
			header = mergeChunks(header->previous, header);

		if (header->next != nullptr && !header->next->allocated)
			header = mergeChunks(header, header->next);
	}
}

MemoryManager* MemoryManager::Get()
{
	return m_current_manager;
}

MemoryManager::ChunkHeader* MemoryManager::getHeader(void* chunk_start)
{
	return (ChunkHeader*)((size_t)chunk_start - sizeof(ChunkHeader));
}

MemoryManager::ChunkHeader* MemoryManager::mergeChunks(ChunkHeader* first, ChunkHeader* second)
{
	// ASSERT

	first->next = second->next;
	first->size += second->size;

	return first;
}
