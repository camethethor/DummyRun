#pragma once
#include "dummyrun.h"

class THDummyRun :	public DummyRun
{
public:
	THDummyRun();
	virtual ~THDummyRun(void);
	
	virtual void	Run();

private:
	virtual Dummy*	CreateDummy( const unsigned int id );
	virtual int		LocalPacketParsing(Packet& packet);
};
