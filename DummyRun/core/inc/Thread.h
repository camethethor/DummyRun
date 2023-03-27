#pragma once
#include <winsock2.h>
#include <thread>

class Thread 
{
public:
	Thread();
	virtual ~Thread();

	void Begin();
	void End();	
	virtual void Run() = 0;

protected:
	bool			_running;		

private:
	std::thread* _t;
	static unsigned int WINAPI HandleRunner( LPVOID param );
};
