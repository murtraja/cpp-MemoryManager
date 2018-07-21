#include "stdafx.h"
#include "MemoryBlockManager.h"
#include <new>


MemoryBlockManager::MemoryBlockManager()
	: m_head{nullptr}
{
}


MemoryBlockManager & MemoryBlockManager::GetInstance()
{
	static MemoryBlockManager manager;
	return manager;
}

void * MemoryBlockManager::Allocate(const char * functionName, int lineNo, const char * fileName, int requestedSize)
{
	int sizeOfMemBlock = sizeof(MemBlock);
	int sizeofGuard = sizeof(GuardBytes);
	int totalSize = sizeOfMemBlock + requestedSize + sizeofGuard;
	char* rawMemory = new char[totalSize];
	
	MemBlock* startPtr = reinterpret_cast<MemBlock*>(rawMemory);
	new (startPtr) MemBlock{ functionName, lineNo, fileName, requestedSize, false, rawMemory + sizeOfMemBlock, nullptr };

	MemBlock* previousHead = m_head;
	m_head = startPtr;
	startPtr->next = previousHead;

	GuardBytes* endPtr = reinterpret_cast<GuardBytes*>(rawMemory + sizeOfMemBlock + requestedSize);
	new (endPtr) GuardBytes(m_guard);

	return reinterpret_cast<void*>(rawMemory + sizeOfMemBlock);
}

void * MemoryBlockManager::Allocate(const char * functionName, int lineNo, const char * fileName, int requestedTypeSize, int count)
{
	return nullptr;
}
bool MemoryBlockManager::IsBlockCorrupt(MemBlock * memBlock)
{
	char *guardStart = memBlock->m_requestMemPtr + memBlock->m_requestedSize;
	GuardBytes* guard = reinterpret_cast<GuardBytes*>(guardStart);
	return IsGuardInvalid(guard);
}
bool MemoryBlockManager::IsGuardInvalid(GuardBytes * guard)
{
	return guard->m_guardBytes != GUARD_BYTES;
}
bool MemoryBlockManager::CheckMemory()
{
	MemBlock* firstCorruptBlock = GetFirstCorruptBlock();
	if (firstCorruptBlock)
	{
		return true;
	}

	return false;
}
MemoryBlockManager::MemBlock * MemoryBlockManager::GetFirstCorruptBlock()
{
	MemBlock* block = m_head;
	while (block)
	{
		if (IsBlockCorrupt(block))
		{
			return block;
		}
		block = block->next;
	}
	return nullptr;
}
MemoryBlockManager::~MemoryBlockManager()
{
}
