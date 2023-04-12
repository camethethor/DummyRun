#pragma once

#include "Map.h"
#include "DummyRun.h"

class DummyRunManager
{
public:
	static DummyRunManager* GetInstance();
	static void ReleaseInstance();

	bool	Initialize(void);
	bool	Start(void);

	long	IncreaseActiveCount() { return InterlockedIncrement(&_active_count); }
	long	DecreaseActiveCount() { return InterlockedDecrement(&_active_count); }

private:
	DummyRunManager(void);
	~DummyRunManager(void);

	long	_active_count;

	Map<unsigned long,DummyRun*>		_dummy_container;
	static	DummyRunManager*			_self;
};
