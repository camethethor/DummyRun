#pragma once
#include <windows.h>

class Sync  
{
public:
	Sync();
	virtual ~Sync() = 0;

	CRITICAL_SECTION* GetCS()	 {return &_cs; }

private:
	CRITICAL_SECTION	_cs;
};