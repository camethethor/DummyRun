#pragma once
#include <string>
#include <windows.h>

const int	PACKET_BUFFERSIZE	 = (1024*8);
const int	PACKET_HEADERSIZE	 = sizeof(unsigned short) + sizeof(unsigned int);

class Packet
{
public:
	Packet();
	Packet( unsigned short _protocol_id );
	Packet( const Packet& src_pack );
	Packet( const char* buf, const int size );
	virtual ~Packet();

	bool CheckValidity();	
	Packet& SetID( unsigned short ID );
	unsigned short GetID() { return *_header._protocol_id; }

	unsigned int GetDataFieldSize() { return *_header._data_size; }
	int GetPacketSize(){ return ( GetDataFieldSize() + PACKET_HEADERSIZE ); }
	int	GetReceivedSize(){ return _recv_size; }

	void Clear( int bufferSize = PACKET_BUFFERSIZE );

	char* GetPacketBuffer() { return (char*)_buffer; }
	void CopyToBuffer( const char* buff,  const int size );

	Packet&	operator = ( Packet& packet );
	Packet&	operator << ( bool src );
	Packet& operator << ( char src );
	Packet& operator << ( BYTE src );
	Packet& operator << ( unsigned short src);
	Packet& operator << ( unsigned int src );
	Packet& operator << ( short src );
	Packet&	operator << ( int src );
	Packet&	operator << ( long src );
	Packet& operator << ( float src );
	Packet&	operator << ( DWORD src );
	Packet&	operator << ( LPCTSTR src );
	Packet& operator << ( const std::string& src);

	Packet&	operator >> ( bool& dest );
	Packet& operator >> ( char& dest );
	Packet& operator >> ( BYTE& dest );
	Packet& operator >> ( unsigned short& dest);
	Packet& operator >> ( unsigned int& dest );
	Packet& operator >> ( short& dest );
	Packet&	operator >> ( int& dest );
	Packet&	operator >> ( long& dest );
	Packet& operator >> ( float& dest );
	Packet&	operator >> ( DWORD& dest );
	Packet& operator >> ( LPTSTR dest );
	Packet& operator >> ( std::string& dest );	

	void ReadData( void* buffer, int size );
	void ReadData( std::string& dest, int size );
	void WriteData( const void* buffer, int size );
	void ResetReadPt() { _read_pos = _data_begin_pos; }

private:
	typedef struct HEADER
	{
		unsigned short*		_protocol_id;
		unsigned int*		_data_size;
	} HEADER;

	HEADER			_header;							
	char			_buffer[PACKET_BUFFERSIZE];

	char*			_read_pos;
	char*			_write_pos;	
	char*			_data_begin_pos;			
	char*			_date_end_pos;			
	unsigned int	_recv_size;		
};

