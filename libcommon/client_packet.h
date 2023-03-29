#pragma once
#include "msgpack.hpp"
#include "packet.h"
#include "packet_domain.h"

namespace client_packet {
	struct  ClientPacket : Packet
	{
	protected:
		ClientPacket(PacketType ty) : Packet(ty) {}
	public:
		void pack(msgpack::Packer& packer) const override {}
		void pack(msgpack::Unpacker& unpacker) override {}
	};

	struct PTCreateRoom : ClientPacket
	{
		PTCreateRoom() : ClientPacket(PacketType::CLI_CREATE_ROOM) {}
		PTCreateRoom(const std::string& name_room) : ClientPacket(PacketType::CLI_CREATE_ROOM), room(name_room) {}

		Room room;

		void pack(msgpack::Packer& packer) const override { packer(room); }
		void pack(msgpack::Unpacker& unpacker) override { unpacker(room); }
	};

	struct PTCloseRoom : ClientPacket
	{
		PTCloseRoom() : ClientPacket(PacketType::CLI_CLOSE_ROOM) {}
		PTCloseRoom(uint32_t id) : ClientPacket(PacketType::CLI_CLOSE_ROOM), id_room(id) {}
		uint32_t id_room = 0;

		void pack(msgpack::Packer& packer) const override { packer(id_room); }
		void pack(msgpack::Unpacker& unpacker) override { unpacker(id_room); }
	};

	struct PTGetRoomList : ClientPacket
	{
		PTGetRoomList() : ClientPacket(PacketType::CLI_GET_ROOM_LIST) {}

		uint8_t t = 0;

		void pack(msgpack::Packer& packer) const override { packer(t); }
		void pack(msgpack::Unpacker& unpacker) override { unpacker(t); }
	};

	struct PacketWithIdChannel
	{
		std::shared_ptr<ClientPacket> packet = nullptr;
		uint32_t id_channel;
	};
} // namespace client_packet