#pragma once
#include "Packet.h"

class IPacketParser
{
public:
	IPacketParser() {}
	virtual ~IPacketParser() {}

	virtual int Disptch(Packet& packet) = 0;
};