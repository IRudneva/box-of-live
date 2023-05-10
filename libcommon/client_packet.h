#pragma once
#include "msgpack.hpp"
#include "game_domain.h"
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
		PTCreateRoom(std::shared_ptr<GameConfig> conf) :ClientPacket(PacketType::CLI_CREATE_ROOM),game_config(std::move(conf)) {}

		std::shared_ptr<GameConfig> game_config = std::make_shared<GameConfig>();
		void pack(msgpack::Packer& packer) const override { packer(*game_config); }
		void pack(msgpack::Unpacker& unpacker) override { unpacker(*game_config); }
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

		void pack(msgpack::Packer& packer) const override { }
		void pack(msgpack::Unpacker& unpacker) override { }
	};

	struct PTChooseRoom : ClientPacket
	{
		PTChooseRoom() : ClientPacket(PacketType::CLI_CHOOSE_ROOM) {}
		PTChooseRoom(uint32_t id) : ClientPacket(PacketType::CLI_CHOOSE_ROOM), id_room(id) {}

		uint32_t id_room = 0;

		void pack(msgpack::Packer& packer) const override { packer(id_room); }
		void pack(msgpack::Unpacker& unpacker) override { unpacker(id_room); }
	};

	struct PTStartGame : ClientPacket
	{
		PTStartGame() :ClientPacket(PacketType::CLI_START_GAME) {}
		PTStartGame(uint32_t id, std::shared_ptr<GameConfig> conf):ClientPacket(PacketType::CLI_START_GAME),id_room(id), game_config(std::move(conf)) {}
		uint32_t id_room = 0;
		std::shared_ptr<GameConfig> game_config = std::make_shared<GameConfig>();
		void pack(msgpack::Packer& packer) const override { packer(id_room, *game_config); }
		void pack(msgpack::Unpacker& unpacker) override { unpacker(id_room, *game_config); }
	};

	struct PacketWithIdChannel
	{
		PacketWithIdChannel(std::shared_ptr<Packet> p, uint32_t id)
			: packet(std::move(p)), id_channel(id){}

		std::shared_ptr<Packet> packet = nullptr;
		uint32_t id_channel;
	};

	struct PTAddSuperGrass : ClientPacket
	{
		PTAddSuperGrass() :ClientPacket(PacketType::CLI_ADD_SUPER_GRASS){}
		PTAddSuperGrass(uint32_t id, uint32_t ox, uint32_t oy) :ClientPacket(PacketType::CLI_ADD_SUPER_GRASS),id_room(id), pos_x(ox), pos_y(oy) {}

		uint32_t id_room = 0;
		uint32_t pos_x = 0;
		uint32_t pos_y = 0;

		void pack(msgpack::Packer& packer) const override { packer(id_room, pos_x, pos_y); }
		void pack(msgpack::Unpacker& unpacker) override { unpacker(id_room,pos_x, pos_y); }
	};
} // namespace client_packet