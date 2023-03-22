#pragma once
#include "msgpack.hpp"
#include <memory>

#include "packet_domain.h"

class DeserializePacketWriter
{
public:
//	template <typename PacketType>
	static SerializedPacketWithIdChannel getSerializePacket(const DeserializePacketWithIdChannel& deserialize_pac)
	{
		DeserializePacket& des_p = *deserialize_pac.packet;
		SerializedPacketWithIdChannel result;
		result.id_channel = deserialize_pac.id_channel;
		result.packet->header.packet_type = deserialize_pac.packet->type;
		switch (deserialize_pac.packet->type)
		{
		case PacketType::PT_CREATE_ROOM:
		{
			auto cr = dynamic_cast<PTCreateRoom&>(des_p);
			result.packet->data = msgpack::pack(cr);
			result.packet->header.data_size = result.packet->data.size();

			return result;
		}
		case PacketType::PT_CLOSE_ROOM:
		{
			auto clr = dynamic_cast<PTCloseRoom&>(des_p);
			result.packet->data = msgpack::pack(clr);
			result.packet->header.data_size = result.packet->data.size();

			return result;
		}
		case PacketType::PT_GET_ROOM_LIST:
		{
			auto grl = dynamic_cast<PTGetRoomList&>(des_p);
			result.packet->data = msgpack::pack(grl);
			result.packet->header.data_size = result.packet->data.size();

			return result;
		}
		case PacketType::PT_ROOM_LIST:
		{
			auto rl = dynamic_cast<PTRoomList&>(des_p);
			result.packet->data = msgpack::pack(rl);
			result.packet->header.data_size = result.packet->data.size();

			return result;
		}
		default:
		{
			break;
		}
		}
	}
};
