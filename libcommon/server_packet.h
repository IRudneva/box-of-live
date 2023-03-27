#pragma once
#include "packet.h"
#include "packet_domain.h"
#include "msgpack.hpp"
#include <vector>

struct ServerPacket : Packet
{
protected:
	ServerPacket(PacketType ty) : Packet(ty) {}
public:
	void pack(msgpack::Packer& packer) override {}
	void pack(msgpack::Unpacker& unpacker) override {}
};

struct PTRoomList : ServerPacket
{
	PTRoomList() :ServerPacket(PacketType::PT_ROOM_LIST) {}
	
	std::vector<Room> room_list;
	
	void pack(msgpack::Packer& packer) override { packer(room_list); }
	void pack(msgpack::Unpacker& unpacker) override { unpacker(room_list); }
};
