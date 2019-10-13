#ifndef QZ_MEMORY_MANAGER
#define QZ_MEMORY_MANAGER

#include "Quartz/core/types.h"

/*
 *          
 * |--------|-----------------|--------------
 * | header | allocated chunk | header ...                               
 * |--------|-----------------|--------------
 *          ^
 *  ptr returned by malloc
 */

class MemoryManager
{
public:
	MemoryManager(size_t start_address, size_t size);
	~MemoryManager();

	void* malloc(size_t size);
	void free(void* ptr);

	static MemoryManager* Get();

private:
	struct ChunkHeader
	{
		ChunkHeader* previous;
		ChunkHeader* next;
		size_t size;
		bool allocated;
	} __attribute__((packed));

	void* m_memory_start;
	size_t m_size;
	ChunkHeader* m_first_chunk = nullptr;
	ChunkHeader* m_last_chunk = nullptr;

	static MemoryManager* m_current_manager;

	ChunkHeader* getHeader(void* chunk_start);
	ChunkHeader* mergeChunks(ChunkHeader* first, ChunkHeader* second);
};

#endif // !QZ_MEMORY_MANAGER
