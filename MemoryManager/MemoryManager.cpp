// MemoryManager.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include "MemoryBlockManager.h"

#define MNEW new(__FUNCSIG__, __LINE__, __FILE__)

class Test
{
	int m_member{};
public:
	Test() = default;

	Test(int no)
		: m_member{ no }
	{}

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

void Test2()
{
	if (MemoryBlockManager::GetInstance().CheckMemory())
	{
		return;
	}

	Test* corrupt = MNEW Test{(signed int)0xDEADBEEF};
	char* endOfMemory = reinterpret_cast<char*>(corrupt + 1);
	*endOfMemory = '\0';
	if (MemoryBlockManager::GetInstance().CheckMemory())
	{
		std::cout << "Memory corrupted\n";
	}
}

int main()
{
	Test1();
	Test2();
    return 0;
}

