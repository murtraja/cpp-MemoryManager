#pragma once
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
		unsigned int m_guardBytes{ 0xFDFDFDFD };
	} m_guard;
public:
	static MemoryBlockManager& GetInstance();
	void* Allocate(const char* functionName, int lineNo, const char* fileName, int requestedSize);
	void* Allocate(const char* functionName, int lineNo, const char* fileName, int requestedTypeSize, int count);
	~MemoryBlockManager();
};

