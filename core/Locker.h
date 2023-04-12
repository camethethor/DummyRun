#pragma once
#include <windows.h>
#include "Sync.h"

class Locker
{
public:
	Locker(Sync& sync);
	~Locker();

private:
	CRITICAL_SECTION* _cs;
};

