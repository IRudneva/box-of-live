#pragma once
#include "packet.h"
#include "packet_domain.h"
#include "msgpack.hpp"
#include <vector>

namespace server_packet {
	struct ServerPacket : Packet
	{
	protected:
		ServerPacket(PacketType ty) : Packet(ty) {}
	public:
		void pack(msgpack::Packer& packer) const  override {}
		void pack(msgpack::Unpacker& unpacker) override {}
	};

	struct PTRoomList : ServerPacket
	{
		PTRoomList() :ServerPacket(PacketType::SRV_ROOM_LIST) {}

		std::vector<Room> room_list;

		void pack(msgpack::Packer& packer) const  override { packer(room_list); }
		void pack(msgpack::Unpacker& unpacker) override { unpacker(room_list); }
	};

	struct PTNewRoom : ServerPacket
	{
		PTNewRoom() :ServerPacket(PacketType::SRV_NEW_ROOM) {}
		Room room;
		void pack(msgpack::Packer& packer) const  override { packer(room); }
		void pack(msgpack::Unpacker& unpacker) override { unpacker(room); }
	};

	struct PTCloseRoom : ServerPacket
	{
		PTCloseRoom() :ServerPacket(PacketType::SRV_CLOSE_ROOM) {}
		uint32_t id_room = 0;
		void pack(msgpack::Packer& packer) const  override { packer(id_room); }
		void pack(msgpack::Unpacker& unpacker) override { unpacker(id_room); }
	};
} // namespace server_packet