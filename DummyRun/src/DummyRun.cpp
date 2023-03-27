#include "DummyRun.h"
#include "protocol.h"
#include "config.h"
#include "Errcode.h"
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
	if( NULL != _dummy )
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
	if( _login_session->IsActive())
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
	if( _game_session->IsActive())
		_game_session->OnDestroy();
	
	if(false == _game_session->OnCreate())
	{
		_is_alive = false;
		return false;
	}

	_dummy->Initialize();
	return true;
}

int DummyRun::Disptch( Packet& packet )
{
	try
	{
		switch( packet.GetID() )
		{
		case LOGIN_ACK:
			OnLoginAck( packet );
			break;
		case SERVERLIST_ACK:
			OnServerListAck( packet );
			break;
		case SERVERIP_ACK:
			OnServerIPAck( packet );
			break;
		case CONNECTION_ACK:
			OnConnectionAck(packet);
			break;
        case ENTERCHANNEL_ACK:
            OnEnterChannelAck(packet);                        
            break;
        case LEAVECHANNEL_ACK:
            OnLeaveChannelAck(packet);
            break;
        case EXITGAMESERVER_ACK:
            OnExitServerAck(packet);
            break;
        case USERLISTINCHANNEL_ACK:
            break;
        case ROOMLIST_ACK:
			OnRoomListAck( packet );
            break;
		case MATCHROOMINFO_ACK:
			OnMatchRoomInfoAck(packet);
			break;
		case XIGNCODE_NOTICE:
			break;
		case REGISTERTEAMMATCH_ACK:
			break;
		case CREATEROOM_ACK:
			OnSFCreateRoomAck(packet);
			break;
		case REWARDITEM_NOTICE:
			break;
		case ACCKEY_NOTCIE:
			break;
		case CREATEROOM_NOTICE:
			break;
		case CHANGEROOMCOUNT_NOTICE:
			break;
		case CHANGEUSERLOCATION_NOTICE:
			break;
		case MESSENGER_FRIENDLOGOUT_NOTICE:
			break;
		case MESSENGER_FRIENDLIST_NOTICE:
			break;
		case MESSENGER_CHANGEFRIENDSTATUS_NOTICE:
			break;
		case MESSENGER_LEAGUEMEMBER_NOTICE:
			break;
		case QUESTINFO_NOTICE:
			break;
		case CLANMEDAL_NOTICE:
			break;
		case SPRAYSLOTDATAINDB_NOTICE:
			break;
		case VCHARSLOT_SLOTDATAINDB_NOTICE:
			break;
		case ICONLIST_NOTICE:
			break;
		case EVENTINFO_NOTICE:
			break;
		case WEBITEM_NOTICE:
			break;
		case GRADE_LEV_NOTICE:
			break;
		case ASSISTRATE_NOTICE:
			break;
		case CLANLADDER_NOTICE:
			break;
		case CLANMASTER_NOTICE:
			break;
		case CLANMEMBERLIST_NOTICE:
			break;
		case ENTER_CLAN_CHANNEL_ACK:
			break;
		default:
			return LocalPacketParsing( packet );
		}
	}
	catch (...)
	{
		Destroy();
		return EXCEPTION; 
	}

	return packet.GetPacketSize();
}

void DummyRun::OnLoginAck( Packet& packet )
{
	byte err_code;
	packet >> err_code;

	if( err_code != LOGIN_SUCCESS )
		Destroy();

	Packet sendmsg(CONNECTCLOSE_REQ);
	_login_session->SendPacket( sendmsg );

	Packet reqmsg(SERVERLIST_REQ);
	reqmsg << LOGINSERVER_ID << true;
	_login_session->SendPacket(reqmsg);
}

void DummyRun::OnServerListAck( Packet& packet )
{
	Packet sendmsg(SERVERIP_REQ);
	sendmsg << LOGINSERVER_ID << GAMESERVER_ID;
	_login_session->SendPacket(sendmsg);
}

void DummyRun::OnServerIPAck( Packet& packet )
{
	GameSessionRun();

	Packet sendmsg( CONNECTION_REQ );
	sendmsg << static_cast<long>(_dummy->GetID()) << LOGINSERVER_ID << false << false << "T.K";
	_game_session->SendPacket(sendmsg);
}


void DummyRun::OnSFCheckOverlapNickNameAck( Packet& packet )
{
	char result;
	packet >> result;

	if( result == 1 )
	{
		Packet sendmsg(SAVENICKNAME_REQ);
		sendmsg << _dummy->GetCodeName();
		_game_session->SendPacket(sendmsg);
	}		
	else
		Destroy();
}

void DummyRun::OnSFSaveNickNameAck(Packet& packet)
{
	bool result;
	packet >> result;

	if( result == true)
	{
		Packet sendmsg(ENTERFORCESETTING_REQ);
		sendmsg << static_cast<byte>(1);
		_game_session->SendPacket(sendmsg);
	}
	else
		Destroy();
}

void DummyRun::OnSFCreateRoomAck(Packet packet)
{
	byte err_code = 0;
	packet >> err_code;

	if( err_code != 0)
	{
		Packet sendmsg(ENTERROOM_REQ);
		sendmsg << "" << "";
		_game_session->SendPacket(sendmsg);
	}
}

void DummyRun::OnSFEnterFoceSettingAck(Packet& packet)
{
	// CheckBalanceReq()

	Packet sendmsg(BUYCHARACTER_REQ);
	sendmsg << static_cast<long>(DEFAULT_CHARACTER_ITEMID);
	_game_session->SendPacket(sendmsg);

	Sleep(1000);
	sendmsg.Clear();
	sendmsg.SetID( LEAVEFORCESETTING_REQ );
	_game_session->SendPacket(sendmsg);

	sendmsg.Clear();
	sendmsg.SetID(ENTERCHANNEL_REQ);
	sendmsg << static_cast<byte>((_dummy->GetID() % MAX_CHANNEL_COUNT)+1);
	_game_session->SendPacket(sendmsg);
}

void DummyRun::OnConnectionAccept( Packet& packet )
{
	Packet sendmsg( CONNECTION_REQ );
	sendmsg << static_cast<long>(_dummy->GetID()) << LOGINSERVER_ID >> false >> false >> "tt";
	_game_session->SendPacket(sendmsg);
}

void DummyRun::OnSFEnterRoomAck(Packet& packet)
{
	BYTE error_code;
	packet >> error_code;

	if( error_code != SUCCESS_ENTER_ROOM )
	{
		//Destroy();
	}
}

void DummyRun::OnConnectionAck( Packet& packet )
{
	BYTE err_code;
	packet >> err_code;

	if( err_code != LOGIN_SUCCESS )
	{
		Destroy();
		return;
	}

	auto time_stamp = GetTickCount64();

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
			sendmsg << static_cast<byte>(TEAM_GAME)			//byteGameType 
				<< "Åä¸£´Ô ¸ÚÀÖ¾î¿ä"			//strRoomName 
				<< static_cast<byte>(16)		//byteMaxUser 
				<< static_cast<byte>(0)			//byteThirdView 
				<< static_cast<byte>(0)			//byteEnemyView 
				<< static_cast<bool>(false)		//bPrivate 
				<< ""							//strPassword 
				<< static_cast<byte>(0)			//byteRoomNameColorID
				<< false						//bObserver 
				<< ""							//strObserverPassword 
				<< static_cast<WORD>(20)		//wMapType 
				<< false						//bArmsChange
				<< true							//bIntrude 
				<< false						//bTracerBullet 
				<< true;						//bTeamSwap;
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


void DummyRun::OnLeaveChannelAck( Packet& packet )
{
    Packet sendmsg( EXITGAMESERVER_REQ );
    _game_session->SendPacket(sendmsg);
}

void DummyRun::OnExitServerAck( Packet& packet )
{
	Destroy();
}

void DummyRun::OnRoomListAck( Packet& packet )
{
}