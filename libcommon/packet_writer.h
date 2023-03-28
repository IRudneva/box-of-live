#pragma once
#include "msgpack.hpp"
#include "network_packet.h"
#include <memory>

#include "packet_domain.h"

class PacketWriter
{
public:
	static std::vector<uint8_t> serialize(const Packet& packet)
	{
		std::vector<uint8_t> packet_data;
		std::vector<uint8_t> data = msgpack::pack(packet);
		PacketHeader header{ packet.type, (uint32_t)data.size() };

		uint8_t* header_data = (uint8_t*)&header;

		for (auto i = 0; i < (int)sizeof(header);)
		{
			packet_data.push_back(header_data[i++]);
		}
		for(auto i = 0; i < data.size();)
		{
			packet_data.push_back(data[i++]);
		}
		return packet_data;
	}
};
