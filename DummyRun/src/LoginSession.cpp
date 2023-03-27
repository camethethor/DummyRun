#include "LoginSession.h"
#include "Networkhandler.h"
#include "IPacketParser.h"
#include "Errcode.h"
#include "config.h"

LoginSession::LoginSession(IPacketParser* ipp )
: Session( INVALID_SOCKET ), _packet_parser(ipp)
{
}

LoginSession::~LoginSession(void)
{
}

bool LoginSession::Initialize()
{
	return true;
}

bool LoginSession::OnCreate()
{
	if( !ConnectTo( LOGINSERVER_IP, LOGINSERVER_PORT ) )
		return false;

	NetworkHandler::GetInstance()->Associate( ( HANDLE )_sock, ( ULONG_PTR )this );

	if( false == Session::OnCreate())
		return false;

	return true;
}

void LoginSession::OnDestroy()
{
	Session::OnDestroy();
}

int LoginSession::ProcessRecv( const char* buf, const int size )
{
	Packet packet( buf, size );

	if( packet.CheckValidity() == false )
		return size;

	return _packet_parser->Disptch(packet);
}


int LoginSession::SendPacket( Packet& packet )
{
	return Session::SendPacket( packet.GetPacketBuffer(), packet.GetPacketSize());
}

