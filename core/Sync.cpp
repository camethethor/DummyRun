#include "Sync.h"

Sync::Sync()
{
	InitializeCriticalSection(&_cs);
}

Sync::~Sync()
{
	::DeleteCriticalSection(&_cs);
}
