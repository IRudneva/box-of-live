#pragma once
#include <memory>

#include "packet_domain.h"

class DeserializePacketWriter
{
public:
	template <typename T>
	void writeDeserializePacket(T deserialize_pac);

	std::shared_ptr<NetworkPacket> getSerializePacket();


private:
	std::shared_ptr<NetworkPacket> des_packet_ = std::make_shared<NetworkPacket>();

	void reset() { des_packet_ = nullptr; }

	std::vector<uint8_t> packPacket(PTCloseRoom p) const;

	std::vector<uint8_t> packPacket(PTCreateRoom p) const;

	std::vector<uint8_t> packPacket(PTGetRoomList p) const;

	std::vector<uint8_t> packPacket(PTRoomList p) const;
};

template <typename T>
void DeserializePacketWriter::writeDeserializePacket(T deserialize_pac)
{
	if (deserialize_pac == nullptr) return;

	des_packet_->header.packet_type = deserialize_pac->type;
	DeserializePacket& c = *deserialize_pac;
	switch (des_packet_->header.packet_type)
	{
	case PacketType::PT_CREATE_ROOM:
	{
		auto p_p = dynamic_cast<PTCreateRoom&>(c);
		des_packet_->data = packPacket(p_p);
		des_packet_->header.data_size = des_packet_->data.size();
		break;

	}
	case PacketType::PT_CLOSE_ROOM:
	{
		auto p_p = dynamic_cast<PTCloseRoom&>(c);
		des_packet_->data = packPacket(p_p);
		des_packet_->header.data_size = des_packet_->data.size();
		break;
	}
	case PacketType::PT_GET_ROOM_LIST:
	{
		auto p_p = dynamic_cast<PTGetRoomList&>(c);
		des_packet_->data = packPacket(p_p);
		des_packet_->header.data_size = des_packet_->data.size();
		break;
	}
	case PacketType::PT_ROOM_LIST:
	{
		auto p_p = dynamic_cast<PTRoomList&>(c);
		des_packet_->data = packPacket(p_p);
		des_packet_->header.data_size = des_packet_->data.size();
		break;
	}
	default:
	{
		break;
	}
	}
}