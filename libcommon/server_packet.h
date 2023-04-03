#pragma once
#include "packet.h"
#include "packet_domain.h"
#include "game_domain.h"
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
		PTRoomList(const std::vector<Room>& oth_list) :ServerPacket(PacketType::SRV_ROOM_LIST), room_list(oth_list) {}
		std::vector<Room> room_list;

		void pack(msgpack::Packer& packer) const  override { packer(room_list); }
		void pack(msgpack::Unpacker& unpacker) override { unpacker(room_list); }
	};

	struct PTNewRoom : ServerPacket
	{
		PTNewRoom() : ServerPacket(PacketType::SRV_NEW_ROOM){}
		PTNewRoom(const Room& other) : ServerPacket(PacketType::SRV_NEW_ROOM), room(other) {}
		PTNewRoom(uint32_t id_room, const std::string& name) :ServerPacket(PacketType::SRV_NEW_ROOM), room(id_room,name) {}
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

	struct PTRoomState : ServerPacket
	{
		PTRoomState() : ServerPacket(PacketType::SRV_ROOM_STATE) {}
		PTRoomState(uint32_t id, const std::vector<GrassInfo>& g_info, const std::vector<BacteriumInfo>& bact_info)
		: ServerPacket(PacketType::SRV_ROOM_STATE),
		id_room(id),
		grass_info(g_info),
		bacterium_info(bact_info) {}

		uint32_t id_room = 0;
		std::vector<GrassInfo> grass_info = {};
		std::vector<BacteriumInfo> bacterium_info = {};

		void pack(msgpack::Packer& packer) const override { packer(id_room, grass_info, bacterium_info); }
		void pack(msgpack::Unpacker& unpacker) override { unpacker(id_room, grass_info, bacterium_info); }
	};

	struct PTNewConfig : ServerPacket
	{
		PTNewConfig() : ServerPacket(PacketType::SRV_NEW_CONFIG) {}
		PTNewConfig(uint32_t id, std::shared_ptr<GameConfig> conf) : ServerPacket(PacketType::SRV_NEW_CONFIG), id_room(id), game_config(conf) {}
		uint32_t id_room;
		std::shared_ptr<GameConfig> game_config;
		void pack(msgpack::Packer& packer) const override { packer(id_room, *game_config); }
		void pack(msgpack::Unpacker& unpacker) override { unpacker(id_room,*game_config); }
	};
} // namespace server_packet