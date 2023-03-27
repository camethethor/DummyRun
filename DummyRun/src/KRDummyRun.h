#pragma once
#include "DummyRun.h"

class KRDummyRun :	public DummyRun
{
public:
	KRDummyRun();
	virtual ~KRDummyRun(void);
	
	virtual void	Run();
private:
	virtual Dummy*	CreateDummy( const unsigned int id );
	virtual int		LocalPacketParsing(Packet& packet);
};
