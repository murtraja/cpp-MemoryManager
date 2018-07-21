#pragma once

#define GUARD_BYTES 0xFDFDFDFD

class MemoryBlockManager
{
	MemoryBlockManager();
	struct MemBlock
	{
		const char* m_functionName;
		int lineNo;
		const char* m_fileName;
		int m_requestedSize;
		bool m_isFree;
		char* m_requestMemPtr;
		MemBlock* next;
	} *m_head;
	struct GuardBytes
	{
		unsigned int m_guardBytes{ GUARD_BYTES };
	} m_guard;

	bool IsBlockCorrupt(MemBlock* memBlock);
	bool IsGuardInvalid(GuardBytes* guard);
	
	MemBlock* GetFirstCorruptBlock();

public:
	static MemoryBlockManager& GetInstance();
	void* Allocate(const char* functionName, int lineNo, const char* fileName, int requestedSize);
	void* Allocate(const char* functionName, int lineNo, const char* fileName, int requestedTypeSize, int count);
	bool CheckMemory(); 
	~MemoryBlockManager();
};

