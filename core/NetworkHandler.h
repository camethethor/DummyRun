#pragma once
#include "Thread.h"

class NetworkHandler  
{
public:
	static NetworkHandler* GetInstance();

	bool Initialize();
	void ReleaseInstance();

	bool Associate(HANDLE hDevice, ULONG_PTR pCompletionKey);
	HANDLE GetIOCPHandle() { return _iocp; }

private:
	NetworkHandler();
	~NetworkHandler();
	
	bool CreateIOCP();
	void DestroyIOCP();
	bool CreateThreadPool();

private:
	static NetworkHandler* _self;

	HANDLE		_iocp;						
	int			_worker_cnt;				
	Thread*		_workers;			
};
