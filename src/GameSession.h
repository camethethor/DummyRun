#pragma once
#include "session.h"
#include "Packet.h"

class IPacketParser;
class GameSession : public Session
{
public:
	GameSession(IPacketParser* ipp);
	virtual ~GameSession(void);
	
	virtual bool OnCreate();
	virtual void OnDestroy();
	virtual int SendPacket(Packet& packet);

private:
	virtual int ProcessRecv(const char* buf, const int size);

private:
	IPacketParser*	_packet_parser;
};
