#include "DummyRun.h"
#include "protocol.h"
#include "Config.h"
#include "ErrCode.h"
#include "GameSession.h"
#include "LoginSession.h"
#include <WS2tcpip.h>

DummyRun::DummyRun() : _is_alive(false), _dummy(NULL)
{
	_login_session = new LoginSession(this);
	_game_session = new GameSession(this);
}

DummyRun::~DummyRun(void)
{
	if(NULL != _dummy)
		delete	_dummy;

	delete _login_session;
	delete _game_session;
}

void DummyRun::Destroy()
{
	_is_alive = false;
	_login_session->OnDestroy();
	_game_session->OnDestroy();
}

bool DummyRun::Initialize(const unsigned int id)
{
	_dummy = CreateDummy(id);

	if(_dummy == NULL)
		return false;

	return true;
}

bool DummyRun::LoginSessionRun()
{
	if(_login_session->IsActive())
		_login_session->OnDestroy();
	
	if(false == _login_session->OnCreate())
	{
		_is_alive = false;
		return false;
	}

	_is_alive = true;
	return true;
}


bool DummyRun::GameSessionRun()
{
	if(_game_session->IsActive())
		_game_session->OnDestroy();
	
	if(false == _game_session->OnCreate())
	{
		_is_alive = false;
		return false;
	}

	_dummy->Initialize();
	return true;
}

int DummyRun::Disptch(Packet& packet)
{
	try
	{
		switch(packet.GetID())
		{
		case LOGIN_ACK:
			OnLoginAck(packet);
			break;
		case CONNECTION_ACK:
			OnConnectionAck(packet);
			break;
        case EXITGAMESERVER_ACK:
            OnExitServerAck(packet);
            break;
		case CREATEROOM_ACK:
			OnSFCreateRoomAck(packet);
			break;
		default:
			return LocalPacketParsing(packet);
		}
	}
	catch (...)
	{
		Destroy();
		return EXCEPTION; 
	}

	return packet.GetPacketSize();
}

void DummyRun::OnLoginAck(Packet& packet)
{
	byte err_code;
	packet >> err_code;

	if(err_code != LOGIN_SUCCESS)
		Destroy();

	GameSessionRun();

	Packet sendmsg(CONNECTCLOSE_REQ);
	_login_session->SendPacket(sendmsg);
}


void DummyRun::OnConnectionAccept(Packet&)
{
	Packet sendmsg(CONNECTION_REQ);
	sendmsg << static_cast<long>(_dummy->GetID()) << LOGINSERVER_ID >> false >> false >> "tt";
	_game_session->SendPacket(sendmsg);
}

void DummyRun::OnSFEnterRoomAck(Packet& packet)
{
	BYTE error_code;
	packet >> error_code;

	if(error_code != SUCCESS_ENTER_ROOM)
	{
		//Destroy();
	}
}

void DummyRun::OnConnectionAck(Packet& packet)
{
	BYTE err_code;
	packet >> err_code;

	if(err_code != LOGIN_SUCCESS)
	{
		Destroy();
		return;
	}

	ULONG addr;
	auto result = inet_pton(AF_INET, DUMMYMACHINE_IP, &addr);
	if (result != SOCKET_ERROR) {
		Destroy();
		return;
	}

	bool bSuccess;
	packet >> bSuccess;

	if (bSuccess)
	{
		int dummy_behavior_type = _dummy->GetID() % DMY_BEHAVIOR_COUNT;

		switch (dummy_behavior_type)
		{
		case DMY_CREATE_ROOM:
		{
			Packet sendmsg(CREATEROOM_REQ);
			sendmsg << static_cast<byte>(TEAM_GAME)
				<< "create dummy room"
				<< static_cast<byte>(16);
			_game_session->SendPacket(sendmsg);
		}
		break;
		case DMY_JOIN_ROOM:
		{
			Packet sendmsg(ROOMLIST_REQ);
			_game_session->SendPacket(sendmsg);
		}
		break;
		case DMY_CHAT_SEND:
		{
			Packet sendmsg(CHATINCHANNEL_REQ);
			sendmsg << "abcdefghijklmnopqrstuvwxyz";
			_game_session->SendPacket(sendmsg);
		}
		break;
		default:
			break;
		}
	}
	else
		Destroy();

}

void DummyRun::OnExitServerAck(Packet&)
{
	Packet sendmsg(EXITGAMESERVER_REQ);
	_game_session->SendPacket(sendmsg);
	Destroy();
}
