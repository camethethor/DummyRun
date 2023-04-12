#include "Packet.h"
#include <stdio.h>
#include <assert.h>

Packet::Packet()
: _data_begin_pos(NULL), _read_pos(NULL), _write_pos(NULL), _recv_size(0)
{
	Clear();
}

Packet::Packet(unsigned short _protocol_id)
: _data_begin_pos(NULL), _read_pos(NULL), _write_pos(NULL), _recv_size(0)
{
	Clear();
	SetID(_protocol_id);
}

Packet::Packet(const Packet& src_pack)
: _data_begin_pos(NULL), _read_pos(NULL), _write_pos(NULL), _recv_size(0)
{
	DWORD offset = 0;

	Clear();

	::CopyMemory(_buffer, src_pack._buffer, PACKET_BUFFERSIZE);
	assert(_read_pos <= _date_end_pos && "ReadPosition over flow");

	offset = static_cast<DWORD>(src_pack._read_pos - src_pack._data_begin_pos);
	_read_pos += offset;
	assert(_read_pos <= _date_end_pos && "ReadPosition over flow");

	offset = static_cast<DWORD>(src_pack._write_pos - src_pack._data_begin_pos);
	_write_pos += offset;
	assert(_write_pos <= _date_end_pos && "WritePosition  over flow");

	_recv_size = src_pack._recv_size;
}

Packet::Packet(const char* buf, const int size)
: _data_begin_pos(NULL), _read_pos(NULL), _write_pos(NULL), _recv_size(0)
{
	Clear();
	CopyToBuffer(buf, size);
}

Packet::~Packet()
{
}

bool Packet::CheckValidity()
{
	if(_recv_size < PACKET_HEADERSIZE + GetDataFieldSize())
		return false;

	return true;
}

Packet& Packet::SetID(unsigned short ID)
{
	*_header._protocol_id = ID; 
	return *this; 
}

void Packet::Clear(int bufferSize)
{
	memset(_buffer, 0, bufferSize);

	_header._protocol_id = (unsigned short*)(_buffer);	
	_header._data_size	= (unsigned int*)(_buffer + sizeof(unsigned short));		

	_read_pos = _data_begin_pos;
	_write_pos = _data_begin_pos;
	_data_begin_pos = &_buffer[PACKET_HEADERSIZE];
	_date_end_pos = &_buffer[bufferSize];

	assert(_read_pos <= _date_end_pos && "ReadPosition이 DataField를 넘어갔습니다.");
	_recv_size = PACKET_HEADERSIZE;
}

void Packet::CopyToBuffer(const char* buff, const int size)
{
	Clear();
	_recv_size = 0;

	::CopyMemory(_buffer + _recv_size, buff, size);
	_recv_size += size;
}

void Packet::ReadData(void* buffer, int size) 
{
	if(_read_pos + size > _data_begin_pos + GetDataFieldSize() || 
		_read_pos + size > _date_end_pos)
	{
		throw(1);
	}

	::CopyMemory(buffer, _read_pos, size);
	_read_pos += size;
}

void Packet::ReadData(std::string& dest, int size)
{
	if(_read_pos + size > _data_begin_pos + GetDataFieldSize() || 
		_read_pos + size > _date_end_pos)
	{
		throw(1);
	}

	dest = _read_pos;
	_read_pos += size;
}

void Packet::WriteData(const void* buffer, int size) 
{
	if(_write_pos + size > _date_end_pos)
	{
		throw(1);
	}

	::CopyMemory(_write_pos, buffer, size);
	_write_pos += size;
	_recv_size += size;
	*_header._data_size += size;
}

Packet& Packet::operator = (Packet& packet)
{
	if(this == &packet) 
		return *this;

	::CopyMemory(_buffer, packet.GetPacketBuffer(), packet.GetPacketSize());

	_header._protocol_id	= (unsigned short*)(_buffer);	
	_header._data_size		= (unsigned int*)(_buffer + sizeof(unsigned short));		
	
	_read_pos = _data_begin_pos;
	_write_pos = _data_begin_pos;
	_data_begin_pos = &_buffer[PACKET_HEADERSIZE];
	_date_end_pos = &_buffer[PACKET_BUFFERSIZE];

	assert(_read_pos <= _date_end_pos && "ReadPosition이 DataField를 넘어갔습니다.");
	_recv_size = packet._recv_size;
	return *this;
}


Packet& Packet::operator << (bool src)
{
	WriteData(&src, sizeof(bool));
	return *this;
}

Packet& Packet::operator >> (bool& dest)
{
	ReadData(&dest, sizeof(bool));
	return *this;
}

Packet& Packet::operator << (char src)
{
	WriteData(&src, sizeof(char));
	return *this;
}

Packet& Packet::operator >> (char& dest)
{
	ReadData(&dest, sizeof(char));
	return *this;
}

Packet& Packet::operator << (BYTE src)
{
	WriteData(&src, sizeof(BYTE));
	return *this;
}

Packet& Packet::operator >> (BYTE& dest)
{
	ReadData(&dest, sizeof(BYTE));
	return *this;
}

Packet& Packet::operator << (unsigned short src)
{
	WriteData(&src, sizeof(unsigned short));
	return *this;
}

Packet& Packet::operator >> (unsigned short& dest)
{
	ReadData(&dest, sizeof(unsigned short));
	return *this;
}


Packet& Packet::operator << (unsigned int src)
{
	WriteData(&src, sizeof(unsigned int));
	return *this;
}

Packet& Packet::operator >> (unsigned int& dest)
{
	ReadData(&dest, sizeof(unsigned int));
	return *this;
}

Packet& Packet::operator << (short src)
{
	WriteData(&src, sizeof(short));
	return *this;
}

Packet& Packet::operator >> (short& dest)
{
	ReadData(&dest, sizeof(short));
	return *this;
}


Packet& Packet::operator << (int src)
{
	WriteData(&src, sizeof(int));
	return *this;
}

Packet& Packet::operator >> (int& dest)
{
	ReadData(&dest, sizeof(int));
	return *this;
}


Packet& Packet::operator << (long src)
{
	WriteData(&src, sizeof(long));
	return *this;
}

Packet& Packet::operator >> (long& dest)
{
	ReadData(&dest, sizeof(long));
	return *this;
}

Packet& Packet::operator << (float src)
{
	WriteData(&src, sizeof(float));
	return *this;
}

Packet& Packet::operator >> (float& dest)
{
	ReadData(&dest, sizeof(float));
	return *this;
}

Packet& Packet::operator << (DWORD src)
{
	WriteData(&src, sizeof(DWORD));
	return *this;
}

Packet& Packet::operator >> (DWORD& dest)
{
	ReadData(&dest, sizeof(DWORD));
	return *this;
}


Packet& Packet::operator << (LPCTSTR src)
{
	WriteData(src, lstrlen(src) * sizeof(TCHAR) + sizeof(TCHAR));
	return *this;
}

Packet& Packet::operator >> (LPTSTR dest)
{
	ReadData(dest, lstrlen((LPTSTR)_read_pos) * sizeof(TCHAR) + sizeof(TCHAR));
	return *this;
}


Packet& Packet::operator << (const std::string& src)
{
	WriteData((LPCTSTR)src.c_str(), static_cast<int>(src.size()) + 1);
	return *this;
}

Packet& Packet::operator >> (std::string& dest)
{
	int strLength = lstrlen((LPTSTR)_read_pos) * sizeof(char) + sizeof(char);
	ReadData(dest, strLength);
	return *this;
}
