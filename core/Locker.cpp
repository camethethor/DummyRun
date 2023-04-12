#include "Locker.h"

Locker::Locker(Sync& sync) : _cs(sync.GetCS())
{
	::EnterCriticalSection(_cs);
}

Locker::~Locker()
{
	::LeaveCriticalSection(_cs);
}
