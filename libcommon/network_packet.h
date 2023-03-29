#pragma once
#include <optional>

#include "packet.h"

struct PacketHeader
{
	PacketType  packet_type;
	uint32_t data_size = 0;
};

//struct NetworkPacket
//{
//	PacketHeader header;
//	std::vector<uint8_t> data = {};
//
//	template<class T>
//	void pack(T& packer) {
//		packer(data);
//	}
//};