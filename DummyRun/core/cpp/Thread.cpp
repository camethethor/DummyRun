#include "Thread.h"
#include <process.h>

Thread::Thread()
:_running( false )
{
}

Thread::~Thread()
{
}

void Thread::Begin()
{
	if( _running )
		return;

	_running = true;
	_t = &std::thread(Thread::HandleRunner);
	_t->join();
}

void Thread::End()
{
	if( !_running)
		return;
	_running = false;
}

unsigned int Thread::HandleRunner( LPVOID param )
{
	Thread* t = (Thread*)param;
	t->Run();
	
	return 0;
}