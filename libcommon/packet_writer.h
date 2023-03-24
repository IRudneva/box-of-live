#pragma once
#include "msgpack.hpp"
#include <memory>

#include "packet_domain.h"

template <typename TPacketType>
class PacketWriter
{
public:
	static std::vector<uint8_t> serialize(const TPacketType& packet)
	{
		std::vector<uint8_t> packet_data;
		PacketHeader header;
		header.packet_type = packet->type;
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
	//	DeserializePacket& des_p = *deserialize_pac.packet;
	//	SerializedPacketWithIdChannel result;
	//	std::shared_ptr<NetworkPacket> network_packet = std::make_shared<NetworkPacket>();
	//	result.id_channel = deserialize_pac.id_channel;
	//	PacketHeader header = { deserialize_pac.packet->type };
	//	//result.packet->header.packet_type = deserialize_pac.packet->type;
	//	switch (deserialize_pac.packet->type)
	//	{
	//	case PacketType::PT_CREATE_ROOM:
	//	{
	//		auto cr = dynamic_cast<PTCreateRoom&>(des_p);
	//		std::vector<uint8_t> data = msgpack::pack(cr);
	//		header.data_size = data.size();
	//		network_packet->header = header;
	//		network_packet->data = data;

	//		result.packet = network_packet;

	//		return result;
	//	}
	//	case PacketType::PT_CLOSE_ROOM:
	//	{
	//		auto clr = dynamic_cast<PTCloseRoom&>(des_p);
	//		std::vector<uint8_t> data = msgpack::pack(clr);
	//		header.data_size = data.size();
	//		network_packet->header = header;
	//		network_packet->data = data;

	//		result.packet = network_packet;

	//		return result;
	//	}
	//	case PacketType::PT_GET_ROOM_LIST:
	//	{
	//		auto grl = dynamic_cast<PTGetRoomList&>(des_p);
	//		std::vector<uint8_t> data = msgpack::pack(grl);
	//		header.data_size = data.size();
	//		network_packet->header = header;
	//		network_packet->data = data;

	//		result.packet = network_packet;

	//		return result;
	//	}
	//	case PacketType::PT_ROOM_LIST:
	//	{
	//		auto rl = dynamic_cast<PTRoomList&>(des_p);
	//		std::vector<uint8_t> data = msgpack::pack(rl);
	//		header.data_size = data.size();
	//		network_packet->header = header;
	//		network_packet->data = data;

	//		result.packet = network_packet;

	//		return result;
	//	}
	//	default:
	//	{
	//		break;
	//	}
	//	}
	//}
};
