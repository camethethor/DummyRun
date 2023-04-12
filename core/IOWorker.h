#pragma once
#include "Thread.h"

class IOWorker : public Thread
{
public:
	IOWorker();
	~IOWorker();

	virtual void Run();
	void Close();
};

