#pragma once
#ifndef _WINSOCKAPI_
#define _WINSOCKAPI_
#endif

#include "Dummy.h"
#include "IPacketParser.h"

class LoginSession;
class GameSession;
class DummyRun :public IPacketParser
{
public:
	DummyRun();
	virtual ~DummyRun(void);

	void Destroy();
	bool Initialize(const unsigned int id);
	bool LoginSessionRun();
	bool GameSessionRun();

	bool	GetAlive() { return _is_alive; }
	void	SetAlive(bool alive) { _is_alive = alive; }

	virtual int Disptch( Packet& packet );
	virtual void Run() = 0;

private:
	DummyRun(const DummyRun& src);
	DummyRun& operator = (const DummyRun& src);

	virtual void OnLoginAck( Packet& packet );
	virtual void OnServerListAck( Packet& packet );
	virtual void OnServerIPAck( Packet& packet );
	virtual void OnConnectionAccept( Packet& packet );
	virtual void OnConnectionAck( Packet& packet );
	virtual void OnSendUserDataAck( Packet& packet );
	virtual void OnWebItemWareDataAck( Packet& packet );
	virtual void OnMatchTeamInfoNotice( Packet& packet );
	virtual void OnMatchRoomInfoAck( Packet& packet );
    virtual void OnEnterChannelAck( Packet& packet );
    virtual void OnLeaveChannelAck( Packet& packet );
    virtual void OnExitServerAck( Packet& packet );
	virtual void OnSFEnterRoomAck( Packet& packet );
	virtual void OnSFUserDataInDBNotice(Packet& packet);
	virtual void OnSFCheckOverlapNickNameAck(Packet& packet);
	virtual void OnSFSaveNickNameAck(Packet& packet);
	virtual void OnSFEnterFoceSettingAck(Packet& packet);
	virtual void OnSFCreateRoomAck(Packet packet);
	virtual void OnRoomListAck(Packet& packet);

	virtual int LocalPacketParsing(Packet& packet) = 0;
	virtual Dummy*	CreateDummy( const unsigned int id ) = 0;

protected:
	Dummy*				_dummy;
	LoginSession*		_login_session;
	GameSession*		_game_session;

private:
	bool				_is_alive;
};
