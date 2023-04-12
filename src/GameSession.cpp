#include "GameSession.h"
#include "Protocol.h"
#include "Networkhandler.h"
#include "ErrCode.h"
#include "Config.h"
#include "DummyRunManager.h"
#include "IPacketParser.h"

GameSession::GameSession(IPacketParser* ipp)
: Session(INVALID_SOCKET), _packet_parser(ipp)
{
}

GameSession::~GameSession(void)
{
}


bool GameSession::OnCreate()
{
	if(!ConnectTo(GAMESERVER_IP, GAMESERVER_PORT))
		return false;

	NetworkHandler::GetInstance()->Associate(reinterpret_cast<HANDLE>(_sock), reinterpret_cast<ULONG_PTR>(this));

	if(false == Session::OnCreate())
		return false;
	
	DummyRunManager::GetInstance()->IncreaseActiveCount();
	return true;
}

void GameSession::OnDestroy()
{
	DummyRunManager::GetInstance()->DecreaseActiveCount();
	Session::OnDestroy();
}

int GameSession::ProcessRecv(const char* buf, const int size)
{
	Packet packet(buf, size);

	if(packet.CheckValidity() == false)
		return size;

	return _packet_parser->Disptch(packet);
}


int GameSession::SendPacket(Packet& packet)
{
	return Session::SendPacket(packet.GetPacketBuffer(), packet.GetPacketSize());
}

