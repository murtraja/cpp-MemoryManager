// MemoryManager.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "MemoryBlockManager.h"

#define MNEW new(__FUNCSIG__, __LINE__, __FILE__)

class Test
{
	int m_member{};
public:
	void* operator new(size_t size, const char * functionName, int lineNo, const char * fileName)
	{
		return MemoryBlockManager::GetInstance()
			.Allocate(functionName, lineNo, fileName, size);
	}

	void* operator new[](size_t size, const char * functionName, int lineNo, const char * fileName)
	{
		return MemoryBlockManager::GetInstance()
			//.Allocate(functionName, lineNo, fileName, sizeof(Test), size/(sizeof(Test)));
			.Allocate(functionName, lineNo, fileName, size);
	}
};

void Test1()
{
	Test *newTest = MNEW Test();
	Test *arrayTest = MNEW Test[5];
	;
	;
}

int main()
{
	Test1();
    return 0;
}

