#include "NetworkHandler.h"
#include "IOWorker.h"

NetworkHandler* NetworkHandler::_self = NULL;

NetworkHandler::NetworkHandler() 
:_iocp(NULL), _workers(NULL), _worker_cnt(0)
{
}

NetworkHandler::~NetworkHandler()
{
	ReleaseInstance();
}

NetworkHandler* NetworkHandler::GetInstance()
{
	if(NULL == _self)
		_self = new NetworkHandler();

	return _self;
}

void NetworkHandler::ReleaseInstance()
{
	DestroyIOCP();

	if(_self)
	{
		delete _self;
		_self = NULL;
	}
}

bool NetworkHandler::Initialize()
{
	WSADATA wsd;
	_iocp = NULL;

	if(::WSAStartup(MAKEWORD(2, 2), &wsd) !=0)
		return false;

	if(false == CreateIOCP())
		return false;

	return true;
}

bool NetworkHandler::CreateIOCP()
{
	_iocp = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);

	if(false == _iocp)
		return false;
	
	if(false == CreateThreadPool())
		return false;

	return true;
}

bool NetworkHandler::CreateThreadPool()
{
	SYSTEM_INFO systemInfo;

	GetSystemInfo(&systemInfo);
	_worker_cnt = systemInfo.dwNumberOfProcessors * 2 + 2;

	_workers = new IOWorker[_worker_cnt];

	for(int idx = 0; idx != _worker_cnt; ++idx)
	{
		_workers[idx].Begin();	
	}
	
	return true;
}

bool NetworkHandler::Associate(HANDLE hDevice, ULONG_PTR pCompletionKey)
{
	HANDLE hdle;

	hdle = CreateIoCompletionPort(hDevice, _iocp, pCompletionKey, 0);
	
	if(hdle != _iocp)
		return false;

	return true;
}

void NetworkHandler::DestroyIOCP()
{	
	if(_iocp)
	{
		for(int i = 0; i != _worker_cnt; ++i)
		{
			PostQueuedCompletionStatus(_iocp, 0, NULL, NULL);
		}

		CloseHandle(_iocp);
		_iocp = NULL;
	}
	
	::Sleep(1000);

	for(int idx = 0; idx != _worker_cnt; ++idx)
	{
		((IOWorker*)&_workers[idx])->Close();
	}

	if(_workers)
	{
		delete [] _workers;
		_workers = NULL;
	}

	::WSACleanup();
}