#include "Session.h"
#include "NetworkHandler.h"
#include "ErrCode.h"
#include <mstcpip.h>
#include "Ws2tcpip.h"

Session::Session(SOCKET s, int max_sending_size, int recv_buf_size)
: _sock(s), _recv_size(0), _event_hdle(INVALID_HANDLE_VALUE), _active(false), 
_send_size(0), _max_send_packet_size(max_sending_size), _recv_buffer(NULL), _recv_buf_size(recv_buf_size)
{
	if(!_recv_buffer)
	{
		_recv_buffer = new char[_recv_buf_size];
		memset(_recv_buffer, 0, _recv_buf_size);
	}

	_event_hdle = ::WSACreateEvent();
	strcpy_s(_ip, "0.0.0.0");
}

Session::~Session()
{
	if(_event_hdle != INVALID_HANDLE_VALUE)
	{
		WSACloseEvent(_event_hdle);
		_event_hdle = INVALID_HANDLE_VALUE;
	}

	OnDestroy();

	if(_recv_buffer)
	{
		delete _recv_buffer;
		_recv_buffer = NULL;
	}
}

bool Session::OnCreate()
{
	_recv_size = 0;

	if(_sock != INVALID_SOCKET)
	{
		GETIP gIP;
		SOCKADDR_IN sockAddr;
		int size = sizeof(sockAddr);

		if(::getpeername(_sock, (SOCKADDR*)&sockAddr, &size) != SOCKET_ERROR)
		{
			gIP.ip = sockAddr.sin_addr.S_un.S_addr;
			sprintf_s(_ip, "%d.%d.%d.%d", gIP.bit_ip.ip_1, gIP.bit_ip.ip_2, gIP.bit_ip.ip_3, gIP.bit_ip.ip_4);
		}
		else
			return false;

		_active = true;
		ReceivePost();
		return true;
	}
	else
	{
		CloseConnection();
		return false;
	}
}

void Session::OnDestroy()
{
	_active = false;
	CloseConnection();

	_recv_size = 0;
	_send_size = 0;
	memset(_recv_buffer, 0, _recv_buf_size);	
}

void Session::SetSocket(SOCKET sock)
{
	_sock = sock;

	DWORD bytes = 0;
	WSAIoctl(_sock, SIO_KEEPALIVE_VALS, &_keep_alive, sizeof(_keep_alive), NULL, NULL, &bytes, &_recv_overlap, NULL);
}

bool Session::ConnectTo(const char* address, WORD port)
{
	CloseConnection();

	_sock = WSASocketW(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
	if(_sock == INVALID_SOCKET)
		return false;

	bool reuse = true;
    ::setsockopt(_sock, SOL_SOCKET, SO_REUSEADDR, (char *)&reuse, sizeof(reuse));

	DWORD dwBytes = 0;
	WSAIoctl(_sock, SIO_KEEPALIVE_VALS, &_keep_alive, sizeof(tcp_keepalive), NULL, NULL, &dwBytes, &_recv_overlap, NULL);
	
	DWORD ret_ip = 0;
	inet_pton(AF_INET, address, &ret_ip);
	
	if(INADDR_NONE == ret_ip)		// DNS ÀÏ °æ¿ì
	{
		HOSTENT FAR * lphostent = gethostbyname(address);
		
		if(!lphostent)
			return false;

		_domain_name = address;
		ret_ip = 0;
		memcpy(&ret_ip, lphostent->h_addr, sizeof(DWORD));

		//char hostname[NI_MAXHOST];
		//struct addrinfo* res, hints;
		//memset(&hints, 0, sizeof(hints));

		//hints.ai_family = PF_UNSPEC;
		//hints.ai_socktype = SOCK_STREAM;
		//hints.ai_flags |= AI_CANONNAME;

		//int err = getaddrinfo(address, NULL, &hints, &res);
		//if (err == 0)
		//{
		//	return false;
		//}

		//if (res) {
		//	inet_pton(AF_INET, res->ai_addr->sa_data, &ret_ip);
		//}
	}

	SOCKADDR_IN serv_sock_addr;
	::ZeroMemory(&serv_sock_addr, sizeof(serv_sock_addr));
	serv_sock_addr.sin_family			= AF_INET;
	serv_sock_addr.sin_addr.s_addr		= ret_ip;
	serv_sock_addr.sin_port				= htons(port);

	if(::connect(_sock, (struct sockaddr*)&serv_sock_addr, sizeof(serv_sock_addr)))
	{
		CloseConnection();
		return false;
	}

	return true;
}

void Session::CloseConnection()
{
	if(_sock == INVALID_SOCKET)
		return;

	::shutdown(_sock, SD_BOTH);
	::closesocket(_sock);
	_sock = INVALID_SOCKET;
}

void Session::ForcedDisconnect(int err)
{
	if(_active && NetworkHandler::GetInstance()->GetIOCPHandle() != INVALID_HANDLE_VALUE)
	{
		PostQueuedCompletionStatus(NetworkHandler::GetInstance()->GetIOCPHandle(), 0, (ULONG_PTR)this, &_recv_overlap);
	}
	_active = false;
}

int Session::SendPacket(char* packet_buf, int size)
{
	if(_sock == INVALID_SOCKET)
		return 0;

	DWORD	send_bytes = 0;
	DWORD	flags		= 0;
	int		io_ret = 0;
	WSABUF	wsaBuffer;

	::ZeroMemory(&_send_overlap, sizeof(OVERLAPPEDEX));
	_send_overlap._flags = ASYNCFLAG_SEND;

	wsaBuffer.buf = packet_buf;
	wsaBuffer.len = size;

	io_ret = WSASend(	_sock, &wsaBuffer, 1, &send_bytes, flags, (OVERLAPPEDEX *)&_send_overlap, NULL);

	if(SOCKET_ERROR == io_ret)
	{
		int err_code = WSAGetLastError();
		if(err_code != ERROR_IO_PENDING)
		{
			if(err_code == WSAECONNRESET || err_code == WSAENETRESET || err_code == WSAENOTCONN || err_code == WSAECONNABORTED)
			{
				ForcedDisconnect(err_code);
			}
			return 0;
		}
	}

	_send_size += send_bytes;

	if(_send_size > _max_send_packet_size || _send_size < -_max_send_packet_size)
	{
		ForcedDisconnect(SENDINGPACKETSIZE_FULL);
	}

	return send_bytes;
}

bool Session::ReceivePost()
{
	if(_sock == INVALID_SOCKET)
		return false;

	DWORD	recv_bytes = 0;
	DWORD	flags = 0;
	DWORD   buf_size = _recv_buf_size - _recv_size;
	int		io_ret = 0;
	WSABUF	wsaBuffer;

	::ZeroMemory(&_recv_overlap, sizeof(OVERLAPPEDEX));
	_recv_overlap._flags = ASYNCFLAG_RECEIVE;

	wsaBuffer.buf = _recv_buffer + _recv_size;
	wsaBuffer.len = buf_size;

	io_ret = WSARecv(	_sock, &wsaBuffer, 1, &recv_bytes, &flags, (OVERLAPPEDEX *)&_recv_overlap, NULL);

	if(SOCKET_ERROR == io_ret)
	{
		int err_code = WSAGetLastError();
		if(err_code != ERROR_IO_PENDING)
		{
			if(err_code == WSAECONNRESET || err_code == WSAENETRESET || err_code == WSAENOTCONN || err_code == WSAECONNABORTED)
			{
				ForcedDisconnect(err_code);
			}
			return false;
		}
	}
	return true;
}

void Session::Dispatch(DWORD transfer_bytes, OVERLAPPED* ov)
{
	if(transfer_bytes == 0)
		return;

	OVERLAPPEDEX* overlapped = (OVERLAPPEDEX*)ov;

	switch(overlapped->_flags)
	{
	case ASYNCFLAG_SEND:
		_send_size -= transfer_bytes;
		break;

	case ASYNCFLAG_RECEIVE:
		OnReceive(transfer_bytes);
		return;
	}
}

void Session::OnReceive(DWORD transfer_bytes)
{
	_recv_size += transfer_bytes;
	char * read_pivot = _recv_buffer;

	while(_recv_size > 0)
	{
		if(_active)
		{
			auto read_size = ProcessRecv(read_pivot, _recv_size);

			if(read_size > 0)
			{
				_recv_size -= read_size;
				read_pivot += read_size;
			}
			else
			{
				if(_active)
					break;
				else
					return;
			}
		}
		else
			return;
	}

	if(_recv_size > 0)
	{
		::memmove(_recv_buffer, read_pivot, _recv_size);
	}

	if(_active)
		ReceivePost();
}

bool Session::IsConnected()
{
	if(_event_hdle == INVALID_HANDLE_VALUE || _sock == INVALID_SOCKET)
		return false;

	WSANETWORKEVENTS netEvent;
	bool ret = false;

	::ZeroMemory(&netEvent, sizeof(netEvent));
	::WSAEventSelect(_sock, _event_hdle, FD_READ | FD_CLOSE);
	::WSAEnumNetworkEvents(_sock, _event_hdle, &netEvent);

	ret = ((netEvent.lNetworkEvents & FD_CLOSE) == FD_CLOSE);

	return !(ret);
}