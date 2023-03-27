#include "THDummyRun.h"
#include "THDummy.h"
#include "protocol.h"
#include "errcode.h"
#include "config.h"
#include "loginSession.h"

THDummyRun::THDummyRun()
{
}

THDummyRun::~THDummyRun(void)
{
}

Dummy*	THDummyRun::CreateDummy( const unsigned int id )
{
	Dummy* dmy = new THDummy(id);
	return dmy;
}

void THDummyRun::Run()
{
	if( false == LoginSessionRun() )
	{
		Destroy();
		return;
	}
	
	Packet sendmsg( LOGIN_REQ );
	sendmsg << TH_CLIENT_VERSION << static_cast<long>(_dummy->GetID()) << static_cast<long>(GetTickCount()) << "account" << "LKAJDSJDUW!@#KDD1" << "aa";
	_login_session->SendPacket(sendmsg);
}

int THDummyRun::LocalPacketParsing(Packet& packet)
{
	switch( packet.GetID() )
	{
	case INVALID_PROTOCOL:
		break;
	default:
		Destroy();
		return FORCED;
	}

	return packet.GetPacketSize();
}