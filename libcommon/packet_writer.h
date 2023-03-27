#pragma once
#include "msgpack.hpp"
#include "network_packet.h"
#include <memory>

#include "packet_domain.h"

template <typename TPacketType>
class PacketWriter
{
public:
	static std::vector<uint8_t> serialize(/*const */TPacketType packet)
	{
		std::vector<uint8_t> packet_data;
		PacketHeader header;
		header.packet_type = packet.type;
		std::vector<uint8_t> data = msgpack::pack(packet);
		header.data_size = data.size();
		uint8_t* pheader = (uint8_t*)&header;
		for (auto i = 0; i < sizeof(PacketHeader); i++)
		{
			packet_data.push_back(pheader[i]);
		}
		packet_data.insert(packet_data.end(), data.begin(), data.end());
		return packet_data;
	}
};
