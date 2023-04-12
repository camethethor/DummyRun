#pragma once
#include "session.h"
#include "Packet.h"

class IPacketParser;
class LoginSession : public Session
{
public:
	LoginSession(IPacketParser* ipp);
	virtual ~LoginSession(void);

	bool Initialize();
	
	virtual bool OnCreate();
	virtual void OnDestroy();
	virtual int SendPacket(Packet& packet );

private:
	virtual int ProcessRecv(const char* buf, const int size);

private:
	IPacketParser*	_packet_parser;
};
